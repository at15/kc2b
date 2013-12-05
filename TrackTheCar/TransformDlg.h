#pragma once
#include "CameraCtrl.h"
#include "ImageProc.h"

// CTransformDlg �Ի���

class CTransformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTransformDlg)

public:
	CTransformDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTransformDlg();

// �Ի�������
	enum { IDD = IDD_TRANSFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    CCameraCtrl m_input_pic;
    CCvPicCtrl m_bin_pic; // show the binary
    CCvPicCtrl m_output_pic;

    // the functions for the process
    void process();
    afx_msg void OnBnClickedTransformOpenImage();
};
