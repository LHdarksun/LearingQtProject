#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->radBtn_INTER_CUBIC,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_AREA,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_LANCZOS4,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_LINEAR,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));
    connect(ui->radBtn_INTER_NEAREST,SIGNAL(clicked()),this,SLOT(do_setFlags_warpAffine()));

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

void MainWindow::showImageFeatures(bool formatChanged){
    if (formatChanged){                                                     //若改变了图像文件格式则显示格式
        QImage::Format fmt = m_qimage.format();
        if (fmt == QImage::Format_RGB32)
            ui->editImg_Format->setText("32-bit RGB(0xffRRGGBB)");
        else if (fmt == QImage::Format_RGB16)
            ui->editImg_Format->setText("16-bit RGB565");
        else if (fmt == QImage::Format_RGB888)
            ui->editImg_Format->setText("24-bit RGB888");
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

        ui->editImg_Depth->setText(QString("%1 bits/pixel").arg(m_qimage.depth()));
        ui->editImg_BitPlane->setText(QString("%1 bits").arg(m_qimage.bitPlaneCount()));
        ui->chkBox_Alpha->setChecked(m_qimage.hasAlphaChannel());
        ui->chkBox_GrayScale->setChecked(m_qimage.isGrayscale());
        ui->chkBox_Alpha->setEnabled(false);
        ui->chkBox_GrayScale->setEnabled(false);
    }
    ui->editImg_Height->setText(QString("%1 像素").arg(m_qimage.height()));
    ui->editImg_Width->setText(QString("%1 像素").arg(m_qimage.width()));
    qsizetype sz = m_qimage.sizeInBytes();
    if (sz < 1024 * 9)
        ui->editImg_SizeByte->setText(QString("%1 Bytes").arg(sz));
    else
        ui->editImg_SizeByte->setText(QString("%1 KB").arg(sz/1024));
    QString dpi = QString("DPI_X = %1,"
    " DPI_Y = %2").arg(m_qimage.dotsPerMeterX()*0.0254).arg(m_qimage.dotsPerMeterY()*0.0254);
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
    }

    if (rawImg.channels() == 3){
        cvtColor(rawImg,rawImg,COLOR_BGR2RGB);
    }
    else if (rawImg.channels() == 4){
        cvtColor(rawImg,rawImg,COLOR_BGRA2RGBA);
    }                                                                       //转换图像从opencv格式到qt格式
    m_cvImg_Origin = rawImg;                                                       //处理完原始图像的通道后赋给cv
    ImgShow(m_cvImg_Origin);
    ui->statusbar->showMessage(fileName);                                   //在底部显示图片名称
    m_filename = fileName;                                                  //保存当前图像的名称
    QFileInfo fileInfo(fileName);                                           //显示图像信息
    QDir::setCurrent(fileInfo.absolutePath());                              /*设置当前位置？*/
    ui->ProcFuncWidget->setCurrentIndex(0);
    ui->radBtn_INTER_LINEAR->setChecked(true);
    setItemEnabled();
    showImageFeatures();                                                    //显示图片属性
}

void MainWindow::on_Save_triggered(){
    m_qimage = cvt_Cv2Qimg(m_cvImg_Temporary);
    m_qimage.save(m_filename);
    imageModified(false);                                                   //将save设为不可选项直至下一修改
}

void MainWindow::on_SaveCopy_triggered(){
    QString filter = "图片文件(*.bmp *.jpg *.png *.jpeg);;"
                     "BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png);;JPEG文件(*.jpeg)";
    QString fileName = QFileDialog::getSaveFileName(this,"保存文件",m_filename,filter);
    if (fileName.isEmpty())
        return;
    m_qimage = cvt_Cv2Qimg(m_cvImg_Temporary);
    m_qimage.save(fileName);                                                 //保存为新的文件
    m_filename = fileName;
    ui->statusbar->showMessage(fileName);
    imageModified(false);
}

void MainWindow::on_Reload_triggered(){
    m_qimage = cvt_Cv2Qimg(m_cvImg_Origin);
    QPixmap pixmap = QPixmap::fromImage(m_qimage.scaled(ui->labPic->size(),Qt::KeepAspectRatio));
    ui->labPic->setPixmap(pixmap);
    ui->ProcFuncWidget->setCurrentIndex(0);
    reSetParameter();
    showImageFeatures(true);
    imageModified(false);
}

void MainWindow::reSetParameter(){
    ui->AngleEdit->setText(QString("%1").arg(0));
    ui->ScaleEdit->setText(QString("%1").arg(1.));
    ui->radBtn_INTER_LINEAR->setChecked(true);
    Img.updateAngle(0);
    Img.updateScale(1.);
    Img.updateFlagWA(INTER_LINEAR);
    Img.setFlippedFalse();
}

void MainWindow::imageModified(bool modified){
    ui->Reload->setEnabled(modified);
    ui->Save->setEnabled(modified);
}

void MainWindow::ImgShow(Mat m_cvImg){
    m_qimage = cvt_Cv2Qimg(m_cvImg);
    /*创建对象用于界面显示
     * scaled()用于限制图像大小
     * KeepAspectRatio意为保持长宽比
     */
    QPixmap pixmap = QPixmap::fromImage(m_qimage.scaled(ui->labPic->size(),Qt::KeepAspectRatio));
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
    Mat src = m_cvImg_Origin;
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
    Mat src = m_cvImg_Origin;
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
    Mat src = m_cvImg_Origin;
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
    Mat src = m_cvImg_Origin;
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
    Mat src = m_cvImg_Origin;
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
    Mat src = m_cvImg_Origin;
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
