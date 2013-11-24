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
    if(m_camera.IsCapturing()){
        m_camera.CaptureAndShow();
    }
    if(m_camera.IsWriting()){
        m_camera.WriteVideo();
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
    // open a dialog to let the user select file path 
    CFileDialog video_file(FALSE,_T(".avi"),_T("test"),NULL,_T("AVI(*.avi)|*.avi|All Files(*.*)|*.*||"));
    CString path;
    if(video_file.DoModal()==IDOK){
        path =video_file.GetPathName();
        //AfxMessageBox(path);
    }
    if(path.IsEmpty()) {
        AfxMessageBox(_T("didn't specify file path!"));
    }
    // start capturing video
    EZ::CStrConv sconv;
    char* cpath = sconv.utf162utf8(path.GetBuffer());
    m_camera.InitVideo(cpath);
    SetTimer(WRITE_VIDEO,20,NULL);
}


void CCameraDialog::OnBnClickedVideoCapStop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_camera.CloseVideo();
    KillTimer(WRITE_VIDEO);
}
