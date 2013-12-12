#pragma once
#include "cvpicctrl.h"
class CCameraCtrl : public CCvPicCtrl
{
public:
    CCameraCtrl(void);
    ~CCameraCtrl(void);

public:
    // only use one of these two functions
    // 1. CaptureAndShow()
    // 2. WriteAndShow()

    bool Init(CWnd*dlg_hwnd,int nID);

    bool OpenCam(int index = -1);
    bool CloseCam();
    bool CaptureAndShow();
    bool CaptureDontShow();

    bool InitVideo(const char* file_path,int fps = 25);
    bool InitVideoEx();// show the dialog 
    bool WriteAndShow();
    bool CloseVideo();

    bool OpenImage(const char* file_path);
    bool OpenImageEx();// show the dialog to open file
    bool OpenImageEx(CString& opath);// save the path in opath
    bool SaveImage(const char* file_path);
    bool SaveImageEx();
    bool SaveImageEx(CString& opath);// show the dialog

    void Pause(bool stop = true);// stop cap and writing 

    bool IsCapturing();
    bool IsWriting();
    bool IsPause(){return m_pause;}
private:
    //CCvPicCtrl m_pic_ctrl;
    bool pic_ctrl_ready;
    CvCapture* m_capture;
    bool capturing;

    // the data for video capturing
    bool writing;
    CvVideoWriter* m_video;
    CString m_video_path;
    int m_video_width;
    int m_video_height;
    int m_video_fps;

    // control the capture
    bool m_pause;
};