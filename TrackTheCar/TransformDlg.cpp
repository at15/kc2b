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
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    CImageProc proc;
    IplImage* redbin = proc.GetRedBinary(m_input_pic.GetCurrentFrame(),global_configs->GetThreshold());
    m_bin_pic.SetCurrentFrame(redbin);
    // find the corners
    std::vector<CvPoint> corners = proc.FindMapCorner(redbin);
    global_configs->SetMapCorners(corners);
    // show the transformed result
    IplImage* transformed_pic= proc.TransformImage(m_input_pic.GetCurrentFrame(),corners);
    m_output_pic.SetCurrentFrame(transformed_pic);
    cvReleaseImage(&redbin);
    cvReleaseImage(&transformed_pic);
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


  /*  // for the corners
    std::vector<CvPoint> corners = proc.FindMapCorner(redbin);
    for(int i = 0;i<corners.size();i++){
        cvCircle(m_camera.GetCurrentFrame(),corners.at(i),5,CV_RGB(255,0,0),3);
    }
    m_camera.UpdateFrame();*/