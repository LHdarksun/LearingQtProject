#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <random>
#include "opencv2/opencv.hpp"
#include "classimage.h"

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:                                                //
    QString m_filename;                                 //
    QString realName;
    QString m_format_Origin;
    QString m_format_Temporary;
    QImage m_qimage_Origin;                                    //
    QImage m_qimage_Temporary;
    QImage m_qimg_MagnitudeSpectrum;
    Mat m_cvImg_Origin;
    Mat m_cvImg_Temporary;
    Mat m_cvImg_BeforeNoise;
    Mat m_cvImg_BeforeBrightness;
    Mat m_cvImg_BeforeContrastRatio;
    Mat m_cvImg_BeforeHist;
    ClassImage Img;
    bool isButterWorthNSetted;
    bool isRadiusSetted;
    bool isThreshSetted;
    bool isThresholdTypeSetted;
    bool isRGB;
    int AddBrightnessType;
    int histCount;
    int Random_uniform_int_distribution();
    QImage cvt_Cv2Qimg(Mat m_cvImg);
    void showImageFeatures(bool formatChanged=true, bool origin=false);    //
    void imageModified(bool modified=true);             //
    void ImgShow(Mat m_cvImg);
    void setItemEnabled();
    Mat res_rotConduct(int angle, double scale, Mat src);
    Mat createLPFilter(Size size,Point center,float radius,int type,int n);
    void reSetParameter();
    void imgProc_Symmetry(int flag);
    void imgProc_Rotate(int step);
    void imgProc_Scale(double step);
    void medianBlur_Del();
    void FourierTransform();
    void conductLowFilter();
    void enableButterWorth();
    void chooseBinaryThresholdType();
    void detectFormat();
    void enableGRAY2BINARY();
    void conductCVT(int type);
    void addBrightness();
    void addContrastRatioDirectly();
    void addContrastRatioHist();
    void chooseAddBrightnessType();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //界面功能
    void on_OpenFile_triggered();
    void on_Save_triggered();
    void on_SaveCopy_triggered();
    void on_Reload_triggered();

    //旋转功能
    void on_LeftRotate_clicked();
    void on_RightRotate_clicked();
    void on_AngleEdit_editingFinished();
    void on_hzSlider_Rotate_sliderMoved(int position);
    void do_setFlags_warpAffine();

    //缩放功能
    void on_pbtn_ScaleDown_clicked();
    void on_pbtn_ScaleUp_clicked();
    void on_ScaleEdit_editingFinished();
    void on_hzSlider_Scale_sliderMoved(int position);

    //对称功能
    void on_pbtn_Up_Down_Symmetry_clicked();
    void on_pbtn_Left_Right_Symmetry_clicked();
    void on_pbtn_Origin_Symmetry_clicked();
    void on_pbtn_Restore_Symmetry_clicked();

    //噪声功能
    void on_pBtn_ImpulseNoise_Add_clicked();
    void on_pBtn_GaussNoise_Add_clicked();
    void do_selectNoise_Del();
    void do_selectNoise_SizeofRect();
    void do_selectButterWorth_N();
    void on_radBtn_IdealLowPass_Filter_clicked();
    void on_radBtn_ButterWorthLowPass_Filter_clicked();
    void on_lineEdit_Radius_editingFinished();
    void on_hzSlider_Radius_sliderMoved(int position);

    //色彩功能
    void on_pBtn_cvt_Grayscale2RGB_clicked();
    void on_pBtn_cvt_Grayscale2Binary_clicked();
    void on_hzSlider_BinaryThreshold_sliderMoved(int position);
    void on_lineEdit_BinaryThreshold_editingFinished();
    void on_pBtn_cvt_RGB2Grayscale_clicked();
    void on_radBtn_BinaryThreshold_Black_clicked();
    void on_radBtn_BinaryThreshold_White_clicked();
    void on_pBtn_cvt_RGB2BGR_clicked();
    void on_pBtn_cvt_BGR2RGB_clicked();

    //真彩色图像增强
    void on_lineEdit_Brightness_editingFinished();
    void on_hzSlider_Brightness_sliderMoved(int position);
    void on_pBtn_ImgBeforeBrightness_clicked();
    void on_radBtn_DirectlyAddBrightness_clicked();
    void on_radBtn_HSVBrightness_clicked();
    void on_lineEdit_ContrastRatio_editingFinished();
    void on_hzSlider_ContrastRatio_sliderMoved(int position);
    void on_pBtn_ImgBeforeContrastRatio_clicked();
    void on_pBtn_HistogramEqualizationContrastRatio_clicked();
    void on_pBtn_ImgBeforeHist_clicked();
    void on_pBtn_GenerateHist_clicked();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
