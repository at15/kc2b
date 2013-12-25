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
    current_mode = RED_MODE;// default mode is set thershold for red
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
    ON_BN_CLICKED(IDC_RADIO1, &CConfigThreshold::OnBnClickedCkRed)
    ON_BN_CLICKED(IDC_RADIO2, &CConfigThreshold::OnBnClickedCkBlue)
END_MESSAGE_MAP()


// CConfigThreshold 消息处理程序





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
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    int t;
    for(int i=0;i<6;i++){
        t = g_configs->default_threshold.Get().at(i);
        m_sliders.at(i)->SetPos(t);
    }
    m_red_threshold = g_configs->default_threshold.Get();
    m_blue_threshold = g_configs->default_threshold.Get();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CConfigThreshold::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if( CONFIG_USE_CAM == nIDEvent) CamProc();
    if( CONFIG_USE_IMAGE == nIDEvent) BasicProc();

    CDialogEx::OnTimer(nIDEvent);
}

void CConfigThreshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SetThreshold();
    BasicProc();
}

// set the main frame of this dialog
void CConfigThreshold::SetMainFrame(IplImage* pSrc){
    m_camera.SetCurrentFrame(pSrc);
}

void CConfigThreshold::SetThreshold(){
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;

    // get the threshold form the sliders
    if(RED_MODE == current_mode){
        for(int i=0;i<6;i++){
            m_red_threshold.at(i) = m_sliders.at(i)->GetPos();
        }
    }
    
    if(BLUE_MODE == current_mode){
        for(int i=0;i<6;i++){
            m_blue_threshold.at(i) = m_sliders.at(i)->GetPos();
        }
    }
    
    // set the global
    g_configs->red_threshold.Set(m_red_threshold);
    g_configs->blue_threshold.Set(m_blue_threshold);

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
    
    if(RED_MODE == current_mode){
        for(int i=0;i<6;i++){
            IplImage* t_bin = proc.GetBinary(hsvbgr.at(i),m_red_threshold.at(i));
            hsvbgrBin.push_back(t_bin);
            m_hsvbgr_ctrls.at(i)->SetCurrentFrame(t_bin);
        }
        IplImage* redbin = proc.GetRedBinary(hsvbgrBin);
        m_red_bin.SetCurrentFrame(redbin);
        cvReleaseImage(&redbin);
    }
    
    if(BLUE_MODE == current_mode){
        for(int i=0;i<6;i++){
            IplImage* t_bin = proc.GetBinary(hsvbgr.at(i),m_blue_threshold.at(i));
            hsvbgrBin.push_back(t_bin);
            m_hsvbgr_ctrls.at(i)->SetCurrentFrame(t_bin);
        }
        IplImage* bluebin = proc.GetBlueBinary(hsvbgrBin);
        m_blue_bin.SetCurrentFrame(bluebin);
        cvReleaseImage(&bluebin);
    }
    
    proc.releaseHSVBGR(hsvbgrBin);
    proc.releaseHSVBGR(hsvbgr);
}

void CConfigThreshold::OnBnClickedConfigOpenCam()
{
    m_camera.OpenCam();
    SetTimer(CONFIG_USE_CAM,20,NULL);
}

void CConfigThreshold::OnBnClickedConfigOpenImage()
{
    if(m_camera.GetCurrentFrame()){
        m_camera.UpdateFrame();
        BasicProc();
        return;
    }

    if(m_camera.OpenImageEx()){
        BasicProc();
    }else{
        AfxMessageBox(_T("you didn't select file!"));
    }
}

void CConfigThreshold::OnBnClickedCkRed()
{
    ChangeMode(RED_MODE);
}


void CConfigThreshold::OnBnClickedCkBlue()
{
    ChangeMode(BLUE_MODE);
}

void CConfigThreshold::ChangeMode( COLOR_MODE mode /*= RED_MODE*/ )
{
    if(current_mode == mode) return;
    current_mode = mode;
    // TODO:maybe we should show more info?
}
