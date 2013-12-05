// TransformDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "TransformDlg.h"
#include "afxdialogex.h"
using namespace std;

// CTransformDlg �Ի���

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTransformDlg::IDD, pParent)
{

}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
    ON_BN_CLICKED(IDC_TRANSFORM_OPEN_IMAGE, &CTransformDlg::OnBnClickedTransformOpenImage)
END_MESSAGE_MAP()


// CTransformDlg ��Ϣ�������


BOOL CTransformDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_input_pic.Init(this,IDC_TRANSFORM_INPUT);
    m_bin_pic.Init(this,IDC_TRANSFORM_BIN);
    m_output_pic.Init(this,IDC_TRANSFORM_OUPUT);
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CTransformDlg::process(){
   // CTrackTheCarApp theApp = AfxGetApp();
   // ((CPWDMgrNewApp*)AfxGetApp())->m_appdata_dir;
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    CImageProc proc;
    IplImage* redbin = proc.GetRedBinary(m_input_pic.GetCurrentFrame(),global_configs->GetThreshold());
    m_bin_pic.SetCurrentFrame(redbin);
    // find the corners
    std::vector<CvPoint> corners = proc.FindMapCorner(redbin);
    global_configs->SetMapCorners(corners);
    // show the transformed result
    cvReleaseImage(&redbin);
}

void CTransformDlg::OnBnClickedTransformOpenImage()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(m_input_pic.OpenImageEx()){
        process();
    }else{
        AfxMessageBox(_T("you didn't select file!"));
    }
    
}
