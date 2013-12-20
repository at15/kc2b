#include "StdAfx.h"
#include "Configs.h"
using namespace std;

CConfigs::CConfigs(void)
{
    RestConfig();
    // LoadDefaultSetting();
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
    m_map_threshold = 99;
    m_thin_iteration = 20; // 20 times for the thin

    m_mcv_quality_level = MCV_QUALITY_LEVEL;
    m_mcv_min_distance = MCV_MIN_DISTANCE;

    m_line_distance_error = LINE_DISTANCE_ERROR;

    // the com port for blue tooth
    m_com_num = 7;

    // distance error and angle error
    m_distance_error = DISTANCE_ERROR;
    m_angle_error = ANGLE_ERROR;

}

void CConfigs::RestConfig(){
    m_btransform_set = false;
    m_bmap_point_set = false;
    m_map_corners.clear();
    m_map_point.clear();
    m_map_line.clear();
    m_threshold.clear();
    LoadDefaultSetting();
}

std::vector<CvPoint> CConfigs::GetMapCorner(){
    if(m_map_corners.empty()){
        throw logic_error("map corners not set!");
    }else{
        return m_map_corners;
    }
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

std::vector<CvPoint2D32f> CConfigs::GetMapPoint(){
    if(m_map_point.empty()){
        throw logic_error("map point not set!");
    }else{
        return m_map_point;
    }
}

vector<CLine> CConfigs::GetMapLine(){
    if(m_map_line.empty()){
        throw logic_error("map line not set!");
    }else{
        return m_map_line;
    }
}