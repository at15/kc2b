
// TrackTheCar.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "stdafx.h"
#include "Configs.h"

// CTrackTheCarApp:
// 有关此类的实现，请参阅 TrackTheCar.cpp
//

class CTrackTheCarApp : public CWinApp
{
public:
	CTrackTheCarApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
    /*
    // the global vars store in the theApp;
    struct opencv_config{
        std::vector<int> threshold;
        std::vector<int> map_corners;
    }; just put it in another file
    */
    CConfigs global_configs;
};

extern CTrackTheCarApp theApp;