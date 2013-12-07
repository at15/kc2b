#include "StdAfx.h"
#include "RouteHelper.h"


CRouteHelper::CRouteHelper(void)
{
}


CRouteHelper::~CRouteHelper(void)
{
}

double CRouteHelper::Distance(CvPoint src,CvPoint dst) //������������룬������С�ڵ���distance_error����ʱ������ΪΪ0
{
    double distance=sqrtl((src.x-dst.x)*(src.x-dst.x)+(src.y-dst.y)*(src.y-dst.y));
    if(distance>DISTANCE_ERROR) return distance;
    else return 0;
}

double CRouteHelper::Angle(CvPoint src,CvPoint dst)//�������������귽��ǣ�0��360��
{
    double angle;
    if(dst.y<=src.y)
    {
        angle=acosl((dst.x-src.x)/Distance(src,dst))/PI*180;
    }
    else
    {
        angle=360-acosl((dst.x-src.x)/Distance(src,dst))/PI*180;
    }
    return angle;
}


CvPoint2D32f nextPoint(CvPoint2D32f current,CvPoint2D32f* points, int num_points, bool* mask) //Ѱ����һ���㣬�������뵱ǰ������ĵ�
{
    double min_distance=1000.0;
    int nearest_num=-1;
    for(int i=0;i<num_points;i++)
    {
        if(mask[i]) continue;
        double dist=Distance( (current),cvPointFrom32f(points[i]));
        if(min_distance>dist)
        {
            min_distance=dist;
            nearest_num=i;
        }
    }
    mask[nearest_num]=1;
    return points[nearest_num];
}