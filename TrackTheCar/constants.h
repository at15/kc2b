#pragma once
// define some vars
#define CORNER_SIZE 10 // the size of the red corner
#define PI 3.1415926 // PI

// the param for the little car
#define CAR_LENGTH 15
#define HEADTAIL_DISTANCE 8.5 

// for opencv functions
// 用于寻找角点 cvgoodfeaturestotrack
#define MCV_QUALITY_LEVEL 0.10
#define MCV_MIN_DISTANCE 10
// Hough变换找直线
#define MCV_LINE_EXIST 10 // 线段是否存在的阀值
#define MCV_MIN_LINE_LENGTH 20 // 最短线段的长度，短于它的将被忽略
#define MCV_MAX_LINE_DISTANCE 40 // 线段间隔最大值，小于它，线段将被连在一起??

#define LINE_DISTANCE_ERROR 10 // 去除重复直线时允许的端点间距离,小于它被认为重复

// for route helper functions
#define DISTANCE_ERROR 30 //当距离小于等于distance_error像素时，可认为为0
#define ANGLE_ERROR 20 // 当角度小于distance_error像素时，可认为是0

// for control the car in MainDlg
#define MAX_OP_TIME 20 // 小车单一操作最多的重复次数，超过它就会修改误差值
#define MAX_ERROR_MODIFY_TIME 5 // 最多可以修改几次误差值
#define ERROR_MODIFY_VALUE 5 // 单次修改的误差值

// 确定小车是不是卡在那里的值
#define CAR_S_MAX_TIME  10// 在一个点附近卡了多少次之后就算卡住了
#define CAR_S_DISTANCE_ERROR_P 0.1 // 离上一个点多近才算卡在这一点,
// 0.1表示这个距离是车长的0.1