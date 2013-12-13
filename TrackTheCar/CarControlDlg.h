#pragma once
#include "CarControl.h"

// CarControlDlg �Ի���

class CarControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CarControlDlg)

public:
	CarControlDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CarControlDlg();

// �Ի�������
	enum { IDD = IDD_CAR_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
