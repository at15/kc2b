
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
    ON_COMMAND(ID_32780, &CTrackTheCarDlg::OnMainOpenCam)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON2, &CTrackTheCarDlg::OnBnClickedStartCar)
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

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    use_config = false;

    m_log_file.Open( L"C:\\Users\\W7_64\\Desktop\\car_log.txt", CFile::modeWrite|CFile::modeCreate, &m_log_error);
    AddToConsole(L">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");// start the log
    if(!m_main_input.Init(this,IDC_MAIN_INPUT)){
        AddToConsole(_T("ERROR: can't init the main picture control!"));
    }
    if(!m_main_output.Init(this,IDC_MAIN_OUTPUT)){
        AddToConsole(_T("ERROR: can't init the output picture control!"));
    }

    // show the configs in the listctrl
    m_list_config.InsertColumn(0,L"Prop", LVCFMT_CENTER,100);
    m_list_config.InsertColumn(1,L"Value", LVCFMT_CENTER,100);
    ShowConfig();

    car_working = false;

    AddToConsole(_T("Track the car app init finished, waiting for orders..."));
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
    m_list_config.DeleteAllItems();
    int index = m_list_config.GetItemCount();
    m_list_config.InsertItem(index, L"��ͼ����");
    m_list_config.SetItemText(index,1,L"δ����");
    index++;
    m_list_config.InsertItem(index, L"��ͼ��ֵ");
    m_list_config.SetItemText(index,1,L"Ĭ��");
    index++;
    m_list_config.InsertItem(index, L"��ͼ��");
    m_list_config.SetItemText(index,1,L"δ����");
    index++;
    m_list_config.InsertItem(index, L"������ֵ");
    m_list_config.SetItemText(index,1,L"Ĭ��");
    index++;
    m_list_config.InsertItem(index, L"�����˿�");
    m_list_config.SetItemText(index,1,L"Ĭ��(3)  ");
}

void CTrackTheCarDlg::OnCapPic()
{
    m_dlg_camera.DoModal();
}


void CTrackTheCarDlg::OnShowAbout()
{
    // TODO: �ڴ���������������
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
}


void CTrackTheCarDlg::OnConfigTransform()
{
    m_main_input.Pause();
    if(m_main_input.GetCurrentFrame()){
        m_dlg_transform.SetMainFrame(m_main_input.GetCurrentFrame());
    }
    m_dlg_transform.DoModal();
    m_main_input.Pause(false);
}

void CTrackTheCarDlg::OnConfigMap()
{
    m_main_input.Pause();
    if(m_main_input.GetCurrentFrame()){
        m_dlg_map.SetMainFrame(m_main_input.GetCurrentFrame());
    }
    m_dlg_map.DoModal();
    m_main_input.Pause(false);
}

void CTrackTheCarDlg::OnMainOpenImage()
{
    CString file_path;
    m_main_input.OpenImageEx(file_path);
    AddToConsole(file_path);

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
    // set the map point in global
    global_configs->SetMapPoint(points);
    proc.DrawMapPoints(m_main_output.GetCurrentFrame(),points); 
    m_main_output.UpdateFrame();

    cvReleaseImage(&grey);
    cvReleaseImage(&bin);
}


void CTrackTheCarDlg::OnCarConfig()
{
    m_dlg_car = new CarControlDlg;
    m_main_input.Pause();
    m_dlg_car->DoModal();
    m_main_input.Pause(false);
    delete m_dlg_car;
    m_dlg_car = NULL;
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

    if(MAIN_CAM == nIDEvent) CamProc();
    if(CAR_PROC == nIDEvent && !car_working) CarProc();
    CDialogEx::OnTimer(nIDEvent);
}

void CTrackTheCarDlg::CamProc(){
    if(m_main_input.IsPause()) return;
    m_main_input.CaptureDontShow();
    process_input(&m_main_input);
    m_main_input.UpdateFrame();
}

void CTrackTheCarDlg::CarProc(){
    car_working = true;
    //CamProc();
    CvPoint from;
    CvPoint to;
    CSmallCar::MOVE_RESULT re;
    CString op;
    try{
        do{
            CamProc(); // have to capture the frame??!!!
            // should not do the loop here

            re = m_car.MoveCarP2P(from,to);

            switch(re){
            case CSmallCar::GO_FORWARD:{
                op = L"go forward";
                break;
                                       }
            case CSmallCar::TURN_LEFT:{
                op = L"go left";
                break;
                                      }
            case CSmallCar::TURN_RIGHT:{
                op = L"go right";
                break;
                                       }
            case CSmallCar::REACH_POINT:{
                op = L"reach point";
                break;
                                        }
            }
            CString str;
            str.Format(L"Op=%s Move from x=%d y=%d to x=%d y=%d",
                op,
                from.x,from.y,
                to.x,to.y);
            AddToConsole(str);
        }while(re != CSmallCar::REACH_POINT);
        AddToConsole("reach point");
    }
    catch(logic_error e){
        AddToConsole("reach last point or error occurred");
        AddToConsole(e.what());
    }

    car_working = false;
}

void CTrackTheCarDlg::process_input(CCvPicCtrl* pic_ctrl){
    // transform the image
    CImageProc proc;
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    try{
        global_configs->GetMapCorner();
    }catch(logic_error e){
        // don't echo error,there will be too many
        return;
    }
    IplImage* transformed_pic= proc.TransformImage(pic_ctrl->GetCurrentFrame(),
        global_configs->GetMapCorner());
    pic_ctrl->SetCurrentFrame(transformed_pic);
    cvReleaseImage(&transformed_pic);
}


void CTrackTheCarDlg::OnBnClickedStartCar()
{
    if(!m_main_input.IsCapturing()){
        AfxMessageBox(L"Open the cam please");
        AddToConsole("start_car_error:the cam is not opened");
        return;
    }else{
        AddToConsole("cam opened,please set the map and the car");
    }

    // then set all the config

    // check if all the config is done
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    try{
        global_configs->GetMapCorner();
    }catch(logic_error e){
        AfxMessageBox(L"didn't set the corner for the map!");
        AddToConsole(e.what());
        return;
    }
    AddToConsole("map corner set!");

    // OnCenCorner();// gen the map point, NO! the map point should be done 
    // without putting the car on the map

    try{
        global_configs->GetMapPoint();
    }catch(logic_error e){
        AfxMessageBox(L"can't get the map point!");
        AddToConsole(e.what());
        return;
    }
    AddToConsole("map point generated!");

    AddToConsole("all config loaded!,start the car");

    // Note: the car will control camera now, you don't have call a timer
    // really? no .... you can't set timer in cpp....so this is just init...
    if(m_car.Init(&m_main_input,&m_main_output,global_configs)){
        AddToConsole("car initialized!");
        m_car.StartCar();
        KillTimer(MAIN_CAM);
        SetTimer(CAR_PROC,20,NULL);
    }else{
        AfxMessageBox(L"Please open the car and set the right com port!");
        AddToConsole("Car init failed!");
    }

}


void CTrackTheCarDlg::PostNcDestroy()
{
    // TODO: �ڴ����ר�ô����/����û���
    m_log_file.Close();
    CDialogEx::PostNcDestroy();
}
