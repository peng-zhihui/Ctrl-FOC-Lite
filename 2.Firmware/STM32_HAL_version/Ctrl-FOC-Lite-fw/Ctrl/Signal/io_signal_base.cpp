#include "io_signal_base.h"


void IoSignalBase::Init(uint16_t _subdivide)
{
    stepDirInterface.Init(_subdivide);
// ToDo   Signal_PWM_Init();
}


void IoSignalBase::ConfigMode(IoSignalBase::Mode_t _mode)
{
    if (_mode == currentMode)
        return;

    ReInitGpioForMode(_mode);
}


void IoSignalBase::UpdateSignals()
{
    switch (currentMode)
    {
        case MODE_DISABLE:
            break;
//        case MoreIO_Mode_PWM_Location:
//            Signal_PWM_Capture_Goal();
//            break;
//        case MoreIO_Mode_PWM_Speed:
//            Signal_PWM_Capture_Goal();
//            break;
//        case MoreIO_Mode_PWM_Current:
//            Signal_PWM_Capture_Goal();
//            break;
        case STEP_DIR_POSITION:
            stepDirInterface.UpdateSignals();
            break;
    }
}


void IoSignalBase::StepDirInterface::SetSubdivide(uint16_t _subdivide)
{
    if ((_subdivide == 2)
        || (_subdivide == 4)
        || (_subdivide == 8)
        || (_subdivide == 16)
        || (_subdivide == 32)
        || (_subdivide == 64)
        || (_subdivide == 128)
        || (_subdivide == 256))
    {
        subdivide = _subdivide;
    } else
    {
        subdivide = DEFAULT_SUBDIVIDE;
    }

    subdivideSteps = 256 / subdivide;
}


void IoSignalBase::StepDirInterface::Init(uint16_t _subdivide)
{
    SetSubdivide(_subdivide);
    RefreshStepCountDirection();
}


void IoSignalBase::StepDirInterface::UpdateSignals()
{
    if (context->ReadSignalEnIOState())
    {
        if (signalEnIOInverted) signalEn = true;
        else signalEn = false;
    } else
    {
        if (signalEnIOInverted) signalEn = false;
        else signalEn = true;
    }

    signalStepLast = signalStep;
    signalStep = context->ReadSignalStepCount();
    signalStepDelta = signalStep - signalStepLast;

    interpOut = signalStepDelta * subdivideSteps;

    if (signalEn)
    {
        context->goalPosition = interpOut;    //Count模式借用目标位置存放目标位置增量
        context->goalDisable = false;
        context->goalBrake = false;
    } else
    {
        context->goalPosition = 0;    //Count模式借用目标位置存放目标位置增量
        context->goalDisable = true;
        context->goalBrake = false;
    }
}


void IoSignalBase::StepDirInterface::RefreshStepCountDirection()
{
    if (signalDirIOInverted)
    {
        if (context->ReadSignalDirIOState())
            context->SetSignalStepCounterDirection(false);
        else
            context->SetSignalStepCounterDirection(true);
    } else
    {
        if (context->ReadSignalDirIOState())
            context->SetSignalStepCounterDirection(true);
        else
            context->SetSignalStepCounterDirection(false);
    }
}

