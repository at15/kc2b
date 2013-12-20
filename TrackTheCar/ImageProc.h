#pragma once
#include "constants.h"
// deal with the image processing
// ÕºœÒ¿‡
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
    std::vector<CvPoint> FindMapCorner(IplImage* img,int corner_size = CORNER_SIZE);
    // transform, corner size can be specified
    IplImage* TransformImage(IplImage* pSrc,std::vector<CvPoint> corners);


    void cvThin( IplImage* src, IplImage* dst, int iterations=1);

    std::vector<CLine> FindLines(IplImage* binary_image,
        double line_distance_error = LINE_DISTANCE_ERROR);
    void DrawLines(IplImage* pSrc,const std::vector<CLine>& v_lines);

    void FindMapPoints(IplImage* pSrc,std::vector<CvPoint2D32f>& v_corners,
        double qualityLevel=MCV_QUALITY_LEVEL,
        double minDistance=MCV_MIN_DISTANCE);

    void DrawMapPoints(IplImage* pSrc,const std::vector<CvPoint2D32f>& v_corners);
    /* abandoned func */
public:
    void CleanUp(); // release all the memory
private:
    IplImage* GetLastPtr();
    std::vector<IplImage*> m_images; // hold the image we need,destroy when destruct
};

