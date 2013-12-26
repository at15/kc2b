// MapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TrackTheCar.h"
#include "MapDlg.h"
#include "afxdialogex.h"
using namespace std;

// CMapDlg 对话框

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapDlg::IDD, pParent)
    , m_map_threshold(0)
    , m_thin_iteration(0)
    , m_line_distance_error(0)
{
}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT2, m_map_threshold);
    DDX_Text(pDX, IDC_EDIT3, m_thin_iteration);
    DDX_Text(pDX, IDC_EDIT9, m_line_distance_error);
    DDX_Control(pDX, IDC_B_MAP_REGEN, m_btn_regen_all);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
    ON_BN_CLICKED(IDC_MAP_OPEN_IMAGE, &CMapDlg::OnBnClickedMapOpenImage)
    ON_BN_CLICKED(IDC_B_MAP_REGEN, &CMapDlg::OnBnClickedBMapRegen)
END_MESSAGE_MAP()


// CMapDlg 消息处理程序


BOOL CMapDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_map_input.Init(this,IDC_MAP_INPUT);
    m_map_bin.Init(this,IDC_MAP_BIN);
    m_map_thin.Init(this,IDC_MAP_THIN);
    m_map_line_gened.Init(this,IDC_MAP_LINE_GENED);

    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    m_map_threshold = g_configs->map_threshold.Get();
    m_thin_iteration = g_configs->map_thin_iteration.Get();
    m_line_distance_error = g_configs->line_distance_error.Get();
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CMapDlg::SetMainFrame(IplImage* pSrc){
    m_map_input.SetCurrentFrame(pSrc);
}

// just process everything in this function
void CMapDlg::map_process(){
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;

     // check if we have transformed the image
     if(!g_configs->map_corner.IsSet()){
         AfxMessageBox(L"请先设置地图变形");
         return;
     }

     // get the binary and then use the thin
    CImageProc proc;
    IplImage* grey = proc.GetGrey(m_map_input.GetCurrentFrame());
    // add true get the different type of binary image.... 
    IplImage* bin = proc.GetBinary(grey,g_configs->map_threshold.Get(),true);
    m_map_bin.SetCurrentFrame(bin);
    // then thin
    proc.cvThin(bin,bin,g_configs->map_thin_iteration.Get());
    m_map_thin.SetCurrentFrame(bin);
    cvReleaseImage(&grey);
    cvReleaseImage(&bin);

    // then find lines
    IplImage* thin = m_map_thin.GetCurrentFrame();
    vector<CLine> v_lines = proc.FindLines(thin,m_line_distance_error);
    m_map_line_gened.SetCurrentFrame(thin);// just need a bw image
    proc.DrawLines(&m_map_line_gened,v_lines);
    m_map_line_gened.UpdateFrame();
    //  set the line
    g_configs->raw_line.Set(v_lines);
    g_configs->map_thin.SetImage(thin);
}

void CMapDlg::OnBnClickedMapOpenImage()
{
    // if the main dialog has set the image to process
    if(m_map_input.GetCurrentFrame()){
        m_map_input.UpdateFrame();
        map_process();
        return;
    }

    if(m_map_input.OpenImageEx()){
        map_process();
    }else{
        AfxMessageBox(_T("you didn't select file!"));
    }
}

// re generate all the map and lines
void CMapDlg::OnBnClickedBMapRegen()
{
    CGConfigs* g_configs = &((CTrackTheCarApp*)AfxGetApp())->g_configs;
    CImageProc proc;

    // update the configs
    UpdateData();
    g_configs->map_threshold.Set(m_map_threshold);
    g_configs->map_thin_iteration.Set(m_thin_iteration);
    g_configs->line_distance_error.Set(m_line_distance_error);

    m_btn_regen_all.EnableWindow(FALSE);
    map_process();
    m_btn_regen_all.EnableWindow(TRUE);
    
}
