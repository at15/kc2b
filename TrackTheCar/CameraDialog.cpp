// CameraDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "CameraDialog.h"
#include "afxdialogex.h"


// CCameraDialog 对话框

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
END_MESSAGE_MAP()


// CCameraDialog 消息处理程序
