#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isButterWorthNSetted = false;
    isRadiusSetted = false;
    isThreshSetted = false;
    isThresholdTypeSetted = false;
    isRGB = false;

    ColorMapType = COLORMAP_HSV;

    histCount = 0;

    //选择warpAffine的参数
    connect(ui->radBtn_INTER_CUBIC,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_AREA,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_LANCZOS4,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_LINEAR,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_NEAREST,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    //选择滤波函数
    connect(ui->radBtn_medianNoise_Blur,SIGNAL(clicked()),this,SLOT(do_selectNoise_Del()));
    connect(ui->radBtn_lowPassFilter_Del,SIGNAL(clicked()),this,SLOT(do_selectNoise_Del()));
    //选择空间滤波函数的模板大小
    connect(ui->radBtn_RectSize_3,SIGNAL(clicked()),this,SLOT(do_selectNoise_SizeofRect()));
    connect(ui->radBtn_RectSize_5,SIGNAL(clicked()),this,SLOT(do_selectNoise_SizeofRect()));
    connect(ui->radBtn_RectSize_7,SIGNAL(clicked()),this,SLOT(do_selectNoise_SizeofRect()));
    connect(ui->radBtn_RectSize_9,SIGNAL(clicked()),this,SLOT(do_selectNoise_SizeofRect()));

    ui->LabWarning001->setStyleSheet("color: red;");        //new

    ui->radBtn_INTER_CUBIC->setEnabled(false);
    ui->radBtn_INTER_AREA->setEnabled(false);
    ui->radBtn_INTER_LANCZOS4->setEnabled(false);
    ui->radBtn_INTER_LINEAR->setEnabled(false);
    ui->radBtn_INTER_NEAREST->setEnabled(false);
    ui->AngleEdit->setEnabled(false);
    ui->LeftRotate->setEnabled(false);
    ui->RightRotate->setEnabled(false);
    ui->chkBox_Alpha->setEnabled(false);
    ui->chkBox_GrayScale->setEnabled(false);
    ui->hzSlider_Rotate->setEnabled(false);
    ui->ScaleEdit->setEnabled(false);
    ui->pbtn_ScaleDown->setEnabled(false);
    ui->pbtn_ScaleUp->setEnabled(false);
    ui->hzSlider_Scale->setEnabled(false);
    ui->pbtn_Left_Right_Symmetry->setEnabled(false);
    ui->pbtn_Up_Down_Symmetry->setEnabled(false);
    ui->pbtn_Origin_Symmetry->setEnabled(false);
    ui->pbtn_Restore_Symmetry->setEnabled(false);
    ui->pBtn_GaussNoise_Add->setEnabled(false);
    ui->pBtn_ImpulseNoise_Add->setEnabled(false);
    ui->radBtn_medianNoise_Blur->setEnabled(false);
    ui->radBtn_lowPassFilter_Del->setEnabled(false);
    ui->radBtn_RectSize_3->setEnabled(false);
    ui->radBtn_RectSize_5->setEnabled(false);
    ui->radBtn_RectSize_7->setEnabled(false);
    ui->radBtn_RectSize_9->setEnabled(false);
    ui->radBtn_ButterWorthLowPass_Filter->setEnabled(false);
    ui->radBtn_IdealLowPass_Filter->setEnabled(false);
    ui->radBtn_ButterWorthN_1->setEnabled(false);
    ui->radBtn_ButterWorthN_2->setEnabled(false);
    ui->radBtn_ButterWorthN_3->setEnabled(false);
    ui->lineEdit_Radius->setEnabled(false);
    ui->hzSlider_Radius->setEnabled(false);
    ui->pBtn_cvt_Grayscale2Binary->setEnabled(false);
    ui->pBtn_cvt_Grayscale2RGB->setEnabled(false);
    ui->pBtn_cvt_RGB2Grayscale->setEnabled(false);
    ui->radBtn_BinaryThreshold_Black->setEnabled(false);
    ui->radBtn_BinaryThreshold_White->setEnabled(false);
    ui->hzSlider_BinaryThreshold->setEnabled(false);
    ui->lineEdit_BinaryThreshold->setEnabled(false);
    ui->pBtn_cvt_RGB2BGR->setEnabled(false);
    ui->pBtn_cvt_BGR2RGB->setEnabled(false);
    ui->lineEdit_Brightness->setEnabled(false);
    ui->hzSlider_Brightness->setEnabled(false);
    ui->pBtn_ImgBeforeBrightness->setEnabled(false);
    ui->radBtn_DirectlyAddBrightness->setEnabled(false);
    ui->radBtn_HSVBrightness->setEnabled(false);
    ui->hzSlider_ContrastRatio->setEnabled(false);
    ui->lineEdit_ContrastRatio->setEnabled(false);
    ui->pBtn_ImgBeforeContrastRatio->setEnabled(false);
    ui->pBtn_ImgBeforeHist->setEnabled(false);
    ui->pBtn_HistogramEqualizationContrastRatio->setEnabled(false);
    ui->pBtn_GenerateHist->setEnabled(false);
    ui->pBtn_ImgBeforeSaturation->setEnabled(false);
    ui->lineEdit_Saturation->setEnabled(false);
    ui->hzSlider_Saturation->setEnabled(false);
    ui->pBtn_ImgBeforeColdHot->setEnabled(false);
    ui->lineEdit_ColdHot->setEnabled(false);
    ui->hzSlider_ColdHot->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*以下为加载，保存图像的部分
 * 其中函数有:
 * cvt_Cv2Qimg(Mat):将图像数据从Mat类转换为QImage类
 * showImageFeatures(bool formatChanged):显示图像的具体数据：
    *Format
    *Depth
    *BitPlane
    *Width
    *Height
    *ByteSize
    *DPM
    *isAlpha
    *isGrayScale
 *on_OpenFile_triggered():按下后选择打开一个图像文件
 *on_Save_triggered():按下后保存当前文件到其自身地址（覆盖）
 *on_SaveCopy_triggered():按下后将图片另存为再另一地址
 *on_Reload_triggered():按下后重新加载图片（加载到上一次保存的状态）
 *setItemEnabled():在加载图片后重新启用禁用的组件
 *reSetParameter():重置部分特性的函数
 *imageModified():解禁保存与另存为的函数
 *ImgShow():显示/更新图像的函数
 *setItemEnabled():载入图像后将功能按钮解禁的函数
 */

QImage MainWindow::cvt_Cv2Qimg(Mat m_cvImg){
    QImage m_qtImage = QImage(m_cvImg.data,m_cvImg.cols,m_cvImg.rows,m_cvImg.step,
                             (m_cvImg.channels() == 3) ? QImage::Format_RGB888 :
                             (m_cvImg.channels() == 4) ? QImage::Format_RGBA8888 :
                             QImage::Format_Grayscale8);
    return m_qtImage;
}

void MainWindow::showImageFeatures(bool formatChanged,bool origin){
    if (!origin)
        m_qimage_Temporary = cvt_Cv2Qimg(m_cvImg_Temporary);
    else
        m_qimage_Temporary = cvt_Cv2Qimg(m_cvImg_Origin);
    if (formatChanged){                                                     //若改变了图像文件格式则显示格式
        QImage::Format fmt = m_qimage_Temporary.format();
        if (fmt == QImage::Format_RGB32)
            ui->editImg_Format->setText("32-bit RGB(0xffRRGGBB)");
        else if (fmt == QImage::Format_RGB16)
            ui->editImg_Format->setText("16-bit RGB565");
        else if (fmt == QImage::Format_RGB888){
            if (isRGB)
                ui->editImg_Format->setText("24-bit RGB888");
            else
                ui->editImg_Format->setText("24-bit BGR888(rgb)");
        }
        else if (fmt == QImage::Format_Grayscale8)
            ui->editImg_Format->setText("8-bit grayscale");
        else if (fmt == QImage::Format_Grayscale16)
            ui->editImg_Format->setText("16-bit grayscale");
        else if (fmt == QImage::Format_RGB16)
            ui->editImg_Format->setText("16-bit RGB565");
        else if (fmt == QImage::Format_ARGB32)
            ui->editImg_Format->setText("32-bit RGB(0xAARRGGBB)");
        else if (fmt == QImage::Format_RGBA8888)
            ui->editImg_Format->setText("32-bit RGB(0xRRGGBBAA)");
        else if (fmt == QImage::Format_Indexed8)
            ui->editImg_Format->setText("8-bit indexes into a colormap");
        else
            ui->editImg_Format->setText(QString("Format= %1,其它格式").arg(fmt));   //用法见someThoughts"2."

        ui->editImg_Depth->setText(QString("%1 bits/pixel").arg(m_qimage_Temporary.depth()));
        ui->editImg_BitPlane->setText(QString("%1 bits").arg(m_qimage_Temporary.bitPlaneCount()));
        ui->editImg_DefineFlag->setText(QString("%1").arg(m_cvImg_Origin.type()));
        ui->chkBox_Alpha->setChecked(m_qimage_Temporary.hasAlphaChannel());
        ui->chkBox_GrayScale->setChecked(m_qimage_Temporary.isGrayscale());
        ui->chkBox_Alpha->setEnabled(false);
        ui->chkBox_GrayScale->setEnabled(false);
    }
    ui->editImg_Height->setText(QString("%1 像素").arg(m_qimage_Temporary.height()));
    ui->editImg_Width->setText(QString("%1 像素").arg(m_qimage_Temporary.width()));
    qsizetype sz = m_qimage_Temporary.sizeInBytes();
    if (sz < 1024 * 9)
        ui->editImg_SizeByte->setText(QString("%1 Bytes").arg(sz));
    else
        ui->editImg_SizeByte->setText(QString("%1 KB").arg(sz/1024));
    QString dpi = QString("DPI_X = %1,"
    " DPI_Y = %2").arg(m_qimage_Temporary.dotsPerMeterX()*0.0254).arg(m_qimage_Temporary.dotsPerMeterY()*0.0254);
    ui->editImg_DPM->setText(dpi);
}

void MainWindow::on_OpenFile_triggered(){
    QString curPath = QDir::currentPath();                                  //得到地址
    QString filter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";        //过滤器，分选对应格式
    /*this:指代父窗口为当前组件，并弹出一个子对话框
     * “选择图片文件”:子对话框的标题
     * curPath:默认打开的目录
     * filter:文本过滤器
     */
    QString fileName = QFileDialog::getOpenFileName(this,"选择图片文件",curPath,filter);
    if (fileName.isEmpty())
        return;                                                                   //若没有成功选择图片则返回
    Mat rawImg = imread(fileName.toStdString(),IMREAD_UNCHANGED);           //提取原图像
    if (rawImg.empty()){
        QMessageBox::critical(this,"Error!!!","打开图像失败");
        return;
    }
    else if (rawImg.channels() == 3){
        cvtColor(rawImg,rawImg,COLOR_BGR2RGB);
        isRGB = true;
    }
    else if (rawImg.channels() == 4){
        cvtColor(rawImg,rawImg,COLOR_BGRA2RGBA);
    }                                                                       //转换图像从opencv格式到qt格式
    m_cvImg_Origin = rawImg.clone();                                        //处理完原始图像的通道后赋给cv
    m_cvImg_Temporary = rawImg.clone();
    ImgShow(m_cvImg_Origin);
    m_qimage_Origin = cvt_Cv2Qimg(m_cvImg_Origin);         //new
    ui->statusbar->showMessage(fileName);                                   //在底部显示图片名称
    m_filename = fileName;                                                  //保存当前图像的名称
    QFileInfo fileInfo(fileName);                                           //显示图像信息
    QDir::setCurrent(fileInfo.absolutePath());                              /*设置当前位置？*/
    ui->ProcFuncWidget->setCurrentIndex(0);
    ui->radBtn_INTER_LINEAR->setChecked(true);
    setItemEnabled();
    showImageFeatures();                                                    //显示图片属性
}

void MainWindow::on_OpenPseudoColor_triggered(){
    QString curPath = QDir::currentPath();
    QString filter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    QString fileName = QFileDialog::getOpenFileName(this,"选择图片文件R(请选择灰度图)",curPath,filter);
    if (fileName.isEmpty()){
        QMessageBox::critical(this,"Error!","打卡错误：该图像无法打开，请选择其他图像！");
        return;
    }
    Mat rawImg = imread(fileName.toStdString(),IMREAD_GRAYSCALE);

    Mat FaultColor;
    applyColorMap(rawImg,FaultColor,ColorMapType);

    m_cvImg_Origin = FaultColor.clone();
    m_cvImg_Temporary = FaultColor.clone();
    ImgShow(m_cvImg_Origin);
    m_qimage_Origin = cvt_Cv2Qimg(m_cvImg_Origin);
    ui->statusbar->showMessage(fileName);
    m_filename = fileName;
    QFileInfo fileInfo(fileName);
    QDir::setCurrent(fileInfo.absolutePath());
    ui->ProcFuncWidget->setCurrentIndex(0);
    ui->radBtn_INTER_LINEAR->setChecked(true);
    setItemEnabled();
    showImageFeatures();
}

void MainWindow::on_OpenFalseColor_triggered(){
    QString curPath = QDir::currentPath();
    QString filter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    QString fileName = QFileDialog::getOpenFileName(this,"选择图片文件R(请选择灰度图)",curPath,filter);
    if (fileName.isEmpty()){
        QMessageBox::critical(this,"Error!","打卡错误：该图像无法打开，请选择其他图像！");
        return;
    }
    Mat rawImg = imread(fileName.toStdString(),IMREAD_GRAYSCALE);
    QString fileName_1 = QFileDialog::getOpenFileName(this,"选择图片文件G(请选择灰度图)",curPath,filter);
    if (fileName_1.isEmpty()){
        QMessageBox::critical(this,"Error!","打卡错误：该图像无法打开，请选择其他图像！");
        return;
    }
    Mat rawImg_1 = imread(fileName_1.toStdString(),IMREAD_GRAYSCALE);
    QString fileName_2 =QFileDialog::getOpenFileName(this,"选择图片文件B(请选择灰度图)",curPath,filter);
    if (fileName_2.isEmpty()){
        QMessageBox::critical(this,"Error!","打卡错误：该图像无法打开，请选择其他图像！");
        return;
    }
    Mat rawImg_2 = imread(fileName_2.toStdString(),IMREAD_GRAYSCALE);

    Mat PseudoColor;
    Mat PseudoColor_1;
    Mat PseudoColor_2;
    applyColorMap(rawImg,PseudoColor,COLORMAP_HOT);
    applyColorMap(rawImg_1,PseudoColor_1,COLORMAP_MAGMA);
    applyColorMap(rawImg,PseudoColor_2,COLORMAP_TURBO);

    Mat dst;
    vector<Mat> channels;
    channels[0] = PseudoColor.clone();
    channels[1] = PseudoColor_1.clone();
    channels[2] = PseudoColor_2.clone();
    merge(channels,dst);

    m_cvImg_Origin = dst.clone();
    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Origin);
    m_qimage_Origin = cvt_Cv2Qimg(m_cvImg_Origin);
    ui->statusbar->showMessage(fileName);
    m_filename = fileName;
    QFileInfo fileInfo(fileName);
    QDir::setCurrent(fileInfo.absolutePath());
    ui->ProcFuncWidget->setCurrentIndex(0);
    ui->radBtn_INTER_LINEAR->setChecked(true);
    setItemEnabled();
    showImageFeatures();
}

