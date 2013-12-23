#pragma once
// helper functions for the route
class CRouteHelper
{
public:
    CRouteHelper(void);
    ~CRouteHelper(void);
public:

    //求两整数点距离，当距离小于等于distance_error像素时，可认为为0
    static double Distance(CvPoint src,CvPoint dst);

    //返回向量极坐标方向角，0～360度
    static double Angle(CvPoint src,CvPoint dst);

    //寻找下一个点，即返回与当前点最近的点,mask来判断某个点是否走过
    CvPoint FindnextPoint(CvPoint current,std::vector<CvPoint> map_points,std::vector<bool>& mask);

};

class CVector{
public:
    // the vector in the math
    CVector();
    CVector(CvPoint start,CvPoint end){
        m_x = end.x - start.x;
        m_y = end.y - start.y;
    }
    double x(){return m_x;}
    double y() {return m_y;}
    // 自己叉乘向量v2的数值（通过它的正负，若正，则转到v2逆时针）
    double Cross(CVector v2){
        return m_x * v2.y() - v2.x()*m_y;
    }
    // 叉乘的模
    double CrossABS(CVector v2){
        // 二维向量叉乘的模 x1 * y2 - x2 * y1
        return fabs(m_x * v2.y() - v2.x()*m_y);
    }
    // 向量的模
    double ABS(){
        return sqrtl(m_x*m_x + m_y*m_y);
    }
private:
    double m_x;
    double m_y;
};

class CLine{
public:
    CLine(){
        m_empty = true;
        m_passed = false;
    }
    CLine(CvPoint* line){
        m_start = line[0];
        m_end = line[1];
        m_empty = false;
        m_passed = false;
    }
    CLine(CvSeq* lines,int i){
        CvPoint* line = (CvPoint*) cvGetSeqElem(lines,i);
        m_start = line[0];
        m_end = line[1];
        m_empty = false;
        m_passed = false;
    }
    CLine(CvPoint start,CvPoint end){
        m_start = start;
        m_end = end;
        m_empty = false;
        m_passed = false;
    }
public:
    bool isEmpty() const {return m_empty;} 
    bool isPassed() const {return m_passed;}
    void SetPassed() {m_passed = true;}
    // 获取线段的起点和终点
    CvPoint start() const {return m_start;} 
    CvPoint end() const {return m_end;} 
    // 交换线段的头尾
    void Swap(){
        CvPoint t = m_start;
        m_start = m_end;
        m_end = t;
    }
    // 线段的长度
    double length() const {
        return Distance(m_start,m_end);
    } 
    // 某点到线段起始点的距离
    double StartDist(CvPoint point) const {
        return Distance(m_start,point);
    }
    // 某点到线段终点的距离
    double EndDist(CvPoint point) const {
        return Distance(m_end,point);
    }
    // 计算某点到该直线的距离
    double PointDist(CvPoint point) const {
        // 向量叉乘的模除以线段长
        CVector v1(m_start,m_end);// a
        CVector v2(m_start,point);// b
        // |a x b| / |a|
        return v1.CrossABS(v2)/v1.ABS();
    }
    // 是否是某条线段的子线段
    bool IsChildLine(const CLine& line,double distance_error = 0){
        // 如果本线段的头和尾都跟新线段距离小于error，且更短，它就是
        if(this->length()>line.length()){
            return false;// 我比你长，怎么可能是你孩子呢？（好邪恶...)
        }
        if((line.PointDist(m_start) > distance_error) ||
            (line.PointDist(m_end) > distance_error)){
                return false; // 端点不在直线上
        }
        return true;
    }
    // return the index of the nearest line from a vector of CLine
    static int FindNearestLine(CvPoint point,const std::vector<CLine>& v_lines);
    static double Distance(CvPoint p1,CvPoint p2);
private:
    bool m_empty;
    bool m_passed;// 这条直线是否已经走过了
    CvPoint m_start;
    CvPoint m_end;
};
