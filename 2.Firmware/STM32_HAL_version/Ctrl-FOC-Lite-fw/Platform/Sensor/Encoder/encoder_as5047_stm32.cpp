#include "encoder_as5047_stm32.h"
#include "spi.h"

void EncoderAS5047::SpiInit()
{
    MX_SPI1_Init();
}


uint16_t EncoderAS5047::SpiTransmitAndRead16Bits(uint16_t _dataTx)
{
    uint16_t dataRx;

    GPIOA->BRR = GPIO_PIN_4; // Chip select
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &_dataTx, (uint8_t*) &dataRx, 1, HAL_MAX_DELAY);
    GPIOA->BSRR = GPIO_PIN_4;

    return dataRx;
}
