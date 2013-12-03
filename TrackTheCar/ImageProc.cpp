#include "StdAfx.h"
#include "ImageProc.h"


CImageProc::CImageProc(void)
{
}


CImageProc::~CImageProc(void)
{
}

IplImage* CImageProc::GetBinary(IplImage* gary_image,int threshold)
{
    IplImage* pBinaryImage = cvCreateImage(cvGetSize(gary_image), IPL_DEPTH_8U, 1); 
    cvThreshold(gary_image, pBinaryImage, threshold, 255, CV_THRESH_BINARY);
    return pBinaryImage;
    //TODO:release it!!
}

std::vector<IplImage*> CImageProc::GetHSVBGR(IplImage* pSrc)
{
    IplImage* img_pocessed=cvCloneImage(pSrc);
    CvSize img_size=cvGetSize(pSrc);

    //rgb to hsv
    IplImage* img_hsv=cvCreateImage(img_size,IPL_DEPTH_8U,3);
    cvCvtColor(pSrc,img_hsv,CV_RGB2HSV);

    // create blank h s v b g r
    IplImage* img_h=cvCreateImage(img_size,IPL_DEPTH_8U,1);
    IplImage* img_s=cvCreateImage(img_size,IPL_DEPTH_8U,1);
    IplImage* img_v=cvCreateImage(img_size,IPL_DEPTH_8U,1);
    IplImage* img_b=cvCreateImage(img_size,IPL_DEPTH_8U,1);
    IplImage* img_g=cvCreateImage(img_size,IPL_DEPTH_8U,1);
    IplImage* img_r=cvCreateImage(img_size,IPL_DEPTH_8U,1);

    // spit the image to b,g,r, h,s,v
    cvSplit(img_pocessed,img_b,img_g,img_r,0);
    cvSplit(img_hsv,img_h,img_s,img_v,0);

    // release the temp imgs
    cvReleaseImage(&img_pocessed);
    cvReleaseImage(&img_hsv);

    // return the splited img they are gray, i suppose
    std::vector<IplImage*> result;
    result.push_back(img_h);
    result.push_back(img_s);
    result.push_back(img_v);
    result.push_back(img_b);
    result.push_back(img_g);
    result.push_back(img_r);
    return result;
}

void CImageProc::releaseHSVBGR(std::vector<IplImage*> hsvbgr)
{
    for(int i = 0; i < 6; i++)
    {
        cvReleaseImage(&hsvbgr.at(i));
    }
}

IplImage* CImageProc::GetRedBinary(std::vector<IplImage*> hsvbgrBin){
    IplImage* img_red=cvCreateImage(cvGetSize(hsvbgrBin.at(0)),IPL_DEPTH_8U,1);//��ź�ɫ��������
    cvAnd(hsvbgrBin.at(C_R),hsvbgrBin.at(C_S),img_red);
    cvErode(img_red,img_red);// fushi....
    return img_red;
    // TODO:release it
}

IplImage* CImageProc::GetRedBinary(IplImage* pSrc,std::vector<int> threshold)
{
    std::vector<IplImage*> hsvbgr = GetHSVBGR(pSrc);
    for(int i = 0; i < 6 ;i++)
    {
        cvThreshold(hsvbgr.at(i),hsvbgr.at(i),threshold.at(i),255,CV_THRESH_BINARY);
    }

    IplImage* img_red=cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_8U,1);//��ź�ɫ��������
    cvAnd(hsvbgr.at(C_R),hsvbgr.at(C_S),img_red);
    // cvAnd(img_r,img_s,img_red);//tx??? 
    // TODO: why add s and r equals to red
    cvErode(img_red,img_red);// fushi....
    releaseHSVBGR(hsvbgr);
    return img_red;
}

IplImage* CImageProc::GetBlueBinary(std::vector<IplImage*> hsvbgrBin){
    IplImage* img_blue=cvCreateImage(cvGetSize(hsvbgrBin.at(0)),IPL_DEPTH_8U,1);//���blue
    cvAnd(hsvbgrBin.at(C_B),hsvbgrBin.at(C_S),img_blue);
    cvErode(img_blue,img_blue,0,2);
    return img_blue;
}

IplImage* CImageProc::GetBlueBinary(IplImage* pSrc,std::vector<int> threshold)
{
    std::vector<IplImage*> hsvbgr = GetHSVBGR(pSrc);
    for(int i = 0; i < 6 ;i++)
    {
        cvThreshold(hsvbgr.at(i),hsvbgr.at(i),threshold.at(i),255,CV_THRESH_BINARY);
    }

    IplImage* img_blue=cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_8U,1);//���blue
    cvAnd(hsvbgr.at(C_B),hsvbgr.at(C_S),img_blue);
    //cvAnd(img_b,img_s,img_blue);//TX ...?
    cvErode(img_blue,img_blue,0,2);
    releaseHSVBGR(hsvbgr);
    return img_blue;
}

CvPoint CImageProc::CalcCore(IplImage* img_binary)
{
    CvMoments moments;
    cvMoments(img_binary,&moments,1);
    double m00,m10,m01;

    m00=cvGetSpatialMoment(&moments, 0, 0);
    m10=cvGetSpatialMoment(&moments, 1, 0);
    m01=cvGetSpatialMoment(&moments, 0, 1);


    int x=cvRound(m10/m00); // turn double to int
    int y=cvRound(m01/m00);

    return cvPoint(x,y);
}

IplImage* CImageProc::GenMaskPoint(IplImage* pSrc,CvRect point_rect)//,CvScalar bgr)
{
    CvSize img_size = cvGetSize(pSrc);
    IplImage* mask=cvCreateImage(img_size,IPL_DEPTH_8U,1);
    cvSetZero(mask);
    cvSetImageROI(mask,point_rect);
    //cvSet(mask,bgr);
    cvSet(mask,cvScalar(255,0,0,0)); 
    cvResetImageROI(mask);
    //cvSet(mask,cvScalar(255,0,0,0)); //ok which means cvRGB got problem? yes, because mask is binary!

    return mask;
}