
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
    //CConfigs global_configs;
    CGConfigs g_configs;
};

extern CTrackTheCarApp theApp;