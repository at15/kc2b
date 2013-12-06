#pragma once
#include "constants.h"
// deal with the image processing
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
    IplImage* GetBinary(IplImage* gary_image,int threshold);
    std::vector<IplImage*> GetHSVBGR(IplImage* pSrc);
    void releaseHSVBGR(std::vector<IplImage*> hsvbgr);

    IplImage* GetRedBinary(std::vector<IplImage*> hsvbgrBin);
    IplImage* GetBlueBinary(std::vector<IplImage*> hsvbgrBin);

    IplImage* GetRedBinary(IplImage* pSrc,std::vector<int> threshold);
    IplImage* GetBlueBinary(IplImage* pSrc,std::vector<int> threshold);

    CvPoint CalcCore(IplImage* img_binary);
    IplImage* GenMaskPoint(IplImage* pSrc,CvRect point_rect);//,CvScalar bgr)

    // draw green circle in the middle to let user put the map in the right pos
    void DrawMiddleCircle(IplImage* img,CvScalar color = CV_RGB(0,255,0));
    

    // find the four red point so we can transform
    std::vector<CvPoint> FindMapCorner(IplImage* img,int corner_size = CORNER_SIZE);
    // transform, corner size can be specified
    IplImage* TransformImage(IplImage* pSrc,std::vector<CvPoint> corners);
   

    void cvThin( IplImage* src, IplImage* dst, int iterations=1);
/* abandoned func */
public:
    void CleanUp(); // release all the memory
private:
    IplImage* GetLastPtr();
    std::vector<IplImage*> m_images; // hold the image we need,destroy when destruct
};

