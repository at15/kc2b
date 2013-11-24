#include "StdAfx.h"
#include "CameraCtrl.h"


CCameraCtrl::CCameraCtrl(void)
{
    pic_ctrl_ready = false;
    capturing = false;
}


CCameraCtrl::~CCameraCtrl(void)
{
    // if don't do this .. the program will not exit
    if(capturing){
        cvReleaseCapture(&m_capture);
    }
}

bool CCameraCtrl::Init(CWnd*dlg_hwnd,int nID){
    // TODO:try to open the cam, if can't throw an error?
    pic_ctrl_ready = CCvPicCtrl::Init(dlg_hwnd,nID);
    return pic_ctrl_ready;
}

bool CCameraCtrl::OpenCam(int index/*=-1*/){
    if(!pic_ctrl_ready) return false;
    m_capture = cvCaptureFromCAM(index);
    if(!m_capture) return false;
    capturing = true;
    // get a frame and show
    CaptureAndShow();
    return true;
}

bool CCameraCtrl::CloseCam(){
    if(capturing){
        cvReleaseCapture(&m_capture);
        m_capture = NULL;
        capturing = false;
        return true;
    }
    return false;
}

bool CCameraCtrl::CaptureAndShow(){
    if(!capturing) return false;
    // get a frame and show
    IplImage* frame;  
    frame=cvQueryFrame(m_capture); 
    SetCurrentFrame(frame);
    return true;
}

bool CCameraCtrl::IsCapturing(){
    return capturing;
}

bool CCameraCtrl::IsWriting(){
    return writing;
}