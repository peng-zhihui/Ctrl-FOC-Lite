#include "common_inc.h"
#include "Platform/Utils/st_hardware.h"

EncoderAS5047 encoder;

/* Component Definitions -----------------------------------------------------*/
BoardConfig_t boardConfig;


#include "adc.h"

/* Main Entry ----------------------------------------------------------------*/
void Main()
{
    uint64_t serialNum = GetSerialNumber();
    uint16_t defaultNodeID = 0;

    encoder.Init();

    //HAL_TIM_Base_Start_IT(&htim1);

    HAL_ADCEx_Calibration_Start(&hadc1);


    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 200);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 300);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 300);

    delay(10);
    //    HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcData, 2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 720);
    HAL_ADCEx_InjectedStart_IT(&hadc1);


    for (;;)
    {
        encoder.update();
//        printf("%.2f,%.2f\n", encoder.GetAngle(), encoder.getVelocity());
        delay(1);

        printf("%d,%d\n", adcData[0], adcData[1]);
    }
}


void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    GPIOB->BSRR = GPIO_PIN_12;
    adcData[0] = hadc->Instance->JDR1;
    adcData[1] = hadc->Instance->JDR2;
    GPIOB->BRR = GPIO_PIN_12;
}