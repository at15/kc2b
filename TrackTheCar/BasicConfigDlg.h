#pragma once


// CBasicConfigDlg �Ի���

class CBasicConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBasicConfigDlg)

public:
	CBasicConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBasicConfigDlg();

// �Ի�������
	enum { IDD = IDD_BASIC_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBasicConfigSave();
    // ����ľ�����PS ��ͼԽС���ֵ��Խ��)
    int m_distance_error;
    // ת�����
    int m_angle_error;
    // ���β��������ҹ���������ظ�����������������Ҫ��ʼ�Զ�������
    int m_max_op_time;
    // ÿ���Զ�������ֵ��PS �ǶȺ;�����ͬʱ����һ����ֵ��͵����)
    int m_error_modify_value;
    // ����Զ���������������������ĸ������ͼһ������Ҳ̫**�ˣ�
    int m_max_modify_time;
};
