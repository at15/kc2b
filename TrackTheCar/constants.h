#pragma once

#define CORNER_SIZE 10 // the size of the red corner
#define PI 3.1415926 // PI

// the param for the little car
#define CAR_LENGTH 15
#define HEADTAIL_DISTANCE 8.5 
#define CAR_SPEED 4

// for the map
#define MAP_THRESHOLD 90
#define MAP_THIN_ITERATION 20
// Hough变换找直线
#define MCV_LINE_EXIST 10 // 线段是否存在的阀值
#define MCV_MIN_LINE_LENGTH 20 // 最短线段的长度，短于它的将被忽略
#define MCV_MAX_LINE_DISTANCE 40 // 线段间隔最大值，小于它，线段将被连在一起??

#define LINE_DISTANCE_ERROR 10 // 去除重复直线时允许的端点间距离,小于它被认为重复
#define LINE_POINT_DIST 10 // 线段排序时，若端点距离小于它合为一点，否则增加一条直线

// for route helper functions
#define DISTANCE_ERROR 30 //当距离小于等于distance_error像素时，可认为已到达该点
#define ANGLE_ERROR 20 // 当角度小于distance_error像素时，可认为是0
#define ANGLE_SIN_OFFSET 0.3	//最大偏移角度正弦

#define CAR_S_SLEEP_TIME 20