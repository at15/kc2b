// BasicConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "BasicConfigDlg.h"
#include "afxdialogex.h"


// CBasicConfigDlg 对话框

IMPLEMENT_DYNAMIC(CBasicConfigDlg, CDialogEx)

CBasicConfigDlg::CBasicConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBasicConfigDlg::IDD, pParent)
    , m_distance_error(0)
    , m_angle_error(0)
    , m_max_op_time(0)
    , m_error_modify_value(0)
    , m_max_modify_time(0)
{

}

CBasicConfigDlg::~CBasicConfigDlg()
{
}

void CBasicConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_distance_error);
    DDX_Text(pDX, IDC_EDIT4, m_angle_error);
    DDV_MinMaxInt(pDX, m_angle_error, 0, 180);
    DDX_Text(pDX, IDC_EDIT3, m_max_op_time);
    DDX_Text(pDX, IDC_EDIT6, m_error_modify_value);
    DDX_Text(pDX, IDC_EDIT7, m_max_modify_time);
}


BEGIN_MESSAGE_MAP(CBasicConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BASIC_CONFIG_SAVE, &CBasicConfigDlg::OnBnClickedBasicConfigSave)
END_MESSAGE_MAP()


// CBasicConfigDlg 消息处理程序


void CBasicConfigDlg::OnBnClickedBasicConfigSave()
{
    // TODO: 在此添加控件通知处理程序代码
}
