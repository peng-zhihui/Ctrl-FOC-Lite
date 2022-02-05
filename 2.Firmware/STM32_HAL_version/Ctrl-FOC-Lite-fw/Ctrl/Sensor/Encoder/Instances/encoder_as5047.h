#ifndef MAGNETICSENSORSPI_LIB_H
#define MAGNETICSENSORSPI_LIB_H

#include "Ctrl/Sensor/Encoder/encoder_base.h"
#include "Ctrl/Utils/foc_utils.h"


class EncoderAS5047Base : public EncoderBase
{
public:
    EncoderAS5047Base();

    void Init();


private:
    uint16_t GetRawData();
    uint8_t SpiCalcEvenParity(uint16_t value);

    uint8_t bit_resolution; //!< the number of bites of angle data
    uint16_t cpr; //!< Maximum range of the magnetic sensor
    // spi variables
    uint16_t angle_register; //!< SPI angle register to read
    uint8_t command_parity_bit; //!< the bit where parity flag is stored in command
    uint8_t command_rw_bit; //!< the bit where read/write flag is stored in command
    uint8_t data_start_bit; //!< the the position of first bit

    float GetSensorAngle() override;


    /***** Port Specified Implements *****/
    virtual void SpiInit() = 0;

    virtual uint16_t SpiTransmitAndRead16Bits(uint16_t _dataTx) = 0;
};


#endif
