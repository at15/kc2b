#pragma once
// helper functions for the route
class CRouteHelper
{
public:
    CRouteHelper(void);
    ~CRouteHelper(void);
public:
    double Distance(CvPoint src,CvPoint dst);
    double Angle(CvPoint src,CvPoint dst)//�������������귽��ǣ�0��360��
};

