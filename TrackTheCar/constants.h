#pragma once
// define some vars
#define CORNER_SIZE 10 // the size of the red corner
#define PI 3.1415926 // PI

// the param for the little car
#define CAR_LENGTH 15
#define HEADTAIL_DISTANCE 8.5 

// for opencv functions
// ����Ѱ�ҽǵ� cvgoodfeaturestotrack
#define MCV_QUALITY_LEVEL 0.10
#define MCV_MIN_DISTANCE 10
// Hough�任��ֱ��
#define MCV_LINE_EXIST 10 // �߶��Ƿ���ڵķ�ֵ
#define MCV_MIN_LINE_LENGTH 20 // ����߶εĳ��ȣ��������Ľ�������
#define MCV_MAX_LINE_DISTANCE 40 // �߶μ�����ֵ��С�������߶ν�������һ��??

#define LINE_DISTANCE_ERROR 10 // ȥ���ظ�ֱ��ʱ����Ķ˵�����,С��������Ϊ�ظ�

// for route helper functions
#define DISTANCE_ERROR 30 //������С�ڵ���distance_error����ʱ������ΪΪ0
#define ANGLE_ERROR 20 // ���Ƕ�С��distance_error����ʱ������Ϊ��0

// for control the car in MainDlg
#define MAX_OP_TIME 20 // С����һ���������ظ��������������ͻ��޸����ֵ
#define MAX_ERROR_MODIFY_TIME 5 // �������޸ļ������ֵ
#define ERROR_MODIFY_VALUE 5 // �����޸ĵ����ֵ

// ȷ��С���ǲ��ǿ��������ֵ
#define CAR_S_MAX_TIME  10// ��һ���㸽�����˶��ٴ�֮����㿨ס��
#define CAR_S_DISTANCE_ERROR_P 0.1 // ����һ���������㿨����һ��,
// 0.1��ʾ��������ǳ�����0.1