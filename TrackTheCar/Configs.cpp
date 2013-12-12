#include "StdAfx.h"
#include "Configs.h"
using namespace std;

CConfigs::CConfigs(void)
{
    m_map_corners.clear();
    m_threshold.clear();
    LoadDefaultSetting();
}


CConfigs::~CConfigs(void)
{
}

void CConfigs::LoadDefaultSetting(){
    // the default setting , for the threshold only at this moment
    // init the threshold value
    m_threshold.push_back(150);
    m_threshold.push_back(138);
    m_threshold.push_back(40);
    m_threshold.push_back(100);
    m_threshold.push_back(80);
    m_threshold.push_back(80);

    // for finding point in the map
    m_map_threshold = 70;
    m_thin_iteration = 20; // 20 times for the thin


    // the com port for blue tooth
    m_com_num = 3;

}

std::vector<CvPoint> CConfigs::GetMapCorner(){
    if(m_map_corners.empty()){
        throw logic_error("map corners not set!");
    }else{
        return m_map_corners;
    }
}

void CConfigs::SetMapCorners(std::vector<CvPoint> corners){
    // TODO:check the value
    m_map_corners = corners;
}

void CConfigs::SetThreshold(std::vector<int> threshold){
    // TODO:check the value
    m_threshold = threshold;
}

std::vector<int> CConfigs::GetThreshold(){
    if(m_threshold.empty()){
        throw logic_error("threshold not set!");
    }else{
        return m_threshold;
    }
}