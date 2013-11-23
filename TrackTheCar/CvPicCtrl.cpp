#include "StdAfx.h"
#include "CvPicCtrl.h"


CCvPicCtrl::CCvPicCtrl(void)
{
}


CCvPicCtrl::~CCvPicCtrl(void)
{
}

/*virtual*/ bool CCvPicCtrl::Init(CWnd*dlg_hwnd,int nID){
    return true;
}

/*virtual*/ bool CCvPicCtrl::SetCurrentFrame(IplImage* new_frame){
    return true;
}

/*virtual*/IplImage* CCvPicCtrl::GetCurrentFrame(){
    return NULL;
}
