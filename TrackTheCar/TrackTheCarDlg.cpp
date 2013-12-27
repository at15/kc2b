
// TrackTheCarDlg.cpp : ʵ���ļ�
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


// CTrackTheCarDlg �Ի���


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
    ON_COMMAND(ID_32783, &CTrackTheCarDlg::OnRestConfig)
    ON_BN_CLICKED(IDC_BUTTON3, &CTrackTheCarDlg::OnBnClickedPrepareCar)
    ON_BN_CLICKED(IDC_B_MAIN_START_CAR, &CTrackTheCarDlg::OnBnClickedBMainStartCar)
    ON_BN_CLICKED(IDC_B_MAIN_STOP_CAR, &CTrackTheCarDlg::OnBnClickedBMainStopCar)
END_MESSAGE_MAP()


// CTrackTheCarDlg ��Ϣ�������

BOOL CTrackTheCarDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: The init code

    // the log file
    CTime tm = CTime::GetCurrentTime();
    CString c_time =tm.Format("%d-%H-%M-%S");
    CString log_current_time = tm.Format("20%y-%m-%d %H:%M:%S");
    CString log_path; 
    log_path.Format(L"%s%s.txt",L"C:\\Users\\W7_64\\Desktop\\",c_time);
    m_log_file.Open(log_path , CFile::modeWrite|CFile::modeCreate, &m_log_error);
    AddToConsole(L">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>",true);// start the log
    AddToConsole(log_current_time);

    // init the pic ctrls
    if(!m_main_input.Init(this,IDC_MAIN_INPUT)){
        AddToConsole(_T("ERROR: can't init the main picture control!"),true);
    }
    if(!m_main_output.Init(this,IDC_MAIN_OUTPUT)){
        AddToConsole(_T("ERROR: can't init the output picture control!"),true);
    }
    if(!m_main_output2.Init(this,IDC_MAIN_OUTPUT2)){
        AddToConsole(_T("ERROR: can't init the output2 picture control!"),true);
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
    AddToConsole(_T("Track the car app init finished, waiting for orders..."),true);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTrackTheCarDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTrackTheCarDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CTrackTheCarDlg::PostNcDestroy()
{
    // TODO: �ڴ����ר�ô����/����û���
    m_log_file.Close();
    CDialogEx::PostNcDestroy();
}

void CTrackTheCarDlg::AddToConsole(const CString& str,bool show /*=true*/){
    CString add_str = str;
    add_str.Append(L"\r\n");
    if(show){
        CString old;
        m_main_console.GetWindowTextW(old);
        old.Append(add_str);
        m_main_console.SetWindowTextW(old);
    }
    m_log_file.WriteString(add_str);// write to log file;
}

void CTrackTheCarDlg::AddToConsole(const char* str,bool show /*=true*/){
    wchar_t* wstr = EZ::CStrConv::ansi2utf16(str);
    CString add_str = wstr;
    add_str.Append(L"\r\n");
    if(show){
        CString old;
        m_main_console.GetWindowTextW(old);
        old.Append(add_str);
        m_main_console.SetWindowTextW(old);
    }
    m_log_file.WriteString(add_str);// write to log file;
    delete wstr;
}

void CTrackTheCarDlg::ShowConfig(){
    /* using CGConfig now
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    m_list_config.DeleteAllItems();
    CString str;
    int index = m_list_config.GetItemCount();
    m_list_config.InsertItem(index, L"��ͼ����");
    if(global_configs->IsTransfromSet()){
    m_list_config.SetItemText(index,1,L"������");
    }else{
    m_list_config.SetItemText(index,1,L"δ����");
    }

    index++;
    m_list_config.InsertItem(index, L"��ͼ��ֵ");
    str.Format(L"%d",global_configs->GetMapThreshold());
    m_list_config.SetItemText(index,1,str);

    index++;
    m_list_config.InsertItem(index, L"��ͼ��");
    if(global_configs->IsMapPointSet()){
    m_list_config.SetItemText(index,1,L"������");
    }else{
    m_list_config.SetItemText(index,1,L"δ����");
    }

    index++;
    m_list_config.InsertItem(index, L"������ֵ");
    m_list_config.SetItemText(index,1,L"Ĭ��");

    index++;
    m_list_config.InsertItem(index, L"�����˿�");
    str.Format(L"%d",global_configs->GetCOM());
    m_list_config.SetItemText(index,1,str);
    */
}

void CTrackTheCarDlg::OnCapPic()
{
    m_dlg_camera.DoModal();
}


void CTrackTheCarDlg::OnShowAbout()
{
    //m_about.DoModal();
    AddToConsole(L"KC2B: Track the car. Using opencv with MFC");
    AddToConsole(L"Author: DXY, FH, GPL");
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
    // TODO: �ڴ���������������
    if(!m_main_input.OpenCam()){
        AfxMessageBox(_T("can\'t open cam"));
        return;
    }
    SetTimer(MAIN_CAM,20,NULL);
}


void CTrackTheCarDlg::OnTimer(UINT_PTR nIDEvent)
{
    if(CAR_PROC == nIDEvent && !car_working){
        CarProc();
        return;
    } 
    if(MAIN_CAM == nIDEvent) CamProc();

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

void CTrackTheCarDlg::CamProc(){
    if(m_main_input.IsPause()) return;
    m_main_input.CaptureDontShow();
    process_input(&m_main_input);
    m_main_input.UpdateFrame();
}

void CTrackTheCarDlg::CarProc(){
    // this is new car proc
    car_working = true;
    CamProc();//  get the new frame
    CvPoint car_head,car_tail,car_target;
    if(!m_car.GetCarInfo(&car_head,&car_tail,&car_target)){
        ExitCarProc();
        AddToConsole(L"Can't find car!.Exit now",true);
        AfxMessageBox(L"Can't find car!.Exit now");
        return;
    }

    CSmallCar::MOVE_RESULT re;
    CString log_str,error_str;
    re = m_car.MoveCar(log_str,error_str);
    // unknown error
    if(CSmallCar::MOVE_ERROR == re){
        ExitCarProc();
        AddToConsole(L"Error in moving car!.Exit now",true);
        AfxMessageBox(L"Error in moving car!.Exit now");
        return;
    }
    // reach the end point
    if(CSmallCar::REACH_END == re){
        ExitCarProc();
        AddToConsole(L"Reach end!",true);
        AfxMessageBox(L"Reach end!");
        return;
    }
    // other return value means the car is working fine
    AddToConsole(log_str,false);
    car_working = false;
}

void CTrackTheCarDlg::ExitCarProc(){
    m_car.StopCar();
    car_working = false;
    KillTimer(CAR_PROC);
    AddToConsole(L"CarProc exit. some error occurred",true);
}

void CTrackTheCarDlg::OnRestConfig()
{   
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    g_configs->ResetConfig();
    ShowConfig();
}


void CTrackTheCarDlg::OnBnClickedPrepareCar()
{
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;

    if(!m_main_input.IsCapturing()){
        AfxMessageBox(L"Open the cam please");
        AddToConsole("start_car_error:the cam is not opened",true);
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
    CarInfo car_info;
    e = m_car.Init(&m_main_input,g_configs);
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
        AddToConsole(L"Car initialized, waiting for order",true);
        CImageProc proc;
        // show the map 
        m_main_output.SetCurrentFrame(g_configs->map_thin_image.GetImage());
        proc.DrawLines(&m_main_output,g_configs->sorted_line.Get(),200);
        // show the car position
        m_car.GetCarInfo(car_info);
        m_main_output2.SetCurrentFrame(m_main_input.GetCurrentFrame());
        cvCircle(m_main_output2.GetCurrentFrame(),car_info.core,10,CV_RGB(0,255,0),3);
        m_main_output.UpdateFrame();
        m_main_output2.UpdateFrame();

        m_btn_start_car.EnableWindow(TRUE);
    }
}

void CTrackTheCarDlg::OnBnClickedBMainStartCar()
{
    m_btn_prepare_car.EnableWindow(FALSE);
    m_btn_stop_car.EnableWindow(TRUE);
    // send the message to start the car, and start the loop
    m_car.StartCar();
    // timer! i forgot... that's why car is always straight
    KillTimer(MAIN_CAM);
    SetTimer(CAR_PROC,5,NULL);
}


void CTrackTheCarDlg::OnBnClickedBMainStopCar()
{
    // stop the car, in case something wrong has happened
    m_btn_prepare_car.EnableWindow(TRUE);
    m_btn_start_car.EnableWindow(TRUE);
    m_btn_stop_car.EnableWindow(FALSE);
    ExitCarProc();
}
