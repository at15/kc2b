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

/* kind of useless... i have to say, just the same as Init
bool Prepare(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config){

}*/

bool CSmallCar::StartCar(){
    if(FAIL == GetCarPosEx()){
        return false;
    }
    return true;

}

bool CSmallCar::SpeedUp(){
    if(!m_car_control.SpeedUp()){
        return false;
    }
    // ���ı䷽��ֻ��Ϊ�˼���
    m_car_control.GoForward(false);
    Sleep(CAR_S_SLEEP_TIME);// ��С����һ��
    m_car_control.SpeedDown();
    m_car_control.GoForward(false);
}

bool CSmallCar::StopCar(){
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
        return FAIL;
    }
    CvPoint carPos = cvPoint((m_head.x+m_tail.x)/2,(m_head.y+m_tail.y)/2);
    m_current_car_pos = carPos;
    if(car_pos){
        *car_pos = carPos;
    }
    return OK;
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

        //��С����������
        double direction_car=m_route.Angle(m_tail,m_head);
        //С�����ĵ���һ��Ŀ������������
        double direction_target=m_route.Angle(m_current_car_pos,m_next_point);
        //С�����ĵ�Ŀ���ľ���
        double distance=m_route.Distance(m_current_car_pos,m_next_point);
        
        // �жϳ��ǲ��ǿ�ס��

        // �ﵽĿ��
        if(distance <= distance_error) {
            m_car_control.Stop();
            return REACH_POINT;
        }
        // �������С������Ǿ�pass����
        if(fabs(direction_car - direction_target)> 90 &&
           fabs(direction_car - direction_target)< 270 ){
               m_car_control.Stop();
               return PASS_POINT;
        }
        // ת��֮�������ǰ�ߣ������һֱ������
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
}

bool CSmallCar::isCarStuck(){
    if(m_stuck_info.empty){
        // ��ʼ��
        m_stuck_info.firstPoint = m_current_car_pos;
        m_stuck_info.stuck_distance = GetCarLength() * CAR_S_DISTANCE_ERROR_P;
        m_stuck_info.empty = false;
        return false;// ��һʹ�ã��϶���û��
    }

    // �ж��Ƿ�ס��
    double dist = m_route.Distance(m_stuck_info.firstPoint,m_current_car_pos);
    if(dist < m_stuck_info.stuck_distance){
        // ����С����ס��+1
        m_stuck_info.stuck_time++;
        // ����һ���������㿨ס
        if(m_stuck_info.stuck_time > m_stuck_info.max_stuck_time ){
            return true;
        }else{
            return false;// ���Ĵ�����������
        }
            
    }else{
        // û�п�ס
        m_stuck_info.firstPoint = m_current_car_pos;
        m_stuck_info.stuck_time = 0;
        return false;
    }
}

// The new routine
bool CSmallCar::GetNewLines(const std::vector<CLine>& o_lines){
     if(o_lines.empty()){
            throw logic_error("can't sort empty vector<CLine>");
        }

        CvPoint start_piont;
        CvPoint car_head = m_car_info.head;
        CvPoint car_tail = m_car_info.tail;

        start_piont.x = (car_head.x + car_tail.x)/2;
        start_piont.y = (car_head.y + car_tail.y)/2;

        // find the first line
        double min_distance = o_lines.at(0).PointDist(start_piont);
        int i_first_line = 0;
        for(int i=0;i<o_lines.size();i++){
            double dist = o_lines.at(i).PointDist(start_piont);
            if(dist < min_distance){
                min_distance = dist;
                i_first_line = i;
            }
        }

        vector<CLine> sorted_lines;
        CLine current_line = o_lines.at(i_first_line);
        // �ж��߶ζ˵�λ��
        if(current_line.StartDist(car_tail) > current_line.StartDist(car_head)){
            current_line.Swap();
        }
        // ������endȥ����һ��ֱ��
        // ������С��ÿ���ƶ���һ��ֱ��֮����ȥ��
}