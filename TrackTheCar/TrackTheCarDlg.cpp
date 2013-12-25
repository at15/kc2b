
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
    DDX_Control(pDX, IDC_LIST1, m_list_config);
    DDX_Control(pDX, IDC_B_MAIN_START_CAR, m_btn_start_car);
    DDX_Control(pDX, IDC_B_MAIN_STOP_CAR, m_btn_stop_car);
    DDX_Control(pDX, IDC_BUTTON3, m_btn_prepare_car);
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
    ON_COMMAND(ID_32777, &CTrackTheCarDlg::OnCarConfig)
    ON_COMMAND(ID_32780, &CTrackTheCarDlg::OnMainOpenCam)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON2, &CTrackTheCarDlg::OnBnClickedStartCar)
    ON_COMMAND(ID_32783, &CTrackTheCarDlg::OnRestConfig)
    ON_BN_CLICKED(IDC_BUTTON3, &CTrackTheCarDlg::OnBnClickedPrepareCar)
    ON_BN_CLICKED(IDC_B_MAIN_START_CAR, &CTrackTheCarDlg::OnBnClickedBMainStartCar)
    ON_BN_CLICKED(IDC_B_MAIN_STOP_CAR, &CTrackTheCarDlg::OnBnClickedBMainStopCar)
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

    // TODO: The init code

    // the log file
    CTime tm = CTime::GetCurrentTime();
    CString c_time =tm.Format("%d-%H-%M-%S");
    CString log_path; 
    log_path.Format(L"%s%s.txt",L"C:\\Users\\W7_64\\Desktop\\",c_time);
    m_log_file.Open(log_path , CFile::modeWrite|CFile::modeCreate, &m_log_error);
    AddToConsole(L">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");// start the log

    // init the pic ctrls
    if(!m_main_input.Init(this,IDC_MAIN_INPUT)){
        AddToConsole(_T("ERROR: can't init the main picture control!"));
    }
    if(!m_main_output.Init(this,IDC_MAIN_OUTPUT)){
        AddToConsole(_T("ERROR: can't init the output picture control!"));
    }
    if(!m_main_output2.Init(this,IDC_MAIN_OUTPUT2)){
        AddToConsole(_T("ERROR: can't init the output2 picture control!"));
    }

    // show the configs in the listctrl
    m_list_config.InsertColumn(0,L"Prop", LVCFMT_CENTER,100);
    m_list_config.InsertColumn(1,L"Value", LVCFMT_CENTER,100);
    ShowConfig();

    // set the flag
    car_working = false;

    // disable the buttons
    m_btn_start_car.EnableWindow(FALSE);
    m_btn_stop_car.EnableWindow(FALSE);
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

void CTrackTheCarDlg::PostNcDestroy()
{
    // TODO: 在此添加专用代码和/或调用基类
    m_log_file.Close();
    CDialogEx::PostNcDestroy();
}

void CTrackTheCarDlg::AddToConsole(const CString& str){
    CString old;
    m_main_console.GetWindowTextW(old);
    old.Append(str);
    old.Append(L"\r\n");
    m_main_console.SetWindowTextW(old);
    m_log_file.WriteString(old);// write to log file;
}

void CTrackTheCarDlg::AddToConsole(const char* str){
    CString old;
    m_main_console.GetWindowTextW(old);
    wchar_t* wstr = EZ::CStrConv::ansi2utf16(str);
    old.Append(wstr);
    delete wstr;
    old.Append(L"\r\n");
    m_main_console.SetWindowTextW(old);
    m_log_file.WriteString(old);// write to log file;
}

