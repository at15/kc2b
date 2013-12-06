#include "StdAfx.h"
#include "SmallCar.h"


CSmallCar::CSmallCar(void)
{
}


CSmallCar::~CSmallCar(void)
{
}

// we need 
// 1 a picture control that can always give us the map
//   PS: we only need a processed image now
// 2 the camera that can get the latest image, so we can know where the car is
bool CSmallCar::Init(CCvPicCtrl* camera,IplImage* map,CConfigs* config){
    m_camera = camera;
    m_map.SetImage(map);
    m_config = config;
    return true;
}

bool CSmallCar::GetCarPos(){
    IplImage* t = m_camera->GetCurrentFrame();
    m_head = m_proc.GetRedCore(t,m_config->GetThreshold());
    m_tail = m_proc.GetBlueCore(t,m_config->GetThreshold());
    return true;
}

void CSmallCar::GoLeft(){

}

void CSmallCar::GoRight(){

}

void CSmallCar::GoBack(){

}

void CSmallCar::GoForward(){

}