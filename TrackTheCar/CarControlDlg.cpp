// CarControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "CarControlDlg.h"
#include "afxdialogex.h"


// CarControlDlg 对话框

IMPLEMENT_DYNAMIC(CarControlDlg, CDialogEx)

    CarControlDlg::CarControlDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CarControlDlg::IDD, pParent)
    , m_com_num(0)
{

}

CarControlDlg::~CarControlDlg()
{
}

void CarControlDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_com_num);
}


BEGIN_MESSAGE_MAP(CarControlDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON4, &CarControlDlg::OnBnClickedInitCar)
    ON_BN_CLICKED(IDC_BUTTON6, &CarControlDlg::OnBnClickedStop)
    ON_BN_CLICKED(IDC_BUTTON1, &CarControlDlg::OnBnClickedForward)
    ON_BN_CLICKED(IDC_BUTTON3, &CarControlDlg::OnBnClickedLeft)
    ON_BN_CLICKED(IDC_BUTTON5, &CarControlDlg::OnBnClickedRight)
END_MESSAGE_MAP()


// CarControlDlg 消息处理程序


void CarControlDlg::OnBnClickedInitCar()
{
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    UpdateData();
    if(!m_com_num){
        AfxMessageBox(_T('no com num!!'));
    }else{
        if(m_car_ctrl.Init(m_com_num)){
            AfxMessageBox(_T("init success!"));
            global_configs->SetCOM(m_com_num);
        }else{
            AfxMessageBox(_T("init fail!"));
        }
    }
}


void CarControlDlg::OnBnClickedStop()
{
    m_car_ctrl.Stop();
}


void CarControlDlg::OnBnClickedForward()
{
    // TODO: 在此添加控件通知处理程序代码
    m_car_ctrl.GoForward();
}


void CarControlDlg::OnBnClickedLeft()
{
    // TODO: 在此添加控件通知处理程序代码
    m_car_ctrl.GoLeft();
}


void CarControlDlg::OnBnClickedRight()
{
    // TODO: 在此添加控件通知处理程序代码
    m_car_ctrl.GoRight();
}


BOOL CarControlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    m_com_num = global_configs->GetCOM();
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
