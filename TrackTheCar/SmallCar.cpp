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

CSmallCar::CAR_ERROR CSmallCar::Init( CCvPicCtrl* camera,CCvPicCtrl* output_map,CCvPicCtrl* output_car,CGConfigs* config )
{
    // check if we can connect the car via blue tooth
    if(!m_car_control.Init(config->com_port.Get())) return CANT_CONNECT_CAR;
    // check if we can find the car
    if(!GetCarInfo(m_car_info)) return CANT_FIND_CAR;
    m_camera = camera;
    m_output_map = output_map;
    m_output_car = output_car;
    m_config = config;
    // sort the lines
    m_map_line = m_proc.SortLines(m_config->raw_line.Get(),
        m_car_info.head,m_car_info.tail);
    m_config->sorted_line.Set(m_map_line);
    // show the image in the output
    cvCircle(m_output_car->GetCurrentFrame(),m_car_info.core,10,CV_RGB(0,255,0),3);
    // show the new map in the output
    m_proc.DrawLines(m_output_map->GetCurrentFrame(),m_config->sorted_line.Get());
    m_output_map->UpdateFrame();
    m_output_car->UpdateFrame();
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

bool CSmallCar::GetCarInfo()
{
    CvPoint head,tail;
    IplImage* t = m_camera->GetCurrentFrame();

    head = m_proc.GetRedCore(t,m_config->red_threshold.Get());
    tail = m_proc.GetBlueCore(t,m_config->blue_threshold.Get());

    if(head.x < 0 || head.y <0 || tail.x < 0 || tail.y < 0 ){
        return false;// can't find the car
    }else{
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

//CSmallCar::MOVE_RESULT CSmallCar::Move2NextPoint(int distance_error/* = DISTANCE_ERROR*/,
//    int angle_error/* = ANGLE_ERROR*/){
//        CImageProc proc;
//        m_output->SetCurrentFrame(m_camera->GetCurrentFrame(),false);
//
//        // a green circle to show the car pos
//        cvCircle(m_output->GetCurrentFrame(),m_current_car_pos,10,CV_RGB(0,255,0),3);
//        // a yellow circle for the dst
//        cvCircle(m_output->GetCurrentFrame(),m_next_point,10,CV_RGB(255,255,50),3);
//        m_output->UpdateFrame();// why the hell i deleted this?
//
//        //��С����������
//        double direction_car=m_route.Angle(m_tail,m_head);
//        //С�����ĵ���һ��Ŀ������������
//        double direction_target=m_route.Angle(m_current_car_pos,m_next_point);
//        //С�����ĵ�Ŀ���ľ���
//        double distance=m_route.Distance(m_current_car_pos,m_next_point);
//        
//        // �жϳ��ǲ��ǿ�ס��
//
//        // �ﵽĿ��
//        if(distance <= distance_error) {
//            m_car_control.Stop();
//            return REACH_POINT;
//        }
//        // �������С������Ǿ�pass����
//        if(fabs(direction_car - direction_target)> 90 &&
//           fabs(direction_car - direction_target)< 270 ){
//               m_car_control.Stop();
//               return PASS_POINT;
//        }
//        // ת��֮�������ǰ�ߣ������һֱ������
//        if(fabs(direction_car - direction_target) > angle_error) //С����Ŀ�겻��ͬһ������ת��
//        {
//            if(direction_car<180)
//            {
//                if(direction_target>direction_car && direction_target<direction_car+180) //��ת
//                {
//                    m_car_control.GoLeft();
//                    //m_car_control.GoForward();
//                    return TURN_LEFT;
//                }
//                else //��ת
//                {
//                    m_car_control.GoRight();
//                    //m_car_control.GoForward();
//                    return TURN_RIGHT;
//                }
//            }
//            else
//            {
//                if(direction_target>direction_car-180 && direction_target<direction_car) //��ת
//                {
//                    m_car_control.GoRight();
//                    //m_car_control.GoForward();
//                    return TURN_RIGHT;
//                }
//                else //��ת
//                {
//                    m_car_control.GoLeft();
//                    //m_car_control.GoForward();
//                    return TURN_LEFT;
//                }
//            }
//        }
//        else //С����Ŀ������ͬһ��������ǰ��
//        {
//            m_car_control.GoForward();// Ӧ�û��ͷת����
//            return GO_FORWARD;
//        }
//}

CSmallCar::MOVE_RESULT CSmallCar::MoveCar(CString& log_str,CString& error_str)
{
    // 1 determine what to do
    // 2 draw the result on the screen
    if(!GetCarInfo(m_car_info)){
        log_str = L"error:";
        error_str = L"can't find the car";
        return MOVE_ERROR;
    }
    if(m_config->sorted_line.Get().size() == (m_current_line_index+1)){
        log_str = L"Reach last point";
        error_str = L"";
        return REACH_END;
    }

    // move the car

    CLine current_line;
    m_config->sorted_line.GetItem(m_current_line_index,current_line);

    //��С����������
    double direction_car=m_route.Angle(m_car_info.tail,m_car_info.head);
    //С�����ĵ���һ��Ŀ������������
    double direction_target=m_route.Angle(m_car_info.core,current_line.end());
    //С�����ĵ�Ŀ���ľ���
    double distance=CLine::Distance(m_car_info.core,current_line.end());

    // reach the target
    if(distance <= distance_error) {
        m_car_control.Stop();
        return REACH_POINT;
    }

    // pass the point if it is behind the car
    if(fabs(direction_car - direction_target)> 90 &&
        fabs(direction_car - direction_target)< 270 ){
            m_car_control.Stop();
            m_current_line_index++;
            return PASS_POINT;
    }

    
    if(fabs(direction_car - direction_target) > angle_error) //С����Ŀ�겻��ͬһ������ת��
    {
        if(direction_car<180)
        {
            if(direction_target>direction_car && direction_target<direction_car+180) //��ת
            {
                m_car_control.GoLeft();
                //m_car_control.GoForward();
                return TURN_LEFT;
            }
            else //��ת
            {
                m_car_control.GoRight();
                //m_car_control.GoForward();
                return TURN_RIGHT;
            }
        }
        else
        {
            if(direction_target>direction_car-180 && direction_target<direction_car) //��ת
            {
                m_car_control.GoRight();
                //m_car_control.GoForward();
                return TURN_RIGHT;
            }
            else //��ת
            {
                m_car_control.GoLeft();
                //m_car_control.GoForward();
                return TURN_LEFT;
            }
        }
    }
    else //С����Ŀ������ͬһ��������ǰ��
    {
        m_car_control.GoForward();// Ӧ�û��ͷת����
        return GO_FORWARD;
    }
    return REACH_POINT;
}