void MainWindow::on_Save_triggered(){
    m_qimage_Temporary = cvt_Cv2Qimg(m_cvImg_Temporary);
    m_qimage_Temporary.save(m_filename);
    imageModified(false);                                                   //将save设为不可选项直至下一修改
}

void MainWindow::on_SaveCopy_triggered(){
    QString filter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    QString fileName = QFileDialog::getSaveFileName(this,"保存文件",m_filename,filter);
    if (fileName.isEmpty())
        return;
    m_qimage_Temporary = cvt_Cv2Qimg(m_cvImg_Temporary);
    m_qimage_Temporary.save(fileName);                                                 //保存为新的文件
    m_filename = fileName;
    ui->statusbar->showMessage(fileName);
    imageModified(false);
}

void MainWindow::on_Reload_triggered(){
    m_qimage_Origin = cvt_Cv2Qimg(m_cvImg_Origin);
    QPixmap pixmap = QPixmap::fromImage(m_qimage_Origin.scaled(ui->labPic->size(),Qt::KeepAspectRatio));
    ui->labPic->setPixmap(pixmap);
    ui->ProcFuncWidget->setCurrentIndex(0);
    reSetParameter();
    showImageFeatures(true,true);
    imageModified(false);
}

void MainWindow::reSetParameter(){
    ui->pBtn_cvt_Grayscale2Binary->setEnabled(false);
    ui->AngleEdit->setText(QString("%1").arg(0));
    ui->ScaleEdit->setText(QString("%1").arg(1.0));
    ui->radBtn_INTER_LINEAR->setChecked(true);
    Img.updateAngle(0);
    Img.updateScale(1.);
    Img.updateFlagWA(INTER_LINEAR);
    Img.updateSizeofRect(1);
    Img.setFlippedFalse();
    Img.updatelpFilterType(0);
    Img.updateRadius(1.0);
    Img.updateButterWorthN(1);
    Img.updateBinaryThreshold(0);
    Img.updateBinaryThresholdType(THRESH_BINARY);
    Img.updateBrightness(0);
    Img.updateContrastRatio(1.0);
    Img.updateSaturation(1.0);
}

