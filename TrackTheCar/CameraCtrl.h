#pragma once
#include "cvpicctrl.h"
class CCameraCtrl
{
public:
    CCameraCtrl(void);
    ~CCameraCtrl(void);

public:
    bool Init(CWnd*dlg_hwnd,int nID);
    bool OpenCam(int index = -1);
    bool CaptureAndShow();
private:
    CCvPicCtrl m_pic_ctrl;
    bool pic_ctrl_ready;

    CvCapture* m_capture;
    bool capturing;
};

