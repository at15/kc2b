#pragma once
class CConfigs
{
public:
    CConfigs(void);
    ~CConfigs(void);
public:
    void SetMapCorners(std::vector<CvPoint> corners);
    std::vector<CvPoint> GetMapCorner(); // get the corner of the map to transform
    void SetCornerSize(int corner_size){m_corner_size = corner_size;}
    int GetCornerSize(){return m_corner_size;}
    void SetThreshold(std::vector<int> threshold);
    std::vector<int> GetThreshold();

private:
    std::vector<CvPoint> m_map_corners;
    int m_corner_size;
    //CvMat m_transform;
    std::vector<int> m_threshold;
    void LoadDefaultSetting();
};

