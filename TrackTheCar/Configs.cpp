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
    // for finding point in the map
    m_map_threshold = 99;
    m_thin_iteration = 20; // 20 times for the thin

    //m_mcv_quality_level = MCV_QUALITY_LEVEL;
    //m_mcv_min_distance = MCV_MIN_DISTANCE;

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

void CGConfigs::ResetConfig()
{
    LoadDefault();
}

void CGConfigs::LoadDefault()
{
    // init the threshold value
    default_threshold.PushBack(150);
    default_threshold.PushBack(138);
    default_threshold.PushBack(40);
    default_threshold.PushBack(100);
    default_threshold.PushBack(80);
    default_threshold.PushBack(80);

    red_threshold = default_threshold;
    blue_threshold = default_threshold;

    map_corner_height.Set(CORNER_SIZE);
    map_corner_width.Set(CORNER_SIZE);

    map_threshold.Set(MAP_THRESHOLD);
    map_thin_iteration.Set(MAP_THIN_ITERATION);

    line_distance_error.Set(LINE_DISTANCE_ERROR);

    route_distance_error.Set(DISTANCE_ERROR);
    route_angle_error.Set(ANGLE_ERROR);

    // default com port
    com_port.Set(7);
}

CGConfigs::CGConfigs()
{
    LoadDefault();
}