void MainWindow::imageModified(bool modified){
    ui->Reload->setEnabled(modified);
    ui->Save->setEnabled(modified);
}

void MainWindow::ImgShow(Mat m_cvImg_Temp){
    m_qimage_Temporary = cvt_Cv2Qimg(m_cvImg_Temp);
    /*创建对象用于界面显示
     * scaled()用于限制图像大小
     * KeepAspectRatio意为保持长宽比
     */
    QPixmap pixmap = QPixmap::fromImage(m_qimage_Temporary.scaled(ui->labPic->size(),Qt::KeepAspectRatio));
    ui->labPic->setPixmap(pixmap);
}

void MainWindow::setItemEnabled(){
    ui->AngleEdit->setEnabled(true);
    ui->RightRotate->setEnabled(true);
    ui->LeftRotate->setEnabled(true);
    ui->hzSlider_Rotate->setEnabled(true);
    ui->ScaleEdit->setEnabled(true);
    ui->pbtn_ScaleDown->setEnabled(true);
    ui->pbtn_ScaleUp->setEnabled(true);
    ui->hzSlider_Scale->setEnabled(true);
    ui->pbtn_Left_Right_Symmetry->setEnabled(true);
    ui->pbtn_Up_Down_Symmetry->setEnabled(true);
    ui->pbtn_Origin_Symmetry->setEnabled(true);
    ui->radBtn_INTER_CUBIC->setEnabled(true);
    ui->radBtn_INTER_LINEAR->setEnabled(true);
    ui->radBtn_INTER_LANCZOS4->setEnabled(true);
    ui->radBtn_INTER_AREA->setEnabled(true);
    ui->radBtn_INTER_NEAREST->setEnabled(true);
    ui->pbtn_Restore_Symmetry->setEnabled(true);
    ui->radBtn_lowPassFilter_Del->setEnabled(true);
    ui->pBtn_GaussNoise_Add->setEnabled(true);
    ui->pBtn_ImpulseNoise_Add->setEnabled(true);
    ui->radBtn_RectSize_3->setEnabled(true);
    ui->radBtn_RectSize_5->setEnabled(true);
    ui->radBtn_RectSize_7->setEnabled(true);
    ui->radBtn_RectSize_9->setEnabled(true);
    ui->radBtn_ButterWorthN_1->setEnabled(true);
    ui->radBtn_ButterWorthN_2->setEnabled(true);
    ui->radBtn_ButterWorthN_3->setEnabled(true);
    ui->lineEdit_Radius->setEnabled(true);
    ui->hzSlider_Radius->setEnabled(true);
    ui->radBtn_IdealLowPass_Filter->setEnabled(true);
    ui->pBtn_cvt_Grayscale2RGB->setEnabled(true);
    ui->pBtn_cvt_RGB2Grayscale->setEnabled(true);
    ui->radBtn_BinaryThreshold_Black->setEnabled(true);
    ui->radBtn_BinaryThreshold_White->setEnabled(true);
    ui->hzSlider_BinaryThreshold->setEnabled(true);
    ui->lineEdit_BinaryThreshold->setEnabled(true);
    ui->pBtn_cvt_RGB2BGR->setEnabled(true);
    ui->pBtn_cvt_BGR2RGB->setEnabled(true);
    ui->radBtn_DirectlyAddBrightness->setEnabled(true);
    ui->radBtn_HSVBrightness->setEnabled(true);
    ui->pBtn_ImgBeforeContrastRatio->setEnabled(true);
    ui->pBtn_ImgBeforeHist->setEnabled(true);
    ui->pBtn_ImgBeforeSaturation->setEnabled(true);
    ui->pBtn_ImgBeforeColdHot->setEnabled(true);
}





/*以下部分用作图像的旋转，对称和缩放操作（此处不会带参数，只显示函数名）
 * !!注意以后的图像处理都要基于原始的m_cvImg,需要变换就设置一个temp来存储变化吧!!
 *on_LeftRotate(RightRotate)_triggered():使图像逆（正）向旋转，scale控制缩放倍率，angle控制旋转角度
 *imgProc_Rotate():在on_LeftRotate(RightRotate)_triggered中实现图像旋转的函数
 *res_rotConduct():执行生成旋转，缩放后的图像矩阵的函数
 *on_AngleEdit_editingFinished():在AngleEdit中修改完值后更新angle_Overall值进行旋转
 *on_ScaleEdit_editingFinished():在ScaleEdit中修改完值后更新scale_Overall值进行缩放
 *on_hzSlider_Rotate_sliderMoved():滑动旋转滚动条后生成滑动后图像的函数
 *on_hzSlider_Scale_sliderMoved():滑动缩放滚动条后生成滑动后的图像的函数
 *on_pbtn_Up_Down_Symmetry_clicked():点击X轴变换后生成对应图像的函数
 *on_pbtn_Left_Right_Symmetry_clicked():点击Y轴变换后生成对应图像的函数
 *on_pbtn_Origin_Symmetry_clicked():点击原点变换后生成对应图像的函数
 *on_pbtn_Restore_Symmetry_clicked():实际上是一个相当于重新载入的函数
 *on_pbtn_UpScale_clicked():点击后scale_Overall增加的函数
 *on_pbtn_DownScale_clicked():点击后scale_Overall减少的函数
 *imgProc_Scale():在on_pbtn_Up(Down)Scale_clicked()函数中实现scale_Overall增减的函数
 */
