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
    // ���͵�ָ������
    enum opcode { 
        kSFront,  // ��ֹͣ��ʼǰ�� 
        kLFront,  // ����ת״̬��ʼ����ǰ��
        kRFront,  // ����ת״̬��ʼ����ǰ��

        kLeft,    // ��ת
        kRight,   // ��ת
        kStop,    // ֹͣ
        speedup,  // ����
        slowdown, // ����
    };

    opcode m_last_op; // store the last op
    /* ------------------------------------------------------------------------*/
    /**
    * @brief ����ָ���С��
    *
    * @param op ָ������
    */
    /* --------------------------------------------------------------------------*/
    void RunCar(opcode op);

    
};

