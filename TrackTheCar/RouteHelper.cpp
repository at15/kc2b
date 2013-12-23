#include "StdAfx.h"
#include "RouteHelper.h"
using namespace std;

CRouteHelper::CRouteHelper(void)
{
}


CRouteHelper::~CRouteHelper(void)
{
}

double CRouteHelper::Distance(CvPoint src,CvPoint dst) //������������룬������С�ڵ���distance_error����ʱ������ΪΪ0
{
    double distance=sqrtl((src.x-dst.x)*(src.x-dst.x)+(src.y-dst.y)*(src.y-dst.y));
    //if(distance>DISTANCE_ERROR) return distance;
    //else return 0;
    return distance;
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


CvPoint CRouteHelper::FindnextPoint(CvPoint current,vector<CvPoint> map_points,vector<bool>& mask) //Ѱ����һ���㣬�������뵱ǰ������ĵ�
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
    if(-1 ==  nearest_num){
        throw logic_error("can't find nearest point!"); // that's why the
        // vector is out of range
    }
    mask.at(nearest_num)=true;
    return map_points.at(nearest_num);
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
