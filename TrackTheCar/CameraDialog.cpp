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
END_MESSAGE_MAP()


// CCameraDialog ��Ϣ�������


void CCameraDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    /*if(SHOW_CAPTURE == nIDEvent){
        m_camera.CaptureAndShow();
    }
    if(WRITE_VIDEO == nIDEvent){

    }*/
    if(m_camera.IsCapturing()){
        m_camera.CaptureAndShow();
    }
    if(m_camera.IsWriting()){
       // 
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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_camera.OpenCam();
    // now set the timer
    SetTimer(SHOW_CAPTURE,20,NULL);
}

void CCameraDialog::OnBnClickedCloseCam()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_camera.CloseCam();
    m_camera.ShowBlack();
    KillTimer(SHOW_CAPTURE);
}



void CCameraDialog::OnBnClickedVideoCap()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // open a dialog to let the user select file path 
    CFileDialog video_file(FALSE,_T(".avi"),_T("test"),NULL,_T("AVI(*.avi)|*.avi|All Files(*.*)|*.*||"));
    
    if(video_file.DoModal()==IDOK){
        CString path =video_file.GetPathName();
        //path.Append(video_file.GetFileExt());
        AfxMessageBox(path);
        // TODO: why don't have the file extension?
    } 
}
