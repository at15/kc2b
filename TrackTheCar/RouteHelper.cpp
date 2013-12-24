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
    //if(distance>DISTANCE_ERROR) return distance;
    //else return 0;
    return distance;
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

int CLine::FindNearestLine( CvPoint p,const vector<CLine>& v_lines )
{
     double min_distance = v_lines.at(0).PointDist(p);
     int n = 0;
     for(int i=1;i<v_lines.size();i++){
         double dist = v_lines.at(i).PointDist(p);
         if(dist < min_distance){
             min_distance = dist;
             n = i;
         }
     }
     return n;
}

double CLine::Distance( CvPoint p1,CvPoint p2 )
{
    // d^2 = (x1 - x2)^2 + (y1 - y2)^2
    return sqrtl((p1.x - p2.x)*(p1.x - p2.x)
        + (p1.y - p2.y)*(p1.y - p2.y));
}
