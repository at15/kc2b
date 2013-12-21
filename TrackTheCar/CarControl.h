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
    // Ĭ�ϻ�ѷ���ĵ���ǰ�����������ٶ�ʱ�ǲ��ķ����
    void GoForward(bool change_direction = true);

    void Stop();

    // ֻ�������ٶȣ����´�ָ��ʱ�Ż���Ч��
    bool SpeedUp();
    bool SpeedDown();
    // �ٶȵ�ֵ��3-9
    bool SetSpeed(int new_speed);
private:
    CSerialPort* m_port;
    bool port_ready;
    int m_speed;
    bool IsValidSpeed(int speed);

    // ���͵�ָ������
    enum opcode { 
        kSFront,  // ��ֹͣ��ʼǰ�� 
        kLFront,  // ����ת״̬��ʼ����ǰ��
        kRFront,  // ����ת״̬��ʼ����ǰ��

        kBack,	  // ����
        kLeft,    // ��ת
        kRight,   // ��ת
        kStop,    // ֹͣ
        speedup,  // ����
        slowdown, // ����
    };
    opcode m_current_op; // current op
    opcode m_last_op; // store the last op
    /* ------------------------------------------------------------------------*/
    /**
    * @brief ����ָ���С��
    *
    * @param op ָ������
    */
    /* --------------------------------------------------------------------------*/
    void RunCar(opcode op);
    void ClosePort();
};

