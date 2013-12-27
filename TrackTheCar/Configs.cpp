#include "StdAfx.h"
#include "Configs.h"
using namespace std;

void CGConfigs::ResetConfig()
{
    // clear all the setting
    map_corner.Clear();
    raw_line.Clear();
    sorted_line.Clear();
    default_threshold.Clear();
    red_threshold.Clear();
    blue_threshold.Clear();
    LoadDefault();
}

void CGConfigs::LoadDefault()
{
    // init the threshold value
    default_threshold.Clear();
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
    com_port.Set(COM_PORT_NUM);
}

CGConfigs::CGConfigs()
{
    LoadDefault();
}
