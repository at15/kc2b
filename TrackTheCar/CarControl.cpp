#include "StdAfx.h"
#include "CarControl.h"

CCarControl::CCarControl(void)
{
    port_ready = false;
    m_port = NULL;
    m_speed = 4;// 初始速度是现在最慢的那个
}

CCarControl::~CCarControl(void)
{
    if(port_ready){
        // stop the fking car!
        Stop();
    }
    ClosePort();
}

bool CCarControl::Init(int com_num){
    if(m_port){
        delete m_port;
        m_port = NULL;
    }
    m_port = new CSerialPort;
     
    if(m_port->InitPort(com_num)){
        port_ready = true;
        m_last_op = kStop;

        return true;
    }else{
        port_ready = false;
        delete m_port;
        m_port = NULL;
        return false;
    }
}

void CCarControl::ClosePort(){
    if(m_port){
        delete m_port;
        m_port = NULL;
    }
}

void CCarControl::GoLeft(){
    if(!port_ready) return;
    //m_last_op = kLeft;
    //RunCar(kLeft);
    //RunCar(kSFront);
}

void CCarControl::GoRight(){
    if(!port_ready) return;
    m_last_op = kRight;
    RunCar(kRight);
    RunCar(kSFront);
}

//退了就再也无法前进了，你妹
void CCarControl::GoBack(){
    if(!port_ready) return;
    m_last_op = kBack;
    RunCar(kBack);
}

void CCarControl::Stop(){
    if(!port_ready) return;
    m_last_op = kStop;
    RunCar(kStop);
}


/*
kSFront,  // 从停止开始前进
kLFront,  // 从左转状态开始正向前进
kRFront,  // 从右转状态开始正向前进
*/
void CCarControl::GoForward(bool change_direction /*= true*/){
    if(!change_direction){
        RunCar(kSFront);
        m_last_op = kSFront;
        return;
    }

    if(kStop == m_last_op){
        RunCar(kSFront);
        m_last_op = kSFront;
        return;
    }
    if(kLeft == m_last_op){
        RunCar(kLFront);
        m_last_op = kLFront;
        return;
    }
    if(kRight == m_last_op){
        RunCar(kRFront);
        m_last_op = kRFront;
        return;
    }
    RunCar(kSFront);
    m_last_op = kSFront;
}
/*

The command string recived from PC has 6 bit(chars):
Bit 0 is '$' as start bit
Bit 1 is direction bit : forwad or backward
Bit 2 is direction bit : left or right
Bit 3 is angle value bit
Bit 4 is speed value bit
Bit 5 is change flag bit (record the changed iterm)
Bit 6 is '#' :stop bit
*/
void CCarControl::RunCar(opcode op)
{
    if(!port_ready) return;

    unsigned char* data;
    CString str;

    switch (op) {
    case kSFront:
        data =(unsigned char*)"$00001#";
        m_port->WriteData(data, 7);
        str.Format(L"$000%d4#",m_speed);
        data = (unsigned char*)EZ::CStrConv::utf162ansi(str.GetBuffer());
        //data =(unsigned char*)"$00064#";
        m_port->WriteData(data, 7);
        delete data;
        data = NULL;
        break;

    case kLFront:
        data =(unsigned char*)"$01203#";
        m_port->WriteData(data, 7);
        //CString str;
        str.Format(L"$000%d4#",m_speed);
        data = (unsigned char*)EZ::CStrConv::utf162ansi(str.GetBuffer());
        //data =(unsigned char*)"$00064#";
        m_port->WriteData(data, 7);
        delete data;
        data = NULL;
        break;

    case kRFront:
        data =(unsigned char*)"$01103#";
        m_port->WriteData(data, 7);
        //CString str;
        str.Format(L"$001%d4#",m_speed);
        data = (unsigned char*)EZ::CStrConv::utf162ansi(str.GetBuffer());
        //data =(unsigned char*)"$00164#";
        m_port->WriteData(data, 7);
        delete data;
        data = NULL;
        break;

    case kBack:
        data =(unsigned char*)"$10001#";
        m_port->WriteData(data, 7);
        break;

    case kLeft:
        data =(unsigned char*)"$00503#";
        m_port->WriteData(data, 7);
        break;

    case kRight:
        data =(unsigned char*)"$01803#";
        m_port->WriteData(data, 7);
        break;
    case kStop:
        data =(unsigned char*)"$00004#";
        m_port->WriteData(data, 7);
        break;
    /*
    case speedup:
        data =(unsigned char*)"$00094#";
        Port.WriteData(data, 7);
        break;
    case slowdown:
        data =(unsigned char*)"$00034#";
        Port.WriteData(data, 7);
        break;
        */
    }
}

bool CCarControl::SetSpeed( int new_speed )
{
    if(!IsValidSpeed(new_speed)){
        return false;
    }
    m_speed = new_speed;
    int t = m_speed;

}

bool CCarControl::SpeedUp()
{
    if(IsValidSpeed(m_speed+1)){
        SetSpeed(m_speed+1);
        return true;
    }
    return false;
}

bool CCarControl::SpeedDown()
{
    if(IsValidSpeed(m_speed-1)){
        SetSpeed(m_speed-1);
        return true;
    }
    return false;
}

bool CCarControl::IsValidSpeed( int speed )
{
    if(speed >= 3 && speed <= 9){
        return true;
    }else{
        return false;
    }
}
