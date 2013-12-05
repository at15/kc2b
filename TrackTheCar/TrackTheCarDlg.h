
// TrackTheCarDlg.h : 头文件
//

#pragma once

// include the dialogs
#include "CameraDialog.h"
#include "ConfigThreshold.h"
#include "TransformDlg.h";

// The about dialog
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

// CTrackTheCarDlg 对话框
class CTrackTheCarDlg : public CDialogEx
{
// 构造
public:
	CTrackTheCarDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRACKTHECAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    // 
    CAboutDlg m_about;
    // this is for all the function dialogs
    CCameraDialog m_dlg_camera;
    CConfigThreshold m_dlg_threshold;
    CTransformDlg m_dlg_transform;

    afx_msg void OnCapPic();
    afx_msg void OnShowAbout();
    afx_msg void OnConfigThreshold();
    afx_msg void OnConfigTransform();
};
