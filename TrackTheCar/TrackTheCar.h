
// TrackTheCar.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "stdafx.h"
#include "Configs.h"

// CTrackTheCarApp:
// �йش����ʵ�֣������ TrackTheCar.cpp
//

class CTrackTheCarApp : public CWinApp
{
public:
	CTrackTheCarApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

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