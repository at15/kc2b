#include "StdAfx.h"
#include "ImageProc.h"


CImageProc::CImageProc(void)
{
}


CImageProc::~CImageProc(void)
{
   // CleanUp();
}

IplImage* CImageProc::GetBinary(IplImage* gary_image,int threshold)
{
    IplImage* pBinaryImage = cvCreateImage(cvGetSize(gary_image), IPL_DEPTH_8U, 1); 
    cvThreshold(gary_image, pBinaryImage, threshold, 255, CV_THRESH_BINARY);
    m_images.push_back(pBinaryImage);
    return pBinaryImage;
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
    IplImage* img_red=cvCreateImage(cvGetSize(hsvbgrBin.at(0)),IPL_DEPTH_8U,1);//存放红色（车身）
    cvAnd(hsvbgrBin.at(C_R),hsvbgrBin.at(C_S),img_red);
    cvErode(img_red,img_red);
    //m_images.push_back(img_red);
    //return GetLastPtr(); // can't use now
    return img_red;
}

IplImage* CImageProc::GetRedBinary(IplImage* pSrc,std::vector<int> threshold)
{
    std::vector<IplImage*> hsvbgr = GetHSVBGR(pSrc);
    for(int i = 0; i < 6 ;i++)
    {
        cvThreshold(hsvbgr.at(i),hsvbgr.at(i),threshold.at(i),255,CV_THRESH_BINARY);
    }

    IplImage* img_red=cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_8U,1);//存放红色（车身）
    cvAnd(hsvbgr.at(C_R),hsvbgr.at(C_S),img_red);
    // cvAnd(img_r,img_s,img_red);//tx??? 
    // TODO: why add s and r equals to red
    cvErode(img_red,img_red);
    releaseHSVBGR(hsvbgr);
    //m_images.push_back(img_red);
    return img_red;
}

IplImage* CImageProc::GetBlueBinary(std::vector<IplImage*> hsvbgrBin){
    IplImage* img_blue=cvCreateImage(cvGetSize(hsvbgrBin.at(0)),IPL_DEPTH_8U,1);//存放blue
    cvAnd(hsvbgrBin.at(C_B),hsvbgrBin.at(C_S),img_blue);
    cvErode(img_blue,img_blue,0,2);
    //m_images.push_back(img_blue);
    //return img_blue;
    //return GetLastPtr();
    return img_blue;
}

IplImage* CImageProc::GetBlueBinary(IplImage* pSrc,std::vector<int> threshold)
{
    std::vector<IplImage*> hsvbgr = GetHSVBGR(pSrc);
    for(int i = 0; i < 6 ;i++)
    {
        cvThreshold(hsvbgr.at(i),hsvbgr.at(i),threshold.at(i),255,CV_THRESH_BINARY);
    }

    IplImage* img_blue=cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_8U,1);//存放blue
    cvAnd(hsvbgr.at(C_B),hsvbgr.at(C_S),img_blue);
    //cvAnd(img_b,img_s,img_blue);//TX ...?
    cvErode(img_blue,img_blue,0,2);
    releaseHSVBGR(hsvbgr);
    //m_images.push_back(img_blue);
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

void CImageProc::CleanUp(){
    for(int i = 0;i<m_images.size();i++){
        if(m_images.at(i)){
            cvReleaseImage(&m_images.at(i));
        }
    }
}

IplImage* CImageProc::GetLastPtr(){
    if(m_images.size()> 0){
        return m_images.at(m_images.size()-1);
    }
    else{
        return NULL;
    }
}
void CImageProc::DrawMiddleCircle(IplImage* img,CvScalar color /* = CV_RGB(0,0,255) */){
    int w = img->width;
    int h = img->height;
    cvCircle(img,cvPoint(w/2,h/2),10,color,3);
}

std::vector<CvPoint> CImageProc::FindMapCorner(IplImage* img){
    // step 1 find the left one
    int w = img->width;
    int h =img->height;
    
    CvRect left_top = cvRect(0,0,w/2,h/2);
    CvRect right_top = cvRect(w/2,0,w/2,h/2);
    CvRect left_bottom = cvRect(0,h/2,w/2,h/2);
    CvRect right_bottom = cvRect(w/2,h/2,w/2,h/2);

    // set the roi
    cvSetImageROI(img,left_top);
    // then calc the core to get the point
    CvPoint left_top_p = CalcCore(img);
    cvResetImageROI(img);

    cvSetImageROI(img,right_top);
    CvPoint right_top_p = CalcCore(img);
    cvResetImageROI(img);

    cvSetImageROI(img,left_bottom);
    CvPoint left_bottom_p = CalcCore(img);
    cvResetImageROI(img);

    cvSetImageROI(img,right_bottom);
    CvPoint right_bottom_p = CalcCore(img);
    cvResetImageROI(img);
    
    int i = 1;
     /*    CvPoint zhong_point = m_pic_binary.CalcCore(red_bin);
     CvRect zhong = cvRect(zhong_point.x,zhong_point.y,10,10);
     // note the mask is the binary!!!
     IplImage* mask = m_pic_binary.GenMaskPoint(red_bin,zhong);*/
    std::vector<CvPoint> points;
    //points.push_back(ltp);
    return points;
}