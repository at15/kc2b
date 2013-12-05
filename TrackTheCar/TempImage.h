#pragma once
class CTempImage
{
public:
    CTempImage(void);
    CTempImage(IplImage* pSrc); // can't use this constructor when new??
    ~CTempImage(void);
protected:
    bool empty;// = true;
    IplImage* m_image;
public:
    bool is_empty(){return empty;}
    void SetImage(IplImage* pSrc);
    IplImage* GetImage(){return m_image;}
};

