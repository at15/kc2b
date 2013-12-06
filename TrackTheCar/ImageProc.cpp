#include "StdAfx.h"
#include "ImageProc.h"
using namespace std;

CImageProc::CImageProc(void)
{
}


CImageProc::~CImageProc(void)
{
    // CleanUp();
}


IplImage* CImageProc::GetGrey(IplImage* color_image){
    IplImage* pGrayImage = cvCreateImage(cvGetSize(color_image), IPL_DEPTH_8U, 1);  
    cvCvtColor(color_image, pGrayImage, CV_BGR2GRAY);
    return pGrayImage;
}

IplImage* CImageProc::GetBinary(IplImage* gary_image,int threshold,bool want_white /*=false*/)
{
    IplImage* pBinaryImage = cvCreateImage(cvGetSize(gary_image), IPL_DEPTH_8U, 1); 
    if(want_white){
        // for the map
        cvThreshold(gary_image, pBinaryImage, threshold,255, CV_THRESH_BINARY_INV);
    }else{
        // for the car
        cvThreshold(gary_image, pBinaryImage, threshold, 255, CV_THRESH_BINARY);
    }
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

// The functions for get the point of red and blue
CvPoint CImageProc::GetRedCore(IplImage* color_image,std::vector<int> threshold){
    IplImage* redbin = GetRedBinary(color_image,threshold);
    CvPoint red_p = CalcCore(redbin);
    cvReleaseImage(&redbin);
    return red_p;
}

CvPoint CImageProc::GetBlueCore(IplImage* color_image,std::vector<int> threshold){
    IplImage* bluebin = GetBlueBinary(color_image,threshold);
    CvPoint blue_p = CalcCore(bluebin);
    cvReleaseImage(&bluebin);
    return blue_p;
}

// the src is binary?? or either is ok? suppose binary
void CImageProc::FindMapPoints(IplImage* pSrc,vector<CvPoint2D32f>& v_corners){
    double qualityLevel=0.1;
    double minDistance=10;
    // Create temporary images required by cvGoodFeaturesToTrack  
    IplImage* corners1 = cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_32F,1);
    IplImage* corners2 = cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_32F,1);
    // Create the array to store the points detected( <= 1000 )  
    int count = 1000;  
    CvPoint2D32f* corners = new CvPoint2D32f[count];  
    cvGoodFeaturesToTrack(pSrc, corners1, corners2, corners, &count,qualityLevel,minDistance,0);
    
    for(int i=0;i<count;i++)  
    {  
        //cvLine(img, cvPoint(corners[i].x, corners[i].y), cvPoint(corners[i].x, corners[i].y), CV_RGB(255,0,0), 5);
        if(!v_corners.empty()) v_corners.clear();
        v_corners.push_back(corners[i]);
    }
    delete corners;
}

void CImageProc::DrawMapPoints(IplImage* pSrc,const std::vector<CvPoint2D32f>& v_corners){
    // just draw the circle on the image for the corners
    for (int i=0;i<v_corners.size();i++){
        cvCircle(pSrc,cvPointFrom32f(v_corners.at(i)),6, CV_RGB(255,0,0),2);
    }
}

/*
// from M16
//生成路径
int i;
IplImage* debug_img = 0;
//强角点识别算法，找路径点
int countnum = POINTS_countnum;
CvPoint2D32f* points = (CvPoint2D32f*)cvAlloc(countnum*sizeof(points[0]));
IplImage* eig = cvCreateImage( cvGetSize(route_img), 32, 1 );
IplImage* temp = cvCreateImage( cvGetSize(route_img), 32, 1 );
double quality = 0.01;
double min_distance = LINE_LEN;

cvGoodFeaturesToTrack( route_img, eig, temp, points, &countnum,
quality, min_distance, 0, 3, 0, 0.04 );
cvReleaseImage( &eig );
cvReleaseImage( &temp );

double qualityLevel=0.05;
double minDistance=15;
IplImage *corners1=0, *corners2=0;

//创建两个与原图大小相同的临时图像 　　
corners1=cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);
corners2=cvCreateImage(cvGetSize(src),IPL_DEPTH_32F,1);

//角点检测
cvGoodFeaturesToTrack(src, corners1, corners2, corners, &num_points,qualityLevel,minDistance,0);

if(num_points>0)
{
for (int k=0; k <num_points;++k)
{cout<<corners[k].x<<' '<<corners[k].y<<endl;}
}
cout<<"Find all "<<num_points<<"points\n";

cvReleaseImage(&corners1);
cvReleaseImage(&corners2);
//cvWaitKey(0);

*/

