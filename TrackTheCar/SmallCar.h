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
    bool StopCar();
private:
    bool init_success;
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CCvPicCtrl* m_output;// show what the car is doing now

    CConfigs* m_config;

    CRouteHelper m_route;
    CvPoint m_head;
    CvPoint m_tail;

    bool GetCarPos();

    std::vector<CvPoint> m_map_point;
    std::vector<bool> m_pass_point;

    CCarControl m_car_control;

    CvPoint m_current_car_pos; //the car's current pos
    CvPoint m_next_point;//the point that the car is trying to move to
public:
    enum FIND_POINT{
       OK,
       FAIL,
       NO_MORE_POINT
    };
    enum MOVE_RESULT{
        REACH_POINT,
        TURN_LEFT,
        TURN_RIGHT,
        GO_FORWARD
    };
    int CalcCarLength();
    FIND_POINT GetCarPosEx(CvPoint* car_pos = NULL);
    FIND_POINT FindNextPoint(CvPoint* nex_point = NULL);
    MOVE_RESULT CSmallCar::Move2NextPoint(int distance_error = DISTANCE_ERROR,
        int angle_error = ANGLE_ERROR);// move to destination point
};
