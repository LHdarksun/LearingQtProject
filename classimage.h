#ifndef CLASSIMAGE_H
#define CLASSIMAGE_H

#include <QLabel>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;


class ClassImage
{
private:
    int angle_Overall = 0;
    double scale_Overall = 1.;
    int flag_warpAffine = INTER_LINEAR;
    Mat dst_Overall;
    bool isFlipped = false;

public:
    //通过getter获取私有变量
    int getAngle() {return angle_Overall;}
    double getScale() {return scale_Overall;}
    int getFlagWA() {return flag_warpAffine;}
    Mat getDst() {return dst_Overall;}
    bool getFlipped() {return isFlipped;}

    void updateAngle(int angle){
        if (angle >= 360){
            angle %= 360;
        }
        angle_Overall = angle;
    }
    void updateScale(double scale){
        if (scale > 0){
            scale_Overall = scale;
        }
    }
    void updateFlagWA(int flag){
        flag_warpAffine = flag;
    }
    void updateDst(Mat dst){
        dst_Overall = dst;
    }
    void deleteDst(){
        dst_Overall.release();
    }
    void setFlippedTrue(){
        isFlipped = true;
    }
    void setFlippedFalse(){
        isFlipped = false;
    }
};
#endif // CLASSIMAGE_H