Mat MainWindow::res_rotConduct(int angle, double scale, Mat src){
    Mat dst;
    Mat rotation;
    Rect2f bbox;
    Point centerP;

    centerP = Point(src.cols / 2, src.rows / 2);
    rotation = getRotationMatrix2D(centerP,angle,scale);
    /*warpAffine(InputArray src, OutputArray dst, InputArray mat,
     *          Size dsize = Size(), int flags = INTER_LINEAR,
     *          int borderMode = BORDER_CONSTANT, Scalar borderValue = Scalar());
     * src:输入图像
     * dst:输出图像
     * mat:2*3变换矩阵
     * dsize:输出图像大小（参数直接写Size()则为输入图像大小）
     * flags:指定的插值方法，可详见插值办法五个按钮的命名（默认INTER_LINEAR）
     * borderMode:边缘像素处理方法（默认常量填充）
     * borderValue:指定填充的边界值（默认0）
     */
    warpAffine(src,dst,rotation,src.size(),Img.getFlagWA(),BORDER_CONSTANT);

    if (dst.empty()){
        qDebug() << "Error : empty image after rotation!";
    }

    return dst;
}

void MainWindow::do_setFlags_warpAffine(){
    if (ui->radBtn_INTER_LINEAR->isChecked())
        Img.updateFlagWA(INTER_LINEAR);
    else if (ui->radBtn_INTER_AREA->isChecked())
        Img.updateFlagWA(INTER_AREA);
    else if (ui->radBtn_INTER_CUBIC->isChecked())
        Img.updateFlagWA(INTER_CUBIC);
    else if (ui->radBtn_INTER_NEAREST->isChecked())
        Img.updateFlagWA(INTER_NEAREST);
    else if (ui->radBtn_INTER_LANCZOS4->isChecked())
        Img.updateFlagWA(INTER_LANCZOS4);
    else
        Img.updateFlagWA(INTER_LINEAR);
}

void MainWindow::on_LeftRotate_clicked(){
    imgProc_Rotate(15);
}

void MainWindow::on_RightRotate_clicked(){
    imgProc_Rotate(-15);
}

void MainWindow::imgProc_Rotate(int step){
    int angle = Img.getAngle();
    double scale = Img.getScale();
    Mat cvImgTemp;
    Mat src = m_cvImg_Temporary;
    angle += step;
    Img.updateAngle(angle);
    Mat dst = res_rotConduct(angle,scale,(Img.getFlipped() ? Img.getDst() : src));
    ui->AngleEdit->setText(QString("%1").arg(angle));
    cvImgTemp = dst.clone();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}//src代表输入图像，dst代表输出图像,.clone()为了防止QImage引用被释放的Mat.data指针

void MainWindow::on_AngleEdit_editingFinished()
{
    QString angleString = ui->AngleEdit->text();
    int angle = angleString.toInt();
    double scale = Img.getScale();
    Mat src = m_cvImg_Temporary;
    Img.updateAngle(angle);
    int angleTemp = angle;
    if (abs(angleTemp) > 360){
        angleTemp = angleTemp % 360;
    }
    ui->hzSlider_Rotate->setValue(angleTemp);
    Mat dst = res_rotConduct(angle,scale,(Img.getFlipped() ? Img.getDst() : src));
    Mat cvImgTemp = dst.clone();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}

void MainWindow::on_hzSlider_Rotate_sliderMoved(int position)
{
    double scale = Img.getScale();
    Img.updateAngle(position);
    Mat src = m_cvImg_Temporary;
    Mat dst = res_rotConduct(position,scale,(Img.getFlipped() ? Img.getDst() : src));
    ui->AngleEdit->setText(QString("%1").arg(position));
    Mat cvImgTemp = dst.clone();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}

void MainWindow::on_pbtn_ScaleDown_clicked(){
    imgProc_Scale(-0.1);
}

void MainWindow::on_pbtn_ScaleUp_clicked(){
    imgProc_Scale(0.1);
}

void MainWindow::imgProc_Scale(double step){
    int angle = Img.getAngle();
    double scale = Img.getScale();
    Mat cvImgTemp;
    Mat src = m_cvImg_Temporary;
    scale += step;
    Img.updateScale(scale);
    Mat dst = res_rotConduct(angle,scale,(Img.getFlipped() ? Img.getDst() : src));
    ui->ScaleEdit->setText(QString("%1").arg(scale));
    cvImgTemp = dst.clone();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}

void MainWindow::on_ScaleEdit_editingFinished(){
    QString scaleString = ui->ScaleEdit->text();
    double scale = scaleString.toDouble();
    int angle = Img.getAngle();
    Mat src = m_cvImg_Temporary;
    Img.updateScale(scale);
    int scaleTemp = scale;
    ui->hzSlider_Rotate->setValue(scaleTemp);
    Mat dst = res_rotConduct(angle,scale,(Img.getFlipped() ? Img.getDst() : src));
    Mat cvImgTemp = dst.clone();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}

void MainWindow::on_hzSlider_Scale_sliderMoved(int position){
    int angle = Img.getAngle();
    double posf = static_cast<double>(position);
    double scaleTemp = posf / 100;
    Mat src = m_cvImg_Temporary;
    Img.updateScale(scaleTemp);
    Mat dst = res_rotConduct(angle,scaleTemp,(Img.getFlipped() ? Img.getDst() : src));
    ui->ScaleEdit->setText(QString("%1").arg(scaleTemp));
    Mat cvImgTemp = dst.clone();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}

void MainWindow::on_pbtn_Up_Down_Symmetry_clicked(){
    imgProc_Symmetry(0);
}

void MainWindow::on_pbtn_Left_Right_Symmetry_clicked(){
    imgProc_Symmetry(1);
}

void MainWindow::on_pbtn_Origin_Symmetry_clicked(){
    imgProc_Symmetry(-1);
}

void MainWindow::on_pbtn_Restore_Symmetry_clicked(){
    reSetParameter();
    showImageFeatures(true,true);
    m_cvImg_Temporary = m_cvImg_Origin.clone();
    ImgShow(m_cvImg_Origin);
    imageModified(true);
}

void MainWindow::imgProc_Symmetry(int flag){
    Mat src = m_cvImg_Origin;
    Mat dst_1;
    /*flip(InputArray src,OutputArray dst,int flipCode)用于图像的翻转
     * src为原始图像
     * dst为处理后的图像
     * flipCode为旋转常量：
        *flipCode = 0:沿X轴旋转
        *flipCode = 1:沿Y轴旋转
        *flipCode < 0(常为-1):同时沿X，Y轴旋转
     */
    flip(src,dst_1,flag);
    Mat dst_1Temp = dst_1.clone();
    Mat dst_2 = res_rotConduct(Img.getAngle(),Img.getScale(),dst_1Temp);
    Mat dst_flipped = dst_1Temp.clone();
    Img.updateDst(dst_flipped);
    Mat cvImgTemp = dst_2.clone();
    Img.setFlippedTrue();
    m_cvImg_Temporary = cvImgTemp;
    ImgShow(cvImgTemp);
    imageModified(true);
}




