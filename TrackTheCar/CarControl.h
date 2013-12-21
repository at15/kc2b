#pragma once
#include "SerialPort.h"
class CCarControl
{
public:
    CCarControl(void);
    ~CCarControl(void);
public:
    // init it with the com number
    bool Init(int com_num);
    
    // control the small car
    void GoLeft();
    void GoRight();
    void GoBack();
    // 默认会把方向改到正前方，但调整速度时是不改方向的
    void GoForward(bool change_direction = true);

    void Stop();

    // 只是设置速度，在下次指令时才会生效。
    bool SpeedUp();
    bool SpeedDown();
    // 速度的值从3-9
    bool SetSpeed(int new_speed);
private:
    CSerialPort* m_port;
    bool port_ready;
    int m_speed;
    bool IsValidSpeed(int speed);

    // 发送的指令类型
    enum opcode { 
        kSFront,  // 从停止开始前进 
        kLFront,  // 从左转状态开始正向前进
        kRFront,  // 从右转状态开始正向前进

        kBack,	  // 后退
        kLeft,    // 左转
        kRight,   // 右转
        kStop,    // 停止
        speedup,  // 加速
        slowdown, // 减速
    };
    opcode m_current_op; // current op
    opcode m_last_op; // store the last op
    /* ------------------------------------------------------------------------*/
    /**
    * @brief 发送指令给小车
    *
    * @param op 指令类型
    */
    /* --------------------------------------------------------------------------*/
    void RunCar(opcode op);
    void ClosePort();
};

