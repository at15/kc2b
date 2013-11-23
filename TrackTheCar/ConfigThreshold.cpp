// ConfigThreshold.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "ConfigThreshold.h"
#include "afxdialogex.h"


// CConfigThreshold 对话框

IMPLEMENT_DYNAMIC(CConfigThreshold, CDialogEx)

CConfigThreshold::CConfigThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigThreshold::IDD, pParent)
{

}

CConfigThreshold::~CConfigThreshold()
{
}

void CConfigThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigThreshold, CDialogEx)
END_MESSAGE_MAP()


// CConfigThreshold 消息处理程序
