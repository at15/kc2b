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
    , m_corner_size_width(0)
    , m_corner_size_height(0)
{

}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT2, m_corner_size_width);
    DDX_Text(pDX, IDC_EDIT10, m_corner_size_height);
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
    // load the default 
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    m_corner_size_width = g_configs->map_corner_width.Get();
    m_corner_size_height = g_configs->map_corner_height.Get();
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CTransformDlg::SetMainFrame(IplImage* pSrc){
    m_input_pic.SetCurrentFrame(pSrc);
}

void CTransformDlg::process(int corner_size/* = 0*/){
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    CImageProc proc;
    IplImage* redbin = proc.GetRedBinary(m_input_pic.GetCurrentFrame(),g_configs->red_threshold.Get());
    m_bin_pic.SetCurrentFrame(redbin);
    // find the corners
    std::vector<CvPoint> corners = proc.FindMapCorner(redbin,g_configs->map_corner_width.Get(),
        g_configs->map_corner_height.Get());
    // store it to the global var
    g_configs->map_corner.Set(corners);
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
    UpdateData();
    if(m_corner_size_width && m_corner_size_height){
         CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
         g_configs->map_corner_width.Set(m_corner_size_width);
         g_configs->map_corner_height.Set(m_corner_size_height);
         process();
    }
}
