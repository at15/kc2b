#pragma once
// helper functions for the route
class CRouteHelper
{
public:
    CRouteHelper(void);
    ~CRouteHelper(void);
public:

    //������������룬������С�ڵ���distance_error����ʱ������ΪΪ0
    static double Distance(CvPoint src,CvPoint dst);

    //�������������귽��ǣ�0��360��
    static double Angle(CvPoint src,CvPoint dst);

    //Ѱ����һ���㣬�������뵱ǰ������ĵ�
    //CvPoint FindnextPoint(CvPoint current,std::vector<CvPoint> map_points,std::vector<bool> mask);
    // !!pass by ref!!!!
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
    // ��˵�ģ
    double CrossABS(CVector v2){
        // ��ά������˵�ģ x1 * y2 - x2 * y1
        return fabs(m_x * v2.y() - v2.x()*m_y);
    }
    // ������ģ
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
    }
    CLine(CvPoint* line){
        m_start = line[0];
        m_end = line[1];
        m_empty = false;
    }
    CLine(CvSeq* lines,int i){
        CvPoint* line = (CvPoint*) cvGetSeqElem(lines,i);
        //CLine(line);// will this work? no!
        m_start = line[0];
        m_end = line[1];
        m_empty = false;
    }
public:
    bool isEmpty() const {return m_empty;} 
    // ��ȡ�߶ε������յ�
    CvPoint start() const {return m_start;} 
    CvPoint end() const {return m_end;} 
    // �����߶ε�ͷβ
    void Swap(){
        CvPoint t = m_start;
        m_start = m_end;
        m_end = t;
    }
    // �߶εĳ���
    double length() const {
        return Distance(m_start,m_end);
    } 
    // ����ĳ�㵽��ֱ�ߵľ���
    double PointDist(CvPoint point) const {
        // ������˵�ģ�����߶γ�
        CVector v1(m_start,m_end);// a
        CVector v2(m_start,point);// b
        // |a x b| / |a|
        return v1.CrossABS(v2)/v1.ABS();
    } 
    bool IsChildLine(CLine ){

    }
private:
    bool m_empty;
    CvPoint m_start;
    CvPoint m_end;
    double Distance(CvPoint src,CvPoint dst) const {
        return sqrtl((src.x - dst.x)*(src.x - dst.x)
            +(src.y-dst.y)*(src.y-dst.y));
    }
};