void CTrackTheCarDlg::ShowConfig(){
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    m_list_config.DeleteAllItems();
    CString str;
    int index = m_list_config.GetItemCount();
    m_list_config.InsertItem(index, L"地图变形");
    if(global_configs->IsTransfromSet()){
        m_list_config.SetItemText(index,1,L"已设置");
    }else{
        m_list_config.SetItemText(index,1,L"未设置");
    }

    index++;
    m_list_config.InsertItem(index, L"地图阀值");
    str.Format(L"%d",global_configs->GetMapThreshold());
    m_list_config.SetItemText(index,1,str);

    index++;
    m_list_config.InsertItem(index, L"地图点");
    if(global_configs->IsMapPointSet()){
        m_list_config.SetItemText(index,1,L"已生成");
    }else{
        m_list_config.SetItemText(index,1,L"未生成");
    }

    index++;
    m_list_config.InsertItem(index, L"红蓝阀值");
    m_list_config.SetItemText(index,1,L"默认");

    index++;
    m_list_config.InsertItem(index, L"蓝牙端口");
    str.Format(L"%d",global_configs->GetCOM());
    m_list_config.SetItemText(index,1,str);

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
    m_main_input.Pause();
    if(m_main_input.GetCurrentFrame()){
        m_dlg_threshold.SetMainFrame(m_main_input.GetCurrentFrame());
    }
    m_dlg_threshold.DoModal();
    m_main_input.Pause(false);
    ShowConfig();
}


void CTrackTheCarDlg::OnConfigTransform()
{
    m_main_input.Pause();
    if(m_main_input.GetCurrentFrame()){
        m_dlg_transform.SetMainFrame(m_main_input.GetCurrentFrame());
    }
    m_dlg_transform.DoModal();
    m_main_input.Pause(false);
    ShowConfig();
    process_input(&m_main_input);
}

void CTrackTheCarDlg::OnConfigMap()
{
    m_main_input.Pause();
    if(m_main_input.GetCurrentFrame()){
        m_dlg_map.SetMainFrame(m_main_input.GetCurrentFrame());
    }
    m_dlg_map.DoModal();
    m_main_input.Pause(false);
    ShowConfig();
}

void CTrackTheCarDlg::OnMainOpenImage()
{
    CString file_path;
    m_main_input.OpenImageEx(file_path);
    AddToConsole(file_path);

}


void CTrackTheCarDlg::OnCarConfig()
{
    m_dlg_car = new CarControlDlg;
    m_main_input.Pause();
    m_dlg_car->DoModal();
    m_main_input.Pause(false);
    delete m_dlg_car;
    m_dlg_car = NULL;
    ShowConfig();
}


void CTrackTheCarDlg::OnMainOpenCam()
{
    // TODO: 在此添加命令处理程序代码
    if(!m_main_input.OpenCam()){
        AfxMessageBox(_T("can\'t open cam"));
        return;
    }
    SetTimer(MAIN_CAM,20,NULL);
}


void CTrackTheCarDlg::OnTimer(UINT_PTR nIDEvent)
{

    if(MAIN_CAM == nIDEvent) CamProc();
    if(CAR_PROC == nIDEvent && !car_working) CarProc();
    CDialogEx::OnTimer(nIDEvent);
}

void CTrackTheCarDlg::process_input(CCvPicCtrl* pic_ctrl){
    // transform the image
    CImageProc proc;
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    // return if the transform is not set
    if(!g_configs->map_corner.IsSet()) return; 
    IplImage* transformed_pic= proc.TransformImage(pic_ctrl->GetCurrentFrame(),
        g_configs->map_corner.Get());
    pic_ctrl->SetCurrentFrame(transformed_pic);
    cvReleaseImage(&transformed_pic);
}


void CTrackTheCarDlg::OnBnClickedStartCar()
{
    //if(!m_main_input.IsCapturing()){
    //    AfxMessageBox(L"Open the cam please");
    //    AddToConsole("start_car_error:the cam is not opened");
    //    return;
    //}else{
    //    AddToConsole("cam opened,please set the map and the car");
    //}

    //// then set all the config

    //// check if all the config is done
    //CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    //try{
    //    global_configs->GetMapCorner();
    //}catch(logic_error e){
    //    AfxMessageBox(L"didn't set the corner for the map!");
    //    AddToConsole(e.what());
    //    return;
    //}
    //AddToConsole("map corner set!");

    //// OnCenCorner();// gen the map point, NO! the map point should be done 
    //// without putting the car on the map

    //try{
    //    global_configs->GetMapPoint();
    //}catch(logic_error e){
    //    AfxMessageBox(L"can't get the map point!");
    //    AddToConsole(e.what());
    //    return;
    //}
    //AddToConsole("map point generated!");

    //AddToConsole("all config loaded!,start the car");

    //// Note: the car will control camera now, you don't have call a timer
    //// really? no .... you can't set timer in cpp....so this is just init...
    //if(m_car.Init(&m_main_input,&m_main_output,global_configs)){
    //    AddToConsole("car initialized!");
    //}else{
    //    AfxMessageBox(L"Please open the car and set the right com port!");
    //    AddToConsole("Car init failed!");
    //    return;
    //}
    //if(m_car.StartCar()){
    //    KillTimer(MAIN_CAM);
    //    SetTimer(CAR_PROC,20,NULL);
    //}else{
    //    AfxMessageBox(L"无法找到小车，请调节阀值！");
    //    OnConfigThreshold();
    //    return;
    //}


}





