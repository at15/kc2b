#pragma once
#include "CameraCtrl.h"
#include "Configs.h"
#include "TempImage.h"

class CSmallCar
{
public:
    CSmallCar(void);
    ~CSmallCar(void);
public:
    bool Init(CCvPicCtrl* camera,IplImage* map,CConfigs* config);

    // control the small car
    void GoLeft();
    void GoRight();
    void GoBack();
    void GoForward();

private:
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CConfigs* m_config;
    CTempImage m_map;

    CvPoint m_head;
    CvPoint m_tail;

    bool GetCarPos();
};

