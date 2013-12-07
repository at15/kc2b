#pragma once
// helper functions for the route
class CRouteHelper
{
public:
    CRouteHelper(void);
    ~CRouteHelper(void);
public:
    double Distance(CvPoint src,CvPoint dst);
    double Angle(CvPoint src,CvPoint dst)//返回向量极坐标方向角，0～360度
};

