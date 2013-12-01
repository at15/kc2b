#pragma once
#include "afxcmn.h"


// CConfigThreshold �Ի���

class CConfigThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigThreshold)

public:
	CConfigThreshold(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigThreshold();

// �Ի�������
	enum { IDD = IDD_THRESHOLD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CSliderCtrl m_slider_h;
    CSliderCtrl m_slider_s;
    CSliderCtrl m_slider_v;
    CSliderCtrl m_slider_b;
    CSliderCtrl m_slider_g;
    CSliderCtrl m_slider_r;
    std::vector<CSliderCtrl*> m_sliders;
    std::vector<int> m_threshold;// in h,s,v,b,g,r order
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    virtual BOOL OnInitDialog();
};