/*
void IoSignalBase::PwmHelper::Signal_PWM_Set_TopWidth(uint16_t width)
{
    if (width <= 50000)
    {
        top_width = width;
        valid_top_width = true;
    } else
    {
        valid_top_width = false;
    }
}
void IoSignalBase::PwmHelper::Signal_PWM_Set_BottomWidth(uint16_t width)
{
    if (width <= 50000)
    {
        bottom_width = width;
        valid_bottom_width = true;
    } else
    {
        valid_bottom_width = false;
    }
}
void IoSignalBase::PwmHelper::Signal_PWM_Set_TopLocation(int32_t location)
{
    top_location = location;
    valid_top_location = true;
}
void IoSignalBase::PwmHelper::Signal_PWM_Set_BottomLocation(int32_t location)
{
    bottom_location = location;
    valid_bottom_location = true;

}
void IoSignalBase::PwmHelper::Signal_PWM_Set_TopSpeed(int32_t speed)
{
    if ((speed >= -motor.config.ratedVelocity) && (speed <= motor.config.ratedVelocity))
    {
        top_speed = speed;
        valid_top_speed = true;
    } else
    {
        valid_top_speed = true;
    }
}
void IoSignalBase::PwmHelper::Signal_PWM_Set_BottomSpeed(int32_t speed)
{
    if ((speed >= -motor.config.ratedVelocity) && (speed <= motor.config.ratedVelocity))
    {
        bottom_speed = speed;
        valid_bottom_speed = true;
    } else
    {
        valid_bottom_speed = true;
    }

}
void IoSignalBase::PwmHelper::Signal_PWM_Set_TopCurrent(int32_t current)
{
    if ((current >= -motor.config.ratedCurrent) && (current <= motor.config.ratedCurrent))
    {
        top_current = current;
        valid_top_current = true;
    } else
    {
        valid_top_current = true;
    }
}
void IoSignalBase::PwmHelper::Signal_PWM_Set_BottomCurrent(int32_t current)
{
    if ((current >= -motor.config.ratedCurrent) && (current <= motor.config.ratedCurrent))
    {
        bottom_current = current;
        valid_bottom_current = true;
    } else
    {
        valid_bottom_current = true;
    }
}
void IoSignalBase::PwmHelper::Signal_PWM_Set_Default(void)
{

}
void IoSignalBase::PwmHelper::Signal_PWM_Init(void)
{
    //配置
    if (!valid_top_width) Signal_PWM_Set_TopWidth(De_TOP_Width);
    if (!valid_bottom_width) Signal_PWM_Set_BottomWidth(De_BOTTOM_Width);
    if (!valid_top_location) Signal_PWM_Set_TopLocation(De_TOP_Location);
    if (!valid_bottom_location) Signal_PWM_Set_BottomLocation(De_BOTTOM_Location);
    if (!valid_top_speed) Signal_PWM_Set_TopSpeed(De_TOP_Speed);
    if (!valid_bottom_speed) Signal_PWM_Set_BottomSpeed(De_BOTTOM_Speed);
    if (!valid_top_current) Signal_PWM_Set_TopCurrent(1000);
    if (!valid_bottom_current) Signal_PWM_Set_BottomCurrent(De_BOTTOM_Current);
}
void IoSignalBase::PwmHelper::Signal_PWM_Config(void)
{ //外设初始化
    REIN_GPIO_SIGNAL_PWM_Init();
    REIN_TIM_SIGNAL_PWM_Init();

    //采集数据
    h_width = 0;
    period = 0;
    count_rising = 0;
    count_falling = 0;
    count_update = 0;
    whole_h_flag = false;
    whole_l_flag = false;
    ready_first = false;
    ready_second = false;
    ready_third = false;

}
void IoSignalBase::PwmHelper::Signal_PWM_DeConfig(void)
{
    //外设清理
    REIN_GPIO_SIGNAL_PWM_DeInit();
    REIN_TIM_SIGNAL_PWM_DeInit();
}
void IoSignalBase::PwmHelper::Signal_PWM_Capture_Goal(void)
{
//各个都已经更新
}
void IoSignalBase::PwmHelper::Signal_PWM_TIM_Callback(void)
{
    //定时器捕获中断通道1 (上升沿中断) (必须在更新中断前执行)
    if (__HAL_TIM_GET_FLAG(&SIGNAL_PWM_Get_HTIM, TIM_FLAG_CC1) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC1) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC1);
            SIGNAL_PWM_Get_HTIM.Channel = HAL_TIM_ACTIVE_CHANNEL_1;

            //采集上升沿数据
            period =
                HAL_TIM_ReadCapturedValue(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_1) + 1;        //获取PWM周期
            count_rising = count_update;                                                                                        //更新计数器镜像
        }
    }
    //定时器捕获中断通道2 (下降沿中断)
    if (__HAL_TIM_GET_FLAG(&SIGNAL_PWM_Get_HTIM, TIM_FLAG_CC2) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC2) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&SIGNAL_PWM_Get_HTIM, TIM_IT_CC2);
            SIGNAL_PWM_Get_HTIM.Channel = HAL_TIM_ACTIVE_CHANNEL_2;

            //采集下降沿数据
            h_width = HAL_TIM_ReadCapturedValue(&SIGNAL_PWM_Get_HTIM, TIM_CHANNEL_2) + 1;    //获取PWM高宽度
            count_falling = count_update;                                                                                        //更新计数器镜像
        }
    }
    //定时器更新中断 (更新事件中断) (由更新事件触发 | 溢出事件触发)
    if (__HAL_TIM_GET_FLAG(&SIGNAL_PWM_Get_HTIM, TIM_FLAG_UPDATE) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(&SIGNAL_PWM_Get_HTIM, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_IT(&SIGNAL_PWM_Get_HTIM, TIM_IT_UPDATE);

            //单次PWM全高全低检测
            if ((count_update != count_rising)    //(上升沿计数器镜像,更新计数器不相等)
                && (count_update != count_falling)    //(下降沿计数器镜像,更新计数器不相等)
                )
            {
                //读取PWM电平用于判定全高或全低
                if (SIGNAL_PWM_PUL_GPIO_Port->IDR & SIGNAL_PWM_PUL_Pin)
                {
                    whole_h_flag = true;        //置位100%标志
                    whole_l_flag = false;    //清位0%标志
                } else
                {
                    whole_h_flag = false;    //清位100%标志
                    whole_l_flag = true;        //置位0%标志
                }
            } else
            {
                count_update++;
                whole_h_flag = false;        //清位100%标志
                whole_l_flag = false;        //清位0%标志
            }

            ///提取本周期PWM
            if (whole_h_flag) h_width = 65535;
            if (whole_l_flag) h_width = 0;

            //单次PWM有效性确认
            if (0)
            {}
            else if ((whole_h_flag)) ready_first = false;    //100%_PWM无效
            else if ((whole_l_flag)) ready_first = false;    //0%_PWM无效
            else if ((top_width < (65535 - 100)) && ((top_width + 100) < h_width))
                ready_first = false;    //脉宽超长
            else if ((bottom_width > (0 + 100)) && ((bottom_width - 100) > h_width))
                ready_first = false;    //脉宽超短
            else ready_first = true;

            //可靠的PWM有效性确认
            if (ready_first)
            {
                if (ready_second)
                {
                    ready_third = true;
                } else
                {
                    ready_second = true;
                    ready_third = false;
                }
            } else
            {
                ready_second = false;
                ready_third = false;
            }

            //第三次采样成功时开始获取数据
            if (ready_third)
            {
                //提取有效PWM
                if (h_width > top_width)
                    valid_width = top_width;            //大于最大取最大
                else if (h_width < bottom_width)
                    valid_width = bottom_width;        //小于最小取最小
                else valid_width = h_width;                //不大不小就合适

                //提取有效目标
                if (context->mode == MoreIO_Mode_PWM_Location)
                {
                    //提取目标位置
                    context->goal_location =
                        (int32_t) ((float) ((int32_t) valid_width - (int32_t) bottom_width)
                                   / (float) ((int32_t) top_width - (int32_t) bottom_width)
                                   *
                                   (float) ((int32_t) top_location - (int32_t) bottom_location)
                        )
                        + (int32_t) (bottom_location);
                } else if (context->mode == MoreIO_Mode_PWM_Speed)
                {
                    //提取目标速度
                    context->goal_speed =
                        (int32_t) ((float) ((int32_t) valid_width - (int32_t) bottom_width)
                                   / (float) ((int32_t) top_width - (int32_t) bottom_width)
                                   * (float) ((int32_t) top_speed - (int32_t) bottom_speed)
                        )
                        + (int32_t) (bottom_speed);
                    //DIR引脚低电平--->目标速度取负值
                    if (!SIGNAL_PWM_READ_DIR_IO())
                        context->goal_speed = -context->goal_speed;
                } else if (context->mode == MoreIO_Mode_PWM_Current)
                {
                    //提取目标电流
                    context->goal_current =
                        (int32_t) ((float) ((int32_t) valid_width - (int32_t) bottom_width)
                                   / (float) ((int32_t) top_width - (int32_t) bottom_width)
                                   * (float) ((int32_t) top_current - (int32_t) bottom_current)
                        )
                        + (int32_t) (bottom_current);
                    //DIR引脚低电平--->目标电流取负值
                    if (!SIGNAL_PWM_READ_DIR_IO())
                        context->goal_current = -context->goal_current;
                }

                //提取特殊目标
                context->goal_disable = false;
                context->goal_brake = false;
            } else
            {
                //提取特殊目标
                context->goal_disable = true;
                context->goal_brake = false;
            }
        }
    }
}
 */