/*以下部分用作图像的加噪与去噪
 *RanDom_uniform_int_distribution()
 *GaussNoise_Add()
 *ImpulseNoise_Add()
 *do_selectNoise_Add()
 *
 */

/*用于生成均匀分布的整型随机数
 *random_device是机器真随机数生成器，mt19937
 *mt19937是生成通用随机数的梅森旋转算法
 *uniform_int_distribution<int> 用于生成均匀分布的整形随机数
 */
int MainWindow::Random_uniform_int_distribution(){
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(-65535,65535);
    int seed = distribution(generator);
    return seed;
}

/*Mat::zeros(size,type)用于生成对应大小和数据类型的初值为零的矩阵
 *randn(src,μ,σ)用于生成符合高斯分布的随机数
 */
void MainWindow::on_pBtn_GaussNoise_Add_clicked(){
    Mat src = m_cvImg_Temporary.clone();
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    Mat noise = Mat::zeros(src.size(),src.type());
    if (noise.channels() == 1){
        randn(noise,25,30);
    }
    else if (noise.channels() == 3){
        randn(noise,Scalar(25,25,25),Scalar(30,30,30));
    }
    else if (noise.channels() == 4){
        randn(noise,Scalar(25,25,25,25),Scalar(30,30,30,30));
    }
    Mat dst;
    add(noise,src,dst);
    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Temporary);
}

/*RNG类（生成的rng是个对象）是随机数生成器类，uniform以生成均匀分布随机数,guassian以生成高斯分布随机数
 *at<DataTyope>指Mat数据中在后面指定的坐标上的位置对应的值，DataType指坐标的类型(如Vec3b,uchar,Vec4f...)
 */
void MainWindow::on_pBtn_ImpulseNoise_Add_clicked(){
    Mat src = m_cvImg_Temporary.clone();
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    Mat noiseImg = Mat::zeros(src.size(),src.type());
    int seed = Random_uniform_int_distribution();
    RNG rng(seed);
    int h = src.rows;
    int w = src.cols;
    int nums = rng.uniform(0,10000);
    Mat dst = src.clone();
    for (int i=0;i<nums;i++){
        int x = rng.uniform(0,h);
        int y = rng.uniform(0,w);
        if (noiseImg.channels() == 1){
            if (i % 2 == 0){
                dst.at<uchar>(x,y) = 255;
                noiseImg.at<uchar>(x,y) = 255;
            }
            else{
                dst.at<uchar>(x,y) = 0;
                noiseImg.at<uchar>(x,y) = 0;
            }
        }
        if (noiseImg.channels() == 3){
            if (i % 2 == 0){
                dst.at<Vec3b>(x,y) = Vec3b(255,255,255);
                noiseImg.at<Vec3b>(x,y) = Vec3b(255,255,255);
            }
            else{
                dst.at<Vec3b>(x,y) = Vec3b(0,0,0);
                noiseImg.at<Vec3b>(x,y) = Vec3b(0,0,0);
            }
        }
        if (noiseImg.channels() == 4){
            if (i % 2 == 0){
                dst.at<Vec4b>(x,y) = Vec4b(255,255,255,255);
                noiseImg.at<Vec4b>(x,y) = Vec4b(255,255,255,255);
            }
            else{
                dst.at<Vec4b>(x,y) = Vec4b(0,0,0,0);
                noiseImg.at<Vec4b>(x,y) = Vec4b(0,0,0,0);
            }
        }
    }
    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::medianBlur_Del(){
    Mat src = m_cvImg_Temporary.clone();
    Mat dst;
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    medianBlur(src,dst,Img.getSizeofRect());
    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Temporary);
}

/*创建低通滤波器的函数
 *size:cv存储图像长宽的数据类型
 *center:中心点
 */
Mat MainWindow::createLPFilter(Size size,Point center,float radius,int type,int n){
    Mat lpFilter = Mat::zeros(size,CV_8UC1);    //
    int rows = size.height;
    int cols = size.width;
    if (radius <= 0){
        return lpFilter;
    }
    //理想低通滤波器
    if (type == 0){
        for (int r=0;r<rows;r++){
            for (int c=0;c<cols;c++){
                float norm2 = pow(abs(float(r-center.y)),2) + pow(abs(float(c-center.x)),2);
                if (sqrt(norm2) < radius){
                    lpFilter.at<float> (r,c) = 1;
                }
                else {
                    lpFilter.at<float> (r,c) = 0;
                }
            }
        }
    }
    //巴特沃斯低通滤波器
    if (type == 1){
        for (int r=0;r<rows;r++){
            for (int c=0;c<cols;c++){
                float norm2 = pow(abs(float(r-center.y)),2 + pow(abs(float(c-center.x)),2));
                lpFilter.at<float> (r,c) = float(1.0/(1.0 + pow(sqrt(norm2)/radius,2.0*n)));
            }
        }
    }
    return lpFilter;
}

void MainWindow::on_radBtn_IdealLowPass_Filter_clicked(){
    Img.updatelpFilterType(0);
}

void MainWindow::on_radBtn_ButterWorthLowPass_Filter_clicked(){
    Img.updatelpFilterType(1);
}

