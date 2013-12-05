#pragma once
#include "afxcmn.h"
#include "CvPicCtrl.h"
#include "CameraCtrl.h"

#define CONFIG_USE_CAM 1
#define CONFIG_USE_IMAGE 2
// CConfigThreshold 对话框

class CConfigThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigThreshold)

public:
	CConfigThreshold(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigThreshold();

// 对话框数据
	enum { IDD = IDD_THRESHOLD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CCameraCtrl m_camera;
    //CImageProc m_proc;

    CCvPicCtrl m_h;
    CCvPicCtrl m_s;
    CCvPicCtrl m_v;
    CCvPicCtrl m_b;
    CCvPicCtrl m_g;
    CCvPicCtrl m_r;
    std::vector<CCvPicCtrl*> m_hsvbgr_ctrls;

    CCvPicCtrl m_red_bin;
    CCvPicCtrl m_blue_bin;

    CSliderCtrl m_slider_h;
    CSliderCtrl m_slider_s;
    CSliderCtrl m_slider_v;
    CSliderCtrl m_slider_b;
    CSliderCtrl m_slider_g;
    CSliderCtrl m_slider_r;
    std::vector<CSliderCtrl*> m_sliders;
    std::vector<int> m_threshold;// in h,s,v,b,g,r order

    // set the threshold from the slider

    void BasicProc();// the only difference between cam image and video is the 
    // frame on the main picture control... so it is ok now
    void CamProc();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedConfigOpenCam();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedConfigOpenImage();
};
