#include "StdAfx.h"
#include "CarControl.h"

CCarControl::CCarControl(void)
{
    port_ready = false;
    m_port = NULL;
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
    m_last_op = kLeft;
    RunCar(kLeft);
    RunCar(kSFront);
}

void CCarControl::GoRight(){
    m_last_op = kRight;
    RunCar(kRight);
    RunCar(kSFront);
}

// not achieved yet
void CCarControl::GoBack(){
}

void CCarControl::Stop(){
    m_last_op = kStop;
    RunCar(kStop);
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

/*
kSFront,  // 从停止开始前进
kLFront,  // 从左转状态开始正向前进
kRFront,  // 从右转状态开始正向前进
*/
void CCarControl::GoForward(){
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

void CCarControl::RunCar(opcode op)
{
    if(!port_ready) return;

    unsigned char* data;

    switch (op) {
    case kSFront:
        data =(unsigned char*)"$00034#";
        m_port->WriteData(data, 7);
        break;

    case kLFront:
        data =(unsigned char*)"$01203#";
        m_port->WriteData(data, 7);
        data =(unsigned char*)"$00034#";
        m_port->WriteData(data, 7);
        break;

    case kRFront:
        data =(unsigned char*)"$01103#";
        m_port->WriteData(data, 7);
        data =(unsigned char*)"$00134#";
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
    case speedup:
        data =(unsigned char*)"$00094#";
        m_port->WriteData(data, 7);
        break;
    case slowdown:
        data =(unsigned char*)"$00034#";
        m_port->WriteData(data, 7);
        break;
    }
}