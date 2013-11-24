#include "StdAfx.h"
#include "CameraCtrl.h"


CCameraCtrl::CCameraCtrl(void)
{
    pic_ctrl_ready = false;
    capturing = false;
    writing = false;
}


CCameraCtrl::~CCameraCtrl(void)
{
    // if don't do this .. the program will not exit
    if(capturing){
        cvReleaseCapture(&m_capture);
    }
    if(writing){
        cvReleaseVideoWriter(&m_video);
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

bool CCameraCtrl::InitVideo(const char* file_path,int fps  /*=25*/){
    if(!capturing){
        // start the cam if not started
        if(!OpenCam()) return false;
    }
    m_video = NULL;
    IplImage* frame=cvQueryFrame(m_capture); //get one frame to get the info
    m_video_width = frame->width;
    m_video_height = frame->height;
    m_video_fps = fps;
    m_video=cvCreateVideoWriter(file_path, CV_FOURCC('X', 'V', 'I', 'D'), fps,
        cvSize(m_video_width,m_video_height));
    if(!m_video)
    {
        // can't create video writer object
        // video = NULL;
        //width = 0;
        // height = 0;
        return false;
    }
    // set the path, may need it in the future
    m_video_path = file_path;
    writing = true;
    return true;
}

bool CCameraCtrl::InitVideoEx(){
    // open a dialog to let the user select file path 
    CFileDialog video_file(FALSE,_T(".avi"),_T("test"),NULL,_T("AVI(*.avi)|*.avi|All Files(*.*)|*.*||"));
    CString path;
    if(video_file.DoModal()==IDOK){
        path =video_file.GetPathName();
    }
    if(path.IsEmpty()) {
        AfxMessageBox(_T("didn't specify file path!"));
        return false;
    }
    EZ::CStrConv sconv;
    char* cpath = sconv.utf162utf8(path.GetBuffer());
    return InitVideo(cpath);
}

bool CCameraCtrl::WriteAndShow(){
    if(!writing){
        return false;
    }
    IplImage*  frame=cvQueryFrame(m_capture);
    SetCurrentFrame(frame);// show it
    int n = cvWriteFrame(m_video,frame);
    if(n == 1){
        return true;
    }
    return false;
}

/*
bool CCameraCtrl::WriteVideo(){
    if(!writing){
        return false;
    }
    IplImage*  frame=cvQueryFrame(m_capture);
    int n = cvWriteFrame(m_video,frame);
    if(n == 1){
        return true;
    }
    return false;
}*/

bool CCameraCtrl::CloseVideo(){
    if(capturing){
        cvReleaseVideoWriter(&m_video);
        m_video = NULL;
        capturing = false;
        return true;
    }
    return false;
}

bool CCameraCtrl::IsCapturing(){
    return capturing;
}

bool CCameraCtrl::IsWriting(){
    return writing;
}