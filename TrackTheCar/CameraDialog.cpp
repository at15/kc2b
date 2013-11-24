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
END_MESSAGE_MAP()


// CCameraDialog ��Ϣ�������


void CCameraDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
}
