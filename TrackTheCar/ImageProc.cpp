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

vector<CLine> CImageProc::FindLines(IplImage* binary_image,
    double line_distance_error /*= LINE_DISTANCE_ERROR */){
        CvMemStorage* storage = cvCreateMemStorage(); //创建一片内存区域存储线段数据
        // 使用Hough变换找到所有的直线
        CvSeq* lines = cvHoughLines2(binary_image, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 
            MCV_LINE_EXIST, MCV_MIN_LINE_LENGTH, MCV_MAX_LINE_DISTANCE);

        // remove the redundant lines
        int total_found = lines->total;
        // store all the lines found by cvHoughLines2 in a CLine vector
        vector<CLine> all_lines;
        for(int i=0;i<total_found;i++){
            all_lines.push_back(CLine(lines,i));
        }
        // return all_lines;// for debug ...
        // store the cleaned up lines
        vector<CLine> final_lines;
        for(int i=0;i<all_lines.size();i++){
            CLine current = all_lines.at(i);
            bool is_child = false;
            for(int j=0;j<all_lines.size();j++){
                if(i == j) continue;
                if(current.IsChildLine(all_lines.at(j),line_distance_error)){
                    is_child = true;
                    break;
                }
            }
            if(!is_child){
                final_lines.push_back(current);
            }
        }

        return final_lines;
}

void CImageProc::DrawLines(IplImage* pSrc,const std::vector<CLine>& v_lines){
    cvSetZero(pSrc);
    for(int i=0;i<v_lines.size();i++){
        cvLine(pSrc,v_lines.at(i).start(),v_lines.at(i).end(),cvScalar(255,0,0));
        //Sleep(500);
    }
}

// Sort the lines with the given start point
vector<CLine> CImageProc::SortLines(std::vector<CLine> o_lines,
    CvPoint car_head,CvPoint car_tail,
    double point_dist/* = LINE_POINT_DIST*/){

        if(o_lines.empty()){
            throw logic_error("can't sort empty vector<CLine>");
        }

        CvPoint start_piont;
        start_piont.x = (car_head.x + car_tail.x)/2;
        start_piont.y = (car_head.y + car_tail.y)/2;

        // find the first line, which is nearest to the start point
        int i_first_line = CLine::FindNearestLine(start_piont,o_lines);

        vector<CLine> sorted_lines;
        CLine current_line,next_line;
        current_line = o_lines.at(i_first_line);
        // make sure the start of the line is closer to the car's tail
        if(current_line.StartDist(car_tail) > current_line.StartDist(car_head)){
            current_line.Swap();
        }
        sorted_lines.push_back(current_line);
        o_lines.erase(o_lines.begin()+i_first_line,o_lines.begin()+i_first_line+1);

        // keep finding all the lines
        //for(int i=0;i<o_lines.size();i++){
        while(!o_lines.empty()){
            int j = CLine::FindNearestLine(current_line.end(),o_lines);
            // The closer point should be the start point
            if(o_lines.at(j).StartDist(current_line.end()) >
                o_lines.at(j).EndDist(current_line.end())){
                    o_lines.at(j).Swap();
            }
            
            // if the end of line1 and start of line2 is close enough, they become
            // a same point, otherwise a new line generated
            if(o_lines.at(j).StartDist(current_line.end()) < point_dist){
                // o_line[j].Start() is close enough to current_line.end()
                next_line = CLine(current_line.end(),o_lines.at(j).end());
            }else{
                next_line = o_lines.at(j);
                // add an extra line
                CLine extra_line = CLine(current_line.end(),next_line.start());
                sorted_lines.push_back(extra_line);
            }
            //next_line = o_lines.at(j); // for debug
            sorted_lines.push_back(next_line);
            o_lines.erase(o_lines.begin()+j,o_lines.begin()+j+1);
            current_line = next_line;
        }
        return sorted_lines;
}

bool CImageProc::FindNearestLine(CLine& r_line,std::vector<CLine>& o_lines,CvPoint c_point){
    if(o_lines.empty()){
        throw logic_error("can't find line in empty vector<CLine>");
    }

    double min_distance = 10000; // 设定一个很大的值
    int line_index = -1;
    for(int i=0;i<o_lines.size();i++){
        if(o_lines.at(i).isPassed()) continue;// 忽略已经走过的线
        double dist = o_lines.at(i).PointDist(c_point);
        if(dist < min_distance){
            min_distance = dist;
            line_index = i;
        }
    }

    if(-1 == line_index){
        // 没有找到最近的线
        return false;
    }else{
        o_lines.at(line_index).SetPassed();
        r_line = o_lines.at(line_index);
        return true;
    }
}

void CImageProc::FindMapPoints(IplImage* pSrc,vector<CvPoint2D32f>& v_corners,
    double qualityLevel/*=MCV_QUALITY_LEVEL*/,
    double minDistance/*=MCV_MIN_DISTANCE*/){
        // Create temporary images required by cvGoodFeaturesToTrack  
        IplImage* corners1 = cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_32F,1);
        IplImage* corners2 = cvCreateImage(cvGetSize(pSrc),IPL_DEPTH_32F,1);
        // Create the array to store the points detected( <= 1000 )  
        int count = 1000;  
        CvPoint2D32f* corners = new CvPoint2D32f[count];  
        cvGoodFeaturesToTrack(pSrc, corners1, corners2, corners, &count,qualityLevel,minDistance,0);

        if(!v_corners.empty()) v_corners.clear();
        for(int i=0;i<count;i++)  
        {  
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

void CImageProc::DrawMiddleCircle(IplImage* img,CvScalar color /* = CV_RGB(0,255,0) */){
    int w = img->width;
    int h = img->height;
    cvCircle(img,cvPoint(w/2,h/2),10,color,3);
}
// the order of fout points is 
// left top,right top, right bottom, left bottom
std::vector<CvPoint> CImageProc::FindMapCorner(IplImage* img_bin,int corner_width,int corner_height){
    // step 1 find the left one
    int w = img_bin->width;
    int h =img_bin->height;

    CvRect left_top = cvRect(0,0,w/2,h/2);
    CvRect right_top = cvRect(w/2,0,w/2,h/2);
    CvRect left_bottom = cvRect(0,h/2,w/2,h/2);
    CvRect right_bottom = cvRect(w/2,h/2,w/2,h/2);

    // set the roi
    cvSetImageROI(img_bin,left_top);
    // then calc the core to get the point
    CvPoint left_top_p = CalcCore(img_bin);
    cvResetImageROI(img_bin);
    left_top_p.x +=  corner_width; // avoid the corner 
    left_top_p.y += corner_height;

    cvSetImageROI(img_bin,right_top);
    CvPoint right_top_p = CalcCore(img_bin);
    cvResetImageROI(img_bin);
    right_top_p.x += w/2 ;
    right_top_p.x -= corner_width;
    right_top_p.y += corner_height;

    cvSetImageROI(img_bin,right_bottom);
    CvPoint right_bottom_p = CalcCore(img_bin);
    cvResetImageROI(img_bin);
    right_bottom_p.x += w/2;
    right_bottom_p.y += h/2;
    right_bottom_p.x -= corner_width;
    right_bottom_p.y -= corner_height;

    cvSetImageROI(img_bin,left_bottom);
    CvPoint left_bottom_p = CalcCore(img_bin);
    cvResetImageROI(img_bin);
    left_bottom_p.y += h/2;
    left_bottom_p.x += corner_width;
    left_bottom_p.y -= corner_height;

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