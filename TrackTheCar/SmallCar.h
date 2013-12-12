#pragma once
#include "CameraCtrl.h"
#include "Configs.h"
#include "TempImage.h"
#include "CarControl.h"
#include "RouteHelper.h"


class CSmallCar
{
public:
    CSmallCar(void);
    ~CSmallCar(void);
public:
    //bool Init(CCvPicCtrl* camera,IplImage* map,CConfigs* config);
    bool Init(CCvPicCtrl* camera,CCvPicCtrl* output,std::vector<CvPoint2D32f> map_point,CConfigs* config);
    bool StartCar();
private:
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CCvPicCtrl* m_output;// show what the car is doing now

    CConfigs* m_config;

    CvPoint m_head;
    CvPoint m_tail;

    bool GetCarPos();
    CvPoint GetCarPosEx();

    std::vector<CvPoint> m_map_point;
    std::vector<bool> m_pass_point;
    
    CCarControl m_car_control;

    CRouteHelper m_route;

    CvPoint m_current_point; // the current point we have
    bool MoveCarP2P();
};

