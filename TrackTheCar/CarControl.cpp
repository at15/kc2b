#include "StdAfx.h"
#include "CarControl.h"


CCarControl::CCarControl(void)
{
}


CCarControl::~CCarControl(void)
{
}

bool CCarControl::Init(int com_num){
    m_last_op = kStop;
    if(m_port.InitPort(com_num)){
        port_ready = true;
        return true;
    }else{
        port_ready = false;
        return false;
    }

}


void CCarControl::GoLeft(){
    m_last_op = kLeft;
    RunCar(kLeft);
}

void CCarControl::GoRight(){
    m_last_op = kRight;
    RunCar(kRight);
}

// not achieved yet
void CCarControl::GoBack(){

}

void CCarControl::Stop(){
    m_last_op = kStop;
    RunCar(kStop);
}

/*
kSFront,  // 从停止开始前进 
kLFront,  // 从左转状态开始正向前进
kRFront,  // 从右转状态开始正向前进
*/
void CCarControl::GoForward(){
    if(kStop == m_last_op){
        RunCar(kSFront);
        m_last_op = kSFront;

    }
    if(kLeft == m_last_op){
        RunCar(kLFront);
        m_last_op = kLFront;
    }
    if(kRight == m_last_op){
        RunCar(kRFront);
        m_last_op = kRFront;
    }
}

void CCarControl::RunCar(opcode op) 
{
    if(!port_ready) return;

    unsigned char* data;

    switch (op) {
    case kSFront:
        data =(unsigned char*)"$00034#";
        m_port.WriteData(data, 7);
        break;

    case kLFront:
        data =(unsigned char*)"$01203#";
        m_port.WriteData(data, 7);
        data =(unsigned char*)"$00034#";
        m_port.WriteData(data, 7);
        break;

    case kRFront:
        data =(unsigned char*)"$01103#";
        m_port.WriteData(data, 7);
        data =(unsigned char*)"$00134#";
        m_port.WriteData(data, 7);
        break;


    case kLeft:
        data =(unsigned char*)"$00503#";
        m_port.WriteData(data, 7);
        break;

    case kRight:
        data =(unsigned char*)"$01803#";
        m_port.WriteData(data, 7);
        break;
    case kStop:
        data =(unsigned char*)"$00004#";
        m_port.WriteData(data, 7);
        break;
    case speedup:
        data =(unsigned char*)"$00094#";
        m_port.WriteData(data, 7);
        break;
    case slowdown:
        data =(unsigned char*)"$00034#";
        m_port.WriteData(data, 7);
        break;
    }
}