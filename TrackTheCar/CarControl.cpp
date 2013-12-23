#include "StdAfx.h"
#include "CarControl.h"

CCarControl::CCarControl(void)
{
    port_ready = false;
    m_port = NULL;
    m_speed = CAR_SPEED;// the speed is from 3-9
}

CCarControl::~CCarControl(void)
{
    if(port_ready){
        Stop();// stop the fking car!
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
    // avoid sending the same message too fast,which will damage the car
    if(m_last_op != kLeft){
        m_last_op = kLeft;
        RunCar(kLeft);
        RunCar(kSFront);
    }
}

void CCarControl::GoRight(){
    if(!port_ready) return;
    if(m_last_op != kRight){
        m_last_op = kRight;
        RunCar(kRight);
        RunCar(kSFront);
    }
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


void CCarControl::RunCar(opcode op)
{
    if(!port_ready) return;

    unsigned char* data;
    CString str;

    switch (op) {
    case kSFront:
        // turn the car's head to front
        data =(unsigned char*)"$00001#";
        m_port->WriteData(data, 7);
        // set the speed
        str.Format(L"$000%d4#",m_speed);
        data = (unsigned char*)EZ::CStrConv::utf162ansi(str.GetBuffer());
        //data =(unsigned char*)"$00064#";
        m_port->WriteData(data, 7);
        // CStrConv will have memory leak if u don't delete the return pointer
        delete data;
        data = NULL;
        break;

    case kLFront:
        data =(unsigned char*)"$01203#";
        m_port->WriteData(data, 7);
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
    }
}