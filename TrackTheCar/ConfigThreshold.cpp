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
    DDX_Control(pDX, IDC_SLIDER_H, m_slider_h);
    DDX_Control(pDX, IDC_SLIDER_S, m_slider_s);
    DDX_Control(pDX, IDC_SLIDER_V, m_slider_v);
    DDX_Control(pDX, IDC_SLIDER_B, m_slider_b);
    DDX_Control(pDX, IDC_SLIDER_G, m_slider_g);
    DDX_Control(pDX, IDC_SLIDER_R, m_slider_r);
}


BEGIN_MESSAGE_MAP(CConfigThreshold, CDialogEx)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_CONFIG_OPEN_CAM, &CConfigThreshold::OnBnClickedConfigOpenCam)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_CONFIG_OPEN_IMAGE, &CConfigThreshold::OnBnClickedConfigOpenImage)
END_MESSAGE_MAP()


// CConfigThreshold 消息处理程序


void CConfigThreshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // set the value
    for(int i=0;i<6;i++){
        m_threshold.at(i) =  m_sliders.at(i)->GetPos();
    }
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CConfigThreshold::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_camera.Init(this,IDC_PIC_MAIN);
    m_red_bin.Init(this,IDC_PIC_BINARY_RED);
    m_blue_bin.Init(this,IDC_PIC_BINARY_BLUE);

    m_h.Init(this,IDC_PIC_H);
    m_hsvbgr_ctrls.push_back(&m_h);
    m_slider_h.SetRange(0,255);
    //m_slider_h.SetPos(10);
    m_sliders.push_back(&m_slider_h);

    m_s.Init(this,IDC_PIC_S);
    m_hsvbgr_ctrls.push_back(&m_s);
    m_slider_s.SetRange(0,255);
    //m_slider_s.SetPos(10);
    m_sliders.push_back(&m_slider_s);

    m_v.Init(this,IDC_PIC_V);
    m_hsvbgr_ctrls.push_back(&m_v);
    m_slider_v.SetRange(0,255);
    //m_slider_v.SetPos(10);
    m_sliders.push_back(&m_slider_v);

    m_b.Init(this,IDC_PIC_B);
    m_hsvbgr_ctrls.push_back(&m_b);
    m_slider_b.SetRange(0,255);
    //m_slider_b.SetPos(10);
    m_sliders.push_back(&m_slider_b);

    m_g.Init(this,IDC_PIC_G);
    m_hsvbgr_ctrls.push_back(&m_g);
    m_slider_g.SetRange(0,255);
    //m_slider_g.SetPos(10);
    m_sliders.push_back(&m_slider_g);

    m_r.Init(this,IDC_PIC_R);
    m_hsvbgr_ctrls.push_back(&m_r);
    m_slider_r.SetRange(0,255);
    //m_slider_r.SetPos(10);
    m_sliders.push_back(&m_slider_r);

    // init the threshold value
    m_threshold.push_back(150);
    m_threshold.push_back(138);
    m_threshold.push_back(40);
    m_threshold.push_back(100);
    m_threshold.push_back(80);
    m_threshold.push_back(80);
    for(int i=0;i<6;i++){
        m_sliders.at(i)->SetPos(m_threshold.at(i));
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


void CConfigThreshold::OnBnClickedConfigOpenCam()
{
    // TODO: 在此添加控件通知处理程序代码
    m_camera.OpenCam();
    SetTimer(CONFIG_USE_CAM,20,NULL);
}


void CConfigThreshold::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if( CONFIG_USE_CAM == nIDEvent) CamProc();
    if( CONFIG_USE_IMAGE == nIDEvent) BasicProc();

    CDialogEx::OnTimer(nIDEvent);
}

void CConfigThreshold::CamProc(){
   m_camera.CaptureAndShow();
   BasicProc();
}

void CConfigThreshold::BasicProc(){
    
    CImageProc proc;
    // show h,s,v,b,g,r
    std::vector<IplImage*> hsvbgr = proc.GetHSVBGR(m_camera.GetCurrentFrame());
    std::vector<IplImage*> hsvbgrBin;
    for(int i=0;i<6;i++){
        IplImage* t_bin = proc.GetBinary(hsvbgr.at(i),m_threshold.at(i));
        hsvbgrBin.push_back(t_bin);
        m_hsvbgr_ctrls.at(i)->SetCurrentFrame(t_bin);
    }
    IplImage* redbin = proc.GetRedBinary(hsvbgrBin);
    m_red_bin.SetCurrentFrame(redbin);
  /*  // for the corners
    std::vector<CvPoint> corners = proc.FindMapCorner(redbin);
    for(int i = 0;i<corners.size();i++){
        cvCircle(m_camera.GetCurrentFrame(),corners.at(i),5,CV_RGB(255,0,0),3);
    }
    m_camera.UpdateFrame();*/
    IplImage* bluebin = proc.GetBlueBinary(hsvbgrBin);
    m_blue_bin.SetCurrentFrame(bluebin);
    cvReleaseImage(&redbin);
    cvReleaseImage(&bluebin);
    proc.releaseHSVBGR(hsvbgrBin);
    proc.releaseHSVBGR(hsvbgr);
}

void CConfigThreshold::OnBnClickedConfigOpenImage()
{
    // TODO: 在此添加控件通知处理程序代码
    m_camera.OpenImageEx();
    CImageProc proc;
    proc.DrawMiddleCircle(m_camera.GetCurrentFrame());// i suppose this won't work
    m_camera.UpdateFrame();// will this work?
    BasicProc();
    //SetTimer(CONFIG_USE_IMAGE,20,NULL);
}