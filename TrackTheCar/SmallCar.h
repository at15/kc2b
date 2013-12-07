#pragma once
#include "CameraCtrl.h"
#include "Configs.h"
#include "TempImage.h"
#include "CarControl.h"


class CSmallCar
{
public:
    CSmallCar(void);
    ~CSmallCar(void);
public:
    bool Init(CCvPicCtrl* camera,IplImage* map,CConfigs* config);
private:
    CImageProc m_proc;

    CCvPicCtrl* m_camera;
    CConfigs* m_config;
    CTempImage m_map;

    CvPoint m_head;
    CvPoint m_tail;

    bool GetCarPos();

    CCarControl m_car_control;

};

