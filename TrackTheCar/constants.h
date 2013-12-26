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
// Hough�任��ֱ��
#define MCV_LINE_EXIST 10 // �߶��Ƿ���ڵķ�ֵ
#define MCV_MIN_LINE_LENGTH 20 // ����߶εĳ��ȣ��������Ľ�������
#define MCV_MAX_LINE_DISTANCE 40 // �߶μ�����ֵ��С�������߶ν�������һ��??

#define LINE_DISTANCE_ERROR 10 // ȥ���ظ�ֱ��ʱ����Ķ˵�����,С��������Ϊ�ظ�
#define LINE_POINT_DIST 10 // �߶�����ʱ�����˵����С������Ϊһ�㣬��������һ��ֱ��

// for route helper functions
#define DISTANCE_ERROR 30 //������С�ڵ���distance_error����ʱ������Ϊ�ѵ���õ�
#define ANGLE_ERROR 20 // ���Ƕ�С��distance_error����ʱ������Ϊ��0
#define ANGLE_SIN_OFFSET 0.3	//���ƫ�ƽǶ�����

#define CAR_S_SLEEP_TIME 20