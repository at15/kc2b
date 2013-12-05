#pragma once
class CConfigs
{
public:
    CConfigs(void);
    ~CConfigs(void);
public:
    void SetMapCorners(std::vector<CvPoint> corners);
    std::vector<CvPoint> GetMapCorner(); // get the corner of the map to transform
    void SetThreshold(std::vector<int> threshold);
    std::vector<int> GetThreshold();

private:
    std::vector<CvPoint> m_map_corners;
    std::vector<int> m_threshold;
};

