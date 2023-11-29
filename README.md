# This repository is to enable the SWO on STM32 nucleo F446RE.
## To enable the SWO you have to
1. Configure the ==DBGMCR==
    1. Enalbe the TRACEIO
    2. Select the trace mode you want, in this project is "asynchronous  mode".
2. Configure the ==TPIU==
    1. Configure TPIU_CSPSR (Current Port Size Register)
    2. Configure TPIU_FFCR (0x102 is a default value from th user manual)
    3. Configure SPP_R (Selected Pin Protocol). Using  "Asynchronous MANCHESTER mode".
    4. Configure CSPS_R (Current port size Register) and choose size 4
    5. Flash control. bit8 always 1 , bit 1 can be set to 1 in asynchronous mode , else reserve 0.
3. Configure the ==ITM==
     1. Unlock ITM lock access
     2. Configure TC_R(ITM_TCR)
     3. Configure TE_R(ITM_TER)
     4. Configure TP_R(ITM_TPR)
4. Configure the ==GPIO==
    1. PA13、PA14、PB3

## To emit word from ITM by using "printf()" function you have to  

1. Definie ITM_SendChar function_write function
    1. Check if the emitted word is from ITM_TCR
    2. Check the ITM_Enable ports (0~31)
2. Definie _write function

## Then you can easily use "STM32 ST-Link utility" to observe the word that emitted by your board!