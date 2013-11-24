#pragma once
#include "cvpicctrl.h"
class CCameraCtrl : public CCvPicCtrl
{
public:
    CCameraCtrl(void);
    ~CCameraCtrl(void);

public:
    bool Init(CWnd*dlg_hwnd,int nID);
    bool OpenCam(int index = -1);
    bool CloseCam();
    bool CaptureAndShow();
    bool WriteVideo();
    bool SaveImage(const char* file_path);
    bool SaveImage(const wchar_t* file_path);
private:
    //CCvPicCtrl m_pic_ctrl;
    bool pic_ctrl_ready;

    CvCapture* m_capture;
    bool capturing;
};

