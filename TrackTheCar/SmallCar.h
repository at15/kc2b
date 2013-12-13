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
    bool Init(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config);

    bool StartCar();
private:
    bool init_success;
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

    CvPoint m_current_point; // the current point we have,which is also the car pos
public:
    enum MOVE_RESULT{
        REACH_POINT,
        TURN_LEFT,
        TURN_RIGHT,
        GO_FORWARD
    };
    MOVE_RESULT CSmallCar::MoveCar2Point(CvPoint dst);// move to destination point
    // MoveCarP2P is wrong .... 
    MOVE_RESULT MoveCarP2P(CvPoint& from,CvPoint& to);// move from one to another
};

