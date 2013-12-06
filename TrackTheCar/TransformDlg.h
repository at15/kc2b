#pragma once
#include "CameraCtrl.h"
#include "ImageProc.h"

// CTransformDlg 对话框

class CTransformDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTransformDlg)

public:
	CTransformDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTransformDlg();

// 对话框数据
	enum { IDD = IDD_TRANSFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    CCameraCtrl m_input_pic;
    CCvPicCtrl m_bin_pic; // show the binary
    CCvPicCtrl m_output_pic;

    // set the main frame of this dialog
    void SetMainFrame(IplImage* pSrc);
    // the functions for the process
    void process(int corner_size = 0);
    afx_msg void OnBnClickedTransformOpenImage();
    afx_msg void OnBnClickedTransformChangeCornerSize();
    // the size of the corner, they are red, so need to be removed to avoid influence on the car
    int m_corner_size;
};
