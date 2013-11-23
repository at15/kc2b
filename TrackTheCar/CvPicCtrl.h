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
    virtual bool SetCurrentFrame(IplImage* new_frame);
    virtual IplImage* GetCurrentFrame();
};

