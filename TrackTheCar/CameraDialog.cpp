// CameraDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "CameraDialog.h"
#include "afxdialogex.h"


// CCameraDialog �Ի���

IMPLEMENT_DYNAMIC(CCameraDialog, CDialogEx)

    CCameraDialog::CCameraDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(CCameraDialog::IDD, pParent)
{

}

CCameraDialog::~CCameraDialog()
{
}

void CCameraDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraDialog, CDialogEx)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_OPEN_CAM, &CCameraDialog::OnBnClickedOpenCam)
    ON_BN_CLICKED(IDC_CLOSE_CAM, &CCameraDialog::OnBnClickedCloseCam)
    ON_BN_CLICKED(IDC_VIDEO_CAP, &CCameraDialog::OnBnClickedVideoCap)
    ON_BN_CLICKED(IDC_VIDEO_CAP_STOP, &CCameraDialog::OnBnClickedVideoCapStop)
END_MESSAGE_MAP()


// CCameraDialog ��Ϣ�������


void CCameraDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: this seems to have problem ..., will lack frame 
    if(m_camera.IsCapturing()&& !m_camera.IsWriting()){
        m_camera.CaptureAndShow();
    }
    if(m_camera.IsWriting()){
        m_camera.WriteAndShow();
    }
    CDialogEx::OnTimer(nIDEvent);
}


BOOL CCameraDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_camera.Init(this,IDC_SHOW_IMAGE); // should pass the pic ctrls id...
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}


void CCameraDialog::OnBnClickedOpenCam()
{
    m_camera.OpenCam();
    // now set the timer
    SetTimer(SHOW_CAPTURE,20,NULL);
}

void CCameraDialog::OnBnClickedCloseCam()
{   
    m_camera.CloseCam();
    m_camera.ShowBlack();
    KillTimer(SHOW_CAPTURE);
}

void CCameraDialog::OnBnClickedVideoCap()
{
    if(m_camera.InitVideoEx()){
        SetTimer(WRITE_VIDEO,20,NULL);
    }else{
        AfxMessageBox(_T("can't init video"));
    }
}


void CCameraDialog::OnBnClickedVideoCapStop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_camera.CloseVideo();
    KillTimer(WRITE_VIDEO);
}
