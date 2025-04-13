#ifndef CLASSIMAGE_H
#define CLASSIMAGE_H

#include <QLabel>
#include <QMessageBox>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;


class ClassImage
{
private:
    int angle_Overall = 0;
    double scale_Overall = 1.;
    int flag_warpAffine = INTER_LINEAR;
    int sizeofRect = 1;
    Mat dst_Overall;
    bool isFlipped = false;
    int lpFilterType = 0;
    double radius = 1;
    int ButterWorth_N = 1;
    int BinaryThreshold = 0;
    int BinaryThresholdType = THRESH_BINARY;
    int Brightness = 0;
    double ContrastRatio = 1.0;
    double Saturation = 1.0;
    int ColdHot = 0;

public:
    //通过getter获取私有变量
    int getAngle() {return angle_Overall;}
    double getScale() {return scale_Overall;}
    int getFlagWA() {return flag_warpAffine;}
    Mat getDst() {return dst_Overall;}
    bool getFlipped() {return isFlipped;}
    int getSizeofRect() {return sizeofRect;}
    int getlpFilterType() {return lpFilterType;}
    int getRadius() {return radius;}
    int getButterWorthN() {return ButterWorth_N;}
    int getBinaryThreshold() {return BinaryThreshold;}
    int getBinaryThresholdType() {return BinaryThresholdType;}
    int getBrightness() {return Brightness;}
    double getContrastRatio() {return ContrastRatio;}
    double getSaturation() {return Saturation;}
    int getColdHot() {return ColdHot;}

    void updateColdHot(int coldhot){
        ColdHot = coldhot;
    }
    void updateSaturation(double saturation){
        Saturation = saturation;
    }
    void updateContrastRatio(double contrast){
        ContrastRatio = contrast;
    }
    void updateBrightness(int bright){
        Brightness = bright;
    }
    void updateBinaryThresholdType(int type){
        BinaryThresholdType = type;
    }
    void updateBinaryThreshold(int threshold){
        BinaryThreshold = threshold;
    }
    void updateButterWorthN(int N){
        ButterWorth_N = N;
    }
    void updateRadius(int radi){
        radius = radi;
    }
    void updatelpFilterType(int type){
        lpFilterType = type;
    }
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
    void updateSizeofRect(int Ssize){
        sizeofRect = Ssize;
    }
};
#endif // CLASSIMAGE_H
