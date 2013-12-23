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
        CANT_FIND_CAR
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
    bool Init(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config);
    bool StartCar();
    bool SpeedUp();
    bool StopCar();
    double GetCarLength();
    FIND_POINT GetCarPosEx(CvPoint* car_pos = NULL);
    FIND_POINT FindNextPoint(CvPoint* nex_point = NULL);
    MOVE_RESULT CSmallCar::Move2NextPoint(int distance_error = DISTANCE_ERROR,
        int angle_error = ANGLE_ERROR);// move to destination point
    // 判断车是不是卡住了
    bool isCarStuck();
    struct StuckInfo{
        StuckInfo(){
            empty = true;
            max_stuck_time = CAR_S_MAX_TIME;
        }
        bool empty;
        CvPoint firstPoint;
        int stuck_time;
        int max_stuck_time;
        double stuck_distance;// TODO:define them in the constants
        // maybe it should be relative
    }m_stuck_info;

    /* The new routine */
    bool Prepare(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config);
    bool GetCarInfo(CarInfo& info);
    bool IsEnd();// if the car has reached the end

private:
    bool init_success;
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CCvPicCtrl* m_output;// show what the car is doing now

    CConfigs* m_config;

    CRouteHelper m_route;

    CvPoint m_head;
    CvPoint m_tail;
    CvPoint m_current_car_pos; //the car's current pos
    double m_car_length;

    

    std::vector<CvPoint> m_map_point;
    std::vector<bool> m_pass_point;

    CCarControl m_car_control;

    
    CvPoint m_next_point;//the point that the car is trying to move to

    /* The new routine */
    CarInfo m_car_info;
    bool m_reach_end;
    // get the new lines, and the end of one line will be the start of the next
    bool GetNewLines(const std::vector<CLine>& );
    std::vector<CLine> m_map_line;
};
