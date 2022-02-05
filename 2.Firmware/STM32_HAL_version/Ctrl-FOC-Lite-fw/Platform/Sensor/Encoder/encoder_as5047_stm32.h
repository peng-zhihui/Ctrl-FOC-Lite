#ifndef CTRL_STEP_FW_AS5047_STM32_H
#define CTRL_STEP_FW_AS5047_STM32_H

#include "Ctrl/Sensor/Encoder/Instances/encoder_as5047.h"

class EncoderAS5047 : public EncoderAS5047Base
{
public:
    explicit EncoderAS5047() : EncoderAS5047Base()
    {}


private:
    void SpiInit() override;

    uint16_t SpiTransmitAndRead16Bits(uint16_t _data) override;
};

#endif
