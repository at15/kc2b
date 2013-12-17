#include "StdAfx.h"
#include "SmallCar.h"
using namespace std;

CSmallCar::CSmallCar(void)
{
    init_success = false;
}


CSmallCar::~CSmallCar(void)
{
}

/*
// we need 
// 1 a picture control that can always give us the map
//   PS: we only need a processed image now
// 2 the camera that can get the latest image, so we can know where the car is
bool CSmallCar::Init(CCvPicCtrl* camera,IplImage* map,CConfigs* config){
m_camera = camera;
m_map.SetImage(map);
m_config = config;
return true;
}*/

bool CSmallCar::Init(CCvPicCtrl* camera,CCvPicCtrl* output,std::vector<CvPoint2D32f> map_point,CConfigs* config){
    // check if we can start the car, if we can, then lets go
    if(!m_car_control.Init(config->GetCOM())) return false;

    m_camera = camera;
    m_output = output;

    m_map_point.clear();
    m_pass_point.clear();
    for(int i=0;i<map_point.size();i++){
        m_map_point.push_back(cvPointFrom32f(map_point.at(i)));
        m_pass_point.push_back(false);
    }
    m_config = config;

    return true;
}

bool CSmallCar::Init(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config){
    return Init(camera,output,config->GetMapPoint(),config);
}

bool CSmallCar::StartCar(){
    GetCarPosEx();
    return true;
}

bool CSmallCar::StopCar(){
    // stop the car
    m_car_control.Stop();
    return true;
}

bool CSmallCar::GetCarPos(){
    IplImage* t = m_camera->GetCurrentFrame();
    m_head = m_proc.GetRedCore(t,m_config->GetThreshold());
    m_tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    return true;
}

CSmallCar::FIND_POINT CSmallCar::GetCarPosEx(CvPoint* car_pos /*= NULL*/){
    IplImage* t = m_camera->GetCurrentFrame();
    m_head = m_proc.GetRedCore(t,m_config->GetThreshold());
    m_tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    if(m_head.x < 0 || m_head.y <0 || m_tail.x < 0 || m_tail.y < 0 ){
        throw logic_error("can't find car position");
    }
    CvPoint carPos = cvPoint((m_head.x+m_tail.x)/2,(m_head.y+m_tail.y)/2); 
    m_current_car_pos = carPos;
    if(car_pos){
        *car_pos = carPos;
    }
    return FIND_POINT::OK;
}

CSmallCar::FIND_POINT CSmallCar::FindNextPoint(CvPoint* nex_point /*= NULL*/){
    try{
        m_next_point = m_route.FindnextPoint(m_current_car_pos,m_map_point,m_pass_point);
        if(nex_point){
            *nex_point = m_next_point;
        }
        return FIND_POINT::OK;
    }catch(logic_error e){
        return FIND_POINT::NO_MORE_POINT;
    }
}

CSmallCar::MOVE_RESULT CSmallCar::Move2NextPoint(){
    CImageProc proc;
    m_output->SetCurrentFrame(m_camera->GetCurrentFrame(),false);

    // a green circle to show the car pos
    cvCircle(m_output->GetCurrentFrame(),m_current_car_pos,10,CV_RGB(0,255,0),3);
    // a yellow circle for the dst
    cvCircle(m_output->GetCurrentFrame(),m_next_point,10,CV_RGB(255,255,50),3);

    //求小车向量方向
    double direction_car=m_route.Angle(m_tail,m_head); 
    //小车中心到下一个目标点的向量方向
    double direction_target=m_route.Angle(m_current_car_pos,m_next_point); 
    //小车中心到目标点的距离
    double distance=m_route.Distance(m_current_car_pos,m_next_point); 

    // 达到目标
    if(distance <= DISTANCE_ERROR) {
        // TODO:should i stop the car?
        return REACH_POINT;
    }

    if(fabs(direction_car - direction_target) > ANGLE_ERROR) //小车与目标不在同一方向，则转向
    {
        if(direction_car<180)
        {
            if(direction_target>direction_car && direction_target<direction_car+180) //左转
            {
                m_car_control.GoLeft();
                return TURN_LEFT;
            }
            else //右转
            {
                m_car_control.GoRight();
                return TURN_RIGHT;
            }
        }
        else
        {
            if(direction_target>direction_car-180 && direction_target<direction_car) //右转
            {
                m_car_control.GoRight();
                return TURN_RIGHT;
            }
            else //左转
            {
                m_car_control.GoLeft();
                return TURN_LEFT;
            }
        }
    }
    else //小车与目标已在同一方向，则向前开
    {
        m_car_control.GoForward();// 应该会把头转回来
        return GO_FORWARD;
    }
}