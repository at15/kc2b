#pragma once
// helper functions for the route
class CRouteHelper
{
public:
    CRouteHelper(void);
    ~CRouteHelper(void);
public:
    
    //求两整数点距离，当距离小于等于distance_error像素时，可认为为0
    double Distance(CvPoint src,CvPoint dst);
    
    //返回向量极坐标方向角，0～360度
    double Angle(CvPoint src,CvPoint dst);

    //寻找下一个点，即返回与当前点最近的点
    CvPoint FindnextPoint(CvPoint current,std::vector<CvPoint> map_points,std::vector<bool> mask);

};

