#pragma once
// helper functions for the route
class CRouteHelper
{
public:
    CRouteHelper(void);
    ~CRouteHelper(void);
public:
    
    //������������룬������С�ڵ���distance_error����ʱ������ΪΪ0
    double Distance(CvPoint src,CvPoint dst);
    
    //�������������귽��ǣ�0��360��
    double Angle(CvPoint src,CvPoint dst);

    //Ѱ����һ���㣬�������뵱ǰ������ĵ�
    CvPoint FindnextPoint(CvPoint current,std::vector<CvPoint> map_points,std::vector<bool> mask);

};

