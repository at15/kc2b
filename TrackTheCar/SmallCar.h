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
    enum MOVE_RESULT{
        REACH_POINT,
        PASS_POINT,
        REACH_END,
        MOVE_ERROR
    };
    CAR_ERROR Init(CCvPicCtrl* cam,CCvPicCtrl* o_map,CCvPicCtrl* o_car,CGConfigs* config);
    bool GetCarInfo();
    bool GetCarInfo(CarInfo& info);
    bool StartCar();
    bool StopCar();
    MOVE_RESULT MoveCar(CString& log_str,CString& error_str);
    bool SpeedUp();

private:
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CCvPicCtrl* m_output_map;// show the map
    CCvPicCtrl* m_output_car;// show the car

    CGConfigs* m_config;

    CRouteHelper m_route;

    CCarControl m_car_control;


    /* The new routine */
    CarInfo m_car_info;
    bool m_reach_end;
    std::vector<CLine> m_map_line;
    int m_current_line_index; // which line the car is running 0 based
};