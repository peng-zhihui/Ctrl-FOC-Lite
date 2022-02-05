#include "encoder_as5047.h"
#include "spi.h"


EncoderAS5047Base::EncoderAS5047Base()
{
    angle_register = 0x3FFF;
    cpr = 16384;
    bit_resolution = 14;
    data_start_bit = 13;
    command_rw_bit = 14;
    command_parity_bit = 15;
}


void EncoderAS5047Base::Init()
{
    SpiInit();
    InitVars();
}


float EncoderAS5047Base::GetSensorAngle()
{
    return ((float) GetRawData() / (float) cpr) * _2PI;
}


uint16_t EncoderAS5047Base::GetRawData()
{
    uint16_t command = angle_register;

    if (command_rw_bit > 0)
    {
        command = angle_register | (1 << command_rw_bit);
    }
    if (command_parity_bit > 0)
    {
        //Add a parity bit on the the MSB
        command |= ((uint16_t) SpiCalcEvenParity(command) << command_parity_bit);
    }

    SpiTransmitAndRead16Bits(command);
    // the minimum time possible in plain arduino. 350ns is the required time for AMS sensors, 80ns for MA730, MA702
    delayMicroseconds(1);
    uint16_t register_value = SpiTransmitAndRead16Bits(0x00);

    //this should shift data to the rightmost bits of the word
    register_value = register_value >> (1 + data_start_bit - bit_resolution);
    const static uint16_t data_mask = 0xFFFF >> (16 - bit_resolution);

    // Return the data, stripping the non data (e.g parity) bits
    return register_value & data_mask;
}


uint8_t EncoderAS5047Base::SpiCalcEvenParity(uint16_t value)
{
    uint8_t cnt = 0;
    uint8_t i;

    for (i = 0; i < 16; i++)
    {
        if (value & 0x1) cnt++;
        value >>= 1;
    }

    return cnt & 0x1;
}
