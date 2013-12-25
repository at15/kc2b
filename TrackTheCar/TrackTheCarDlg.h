
// TrackTheCarDlg.h : ͷ�ļ�
//

#pragma once
#define MAIN_CAM 1  
#define CAR_PROC 2
// include the dialogs
#include "CameraDialog.h"
#include "ConfigThreshold.h"
#include "TransformDlg.h"
#include "MapDlg.h"
#include "SmallCar.h"
#include "CarControlDlg.h"
#include "afxwin.h"
#include "afxcmn.h"

// The about dialog
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

};

// CTrackTheCarDlg �Ի���
class CTrackTheCarDlg : public CDialogEx
{
    // ����
public:
    CTrackTheCarDlg(CWnd* pParent = NULL);	// ��׼���캯��

    // �Ի�������
    enum { IDD = IDD_TRACKTHECAR_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


    // ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    // the about dialog... which is kind of useless
    CAboutDlg m_about;

    // the car
    CSmallCar m_car;
    bool car_working;// the car proc is working,don't call it

    // this is for all the function dialogs
    CCameraDialog m_dlg_camera;
    CConfigThreshold m_dlg_threshold;
    CTransformDlg m_dlg_transform;
    CMapDlg m_dlg_map;
    CarControlDlg* m_dlg_car;

    // this for the main picture control 
    CCameraCtrl m_main_input;
    CCvPicCtrl m_main_output;
    CCvPicCtrl m_main_output2;

    // transform the image in the main input
    void process_input(CCvPicCtrl* pic_ctrl);
    
    // the console to show messages 
    CEdit m_main_console;
    // func for add message to the console
    void AddToConsole(const CString& str);
    void AddToConsole(const char* str);
    // save the console messages to log file
    CStdioFile m_log_file;
    CFileException m_log_error;
    
    // show all the config in the list ctrl
    CListCtrl m_list_config;
    void ShowConfig();
    afx_msg void OnRestConfig();

public:
    afx_msg void OnCapPic();
    afx_msg void OnShowAbout();
    afx_msg void OnConfigThreshold();
    afx_msg void OnConfigTransform();
    afx_msg void OnMainOpenImage();
    afx_msg void OnConfigMap();
    afx_msg void OnCenCorner();
    afx_msg void OnCarConfig();
    afx_msg void OnMainOpenCam();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
    // The functions which are called in timer
    void CamProc();
    void CarProc();
    void ExitCarProc(bool forever = true);// ��������,�˳������������Ƿ���Զ�˳�
public:
    afx_msg void OnBnClickedStartCar();
    virtual void PostNcDestroy();
    afx_msg void OnBnClickedPrepareCar();
    CButton m_btn_start_car;
    CButton m_btn_stop_car;
    afx_msg void OnBnClickedBMainStartCar();
    CButton m_btn_prepare_car;
    afx_msg void OnBnClickedBMainStopCar();
};