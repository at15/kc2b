
// TrackTheCarDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "TrackTheCarDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}





// CTrackTheCarDlg 对话框




CTrackTheCarDlg::CTrackTheCarDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CTrackTheCarDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrackTheCarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_main_console);
}

BEGIN_MESSAGE_MAP(CTrackTheCarDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_32775, &CTrackTheCarDlg::OnCapPic)
    ON_COMMAND(ID_32771, &CTrackTheCarDlg::OnShowAbout)
    ON_COMMAND(ID_32774, &CTrackTheCarDlg::OnConfigThreshold)
    ON_COMMAND(ID_32773, &CTrackTheCarDlg::OnConfigTransform)
    ON_COMMAND(ID_32778, &CTrackTheCarDlg::OnMainOpenImage)
    ON_COMMAND(ID_32781, &CTrackTheCarDlg::OnConfigMap)
    ON_COMMAND(ID_32782, &CTrackTheCarDlg::OnCenCorner)
    ON_COMMAND(ID_32777, &CTrackTheCarDlg::OnCarConfig)
END_MESSAGE_MAP()


// CTrackTheCarDlg 消息处理程序

BOOL CTrackTheCarDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    use_config = false;
    if(!m_main_input.Init(this,IDC_MAIN_INPUT)){
        AddToConsole(_T("ERROR: can't init the main picture control!"));
    }
    if(!m_main_output.Init(this,IDC_MAIN_OUTPUT)){
        AddToConsole(_T("ERROR: can't init the output picture control!"));
    }
    AddToConsole(_T("Track the car app init finished, waiting for orders..."));
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTrackTheCarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTrackTheCarDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTrackTheCarDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// apply the config to the main frame (basically the transform...)
void CTrackTheCarDlg::ApplyConfig(){
    use_config = true;
    process_input();
}

void CTrackTheCarDlg::process_input(){
    // transform the image
    CImageProc proc;
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;

    IplImage* transformed_pic= proc.TransformImage(m_main_input.GetCurrentFrame(),
        global_configs->GetMapCorner());
    m_main_input.SetCurrentFrame(transformed_pic);
    cvReleaseImage(&transformed_pic);

    // also need to set the threshold .. but the main input only need this at this moment
}

void CTrackTheCarDlg::RestConfig(){
    use_config = false;
}

void CTrackTheCarDlg::AddToConsole(const CString& str){
    CString old;
    m_main_console.GetWindowTextW(old);
    old.Append(str);
    old.Append(L"\r\n");
    m_main_console.SetWindowTextW(old);
}

void CTrackTheCarDlg::OnCapPic()
{
    m_dlg_camera.DoModal();
}


void CTrackTheCarDlg::OnShowAbout()
{
    // TODO: 在此添加命令处理程序代码
    m_about.DoModal();
    AddToConsole(_T("opend the about dialog box"));
}


void CTrackTheCarDlg::OnConfigThreshold()
{
    // TODO: 在此添加命令处理程序代码
    m_dlg_threshold.DoModal();
}


void CTrackTheCarDlg::OnConfigTransform()
{
    // TODO: 在此添加命令处理程序代码
    m_dlg_transform.SetMainFrame(m_main_input.GetCurrentFrame());
    m_dlg_transform.DoModal();
    ApplyConfig();
}

void CTrackTheCarDlg::OnConfigMap()
{
    m_dlg_map.SetMainFrame(m_main_input.GetCurrentFrame());
    m_dlg_map.DoModal();
}

void CTrackTheCarDlg::OnMainOpenImage()
{
    CString file_path;
    m_main_input.OpenImageEx(file_path);
    AddToConsole(file_path);
    //this->SetFocus();// ? will this work? the focus is set to the editor automaticlly
}


void CTrackTheCarDlg::OnCenCorner()
{
     CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    // TODO: typo ... should be OnGenCorner
    CImageProc proc;
    m_main_output.SetCurrentFrame(m_main_input.GetCurrentFrame());

    // first get the bin image
    IplImage* grey = proc.GetGrey(m_main_output.GetCurrentFrame());
    // add true get the different type of binary image.... 
    IplImage* bin = proc.GetBinary(grey,global_configs->GetMapThreshold(),true);
    proc.cvThin(bin,bin,global_configs->GetThinIteration());
    
    vector<CvPoint2D32f> points;
    proc.FindMapPoints(bin,points);
    proc.DrawMapPoints(m_main_output.GetCurrentFrame(),points); // seems not work!
    m_main_output.UpdateFrame();

    cvReleaseImage(&grey);
    cvReleaseImage(&bin);
}


void CTrackTheCarDlg::OnCarConfig()
{
    // TODO: 在此添加命令处理程序代码
    m_dlg_car.DoModal();
}
