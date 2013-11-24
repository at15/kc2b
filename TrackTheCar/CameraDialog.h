#pragma once
#include "CameraCtrl.h"

// CCameraDialog 对话框

class CCameraDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDialog)

public:
	CCameraDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCameraDialog();

// 对话框数据
	enum { IDD = IDD_CAMERA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual BOOL OnInitDialog();
public:
    // the pic ctrls
    CCameraCtrl m_camera;
    afx_msg void OnBnClickedOpenCam();
};
