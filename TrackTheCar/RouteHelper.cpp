#include "StdAfx.h"
#include "RouteHelper.h"
using namespace std;

CRouteHelper::CRouteHelper(void)
{
}


CRouteHelper::~CRouteHelper(void)
{
}

double CRouteHelper::Distance(CvPoint src,CvPoint dst) //求两整数点距离，当距离小于等于distance_error像素时，可认为为0
{
    double distance=sqrtl((src.x-dst.x)*(src.x-dst.x)+(src.y-dst.y)*(src.y-dst.y));
    if(distance>DISTANCE_ERROR) return distance;
    else return 0;
}

double CRouteHelper::Angle(CvPoint src,CvPoint dst)//返回向量极坐标方向角，0～360度
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


CvPoint CRouteHelper::FindnextPoint(CvPoint current,vector<CvPoint> map_points,vector<bool>& mask) //寻找下一个点，即返回与当前点最近的点
{
    double min_distance=1000.0;// just make it big enough
    int nearest_num=-1;
    for(int i=0;i<map_points.size();i++)
    {
        if(mask.at(i)) continue; // pass the points that are passed
        double dist=Distance(current,map_points.at(i));
        if(min_distance>dist)
        {
            min_distance=dist;
            nearest_num=i;
        }
    }
    mask.at(nearest_num)=true;
    return map_points.at(nearest_num);
}

