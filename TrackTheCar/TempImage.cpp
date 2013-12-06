#include "StdAfx.h"
#include "TempImage.h"


CTempImage::CTempImage(void)
{
    empty = true;
    m_image = NULL;
}

CTempImage::CTempImage(IplImage* pSrc){

    SetImage(pSrc);
}

CTempImage::~CTempImage(void)
{
    if(!empty && m_image){
        cvReleaseImage(&m_image);
    }
}

void CTempImage::SetImage(IplImage* pSrc){
   /* m_image = cvCloneImage(pSrc);
    if(m_image) {
        empty = false;
    }*/
    m_image = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
    cvCopy(pSrc,m_image);
    if(m_image) {
        empty = false;
    }
}

