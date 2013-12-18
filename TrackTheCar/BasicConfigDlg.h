#pragma once


// CBasicConfigDlg 对话框

class CBasicConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBasicConfigDlg)

public:
	CBasicConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBasicConfigDlg();

// 对话框数据
	enum { IDD = IDD_BASIC_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBasicConfigSave();
    // 允许的距离误差（PS 地图越小这个值就越大)
    int m_distance_error;
    // 转角误差
    int m_angle_error;
    // 单次操作，如右拐最多连续重复次数。（超过它就要开始自动修正）
    int m_max_op_time;
    // 每次自动修正的值（PS 角度和距离是同时增加一样的值，偷个懒)
    int m_error_modify_value;
    // 最大自动修正次数（如果误差允许的跟半个地图一样大，那也太**了）
    int m_max_modify_time;
};
