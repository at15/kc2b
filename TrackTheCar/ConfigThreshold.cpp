// ConfigThreshold.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "ConfigThreshold.h"
#include "afxdialogex.h"


// CConfigThreshold �Ի���

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
    DDX_Control(pDX, IDC_SLIDER_H, m_slider_h);
    DDX_Control(pDX, IDC_SLIDER_S, m_slider_s);
    DDX_Control(pDX, IDC_SLIDER_V, m_slider_v);
    DDX_Control(pDX, IDC_SLIDER_B, m_slider_b);
    DDX_Control(pDX, IDC_SLIDER_G, m_slider_g);
    DDX_Control(pDX, IDC_SLIDER_R, m_slider_r);
}


BEGIN_MESSAGE_MAP(CConfigThreshold, CDialogEx)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CConfigThreshold ��Ϣ�������


void CConfigThreshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    
    // set the value
    for(int i=0;i<6;i++){
       m_threshold.at(i) =  m_sliders.at(i)->GetPos();
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CConfigThreshold::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_slider_r.SetRange(0,255);
    m_slider_r.SetPos(10);
    m_sliders.push_back(&m_slider_r);
    m_slider_g.SetRange(0,255);
    m_slider_g.SetPos(10);
    m_sliders.push_back(&m_slider_g);
    m_slider_b.SetRange(0,255);
    m_slider_b.SetPos(10);
    m_sliders.push_back(&m_slider_b);
    m_slider_h.SetRange(0,255);
    m_slider_h.SetPos(10);
    m_sliders.push_back(&m_slider_h);
    m_slider_s.SetRange(0,255);
    m_slider_s.SetPos(10);
    m_sliders.push_back(&m_slider_s);
    m_slider_v.SetRange(0,255);
    m_slider_v.SetPos(10);
    m_sliders.push_back(&m_slider_v);

    // init the threshold value
    for(int i = 0;i<6;i++){
        
        m_threshold.push_back(0);
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
