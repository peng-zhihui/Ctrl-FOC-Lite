#include "time_utils.h"
#include "stm32f1xx_hal.h"

void delay(uint32_t _ms)
{
    uint32_t t0 = micros();
    while (micros() - t0 < _ms * 1000)
        __NOP();
}


void delayMicroseconds(uint32_t _us)
{
    uint32_t t0 = micros();
    while (micros() - t0 < _us)
        __NOP();
}


__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag()
{
    return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}


uint64_t micros()
{
    /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
    LL_SYSTICK_IsActiveCounterFlag();
    uint32_t m = HAL_GetTick();
    static const uint32_t tms = SysTick->LOAD + 1;
    __IO uint32_t u = tms - SysTick->VAL;
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
        m = HAL_GetTick();
        u = tms - SysTick->VAL;
    }
    return (m * 1000 + (u * 1000) / tms);
}


uint32_t millis()
{
    return HAL_GetTick();
}


