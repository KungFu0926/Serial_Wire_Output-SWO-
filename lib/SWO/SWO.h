/**
 * @file SWO.h
 * @brief 1. For Stm32 F446RE SWO (Asynchronous mode)
 *           To use this swo function, just call "swo_enable()".
 *        2. etm_setup is unuse.
 *        3. _write function is for ITM output
 *        4. Not suring why it will emit "Garbled characters" while pressing start buttom first time.
 *           This function will become normal when pressing the start button second time.
 * @author ZhengKF (nfu202208@gmail.com)
 * @copyright MIT License.
 */
#ifndef USER_SWO_H_
#define USER_SWO_H_

#include <stdint.h>

#define TPIU_SPPR_MASK (0x3)
#define ETM_BASE (PPBI_BASE + 0x41000)
#define ETM_CR MMIO32(ETM_BASE + 0x000)
#define ETM_SR MMIO32(ETM_BASE + 0x010)
#define ETM_TER MMIO32(ETM_BASE + 0x008)
#define ETM_TECR MMIO32(ETM_BASE + 0x01C)
#define ETM_TEER MMIO32(ETM_BASE + 0x020)
#define ETM_TSTART_OR_STOPR MMIO32(ETM_BASE + 0x024)

void swo_enable(void);

void swo_gpio_setup(void);
void dbg_setup(void);
void tpiu_setup(void);
void itm_setup(void);

void etm_setup(void);

int _write(int file, char *ptr, int len);
uint32_t ITM_SendChar(uint32_t ch);

#endif