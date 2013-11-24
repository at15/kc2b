#pragma once
#include "CameraCtrl.h"

// CCameraDialog �Ի���

class CCameraDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraDialog)

public:
	CCameraDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCameraDialog();

// �Ի�������
	enum { IDD = IDD_CAMERA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual BOOL OnInitDialog();
public:
    // the pic ctrls
    CCameraCtrl m_camera;
    afx_msg void OnBnClickedOpenCam();
};
