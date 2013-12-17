#pragma once
// define some vars
#define CORNER_SIZE 10 // the size of the red corner
#define PI 3.1415926 // PI
// for opencv functions
#define MCV_QUALITY_LEVEL 0.10
#define MCV_MIN_DISTANCE 10

// for route helper functions
#define DISTANCE_ERROR 30 //当距离小于等于distance_error像素时，可认为为0
#define ANGLE_ERROR 20 // 当角度小于distance_error像素时，可认为是0

// for control the car in MainDlg
#define MAX_OP_TIME 20 // 小车单一操作最多的重复次数，超过它就会修改误差值
#define MAX_ERROR_MODIFY_TIME 5 // 最多可以修改几次误差值
#define ERROR_MODIFY_VALUE 5 // 单次修改的误差值