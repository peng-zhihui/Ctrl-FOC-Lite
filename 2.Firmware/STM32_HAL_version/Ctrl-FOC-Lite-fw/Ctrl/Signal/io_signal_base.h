#ifndef CTRL_STEP_FW_IO_SIGNAL_BASE_H
#define CTRL_STEP_FW_IO_SIGNAL_BASE_H

#include <stdint-gcc.h>

class IoSignalBase
{
public:
    IoSignalBase()
    = default;


    typedef enum
    {
        MODE_DISABLE,
        STEP_DIR_POSITION,
        PWM_CURRENT,
        PWM_VELOCITY,
        PWM_POSITION,
        SERVO_PPM_POSITION
    } Mode_t;


    class StepDirInterface
    {
    public:
        static const uint16_t DEFAULT_SUBDIVIDE = 16;


        explicit StepDirInterface(IoSignalBase* _context) :
            context(_context)
        {}

        StepDirInterface(IoSignalBase* _context, bool _signalEnIOInverted, bool _signalDirIOInverted) :
            context(_context), signalDirIOInverted(_signalDirIOInverted),
            signalEnIOInverted(_signalEnIOInverted)
        {}


        void Init(uint16_t _subdivide);
        void UpdateSignals();
        void RefreshStepCountDirection();


    private:
        IoSignalBase* context;
        uint16_t subdivide = DEFAULT_SUBDIVIDE;
        uint16_t subdivideSteps = 0;
        bool signalEnIOInverted = false;
        bool signalDirIOInverted = false;
        bool signalEn = false;
        int16_t signalStepLast = 0;
        int16_t signalStep = 0;
        int16_t signalStepDelta = 0;
        int32_t interpOut = 0;

        void SetSubdivide(uint16_t _subdivide);
    };
    StepDirInterface stepDirInterface = StepDirInterface(this);

    /*
    class PwmHelper
    {
    public:
        PwmHelper(IoSignalBase* _context)
        {
            context = _context;
        }

        int32_t De_TOP_Width = (20 * 1000);                        //默认(20ms)
        int32_t De_BOTTOM_Width = (0);                                 //默认(0ms)
        int32_t De_TOP_Location = (51200 / 2);      //默认(半圈)
        int32_t De_BOTTOM_Location = (0);                             //默认(0p)
        int32_t De_TOP_Speed = (30 * 51200);         //默认(额定速度)
        int32_t De_BOTTOM_Speed = (0);                                      //默认(0p/s)
        int32_t De_TOP_Current = (1000);  //默认(额定电流)
        int32_t De_BOTTOM_Current = (0);                                    //默认(0A)
        bool valid_top_width;                //各种有效标志
        bool valid_bottom_width;            //各种有效标志
        bool valid_top_location;            //各种有效标志
        bool valid_bottom_location;    //各种有效标志
        bool valid_top_speed;                //各种有效标志
        bool valid_bottom_speed;            //各种有效标志
        bool valid_top_current;            //各种有效标志
        bool valid_bottom_current;        //各种有效标志
        uint16_t top_width;                //Top_PWM
        uint16_t bottom_width;            //Bottom_PWM
        int32_t top_location;            //Top_Location
        int32_t bottom_location;    //Bottom_Location
        int32_t top_speed;                //Top_Speed
        int32_t bottom_speed;            //Bottom_Speed
        int16_t top_current;            //Top_Current
        int16_t bottom_current;        //Bottom_Current
        //采集数据(PWM)
        uint16_t h_width;                //PWM高宽度
        uint16_t period;                    //PWM周期
        uint8_t count_rising;        //PWM上升沿计数器镜像
        uint8_t count_falling;    //PWM下降沿计数器镜像
        uint8_t count_update;        //PWM更新计数器
        bool whole_h_flag;            //PWM全高标志
        bool whole_l_flag;            //PWM全低标志
        bool ready_first;            //PWM就绪标志(完成一次PWM采集后置位)
        bool ready_second;            //PWM就绪标志(完成二次PWM采集后置位)
        bool ready_third;            //PWM就绪标志(完成三次PWM采集后置位)
        uint16_t valid_width;        //PWM有效宽度


        void Signal_PWM_Set_TopWidth(uint16_t width);                    //Signal_PWM设置最大脉宽
        void Signal_PWM_Set_BottomWidth(uint16_t width);            //Signal_PWM设置最小脉宽
        void Signal_PWM_Set_TopLocation(int32_t location);        //Signal_PWM设置最大脉宽表示位置
        void Signal_PWM_Set_BottomLocation(int32_t location);    //Signal_PWM设置最小脉宽表示位置
        void Signal_PWM_Set_TopSpeed(int32_t speed);                    //Signal_PWM设置最大脉宽表示速度
        void Signal_PWM_Set_BottomSpeed(int32_t speed);                //Signal_PWM设置最小脉宽表示速度
        void Signal_PWM_Set_TopCurrent(int32_t current);            //Signal_PWM设置最大脉宽表示电流
        void Signal_PWM_Set_BottomCurrent(int32_t current);        //Signal_PWM设置最小脉宽表示电流
        void Signal_PWM_Init(void);                            //Signal_PWM采集初始化
        void Signal_PWM_Config(void);                        //Signal_PWM采集配置
        void Signal_PWM_DeConfig(void);                    //Signal_PWM清理配置
        void Signal_PWM_Capture_Goal(void);            //Signal_PWM获取目标
        void Signal_PWM_TIM_Callback(void);            //Signal_PWM采集中断回调
    private:
        IoSignalBase* context;
    };
    PwmHelper pwmHelper = PwmHelper(this);
    */


    int32_t goalPosition = 0;
    int32_t goalVelocity = 0;
    int16_t goalCurrent = 0;
    bool goalDisable = false;
    bool goalBrake = false;


    void Init(uint16_t _subdivide);
    void ConfigMode(Mode_t _mode);
    void UpdateSignals();


protected:
    Mode_t currentMode = MODE_DISABLE;


    /***** Port Specified Implements *****/
    virtual void ReInitGpioForMode(Mode_t _mode) = 0;
    virtual bool ReadSignalEnIOState() = 0;
    virtual bool ReadSignalDirIOState() = 0;
    virtual int16_t ReadSignalStepCount() = 0;
    virtual void SetSignalStepCounterDirection(bool _isCountUp) = 0;
};

#endif
