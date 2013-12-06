#pragma once
class CConfigs
{
public:
    CConfigs(void);
    ~CConfigs(void);
public:
    void SetCornerSize(int corner_size){m_corner_size = corner_size;}
    int GetCornerSize(){return m_corner_size;}

    void SetMapCorners(std::vector<CvPoint> corners);
    std::vector<CvPoint> GetMapCorner(); // get the corner of the map to transform
    
    void SetThreshold(std::vector<int> threshold);
    std::vector<int> GetThreshold();

private:
    int m_corner_size;

    std::vector<CvPoint> m_map_corners;
    
    //CvMat m_transform;// just dont use it at this moment...
    std::vector<int> m_threshold;
    void LoadDefaultSetting();
};