void CImageProc::DrawMiddleCircle(IplImage* img,CvScalar color /* = CV_RGB(0,255,0) */){
    int w = img->width;
    int h = img->height;
    cvCircle(img,cvPoint(w/2,h/2),10,color,3);
}

// the order of fout points is 
// left top,right top, right bottom, left bottom
std::vector<CvPoint> CImageProc::FindMapCorner(IplImage* img,int corner_size/* = CORNER_SIZE*/){
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
    left_top_p.x +=  corner_size; // avoid the corner 
    left_top_p.y += corner_size;

    cvSetImageROI(img,right_top);
    CvPoint right_top_p = CalcCore(img);
    cvResetImageROI(img);
    right_top_p.x += w/2 ;
    right_top_p.x -= corner_size;
    right_top_p.y += corner_size;

    cvSetImageROI(img,right_bottom);
    CvPoint right_bottom_p = CalcCore(img);
    cvResetImageROI(img);
    right_bottom_p.x += w/2;
    right_bottom_p.y += h/2;
    right_bottom_p.x -= corner_size;
    right_bottom_p.y -= corner_size;

    cvSetImageROI(img,left_bottom);
    CvPoint left_bottom_p = CalcCore(img);
    cvResetImageROI(img);
    left_bottom_p.y += h/2;
    left_bottom_p.x += corner_size;
    left_bottom_p.y -= corner_size;

    vector<CvPoint> points;
    points.push_back(left_top_p);
    points.push_back(right_top_p);
    points.push_back(right_bottom_p);
    points.push_back(left_bottom_p);

    return points;
}


IplImage* CImageProc::TransformImage(IplImage* pSrc,std::vector<CvPoint> corners){
    int w = pSrc->width;
    int h = pSrc->height;

    CvMat* transmat = cvCreateMat(3, 3, CV_32FC1); // the mat for transform image

    CvPoint2D32f origin_points[4];
    for(int i=0;i<4;i++){
        origin_points[i] = cvPoint2D32f(corners.at(i).x,corners.at(i).y);
    }
    CvPoint2D32f new_points[4]={cvPoint2D32f(0, 0), cvPoint2D32f(w, 0),
        cvPoint2D32f(w, h), cvPoint2D32f(0, h)};

    cvGetPerspectiveTransform(origin_points,new_points,transmat);
    IplImage* trans_img = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);

    cvWarpPerspective(pSrc, trans_img, transmat);
    cvReleaseMat(&transmat);
    return trans_img;
}

void CImageProc::cvThin (IplImage* src, IplImage* dst, int iterations /*= 1*/) { 
    cvCopy(src, dst); 
    BwImage dstdat(dst); 
    IplImage* t_image = cvCloneImage(src); 
    BwImage t_dat(t_image); 
    for (int n = 0; n < iterations; n++) 
        for (int s = 0; s <= 1; s++) { 
            // cvCopyImage(dst, t_image); 
            cvCopy(dst, t_image);
            for (int i = 0; i < src->height; i++) 
                for (int j = 0; j < src->width; j++) 
                    if (t_dat[i][j]) { 
                        int a = 0, b = 0; 
                        int d[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1},  
                        {1, 0}, {1, -1}, {0, -1}, {-1, -1}}; 
                        int p[8]; 
                        p[0] = (i == 0) ? 0 : t_dat[i-1][j]; 
                        for (int k = 1; k <= 8; k++) { 
                            if (i+d[k%8][0] < 0 || i+d[k%8][0] >= src->height || 
                                j+d[k%8][1] < 0 || j+d[k%8][1] >= src->width)  
                                p[k%8] = 0; 
                            else p[k%8] = t_dat[ i+d[k%8][0] ][ j+d[k%8][1] ]; 
                            if (p[k%8]) { 
                                b++; 
                                if (!p[k-1]) a++; 
                            } 
                        } 
                        if (b >= 2 && b <= 6 && a == 1) 
                            if (!s && !(p[2] && p[4] && (p[0] || p[6]))) 
                                dstdat[i][j] = 0; 
                            else if (s && !(p[0] && p[6] && (p[2] || p[4]))) 
                                dstdat[i][j] = 0; 
                    } 
        } 
        cvReleaseImage(&t_image); 
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