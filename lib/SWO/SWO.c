#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/tpiu.h>
#include <libopencm3/cm3/itm.h>
#include <libopencm3/stm32/dbgmcu.h>

#include "SWO.h"

void swo_enable()
{
  swo_gpio_setup();
  dbg_setup();
  tpiu_setup();
  itm_setup();
}

void swo_gpio_setup()
{
  /* PA13、PA14 */
  gpio_mode_setup(GPIOA,
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE,
                  GPIO13 | GPIO14);

  gpio_set_output_options(GPIOA, GPIO_PUPD_NONE, GPIO_OSPEED_100MHZ, GPIO13 | GPIO14);

  gpio_set_af(GPIOA,
              GPIO_AF0,
              GPIO13 | GPIO14);

  /* PB3 */
  gpio_mode_setup(GPIOB,
                  GPIO_MODE_AF,
                  GPIO_PUPD_NONE,
                  GPIO3);

  gpio_set_output_options(GPIOB, GPIO_PUPD_NONE, GPIO_OSPEED_100MHZ, GPIO3);

  gpio_set_af(GPIOB,
              GPIO_AF0,
              GPIO3);
}

void dbg_setup()
{
  // Enable Trace IO
  DBGMCU_CR |= DBGMCU_CR_TRACE_IOEN;

  // set debug mode as mode 0 "Asynchronous Mode"
  uint32_t dbmode = DBGMCU_CR;
  dbmode &= ~DBGMCU_CR_TRACE_MODE_MASK;
  dbmode |= DBGMCU_CR_TRACE_MODE_ASYNC;
  DBGMCU_CR = dbmode;
}

void tpiu_setup()
{
  // Configure TPIU_CSPSR (Current Port Size Register)

  TPIU_CSPSR |= (0x1 << 0);

  // Configure TPIU_FFCR ,0x102 is a default value
  TPIU_FFCR |= (0x102);

  /*
    Configure SPP_R (Selected Pin Protocol)
    Using  Asynchronous MANCHESTER mode.
  */
  uint32_t sppr = TPIU_SPPR;
  sppr &= ~TPIU_SPPR_MASK;
  sppr |= TPIU_SPPR_ASYNC_MANCHESTER;
  TPIU_SPPR = sppr;

  /*
      Configure CSPS_R (Current port size Register)
      and choose size 4
  */
  TPIU_CSPSR |= (0x1 << 3);

  /*
    Flash control
    bit8 always 1
    bit 1 can be set to 1 in asynchronous mode
    else reserve 0.
  */
  TPIU_FFCR |= TPIU_FFCR_TRIGIN;
  TPIU_FFCR |= TPIU_FFCR_ENFCONT;
}

void itm_setup()
{
  /*
     Configure the ITM
     1. Unlock ITM lock access
     2. Configure TC_R(ITM_TCR)
     3. Configure TE_R(ITM_TER)
     4. Configure TP_R(ITM_TPR)
  */
  ITM_LAR |= CORESIGHT_LAR_KEY;

  uint32_t tcr_config = ITM_TCR;
  // tcr_config |= ITM_TCR_SWOENA;
  // tcr_config |= ITM_TCR_TXENA;
  // tcr_config |= ITM_TCR_SYNCENA;
  // tcr_config |= ITM_TCR_TSENA;
  // tcr_config |= ITM_TCR_ITMENA;
  tcr_config |= (0x00010005);
  ITM_TCR = tcr_config;

  *ITM_TER |= (0x1 << 0);

  ITM_TPR |= (0x1 << 0);
}

void etm_setup()
{
  ETM_CR |= (0x00001D1E);
  ETM_TER |= (0x0000406F);
  ETM_TEER |= (0x0000006F);
  ETM_TSTART_OR_STOPR |= (0x00000001);
  ETM_CR |= (0x0000191E);
}

/* For printf(). */
int _write(int file, char *ptr, int len)
{
  for (int index = 0; index < len; index++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
}

uint32_t ITM_SendChar(uint32_t ch)
{
  if ((ITM_TCR & ITM_TCR_ITMENA) != 0UL &&
      (*ITM_TER & (1ul << 0)) != 0UL)
  {
    while (ITM_STIM32(0) == 0UL)
    {
      __asm__("nop"); /* Do nothing. */
    }
    ITM_STIM8(0) = (uint8_t)ch;
  }
  return ch;
}