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


bool CSmallCar::Init(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config){
    // check if we can start the car, if we can, then lets go
    if(!m_car_control.Init(config->GetCOM())) return false;

    m_camera = camera;
    m_output = output;

    m_map_point.clear();
    m_pass_point.clear();
    for(int i=0;i<config->GetMapPoint().size();i++){
        m_map_point.push_back(cvPointFrom32f(config->GetMapPoint().at(i)));
        m_pass_point.push_back(false);
    }
    m_config = config;
    return true;
}

bool CSmallCar::GetCarInfo(CarInfo& info){
    CvPoint head,tail,core;
    IplImage* t = m_camera->GetCurrentFrame();

    head = m_proc.GetRedCore(t,m_config->GetThreshold());
    tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    // 没有找到小车
    if(head.x < 0 || head.y <0 || tail.x < 0 || tail.y < 0 ){
        return false;
    }

    core = cvPoint((head.x+tail.x)/2,(head.y+tail.y)/2);

    double head_tail_dist = m_route.Distance(m_head,m_tail);
    int car_length = head_tail_dist / HEADTAIL_DISTANCE * CAR_LENGTH;

    info.head = head;
    info.tail = tail;
    info.core = core;
    info.length = car_length;

    return true;
}

bool CSmallCar::StartCar(){
    if(FIND_POINT::FAIL == GetCarPosEx()){
        return false;
    }
    return true;

}

bool CSmallCar::SpeedUp(){
    if(!m_car_control.SpeedUp()){
        return false;
    }
    // 不改变方向，只是为了加速
    m_car_control.GoForward(false);
    Sleep(CAR_S_SLEEP_TIME);// 让小车跑一会
    m_car_control.SpeedDown();
    m_car_control.GoForward(false);
}

bool CSmallCar::StopCar(){
    // stop the car
    m_car_control.Stop();
    return true;
}

double CSmallCar::GetCarLength(){
    double head_tail_dist = m_route.Distance(m_head,m_tail);
    double car_length = head_tail_dist / HEADTAIL_DISTANCE * CAR_LENGTH;
    return car_length;
}

CSmallCar::FIND_POINT CSmallCar::GetCarPosEx(CvPoint* car_pos /*= NULL*/){
    IplImage* t = m_camera->GetCurrentFrame();
    m_head = m_proc.GetRedCore(t,m_config->GetThreshold());
    m_tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    if(m_head.x < 0 || m_head.y <0 || m_tail.x < 0 || m_tail.y < 0 ){
        //throw logic_error("can't find car position");
        return FIND_POINT::FAIL;
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

CSmallCar::MOVE_RESULT CSmallCar::Move2NextPoint(int distance_error/* = DISTANCE_ERROR*/,
    int angle_error/* = ANGLE_ERROR*/){
        CImageProc proc;
        m_output->SetCurrentFrame(m_camera->GetCurrentFrame(),false);

        // a green circle to show the car pos
        cvCircle(m_output->GetCurrentFrame(),m_current_car_pos,10,CV_RGB(0,255,0),3);
        // a yellow circle for the dst
        cvCircle(m_output->GetCurrentFrame(),m_next_point,10,CV_RGB(255,255,50),3);
        m_output->UpdateFrame();// why the hell i deleted this?

        //求小车向量方向
        double direction_car=m_route.Angle(m_tail,m_head);
        //小车中心到下一个目标点的向量方向
        double direction_target=m_route.Angle(m_current_car_pos,m_next_point);
        //小车中心到目标点的距离
        double distance=m_route.Distance(m_current_car_pos,m_next_point);
        
        // 判断车是不是卡住了

        // 达到目标
        if(distance <= distance_error) {
            //m_car_control.Stop();
            return REACH_POINT;
        }
        // 如果点在小车身后那就pass了它
        if(fabs(direction_car - direction_target)> 90 &&
           fabs(direction_car - direction_target)< 270 ){
               m_car_control.Stop();
               return PASS_POINT;
        }
        // 转弯之后必须向前走，否则就一直卡那了
        if(fabs(direction_car - direction_target) > angle_error) //小车与目标不在同一方向，则转向
        {
            if(direction_car<180)
            {
                if(direction_target>direction_car && direction_target<direction_car+180) //左转
                {
#ifdef USE_BACK
                    m_car_control.GoBack();
                    Sleep(CAR_S_SLEEP_TIME);
                    m_car_control.GoForward();
#endif
                    m_car_control.GoLeft();
                    //m_car_control.GoForward();
                    return TURN_LEFT;
                }
                else //右转
                {
#ifdef USE_BACK
                    m_car_control.GoBack();
                    Sleep(CAR_S_SLEEP_TIME);
                    m_car_control.GoForward();
#endif
                    m_car_control.GoRight();
                    //m_car_control.GoForward();
                    return TURN_RIGHT;
                }
            }
            else
            {
                if(direction_target>direction_car-180 && direction_target<direction_car) //右转
                {
#ifdef USE_BACK
                    m_car_control.GoBack();
                    Sleep(CAR_S_SLEEP_TIME);
                    m_car_control.GoForward();
#endif
                    m_car_control.GoRight();
                    //m_car_control.GoForward();
                    return TURN_RIGHT;
                }
                else //左转
                {
#ifdef USE_BACK
                    m_car_control.GoBack();
                    Sleep(CAR_S_SLEEP_TIME);
                    m_car_control.GoForward();
#endif
                    m_car_control.GoLeft();
                    //m_car_control.GoForward();
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

bool CSmallCar::isCarStuck(){
    if(m_stuck_info.empty){
        // 初始化
        m_stuck_info.firstPoint = m_current_car_pos;
        m_stuck_info.stuck_distance = GetCarLength() * CAR_S_DISTANCE_ERROR_P;
        m_stuck_info.empty = false;
        return false;// 第一使用，肯定是没卡
    }

    // 判断是否卡住了
    double dist = m_route.Distance(m_stuck_info.firstPoint,m_current_car_pos);
    if(dist < m_stuck_info.stuck_distance){
        // 距离小，卡住了+1
        m_stuck_info.stuck_time++;
        // 卡够一定次数才算卡住
        if(m_stuck_info.stuck_time > m_stuck_info.max_stuck_time ){
            return true;
        }else{
            return false;// 卡的次数还不够多
        }
            
    }else{
        // 没有卡住
        m_stuck_info.firstPoint = m_current_car_pos;
        m_stuck_info.stuck_time = 0;
        return false;
    }
}