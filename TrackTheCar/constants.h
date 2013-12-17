#pragma once
// define some vars
#define CORNER_SIZE 10 // the size of the red corner
#define PI 3.1415926 // PI
// for opencv functions
#define MCV_QUALITY_LEVEL 0.10
#define MCV_MIN_DISTANCE 10

// for route helper functions
#define DISTANCE_ERROR 30 //������С�ڵ���distance_error����ʱ������ΪΪ0
#define ANGLE_ERROR 20 // ���Ƕ�С��distance_error����ʱ������Ϊ��0

// for control the car in MainDlg
#define MAX_OP_TIME 20 // С����һ���������ظ��������������ͻ��޸����ֵ
#define MAX_ERROR_MODIFY_TIME 5 // �������޸ļ������ֵ
#define ERROR_MODIFY_VALUE 5 // �����޸ĵ����ֵ