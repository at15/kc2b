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
    void GoForward();
    void Stop();
private:
    CSerialPort m_port;
    bool port_ready;
    // 发送的指令类型
    enum opcode { 
        kSFront,  // 从停止开始前进 
        kLFront,  // 从左转状态开始正向前进
        kRFront,  // 从右转状态开始正向前进

        kLeft,    // 左转
        kRight,   // 右转
        kStop,    // 停止
        speedup,  // 加速
        slowdown, // 减速
    };

    opcode m_last_op; // store the last op
    /* ------------------------------------------------------------------------*/
    /**
    * @brief 发送指令给小车
    *
    * @param op 指令类型
    */
    /* --------------------------------------------------------------------------*/
    void RunCar(opcode op);

    
};

