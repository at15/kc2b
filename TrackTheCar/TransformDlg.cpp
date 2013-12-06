// TransformDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "TransformDlg.h"
#include "afxdialogex.h"
using namespace std;

// CTransformDlg 对话框

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

    CTransformDlg::CTransformDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CTransformDlg::IDD, pParent)
    , m_corner_size(0)
{

}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT2, m_corner_size);
}


BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
    ON_BN_CLICKED(IDC_TRANSFORM_OPEN_IMAGE, &CTransformDlg::OnBnClickedTransformOpenImage)
    ON_BN_CLICKED(IDC_TRANSFORM_CHANGE_CORNER_SIZE, &CTransformDlg::OnBnClickedTransformChangeCornerSize)
END_MESSAGE_MAP()


// CTransformDlg 消息处理程序


BOOL CTransformDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_input_pic.Init(this,IDC_TRANSFORM_INPUT);
    m_bin_pic.Init(this,IDC_TRANSFORM_BIN);
    m_output_pic.Init(this,IDC_TRANSFORM_OUPUT);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CTransformDlg::SetMainFrame(IplImage* pSrc){
    m_input_pic.SetCurrentFrame(pSrc);
}

void CTransformDlg::process(int corner_size/* = 0*/){
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    CImageProc proc;
    IplImage* redbin = proc.GetRedBinary(m_input_pic.GetCurrentFrame(),global_configs->GetThreshold());
    m_bin_pic.SetCurrentFrame(redbin);
    // find the corners
    std::vector<CvPoint> corners = proc.FindMapCorner(redbin);
    // store it to the global var
    global_configs->SetMapCorners(corners);
    // show the transformed result
    IplImage* transformed_pic= proc.TransformImage(m_input_pic.GetCurrentFrame(),corners);
    m_output_pic.SetCurrentFrame(transformed_pic);
    cvReleaseImage(&redbin);
    cvReleaseImage(&transformed_pic);
}

void CTransformDlg::OnBnClickedTransformOpenImage()
{
    // if the main dialog has set the image to process
    if(m_input_pic.GetCurrentFrame()){
        m_input_pic.UpdateFrame();
        process();
        return;
    }

    if(m_input_pic.OpenImageEx()){
        process();
    }else{
        AfxMessageBox(_T("you didn't select file!"));
    }

}

void CTransformDlg::OnBnClickedTransformChangeCornerSize()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData();
    if(m_corner_size){

    }
}
