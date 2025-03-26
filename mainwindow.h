#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
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
    QImage m_qimage;                                    //
    Mat m_cvImg_Origin;
    Mat m_cvImg_Temporary;
    ClassImage Img;
    QImage cvt_Cv2Qimg(Mat m_cvImg);
    void showImageFeatures(bool formatChanged=true);    //
    void imageModified(bool modified=true);             //
    void ImgShow(Mat m_cvImg);
    void setItemEnabled();
    Mat res_rotConduct(int angle, double scale, Mat src);
    void reSetParameter();
    void imgProc_Symmetry(int flag);
    void imgProc_Rotate(int step);
    void imgProc_Scale(double step);

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
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
