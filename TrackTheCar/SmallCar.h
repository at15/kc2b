#pragma once
#include "CameraCtrl.h"
#include "Configs.h"
#include "TempImage.h"
#include "CarControl.h"
#include "RouteHelper.h"

struct CarInfo{
    CvPoint head;
    CvPoint tail;
    CvPoint core;
    double length;
};

class CSmallCar
{
public:
    CSmallCar(void);
    ~CSmallCar(void);
public:
    enum CAR_ERROR{
        NO_CAR_ERROR,
        CANT_FIND_CAR,
        CANT_CONNECT_CAR
    };
    enum FIND_POINT{
        OK,
        FAIL,
        NO_MORE_POINT
    };
    enum MOVE_RESULT{
        REACH_POINT,
        PASS_POINT,
        TURN_LEFT,
        TURN_RIGHT,
        GO_FORWARD,
        CAR_STUCK// 车卡住了，需要加一下速
    };
    CAR_ERROR Init(CCvPicCtrl* camera,CCvPicCtrl* output_map,CCvPicCtrl* output_car,CConfigs* config);
    bool StartCar();
    bool SpeedUp();
    bool StopCar();
     /* The new routine */
    bool GetCarInfo(CarInfo& info);
    bool IsEnd();// if the car has reached the end
    // Method 1 move in lines
    // Method 2 move in points

private:
    bool init_success;
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CCvPicCtrl* m_output;// show what the car is doing now

    CConfigs* m_config;

    CRouteHelper m_route;

    CCarControl m_car_control;


    /* The new routine */
    CarInfo m_car_info;
    bool m_reach_end;
    std::vector<CLine> m_map_line;
};