void CTrackTheCarDlg::CamProc(){
    if(m_main_input.IsPause()) return;
    m_main_input.CaptureDontShow();
    process_input(&m_main_input);
    m_main_input.UpdateFrame();
}

void CTrackTheCarDlg::CarProc(){
    // this is new car proc
    car_working = true;

    if(!m_car.GetCarInfo()){
        ExitCarProc();
        AddToConsole(L"Can't find car!.Exit now");
        AfxMessageBox(L"Can't find car!.Exit now");
        return;
    }

    CSmallCar::MOVE_RESULT re;
    CString log_str,error_str;
    re = m_car.MoveCar(log_str,error_str);
    // unknown error
    if(CSmallCar::MOVE_ERROR == re){
        ExitCarProc();
        AddToConsole(L"Error in moving car!.Exit now");
        AfxMessageBox(L"Error in moving car!.Exit now");
        return;
    }
    // reach the end point
    if(CSmallCar::REACH_END == re){
        ExitCarProc();
        AddToConsole(L"Reach end!");
        AfxMessageBox(L"Reach end!");
        return;
    }
    // other return value means the car is working fine
    AddToConsole(log_str);
    car_working = false;
}

void CTrackTheCarDlg::ExitCarProc(){
    m_car.StopCar();
    car_working = false;
    KillTimer(CAR_PROC);
    AddToConsole(L"CarProc exit. some error may have occurred");
}

void CTrackTheCarDlg::OnRestConfig()
{
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    global_configs->RestConfig();
    ShowConfig();
}


void CTrackTheCarDlg::OnBnClickedPrepareCar()
{
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;

    if(!m_main_input.IsCapturing()){
        AfxMessageBox(L"Open the cam please");
        AddToConsole("start_car_error:the cam is not opened");
        return;
    }

    // check if config is set
    if(!g_configs->map_corner.IsSet()){
        AfxMessageBox(L"Map corner not set!");
        return;
    }

    if(!g_configs->raw_line.IsSet()){
        AfxMessageBox(L"Map line not generated!");
        return;
    }

    // TODO: Connect and find the car, and find the lines
    CSmallCar::CAR_ERROR e;
    e = m_car.Init(&m_main_input,&m_main_output,&m_main_output2,g_configs);
    if(CSmallCar::CANT_FIND_CAR == e){
        AfxMessageBox(L"Can't find car, please set the threshold!");
        OnConfigThreshold();
        return;
    }
    if(CSmallCar::CANT_CONNECT_CAR == e){
        AfxMessageBox(L"Can't connect car, please set the right com port,and open the car");
        return;
    }
    if(CSmallCar::NO_CAR_ERROR == e){
        AddToConsole(L"Car initialized, waiting for order");
        m_btn_start_car.EnableWindow(TRUE);
    }
}

void CTrackTheCarDlg::OnBnClickedBMainStartCar()
{
    m_btn_prepare_car.EnableWindow(FALSE);
    m_btn_stop_car.EnableWindow(TRUE);
    // send the message to start the car, and start the loop
    m_car.StartCar();
    // timer!
}


void CTrackTheCarDlg::OnBnClickedBMainStopCar()
{
     // stop the car, in case something wrong has happened
    m_btn_prepare_car.EnableWindow(TRUE);
    m_btn_start_car.EnableWindow(TRUE);
    m_btn_stop_car.EnableWindow(FALSE);
    m_car.StopCar();
    // killer timer
}
