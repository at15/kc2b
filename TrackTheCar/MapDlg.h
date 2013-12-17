#pragma once
#include "CameraCtrl.h"

// CMapDlg �Ի���

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)

public:
	CMapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMapDlg();

// �Ի�������
	enum { IDD = IDD_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

    // set the main frame of this dialog
    void SetMainFrame(IplImage* pSrc);

    // process
    void map_process();
private:
    CCameraCtrl m_map_input;
    CCvPicCtrl m_map_bin;
    CCvPicCtrl m_map_thin;
    CCvPicCtrl m_map_point_gened;
public:
    afx_msg void OnBnClickedMapOpenImage();
    afx_msg void OnBnClickedMapChange();
    // the threshold for the map binary
    int m_map_threshold;
    // the time for the thin
    int m_thin_iteration;
    // used in cvgoodfeaturetrack
    double m_quality_level;
    // used in cvgoodfeature track,the min distance between two points
    int m_min_distance;
    afx_msg void OnBnClickedMapPointGen();
private:
    bool map_gened;
    bool map_point_gened;
};
