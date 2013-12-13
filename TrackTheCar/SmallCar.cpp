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

    // check if we can start the car, if we can, then lets go
    if(!m_car_control.Init(m_config->GetCOM())) return false;

    return true;
}

bool CSmallCar::Init(CCvPicCtrl* camera,CCvPicCtrl* output,CConfigs* config){
    return Init(camera,output,config->GetMapPoint(),config);
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

bool CSmallCar::MoveCarP2P(CvPoint& from,CvPoint& to){
    CImageProc proc;
    m_output->SetCurrentFrame(m_camera->GetCurrentFrame(),false);

    //��С����������,��������Ȧ
    CvPoint carPos = GetCarPosEx();
    cvCircle(m_output->GetCurrentFrame(),carPos,10,CV_RGB(0,255,0),3);// a green circle for the pos
    m_output->UpdateFrame();// show it
    m_current_point = carPos;

    from = carPos;// for log

    //���¸��㣬������Ȧ
    // The to has never changed!
    CvPoint nextPoint = m_route.FindnextPoint(m_current_point,m_map_point,m_pass_point);
    cvCircle(m_output->GetCurrentFrame(),nextPoint,10,CV_RGB(255,255,50),3);// a green circle for the pos
    m_output->UpdateFrame();// show it

    to = nextPoint;// for log

    //��С����������
    double direction_car=m_route.Angle(m_tail,m_head); 
    //С�����ĵ���һ��Ŀ������������
    double direction_target=m_route.Angle(carPos,nextPoint); 
    //С�����ĵ�Ŀ���ľ���
    double distance=m_route.Distance(carPos,nextPoint); 


    if(distance>DISTANCE_ERROR) //δ����Ŀ�꣬��������
    {
        if(fabs(direction_car - direction_target) > ANGLE_ERROR) //С����Ŀ�겻��ͬһ������ת��
        {
            if(direction_car<180)
            {
                if(direction_target>direction_car && direction_target<direction_car+180) //��ת
                {
                    m_car_control.GoLeft();
                    return true;
                }
                else //��ת
                {
                    m_car_control.GoRight();
                    return true;
                }
            }
            else
            {
                if(direction_target>direction_car-180 && direction_target<direction_car) //��ת
                {
                    m_car_control.GoRight();
                    return true;
                }
                else //��ת
                {
                    m_car_control.GoLeft();
                    return true;
                }
            }
        }
        else //С����Ŀ������ͬһ��������ǰ��
        {
            m_car_control.GoForward();// Ӧ�û��ͷת����
            return true;
        }
    }
    else //����Ŀ��㣬ֹͣ������
    {
        m_car_control.Stop();
        return false;
    }
}


