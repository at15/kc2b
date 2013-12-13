#pragma once
#include "CarControl.h"

// CarControlDlg 对话框

class CarControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CarControlDlg)

public:
	CarControlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CarControlDlg();

// 对话框数据
	enum { IDD = IDD_CAR_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedInitCar();
    afx_msg void OnBnClickedStop();
    // the num of the com 
    int m_com_num;

    CCarControl m_car_ctrl;
    afx_msg void OnBnClickedForward();
    afx_msg void OnBnClickedLeft();
    afx_msg void OnBnClickedRight();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
};
