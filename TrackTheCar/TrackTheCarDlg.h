
// TrackTheCarDlg.h : ͷ�ļ�
//

#pragma once

// include the dialogs
#include "CameraDialog.h"
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
    // this is for all the function dialogs
    CCameraDialog m_dlg_camera;
    CAboutDlg m_about;
    afx_msg void OnCapPic();
    afx_msg void OnShowAbout();
};

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

