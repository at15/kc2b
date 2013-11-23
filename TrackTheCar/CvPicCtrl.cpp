#include "StdAfx.h"
#include "CvPicCtrl.h"


CCvPicCtrl::CCvPicCtrl(void)
{
}


CCvPicCtrl::~CCvPicCtrl(void)
{
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

/*virtual*/ IplImage* CCvPicCtrl::SetCurrentFrame(IplImage* new_frame){
    CvvImage m_CvvImage;  
    m_CvvImage.CopyOf(new_frame,1);     
    m_CvvImage.DrawToHDC(pic_hDC, &pic_rect);
    IplImage* t = m_current_frame;
    m_current_frame = new_frame;
    return t;
}

/*virtual*/IplImage* CCvPicCtrl::GetCurrentFrame(){
    return m_current_frame;
}

void CCvPicCtrl::ShowBlack(){
    // make the pic_control black
    CDC MemDC;
    MemDC.CreateCompatibleDC(NULL);  
    MemDC.FillSolidRect(pic_rect,RGB(0,0,0)); 
    HDC tDC = MemDC.GetSafeHdc();
    pic_CDC->StretchBlt(pic_rect.left,pic_rect.top,pic_rect.Width(),pic_rect.Height(),&MemDC,0,0,48,48,SRCCOPY);
}
