#include "StdAfx.h"
#include "SmallCar.h"
using namespace std;

CSmallCar::CSmallCar(void)
{
    m_current_line_index = 0;
}

CSmallCar::~CSmallCar(void)
{
}

CSmallCar::CAR_ERROR CSmallCar::Init( CCvPicCtrl* camera,CGConfigs* config )
{
    m_camera = camera;
    m_config = config;

    // check if we can connect the car via blue tooth
    if(!m_car_control.Init(config->com_port.Get())) return CANT_CONNECT_CAR;
    
    // check if we can find the car
    if(!GetCarInfo(m_car_info)) return CANT_FIND_CAR;
    
    // sort the lines
    m_map_line = m_proc.SortLines(m_config->raw_line.Get(),
        m_car_info.head,m_car_info.tail);
    m_config->sorted_line.Set(m_map_line);
    
    return NO_CAR_ERROR;
}

bool CSmallCar::GetCarInfo(CarInfo& info){
    CvPoint head,tail,core;
    IplImage* t = m_camera->GetCurrentFrame();

    head = m_proc.GetRedCore(t,m_config->red_threshold.Get());
    tail = m_proc.GetBlueCore(t,m_config->blue_threshold.Get());
    // can't find the car
    if(head.x < 0 || head.y <0 || tail.x < 0 || tail.y < 0 ){
        return false;
    }

    core = cvPoint((head.x+tail.x)/2,(head.y+tail.y)/2);

    double head_tail_dist = CLine::Distance(head,tail);
    double car_length = head_tail_dist / HEADTAIL_DISTANCE * CAR_LENGTH;

    info.head = head;
    info.tail = tail;
    info.core = core;
    info.length = car_length;

    return true;
}

bool CSmallCar::GetCarInfo(CvPoint* head,CvPoint* tail,CvPoint* target)
{
    CvPoint t_head,t_tail;
    IplImage* t = m_camera->GetCurrentFrame();

    t_head = m_proc.GetRedCore(t,m_config->red_threshold.Get());
    t_tail = m_proc.GetBlueCore(t,m_config->blue_threshold.Get());

    if(t_head.x < 0 || t_head.y <0 || t_tail.x < 0 || t_tail.y < 0 ){
        return false;// can't find the car
    }else{
        *head = t_head;
        *tail = t_tail;
        *target = m_config->sorted_line.Get().at(m_current_line_index).end();
        return true;
    }
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

CSmallCar::MOVE_RESULT CSmallCar::MoveCar(CString& log_str,CString& error_str)
{
    error_str = L"";

    if(!GetCarInfo(m_car_info)){
        log_str = L"error:";
        error_str = L"can't find the car";
        return MOVE_ERROR;
    }

    // reach the end
    if(m_config->sorted_line.Get().size() == (m_current_line_index+1)){
        m_car_control.Stop();
        log_str = L"Reach last point";
        return REACH_END;
    }

    // move the car

    CLine current_line;
    current_line = m_config->sorted_line.Get().at(m_current_line_index);
  
    double distance=CLine::Distance(m_car_info.head,current_line.end());
     
    // reach the target
    if(distance <= m_config->route_distance_error.Get()) {
        m_car_control.Stop();
        m_current_line_index++;
        log_str.Format(L"reach point x=%d y=%d",current_line.end().x,
            current_line.end().y);
        return REACH_POINT;
    }
    else
    {
        unsigned char data;
        CVector car_vec(m_car_info.tail,m_car_info.head);
        CVector drct_vec(m_car_info.tail,current_line.end());
        double coss = car_vec.x() * drct_vec.x() + car_vec.y() * drct_vec.y();
        double abss = car_vec.ABS() * drct_vec.ABS();
        double sinn = car_vec.x() * drct_vec.y() - car_vec.y() * drct_vec.x();
        coss = coss / abss;
        if( abss < 0 )
        {
            if(coss > 9.5)
            {
                //m_car_control.CMD('q');
                m_car_control.RunCar(CCarControl::mLeft);
            }
            else
            {
                //m_car_control.CMD('w');
                m_car_control.RunCar(CCarControl::mRight);

            }

        }
        else
        {
            if(coss > 0.9) // zhuan quan
            {
                //m_car_control.CMD('e');
                m_car_control.RunCar(CCarControl::kLeft);

            }
            else
            {
                //m_car_control.CMD('r');
                m_car_control.RunCar(CCarControl::kRight);
            }


        }


    }





    /*
    // 判断向左向右
    CVector car_vec(m_car_info.tail,m_car_info.head);
    //CVector drct_vec(current_line.start(),current_line.end());
    CVector drct_vec(m_car_info.tail,current_line.end());
    // PS:如果一直给小车发转向的指令，舵机会坏掉。所以在car control里会记录
    // 上一次的操作，如果相同就不再发出指令。

    if(fabs(car_vec.Cross(drct_vec)) < car_vec.ABS()*drct_vec.ABS()*ANGLE_SIN_OFFSET){
        m_car_control.GoForward();// should go straight
        log_str.Format(L"Forward from x=%d y=%d to x=%d y=%d",
            m_car_info.core.x,
            m_car_info.core.y,
            current_line.end().x,
            current_line.end().y);
        return MOVE_FORWARD;
    }

    if(car_vec.Cross(drct_vec) < 0){
        m_car_control.GoRight();
        log_str.Format(L"Right from x=%d y=%d to x=%d y=%d",
            m_car_info.core.x,
            m_car_info.core.y,
            current_line.end().x,
            current_line.end().y);
        return TURN_RIGHT;
    }else{
        m_car_control.GoLeft();
        log_str.Format(L"Left from x=%d y=%d to x=%d y=%d",
            m_car_info.core.x,
            m_car_info.core.y,
            current_line.end().x,
            current_line.end().y);
        return TURN_LEFT;
    }

    // 需要倒车的功能么？应该不需要吧？
    */
}