/*这是我自己写的错误代码
void MainWindow::FourierTransform(){
    //一、得实矩阵
    Mat cvImg_In = m_cvImg_Temporary.clone();
    vector<Mat> channelsIn(3);
    vector<Mat> channelsReal(3);
    int r = cvImg_In.rows;
    int c = cvImg_In.cols;
    split(cvImg_In,channelsIn);
    for (int i=0;i<3;i++){
        channelsReal[i] = channelsIn[i] * pow(-1,(r+c % 2 == 1 ? 1 : 2));
    }
    Mat cvImg_Real;
    merge(channelsReal,cvImg_Real);

    //二、得扩充后的矩阵
    Mat cvImg_Padded;
    int DownPixel = getOptimalDFTSize(r) - r;
    int RightPixel = getOptimalDFTSize(c) - c;
    copyMakeBorder(cvImg_Real,cvImg_Padded,0,DownPixel,0,RightPixel,BORDER_CONSTANT,Scalar::all(0));

    //三、傅里叶变换
    Mat cvImg_DFTed;
    Mat cvImg_Complex;
    Mat planesDFT[2] = {Mat_<float> (cvImg_Padded), Mat::zeros(cvImg_Padded.size(),CV_32F)};
    merge(planesDFT,2,cvImg_Complex);

    dft(cvImg_Complex,cvImg_DFTed,DFT_COMPLEX_OUTPUT);

    //四（1）、应用滤波器
    Mat planesDFTed[2];
    split(cvImg_DFTed,planesDFTed);
    vector<Mat> channelsDFTed(3);
    vector<Mat> channelsFiltered(3);
    Mat channelFiltered;
    split(planesDFTed[0],channelsDFTed);
    int r_P = planesDFTed[0].rows;
    int c_P = planesDFTed[0].cols;
    Point center(c_P,r_P);
    Mat lpFilter = createLPFilter(planesDFTed[0].size(),center,Img.getRadius(),Img.getlpFilterType(),Img.getButterWorthN());

    for (int i=0;i<3;i++){
        multiply(channelsDFTed[i],lpFilter,channelFiltered);
        channelsFiltered.push_back(channelFiltered);
    }

    Mat planesFiltered[2];
    merge(channelsFiltered,planesFiltered[0]);
    multiply(planesDFTed[1],lpFilter,planesFiltered[1]);

    //四（2）、得幅度谱
    Mat magnitudeSpectrum;
    magnitude(planesFiltered[0],planesFiltered[1],magnitudeSpectrum);
    magnitudeSpectrum += Scalar::all(1);
    log(magnitudeSpectrum,magnitudeSpectrum);
    normalize(magnitudeSpectrum,magnitudeSpectrum,0,255,NORM_MINMAX);

    int cx = magnitudeSpectrum.cols / 2;
    int cy = magnitudeSpectrum.rows / 2;
    Mat q0 = magnitudeSpectrum(Rect(0,0,cx,cy));
    Mat q1 = magnitudeSpectrum(Rect(cx,cy,cx,cy));
    Mat q2 = magnitudeSpectrum(Rect(0,cy,cx,cy));
    Mat q3 = magnitudeSpectrum(Rect(cx,0,cx,cy));

    swap(q0,q1);
    swap(q2,q3);

    realName = QFileInfo(m_filename).baseName();
    m_qimg_MagnitudeSpectrum = cvt_Cv2Qimg(magnitudeSpectrum);
    m_qimg_MagnitudeSpectrum.save(QString(realName + "_MagnitudeSpectrum.png"));

    swap(q0,q1);
    swap(q2,q3);

    //五、傅里叶逆变换
    Mat cvImg_ComplexFiltered;
    Mat cvImg_Intransformed;
    merge(planesFiltered,2,cvImg_ComplexFiltered);
    dft(cvImg_ComplexFiltered,cvImg_Intransformed,DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE);
    cvImg_Intransformed.convertTo(cvImg_Intransformed,CV_8UC1);

    //六、裁剪
    Mat cvImg_Reconstructed;
    cvImg_Reconstructed = cvImg_Intransformed(Rect(0,0,c,r));

    //七、得图像
    Mat cvImg_Final;
    vector<Mat> channelsReconstructed(3);
    vector<Mat> channelsFinal(3);
    split(cvImg_Reconstructed,channelsFinal);
    for (int i=0;i<3;i++){
        channelsFinal[i] = channelsReconstructed[i] * pow(-1,(r+c % 2 == 1 ? 1 : 2));
    }
    merge(channelsFinal,cvImg_Final);

    m_cvImg_Temporary = cvImg_Final.clone();
    ImgShow(m_cvImg_Temporary);
}
*/

//AI跑的
void MainWindow::FourierTransform() {
    // 输入图像处理
    Mat cvImg_In = m_cvImg_Temporary.clone();
    if (cvImg_In.channels() > 1) {
        cvtColor(cvImg_In, cvImg_In, COLOR_BGR2GRAY);
    }
    cvImg_In.convertTo(cvImg_In, CV_32F);

    // 中心化
    Mat shiftMat = Mat::zeros(cvImg_In.size(), CV_32F);
    for (int y = 0; y < cvImg_In.rows; y++) {
        for (int x = 0; x < cvImg_In.cols; x++) {
            shiftMat.at<float>(y, x) = ((x + y) % 2 == 0) ? 1 : -1;
        }
    }
    multiply(cvImg_In, shiftMat, cvImg_In);

    // 扩展图像
    Mat padded;
    int optRows = getOptimalDFTSize(cvImg_In.rows);
    int optCols = getOptimalDFTSize(cvImg_In.cols);
    copyMakeBorder(cvImg_In, padded, 0, optRows - cvImg_In.rows, 0, optCols - cvImg_In.cols, BORDER_CONSTANT, Scalar::all(0));

    // 傅里叶变换
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);

    // 创建滤波器
    Mat lpFilter = createLPFilter(complexImg.size(), Point(complexImg.cols/2, complexImg.rows/2), Img.getRadius(), Img.getlpFilterType(), Img.getButterWorthN());

    // 应用滤波器
    Mat filteredPlanes[2];
    split(complexImg, filteredPlanes);
    multiply(filteredPlanes[0], lpFilter, filteredPlanes[0]);
    multiply(filteredPlanes[1], lpFilter, filteredPlanes[1]);
    merge(filteredPlanes, 2, complexImg);

    // 逆变换
    dft(complexImg, complexImg, DFT_INVERSE | DFT_REAL_OUTPUT);

    // 裁剪和去中心化
    Mat result = complexImg(Rect(0, 0, cvImg_In.cols, cvImg_In.rows));
    multiply(result, shiftMat, result); // 再次中心化还原

    // 归一化显示
    normalize(result, result, 0, 255, NORM_MINMAX);
    result.convertTo(result, CV_8U);
    m_cvImg_Temporary = result.clone();
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::conductLowFilter(){
    FourierTransform();
}

void MainWindow::do_selectNoise_Del(){
    if (ui->radBtn_medianNoise_Blur->isChecked())
        medianBlur_Del();
    else if (ui->radBtn_lowPassFilter_Del->isChecked())
        conductLowFilter();

    ui->radBtn_lowPassFilter_Del->setEnabled(true);
}

void MainWindow::do_selectNoise_SizeofRect(){
    if (ui->radBtn_RectSize_3->isChecked())
        Img.updateSizeofRect(3);
    else if (ui->radBtn_RectSize_5->isChecked())
        Img.updateSizeofRect(5);
    else if (ui->radBtn_RectSize_7->isChecked())
        Img.updateSizeofRect(7);
    else if (ui->radBtn_RectSize_9->isChecked())
        Img.updateSizeofRect(9);

    ui->radBtn_medianNoise_Blur->setEnabled(true);
}

void MainWindow::enableButterWorth(){
    if (isButterWorthNSetted && isRadiusSetted)
        ui->radBtn_ButterWorthLowPass_Filter->setEnabled(true);
}

void MainWindow::do_selectButterWorth_N(){
    if (ui->radBtn_ButterWorthN_1->isChecked())
        Img.updateButterWorthN(1);
    else if (ui->radBtn_ButterWorthN_2->isChecked())
        Img.updateButterWorthN(2);
    else if (ui->radBtn_ButterWorthN_3->isChecked())
        Img.updateButterWorthN(3);

    isButterWorthNSetted = true;
    enableButterWorth();
}

void MainWindow::on_lineEdit_Radius_editingFinished(){
    QString radiusString = ui->lineEdit_Radius->text();
    double radi = radiusString.toDouble();
    Img.updateRadius(radi);

    isRadiusSetted = true;
    enableButterWorth();
}

void MainWindow::on_hzSlider_Radius_sliderMoved(int position){
    ui->lineEdit_Radius->setText(QString("%1").arg(position));
}




/*以下为色彩部分
 *
 *
 *
 *
 *
 */
void MainWindow::on_pBtn_cvt_Grayscale2RGB_clicked(){
    if (m_cvImg_Temporary.channels() == 3){
        QMessageBox::critical(this,"Error","转换失败：图像已经是彩色三通道图像!");
        return;
    }
    if (m_cvImg_Temporary.channels() != 1){
        QMessageBox::critical(this,"Error","转换失败：原始图像不为灰度图!");
        return;
    }
    conductCVT(COLOR_GRAY2RGB);
}

