#include "StdAfx.h"
#include "CvPicCtrl.h"


CCvPicCtrl::CCvPicCtrl(void)
{
    m_temp_image = NULL;
    m_current_frame = NULL;
}


CCvPicCtrl::~CCvPicCtrl(void)
{
    if(m_temp_image){
        delete m_temp_image;
        m_temp_image = NULL;// useless...?
    }
}

/*virtual*/ bool CCvPicCtrl::Init(CWnd*dlg_hwnd,int nID){
    // get hwnd for the pic control 
    pic_CWnd = dlg_hwnd->GetDlgItem(nID);//GetDlgItem(hdlg,nID); 
    if(!pic_CWnd) return false;

    // get the rect( size and position ) of the pic control
    pic_CWnd->GetClientRect(&pic_rect);

    // get the CDC of the pic control
    pic_CDC = pic_CWnd->GetDC();  
    // get the HDC from CDC
    pic_hDC = pic_CDC->GetSafeHdc();
    if(!pic_hDC) return false;

    return true;
}

/*virtual*/ void CCvPicCtrl::SetCurrentFrame(IplImage* new_frame,bool show /*= true*/){
    if(show){
        CvvImage m_CvvImage;  
        m_CvvImage.CopyOf(new_frame,1);     
        m_CvvImage.DrawToHDC(pic_hDC, &pic_rect);
    }

    if(m_temp_image){
        delete m_temp_image;
        m_temp_image = NULL;
    }
    m_temp_image = new CTempImage(new_frame);
    m_current_frame = m_temp_image->GetImage();
}

/*virtual*/IplImage* CCvPicCtrl::GetCurrentFrame(){
    return m_current_frame;
}

void CCvPicCtrl::UpdateFrame(){
    CvvImage m_CvvImage;  
    m_CvvImage.CopyOf(m_current_frame,1);     
    m_CvvImage.DrawToHDC(pic_hDC, &pic_rect);
}


/* virtual */ void CCvPicCtrl::DebugImage(IplImage* frame,int delay/* = 2000*/){
    cvNamedWindow("debug_image");
    cvShowImage("debug_image",frame);
    cvWaitKey(delay);
}

void CCvPicCtrl::ShowBlack(){
    // make the pic_control black
    CDC MemDC;
    CBitmap MemBmp;
    MemDC.CreateCompatibleDC(NULL);
    MemBmp.CreateCompatibleBitmap(pic_CDC,pic_rect.Width(),pic_rect.Height());
    MemDC.SelectObject(&MemBmp);
    MemDC.FillSolidRect(pic_rect,RGB(0,0,0)); 
    HDC tDC = MemDC.GetSafeHdc();
    pic_CDC->StretchBlt(pic_rect.left,pic_rect.top,pic_rect.Width(),pic_rect.Height(),&MemDC,0,0,48,48,SRCCOPY);
    // clean up
    MemBmp.DeleteObject();
    MemDC.DeleteDC();
}