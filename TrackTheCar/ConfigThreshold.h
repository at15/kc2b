#pragma once


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
};
