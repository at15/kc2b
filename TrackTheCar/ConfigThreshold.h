#pragma once


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
};