void MainWindow::on_pBtn_cvt_Grayscale2Binary_clicked(){
    if (m_cvImg_Temporary.channels() != 1){
        QMessageBox::critical(this,"Error","转换失败：图像不为灰度图!");
        return;
    }
    Mat trans = m_cvImg_Temporary.clone();
    Mat newImg = Mat::zeros(trans.size(),CV_8UC1);
    threshold(trans,newImg,Img.getBinaryThreshold(),255,Img.getBinaryThresholdType());
    m_cvImg_Temporary = newImg.clone();
    showImageFeatures(true);
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::chooseBinaryThresholdType(){
    if (ui->radBtn_BinaryThreshold_Black->isChecked()){
        Img.updateBinaryThresholdType(THRESH_BINARY_INV);
    }
    else if (ui->radBtn_BinaryThreshold_White->isChecked()){
        Img.updateBinaryThresholdType(THRESH_BINARY);
    }

    isThresholdTypeSetted = true;
    enableGRAY2BINARY();
}

void MainWindow::on_radBtn_BinaryThreshold_Black_clicked(){
    chooseBinaryThresholdType();
}

void MainWindow::on_radBtn_BinaryThreshold_White_clicked(){
    chooseBinaryThresholdType();
}

void MainWindow::on_lineEdit_BinaryThreshold_editingFinished(){
    QString thresholdString = ui->lineEdit_BinaryThreshold->text();
    int threshold = thresholdString.toInt();
    ui->lineEdit_BinaryThreshold->setText(QString("%1").arg(threshold));
    Img.updateBinaryThreshold(threshold);
    ui->hzSlider_BinaryThreshold->setValue(threshold);
    isThreshSetted = true;
    enableGRAY2BINARY();
}

void MainWindow::on_hzSlider_BinaryThreshold_sliderMoved(int position){
    ui->lineEdit_BinaryThreshold->setText(QString("%1").arg(position));
    Img.updateBinaryThreshold(position);
    isThreshSetted = true;
    enableGRAY2BINARY();
}

void MainWindow::enableGRAY2BINARY(){
    if ((isThreshSetted) && (isThresholdTypeSetted)){
        ui->pBtn_cvt_Grayscale2Binary->setEnabled(true);
    }
}

void MainWindow::on_pBtn_cvt_RGB2Grayscale_clicked(){
    if (m_cvImg_Temporary.channels() == 1){
        QMessageBox::critical(this,"Error","转换失败：图像已经是灰度图!");
        return;
    }
    if (m_cvImg_Temporary.channels() != 3){
        QMessageBox::critical(this,"Error","转换失败：图像不为彩色三通道图!");
        return;
    }
    conductCVT(COLOR_RGB2GRAY);
}

void MainWindow::on_pBtn_cvt_RGB2BGR_clicked(){
    if ((m_cvImg_Temporary.channels() != 3) || (m_cvImg_Temporary.channels() == 3 && !isRGB)){
        QMessageBox::critical(this,"Error","转换失败：图像不为RGB图");
        return;
    }
    isRGB = false;
    conductCVT(COLOR_RGB2BGR);
}

void MainWindow::on_pBtn_cvt_BGR2RGB_clicked(){
    if ((m_cvImg_Temporary.channels() != 3) || (m_cvImg_Temporary.channels() == 3 && isRGB)){
        QMessageBox::critical(this,"Error","转换失败：图像不为BGR图");
        return;
    }
    isRGB = true;
    conductCVT(COLOR_BGR2RGB);
}

void MainWindow::conductCVT(int type){
    Mat trans = m_cvImg_Temporary.clone();
    Mat newImg;
    cvtColor(trans,newImg,type);
    m_cvImg_Temporary = newImg.clone();
    showImageFeatures(true);
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::on_lineEdit_Brightness_editingFinished(){
    QString brightnessString = ui->lineEdit_Brightness->text();
    int brightness = brightnessString.toInt();
    Img.updateBrightness(brightness);
    ui->hzSlider_Brightness->setValue(brightness);
    addBrightness();
}

void MainWindow::on_hzSlider_Brightness_sliderMoved(int position){
    ui->lineEdit_Brightness->setText(QString("%1").arg(position));
    Img.updateBrightness(position);
    addBrightness();
}

void MainWindow::on_pBtn_ImgBeforeBrightness_clicked(){
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    ui->hzSlider_Brightness->setEnabled(true);
    ui->lineEdit_Brightness->setEnabled(true);
}

void MainWindow::addBrightness(){
    Mat hsv;
    Mat dst;
    Mat src = m_cvImg_BeforeChange.clone();
    vector<Mat> channels;
    int brightness = Img.getBrightness();

    if (src.channels() == 1){
        for (int i=0;i<src.cols;i++){
            for (int j=0;j<src.rows;j++){
                src.at<uchar>(i,j) += brightness;
            }
        }
        m_cvImg_Temporary = src.clone();
        ImgShow(m_cvImg_Temporary);
        return;
    }

    if (src.channels() == 3 && AddBrightnessType == 0){
        if (isRGB){
            cvtColor(src,hsv,COLOR_RGB2HSV);
            split(hsv,channels);
            channels[2] += brightness;
            merge(channels,hsv);
            cvtColor(hsv,dst,COLOR_HSV2RGB);
        }

        else if (!isRGB){
            cvtColor(src,hsv,COLOR_BGR2HSV);
            split(hsv,channels);
            channels[2] += brightness;
            merge(channels,hsv);
            cvtColor(hsv,dst,COLOR_HSV2BGR);
        }
    }

    else if (src.channels() == 3 && AddBrightnessType == 1){
        add(src,Scalar::all(brightness),dst);
    }

    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::chooseAddBrightnessType(){
    if (ui->radBtn_HSVBrightness->isChecked()){
        AddBrightnessType = 0;
    }
    else if (ui->radBtn_DirectlyAddBrightness->isChecked()){
        AddBrightnessType = 1;
    }

    ui->pBtn_ImgBeforeBrightness->setEnabled(true);
}

void MainWindow::on_radBtn_DirectlyAddBrightness_clicked(){
    chooseAddBrightnessType();
}

void MainWindow::on_radBtn_HSVBrightness_clicked(){
    chooseAddBrightnessType();
}

void MainWindow::on_pBtn_ImgBeforeContrastRatio_clicked(){
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    ui->hzSlider_ContrastRatio->setEnabled(true);
    ui->lineEdit_ContrastRatio->setEnabled(true);
}

void MainWindow::addContrastRatioDirectly(){
    Mat src = m_cvImg_BeforeChange.clone();
    Mat dst;
    Mat dst8u;
    double contrastRatio = Img.getContrastRatio();
    src.convertTo(dst,CV_32F,contrastRatio,0);
    dst.convertTo(dst8u,CV_8U);
    m_cvImg_Temporary = dst8u.clone();
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::on_lineEdit_ContrastRatio_editingFinished(){
    QString contrastString = ui->lineEdit_ContrastRatio->text();
    double contrast = contrastString.toDouble();
    if (contrast < 0.0 || contrast > 3.0){
        QMessageBox::critical(this,"Error!","输入失败：对比度应在0-3之间！");
        ui->lineEdit_ContrastRatio->setText("1.0");
        return;
    }
    Img.updateContrastRatio(contrast);
    ui->hzSlider_ContrastRatio->setValue(int(contrast*100));
    addContrastRatioDirectly();
}

void MainWindow::on_hzSlider_ContrastRatio_sliderMoved(int position){
    ui->lineEdit_ContrastRatio->setText(QString::number(position/100.0,'f',2));
    Img.updateContrastRatio(position/100.0);
    addContrastRatioDirectly();
}

void MainWindow::on_pBtn_ImgBeforeHist_clicked(){
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    ui->pBtn_HistogramEqualizationContrastRatio->setEnabled(true);
    ui->pBtn_GenerateHist->setEnabled(true);
}

void MainWindow::on_pBtn_HistogramEqualizationContrastRatio_clicked(){
    Mat src = m_cvImg_BeforeChange.clone();
    Mat dst;
    vector<Mat> Channels;
    Mat hsv;
    Mat histed;
    if (src.channels() == 1){
        equalizeHist(src,dst);
    }
    else if (src.channels() == 3 && isRGB){
        cvtColor(src,hsv,COLOR_RGB2HSV);
        split(hsv,Channels);
        equalizeHist(Channels[2],histed);
        Channels[2] = histed.clone();
        merge(Channels,hsv);
        cvtColor(hsv,dst,COLOR_HSV2RGB);
    }
    else if (src.channels() == 3 && !isRGB){
        cvtColor(src,hsv,COLOR_BGR2HSV);
        split(hsv,Channels);
        equalizeHist(Channels[2],histed);
        Channels[2] = histed.clone();
        merge(Channels,hsv);
        cvtColor(hsv,dst,COLOR_HSV2BGR);
    }
    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::on_pBtn_GenerateHist_clicked(){
    Mat src = m_cvImg_Temporary.clone();
    Mat Gray;
    Mat hist;

    if (src.channels() == 3 && isRGB){
        cvtColor(src,Gray,COLOR_RGB2GRAY);
    }
    else if (src.channels() == 3 && !isRGB){
        cvtColor(src,Gray,COLOR_BGR2GRAY);
    }
    else if (src.channels() == 1){
        Gray = src.clone();
    }

    int histSize = 256;
    float ranges[] = {0,256};
    const float* histRanges = {ranges};
    calcHist(&Gray,1,0,Mat(),hist,1,&histSize,&histRanges,true,false);

    int hist_h = 300;
    int hist_w = 512;
    int bin_w = hist_w / histSize;
    Mat histImage(hist_h,hist_w,CV_8UC3,Scalar::all(0));

    normalize(hist,hist,0,hist_h,NORM_MINMAX,-1,Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(hist.at<float>(i - 1))),
             Point((i)*bin_w, hist_h - cvRound(hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
    }

    histCount++;
    m_qimg_MagnitudeSpectrum = cvt_Cv2Qimg(histImage);
    m_qimg_MagnitudeSpectrum.save(QString(realName + "_histImage_%1.png").arg(histCount));
}
void MainWindow::on_pBtn_ImgBeforeSaturation_clicked(){
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    ui->lineEdit_Saturation->setEnabled(true);
    ui->hzSlider_Saturation->setEnabled(true);
}

void MainWindow::changeSaturation(){
    Mat src = m_cvImg_BeforeChange.clone();
    Mat hsv;
    Mat dst;
    vector<Mat> Channels;
    double saturation = Img.getSaturation();

    if (src.channels() == 1){
        QMessageBox::critical(this,"Error!","编辑错误：灰度图无法改变饱和度!");
        return;
    }
    else if (src.channels() == 3 && isRGB){
        cvtColor(src,hsv,COLOR_RGB2HSV);
        split(hsv,Channels);
        for (int i=0;i<Channels[1].rows;i++){
            for (int j=0;j<Channels[1].cols;j++){
                Channels[1].at<uchar>(i,j) *= saturation;
            }
        }
        merge(Channels,hsv);
        cvtColor(hsv,dst,COLOR_HSV2RGB);
    }
    else if (src.channels() == 3 && !isRGB){
        cvtColor(src,hsv,COLOR_BGR2HSV);
        split(hsv,Channels);
        for (int i=0;i<Channels[1].rows;i++){
            for (int j=0;j<Channels[1].cols;j++){
                Channels[1].at<uchar>(i,j) *= saturation;
            }
        }
        merge(Channels,hsv);
        cvtColor(hsv,dst,COLOR_HSV2BGR);
    }

    m_cvImg_Temporary = dst;
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::on_lineEdit_Saturation_editingFinished(){
    QString saturationString = ui->lineEdit_Saturation->text();
    double saturation = saturationString.toDouble();
    if (saturation > 3.0 || saturation < 0.0){
        QMessageBox::critical(this,"Error!","输入失败：饱和度范围应为0-3");
        ui->lineEdit_Saturation->setText("1.0");
        return;
    }
    Img.updateSaturation(saturation);
    ui->hzSlider_Saturation->setValue(int(saturation*100));
    changeSaturation();
}

void MainWindow::on_hzSlider_Saturation_sliderMoved(int position){
    ui->lineEdit_Saturation->setText(QString::number(position/100.0,'f',2));
    Img.updateSaturation(position/100.0);
    changeSaturation();
}

void MainWindow::on_pBtn_ImgBeforeColdHot_clicked(){
    m_cvImg_BeforeChange = m_cvImg_Temporary.clone();
    ui->lineEdit_ColdHot->setEnabled(true);
    ui->hzSlider_ColdHot->setEnabled(true);
}

void MainWindow::changeColdHot(){
    Mat src = m_cvImg_BeforeChange.clone();
    Mat dst;
    int coldHot = Img.getColdHot();

    if (src.channels() == 1){
        QMessageBox::critical(this,"Error!","编辑错误：灰度图无法调整冷暖色调！");
        return;
    }
    else if (src.channels() == 3 && isRGB){
        add(src,Scalar(coldHot*0.587,coldHot*0.299,coldHot*0.144),dst);
    }
    else if (src.channels() == 3 && !isRGB){
        add(src,Scalar(coldHot*0.144,coldHot*0.299,coldHot*0.587),dst);
    }

    m_cvImg_Temporary = dst.clone();
    ImgShow(m_cvImg_Temporary);
}

void MainWindow::on_lineEdit_ColdHot_editingFinished(){
    QString coldHotString = ui->lineEdit_ColdHot->text();
    int coldHot = coldHotString.toInt();
    Img.updateColdHot(coldHot);
    ui->hzSlider_ColdHot->setValue(coldHot);
    changeColdHot();
}

void MainWindow::on_hzSlider_ColdHot_sliderMoved(int position){
    ui->lineEdit_ColdHot->setText(QString("%1").arg(position));
    Img.updateColdHot(position);
    changeColdHot();
}

void MainWindow::chooseColorMapType(){
    if (ui->radBtn_ColorMap_Autumn->isChecked())
        ColorMapType = COLORMAP_AUTUMN;
    else if (ui->radBtn_ColorMap_Magma->isChecked())
        ColorMapType = COLORMAP_MAGMA;
    else if (ui->radBtn_ColorMap_Winter->isChecked())
        ColorMapType = COLORMAP_WINTER;
    else if (ui->radBtn_ColorMap_Spring->isChecked())
        ColorMapType = COLORMAP_SPRING;
    else if (ui->radBtn_ColorMap_HSV->isChecked())
        ColorMapType = COLORMAP_HSV;

    ui->OpenFalseColor->setEnabled(true);
}

void MainWindow::on_radBtn_ColorMap_Autumn_clicked(){
    chooseColorMapType();
}

void MainWindow::on_radBtn_ColorMap_Magma_clicked(){
    chooseColorMapType();
}

void MainWindow::on_radBtn_ColorMap_Winter_clicked(){
    chooseColorMapType();
}

void MainWindow::on_radBtn_ColorMap_DeepGreen_clicked(){
    chooseColorMapType();
}

void MainWindow::on_radBtn_ColorMap_Spring_clicked(){
    chooseColorMapType();
}

void MainWindow::on_radBtn_ColorMap_HSV_clicked(){
    chooseColorMapType();
}
