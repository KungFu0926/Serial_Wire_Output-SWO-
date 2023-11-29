#include <libopencm3/stm32/rcc.h>

#include "SWO.h"

void rcc_setup(void);

int main(void)
{
  rcc_setup();
  swo_enable();

  while (1)
  {
    printf("abc\n");
  }
}

void rcc_setup(void)
{
  /*
      Using 180MHz as system frequency with 8MHz HSE source
      ahb_frequency  = 180MHz,
      apb1_frequency = 90MHz,
      apb2_frequency = 180MHz,
  */
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

  /*
      You must to enable every peripheral of rcc
      include every GPIO PORT and Alternate functions
  */
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
}