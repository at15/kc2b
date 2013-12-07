
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
    if(!m_main_input.Init(this,IDC_MAIN_INPUT)){
        AddToConsole(_T("ERROR: can't init the main picture control!"));
    }
    if(!m_main_output.Init(this,IDC_MAIN_OUTPUT)){
        AddToConsole(_T("ERROR: can't init the output picture control!"));
    }
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
    // TODO: �ڴ���������������
    m_about.DoModal();
    AddToConsole(_T("opend the about dialog box"));
}


void CTrackTheCarDlg::OnConfigThreshold()
{
    // TODO: �ڴ���������������
    m_dlg_threshold.DoModal();
}


void CTrackTheCarDlg::OnConfigTransform()
{
    // TODO: �ڴ���������������
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
    // TODO: �ڴ���������������
    m_dlg_car.DoModal();
}
