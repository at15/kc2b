#pragma once
#include "stdafx.h"
#include <vector>
// this is the base class for the picture control
class CCvPicCtrl
{
public:
    CCvPicCtrl(void);
    ~CCvPicCtrl(void);

public:
    virtual bool Init(CWnd*dlg_hwnd,int nID);
    virtual IplImage* SetCurrentFrame(IplImage* new_frame);
    virtual IplImage* GetCurrentFrame();
    void ShowBlack();
private:
    IplImage* m_current_frame;

    /*=========================================================*/
    /* the handles for the pic control */
    // The CWnd for the dlg, get its HWND by pic_CWnd->m_hwnd
    CWnd* pic_CWnd;
    // The rect of the pic control
    // TODO:note: there seems to be differences between the axis in opencv and Windows
    CRect pic_rect; 
    // The HDC will be used by CvvImage to show OpenCV image to the pic control
    // See MyCvvImage.h .cpp to know more... ( seems to be about fill a structure and 
    // stretch if needed )
    HDC pic_hDC;  
    // The CDC is the wrapper for HDC by MFC, use it to manager easier
    // Although it is only used in
    // Init and ShowDefault
    CDC* pic_CDC;
};

