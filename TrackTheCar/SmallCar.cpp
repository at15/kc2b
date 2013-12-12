#include "StdAfx.h"
#include "SmallCar.h"


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
bool CSmallCar::StartCar(){
    m_current_point = GetCarPosEx();
    return true;
}

bool CSmallCar::GetCarPos(){
    IplImage* t = m_camera->GetCurrentFrame();
    m_head = m_proc.GetRedCore(t,m_config->GetThreshold());
    m_tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    return true;
}

CvPoint CSmallCar::GetCarPosEx(){
    IplImage* t = m_camera->GetCurrentFrame();
    m_head = m_proc.GetRedCore(t,m_config->GetThreshold());
    m_tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    CvPoint carPos = cvPoint((m_head.x+m_tail.x)/2,(m_head.y+m_tail.y)/2); 
    return carPos;
}

bool CSmallCar::MoveCarP2P(){
    //求小车中心坐标
    CvPoint carPos = GetCarPosEx(); 
    //find the nexPoint
    CvPoint nextPoint = m_route.FindnextPoint(m_current_point,m_map_point,m_pass_point);
    //求小车向量方向
    double direction_car=m_route.Angle(m_tail,m_head); 
    //小车中心到下一个目标点的向量方向
    double direction_target=m_route.Angle(carPos,nextPoint); 
    //小车中心到目标点的距离
    double distance=m_route.Distance(carPos,nextPoint); 


    if(distance>DISTANCE_ERROR) //未到达目标，继续调整
    {
        if(fabs(direction_car - direction_target) > ANGLE_ERROR) //小车与目标不在同一方向，则转向
        {
            if(direction_car<180)
            {
                if(direction_target>direction_car && direction_target<direction_car+180) //左转
                {
                    m_car_control.GoLeft();
                    return true;
                }
                else //右转
                {
                    m_car_control.GoRight();
                    return true;
                }
            }
            else
            {
                if(direction_target>direction_car-180 && direction_target<direction_car) //右转
                {
                    m_car_control.GoRight();
                    return true;
                }
                else //左转
                {
                    m_car_control.GoLeft();
                    return true;
                }
            }
        }
        else //小车与目标已在同一方向，则向前开
        {
            m_car_control.GoForward();// 应该会把头转回来
            return true;
        }
    }
    else //到达目标，停止
    {
        m_car_control.Stop();
        return false;
    }
}


