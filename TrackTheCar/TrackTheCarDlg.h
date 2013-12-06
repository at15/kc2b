
// TrackTheCarDlg.h : ͷ�ļ�
//

#pragma once

// include the dialogs
#include "CameraDialog.h"
#include "ConfigThreshold.h"
#include "TransformDlg.h"
#include "afxwin.h"

// The about dialog
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

// CTrackTheCarDlg �Ի���
class CTrackTheCarDlg : public CDialogEx
{
// ����
public:
	CTrackTheCarDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TRACKTHECAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    // the about dialog... which is kind of useless
    CAboutDlg m_about;
    // this is for all the function dialogs
    CCameraDialog m_dlg_camera;
    CConfigThreshold m_dlg_threshold;
    CTransformDlg m_dlg_transform;

    // this for the main picture control 
    CCameraCtrl m_main_input;

    // This is for update the image in the main control after all the config setting
    // example: transformed the image
    void ApplyConfig();
    void RestConfig();

    void process_input();

    afx_msg void OnCapPic();
    afx_msg void OnShowAbout();
    afx_msg void OnConfigThreshold();
    afx_msg void OnConfigTransform();
    // the console to show messages like chage the threshold and transform etc
    CEdit m_main_console;
    // func for add message to the console
    void AddToConsole(const CString& str);
    afx_msg void OnMainOpenImage();
private:
    bool use_config;
};
