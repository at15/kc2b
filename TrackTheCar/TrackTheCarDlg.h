
// TrackTheCarDlg.h : ͷ�ļ�
//

#pragma once

// include the dialogs
#include "CameraDialog.h"

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
    afx_msg void OnCapPic();
};
