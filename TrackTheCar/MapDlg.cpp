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
    , m_quality_level(0)
    , m_min_distance(0)
{
    map_gened = false;
    map_point_gened = false;
}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT2, m_map_threshold);
    DDX_Text(pDX, IDC_EDIT3, m_thin_iteration);
    DDX_Text(pDX, IDC_EDIT5, m_quality_level);
    DDX_Text(pDX, IDC_EDIT8, m_min_distance);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
    ON_BN_CLICKED(IDC_MAP_OPEN_IMAGE, &CMapDlg::OnBnClickedMapOpenImage)
    ON_BN_CLICKED(IDC_MAP_CHANGE, &CMapDlg::OnBnClickedMapChange)
    ON_BN_CLICKED(IDC_MAP_POINT_GEN, &CMapDlg::OnBnClickedMapPointGen)
END_MESSAGE_MAP()


// CMapDlg 消息处理程序


BOOL CMapDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_map_input.Init(this,IDC_MAP_INPUT);
    m_map_bin.Init(this,IDC_MAP_BIN);
    m_map_thin.Init(this,IDC_MAP_THIN);
    m_map_point_gened.Init(this,IDC_MAP_PONIT_GENED);
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    m_map_threshold = global_configs->GetMapThreshold();
    m_thin_iteration = global_configs->GetThinIteration();
    m_quality_level = global_configs->GetQualityLevel();
    m_min_distance = global_configs->GetMinDistance();
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CMapDlg::SetMainFrame(IplImage* pSrc){
    m_map_input.SetCurrentFrame(pSrc);
}

void CMapDlg::map_process(){
     CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    // get the binary and then use the thin
    CImageProc proc;
    IplImage* grey = proc.GetGrey(m_map_input.GetCurrentFrame());
    // add true get the different type of binary image.... 
    IplImage* bin = proc.GetBinary(grey,global_configs->GetMapThreshold(),true);
    m_map_bin.SetCurrentFrame(bin);
    // then thin
    proc.cvThin(bin,bin,global_configs->GetThinIteration());
    m_map_thin.SetCurrentFrame(bin);
    cvReleaseImage(&grey);
    cvReleaseImage(&bin);

    map_gened = true;
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


void CMapDlg::OnBnClickedMapChange()
{
    UpdateData();
     CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;
    if(m_map_threshold){
       
        global_configs->SetMapThreshold(m_map_threshold);
    }
    if(m_thin_iteration){
        global_configs->SetThinIteration(m_thin_iteration);
    }
    map_process();
}


void CMapDlg::OnBnClickedMapPointGen()
{
    //生成角点(地图路径点)
    CConfigs* global_configs = &((CTrackTheCarApp*)AfxGetApp())->global_configs;

    CImageProc proc;
    // get the bin image form other control
    if(!map_gened){
        AfxMessageBox(L"请先生成地图！");
        return;
    }
    // get the binary map and the config
    UpdateData();

    IplImage* bin = m_map_thin.GetCurrentFrame();

    vector<CvPoint2D32f> points;
    proc.FindMapPoints(bin,points,m_quality_level,m_min_distance);
    // set the map point in global
    global_configs->SetMapPoint(points);
    // show it in the dialog
    m_map_point_gened.SetCurrentFrame(m_map_input.GetCurrentFrame(),false);
    proc.DrawMapPoints(m_map_point_gened.GetCurrentFrame(),points); 
    m_map_point_gened.UpdateFrame();

    map_point_gened = true;
}
