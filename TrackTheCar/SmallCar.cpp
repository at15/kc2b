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

CSmallCar::CAR_ERROR CSmallCar::Init( CCvPicCtrl* camera,CCvPicCtrl* output_map,CCvPicCtrl* output_car,CConfigs* config )
{
    // check if we can connect the car via bluetooth
    if(!m_car_control.Init(config->GetCOM())) return CANT_CONNECT_CAR;
    // check if we can find the car
    if(!GetCarInfo(m_car_info)) return CANT_FIND_CAR;
    m_camera = camera;
    m_output_map = output_map;
    m_config = config;
    return NO_CAR_ERROR;
}

bool CSmallCar::GetCarInfo(CarInfo& info){
    CvPoint head,tail,core;
    IplImage* t = m_camera->GetCurrentFrame();

    head = m_proc.GetRedCore(t,m_config->GetThreshold());
    tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    // can't find the car
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
    m_car_control.GoForward();
    return true;
}

bool CSmallCar::SpeedUp(){
    if(!m_car_control.SpeedUp()){
        return false;
    }
    // don't change the direction
    m_car_control.GoForward(false);
    Sleep(CAR_S_SLEEP_TIME);// just let the car run for a while
    m_car_control.SpeedDown();
    m_car_control.GoForward(false);
}

bool CSmallCar::StopCar(){
    m_car_control.Stop();
    return true;
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
            m_car_control.Stop();
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
                    m_car_control.GoLeft();
                    //m_car_control.GoForward();
                    return TURN_LEFT;
                }
                else //右转
                {
                    m_car_control.GoRight();
                    //m_car_control.GoForward();
                    return TURN_RIGHT;
                }
            }
            else
            {
                if(direction_target>direction_car-180 && direction_target<direction_car) //右转
                {
                    m_car_control.GoRight();
                    //m_car_control.GoForward();
                    return TURN_RIGHT;
                }
                else //左转
                {
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

CSmallCar::MOVE_RESULT CSmallCar::CarProc()
{
    // 1 determine what to do
    // 2 draw the result on the screen

}

