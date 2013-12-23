#pragma once
// The config item that store int or bool
template<class T>
class CConfigItem{
public:
    CConfigItem(){
        m_set = false;
    }
    bool IsSet(){return m_set;}
    T Get(){return m_value;}
    void Set(T value){
        m_set = true;
        m_value = value;
    }
private:
    T m_value;
    bool m_set;
};

typedef CConfigItem<int> CConfigInt;
typedef CConfigItem<bool> CConfigBool;

// Store the vector
template<class T>
class CConfigVector{
public:
    CConfigVector(){
        m_set = false;
        m_value.clear();
    }
    // get a copy of all value
    std::vector<T> Get(){
        if(!m_set && b_throw){
            throw std::logic_error("vector is not set!");
        }else if(!m_set){
            m_value.clear();
        }
        return m_value;
    }
    bool Get(std::vector<T>& v){
        if(!m_set){
            return false;
        }
        v = m_value;
        return true;
    }
    void Set(std::vector<T> value){
        m_value = value;
        m_set = true;
    }
    // get a pointer to the value
    const std::vector<T>* GetPtr(){
        if(!m_set){
            return NULL;
        }
        return &m_value;
    }
    // get a copy of one specific item
    bool GetItem(int index,T& item){
        if(!m_set){
            return false;
        }
        if((index < 0) || (index-1 > m_value.size)){
            return false;
        }
        return m_value.at(index);
    }

private:
    std::vector<T> m_value;
    bool m_set;
    bool b_throw;
};

// global configs
class CGConfigs{
public:
    // Bluetooth com port
    CConfigInt com_port;

    // The error for the route
    CConfigInt route_distance_error;
    CConfigInt route_angle_error;

    // The four corner of the map,used for transform the image
    CConfigVector<int> map_corner;
    // The size of the corner,the shouldn't be in the transformed map
    CConfigInt map_corner_size;

    // Get the binary for map and thin it to get points or lines
    CConfigInt map_threshold;
    CConfigInt map_thin_iteration;

    // The lines for the car?


};

class CConfigs
{
public:
    CConfigs(void);
    ~CConfigs(void);
public:
    // for transform the map
    bool IsTransfromSet(){return m_btransform_set;}
    void SetCornerSize(int corner_size){m_corner_size = corner_size;}
    int GetCornerSize(){return m_corner_size;}
    void SetMapCorners(std::vector<CvPoint> corners){
        m_btransform_set = true;
        m_map_corners = corners;
    }
    std::vector<CvPoint> GetMapCorner(); // get the corner of the map to transform

    // the threshold,and thin times for map
    void SetMapThreshold(int threshold){m_map_threshold = threshold;}
    int GetMapThreshold(){return m_map_threshold;}
    void SetThinIteration(int iteration){m_thin_iteration = iteration;}
    int GetThinIteration(){return m_thin_iteration;}

    // the config for map point gen
    void SetQualityLevel(double ql){m_mcv_quality_level = ql;}
    double GetQualityLevel(){return m_mcv_quality_level;}
    void SetMinDistance(int ds){m_mcv_min_distance = ds;}
    int GetMinDistance(){return m_mcv_min_distance;}

    // if the map point has been generated
    bool IsMapPointSet(){return m_bmap_point_set;}
    void SetMapPoint(std::vector<CvPoint2D32f> map_point){
        m_bmap_point_set = true;
        m_map_point = map_point;
    }
    std::vector<CvPoint2D32f> GetMapPoint();

    // 去除重复直线时端点到线的距离
    void SetLineDistanceError(double e){m_line_distance_error = e;}
    double GetLineDistanceError(){return m_line_distance_error;}

    bool IsMapLineSet(){return m_bmap_line_set;}
    void SetMapLine(const std::vector<CLine>& lines){
        m_bmap_line_set = true;       
        m_map_line = lines;
    }
    std::vector<CLine> GetMapLine();

    // the threshold for red and blue, to get the map corner and the car position
    void SetThreshold(std::vector<int> threshold);
    std::vector<int> GetThreshold();



    // the com port for blue tooth
    void SetCOM(int com_num){m_com_num = com_num;}
    int GetCOM(){return m_com_num;}

    // the distance error and angle error
    void SetDistanceError(int length){m_distance_error = length;}
    int GetDistanceError(){return m_distance_error;}
    void SetAngleError(int angle){m_angle_error = angle;}
    int GetAngleError() {return m_angle_error;}

    // the length of the car, in the transformed image
    void SetCarLength(int length){m_car_length = length;}
    int GetCarLength(){return m_car_length;}

    void RestConfig();
private:
    bool m_btransform_set;
    int m_corner_size;
    std::vector<CvPoint> m_map_corners;

    // the threshold for the map
    int m_map_threshold;
    // the thin time for the map
    int m_thin_iteration;
    // the config for cvtrackgoodfeatures
    double m_mcv_quality_level;
    int m_mcv_min_distance;
    bool m_bmap_point_set;
    std::vector<CvPoint2D32f> m_map_point;

    // 去除重复直线时端点到线的距离
    double m_line_distance_error;
    // 是否已生成地图线
    bool m_bmap_line_set;
    std::vector<CLine> m_map_line;

    // the threshold for the red and blue
    std::vector<int> m_threshold;

    // the com port number for the port
    int m_com_num;

    // the distance error and angle error
    int m_distance_error;
    int m_angle_error;

    // the length of the car in the transformed image
    int m_car_length;
    void LoadDefaultSetting();
};