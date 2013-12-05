#include "StdAfx.h"
#include "Configs.h"
using namespace std;

CConfigs::CConfigs(void)
{
   // m_map_corners.empty();
   // m_threshold.empty();
    m_map_corners.clear();
    m_threshold.clear();
}


CConfigs::~CConfigs(void)
{
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