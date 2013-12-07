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

    void SetMapThreshold(int threshold){m_map_threshold = threshold;}
    int GetMapThreshold(){return m_map_threshold;}

    void SetThinIteration(int iteration){m_thin_iteration = iteration;}
    int GetThinIteration(){return m_thin_iteration;}

    void SetCOM(int com_num){m_com_num = com_num;}
    int GetCOM(){return m_com_num;}

private:
    int m_corner_size;

    std::vector<CvPoint> m_map_corners;
    // the threshold for the red and blue
    std::vector<int> m_threshold;
    // the threshold for the map
    int m_map_threshold;
    // the thin time for the map
    int m_thin_iteration;

    // the com num for the port
    int m_com_num;
    void LoadDefaultSetting();
};