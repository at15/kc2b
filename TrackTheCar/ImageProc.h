#pragma once
#include "constants.h"
#include "CvPicCtrl.h"
#include "seqQueue.h"
// deal with the image processing
// 图像类
template<class T> class Image {
private:
    IplImage* imgp;
public:
    Image(IplImage* img=0) {imgp=img;}
    ~Image(){imgp=0;}
    inline T* operator[](const int rowIndx) {
        return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));
    }
}; 
typedef Image<unsigned char>  BwImage;

class CImageProc
{
public:
    CImageProc(void);
    ~CImageProc(void);

public:
    // to make it easy to get channel from result of GetHSVBGR
    enum {
        C_H = 0,
        C_S,
        C_V,
        C_B,
        C_G,
        C_R
    };
    // you have to release the image manually ... 
    IplImage* GetGrey(IplImage* color_image);
    IplImage* GetBinary(IplImage* gary_image,int threshold,bool want_white = false);
    std::vector<IplImage*> GetHSVBGR(IplImage* pSrc);
    void releaseHSVBGR(std::vector<IplImage*> hsvbgr);

    IplImage* GetRedBinary(std::vector<IplImage*> hsvbgrBin);
    IplImage* GetBlueBinary(std::vector<IplImage*> hsvbgrBin);

    IplImage* GetRedBinary(IplImage* pSrc,std::vector<int> threshold);
    IplImage* GetBlueBinary(IplImage* pSrc,std::vector<int> threshold);

    CvPoint CalcCore(IplImage* img_binary);
    IplImage* GenMaskPoint(IplImage* pSrc,CvRect point_rect);//,CvScalar bgr)

    // The functions for get the point of red and blue
    CvPoint GetRedCore(IplImage* color_image,std::vector<int> threshold);
    CvPoint GetBlueCore(IplImage* color_image,std::vector<int> threshold);

    // draw green circle in the middle to let user put the map in the right pos
    void DrawMiddleCircle(IplImage* img,CvScalar color = CV_RGB(0,255,0));


    // find the four red point so we can transform
    std::vector<CvPoint> FindMapCorner(IplImage* img_bin,int corner_width,int corner_height);
    // transform, corner size can be specified
    IplImage* TransformImage(IplImage* pSrc,std::vector<CvPoint> corners);


    void cvThin( IplImage* src, IplImage* dst, int iterations=1);

    // 使用线段的走法
    std::vector<CLine> FindLines(IplImage* binary_image,
        double line_distance_error = LINE_DISTANCE_ERROR);
    void DrawLines(CCvPicCtrl* pic_ctrl,const std::vector<CLine>& v_lines,int delay = 0);
    std::vector<CLine> SortLines(std::vector<CLine> o_lines,
        CvPoint car_head,CvPoint car_tail,
        double point_dist = LINE_POINT_DIST);

    // abandoned:根据c_point的位置找离他最近的直线，找到后把该直线的passed设为true
    bool FindNearestLine(CLine& r_line,std::vector<CLine>& o_lines,CvPoint c_point);

    double distance2(int x1,int y1,int x2,int y2);
    std::vector<CvPoint> RoadPoint(IplImage* src,CvPoint begin,seqQueue<CvPoint>& que);
};

