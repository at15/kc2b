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

    //Ѱ����һ���㣬�������뵱ǰ������ĵ�,mask���ж�ĳ�����Ƿ��߹�
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
    // �Լ��������v2����ֵ��ͨ��������������������ת��v2��ʱ�룩
    double Cross(CVector v2){
        return m_x * v2.y() - v2.x()*m_y;
    }
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
    // ĳ�㵽�߶���ʼ��ľ���
    double StartDist(CvPoint point) const {
        return Distance(m_start,point);
    }
    // ĳ�㵽�߶��յ�ľ���
    double EndDist(CvPoint point) const {
        return Distance(m_end,point);
    }
    // ����ĳ�㵽��ֱ�ߵľ���
    double PointDist(CvPoint point) const {
        // ������˵�ģ�����߶γ�
        CVector v1(m_start,m_end);// a
        CVector v2(m_start,point);// b
        // |a x b| / |a|
        return v1.CrossABS(v2)/v1.ABS();
    }
    // �Ƿ���ĳ���߶ε����߶�
    bool IsChildLine(const CLine& line,double distance_error = 0){
        // ������߶ε�ͷ��β�������߶ξ���С��error���Ҹ��̣�������
        if(this->length()>line.length()){
            return false;// �ұ��㳤����ô�������㺢���أ�����а��...)
        }
        if((line.PointDist(m_start) > distance_error) ||
            (line.PointDist(m_end) > distance_error)){
                return false; // �˵㲻��ֱ����
        }
        return true;
    }
    // return the index of the nearest line from a vector of CLine
    static int FindNearestLine(CvPoint point,const std::vector<CLine>& v_lines);
    static double Distance(CvPoint p1,CvPoint p2);
private:
    bool m_empty;
    bool m_passed;// ����ֱ���Ƿ��Ѿ��߹���
    CvPoint m_start;
    CvPoint m_end;
};
