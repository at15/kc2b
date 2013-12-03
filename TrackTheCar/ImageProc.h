#pragma once
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
};

