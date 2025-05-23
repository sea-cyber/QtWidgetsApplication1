#include "stdafx.h"
#include "QtWidgetsApplication1.h"
//#include"readShapefile.h"
//#include "QtWidgetsApplication1.h"
#define GLFW_INCLUDE_NONE
#include <QMessageBox>
#include <QFileDialog>
#include"ogrsf_frmts.h"
#include"clip.h"
#include<iostream>
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("SHP文件查看器");
    createMenus();  // 创建菜单栏
    GDALAllRegister();
    OGRRegisterAll();
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    QMessageBox::information(nullptr, "tips", " frame will be closed！", QMessageBox::Ok);
    //delete ui;
}

// 创建菜单栏
void QtWidgetsApplication1::createMenus()
{
    // 创建"文件"菜单
    QMenu* fileMenu = menuBar()->addMenu("file");

    // 添加菜单项    
    QAction* newAction = new QAction("new", this);
    QAction* openAction = new QAction("open", this);
    QAction* saveAction = new QAction("save", this);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();  // 添加分隔线

    QAction* exitAction = new QAction("exit", this);
    fileMenu->addAction(exitAction);

    // 创建"帮助"菜单
    QMenu* helpMenu = menuBar()->addMenu("help");



    // 添加菜单项
    QAction* aboutAction = new QAction("about", this);
    helpMenu->addAction(aboutAction);
    QMenu* contactMenu = menuBar()->addMenu("contact");

    QMenu* toolsMenu = menuBar()->addMenu("tools");
    //QAction* clip = new QAction("clip", this);    
    QAction* clipAction = new QAction("clip", this);
    toolsMenu->addAction(clipAction);
    helpMenu->addAction(clipAction);
    //QAction* clip = new QAction("clip", this);
    //QAction* clip = new QAction("clip", this);
    // 连接信号和槽
    connect(newAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionNew_triggered);
    connect(openAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionOpen_triggered);
    connect(saveAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionSave_triggered);
    connect(exitAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionExit_triggered);
    connect(aboutAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionAbout_triggered);
    connect(clipAction, &QAction::triggered, this, &QtWidgetsApplication1::on_clipAction_triggered);
}

// 菜单项对应的槽函数实现
void QtWidgetsApplication1::on_actionNew_triggered()
{
    // 新建文件的操作
    QMessageBox::information(this, "tips", "new file");
}
// 菜单项对应的槽函数实现
void QtWidgetsApplication1::on_clipAction_triggered()
{
    qDebug() << "裁剪功能触发"; // 使用Qt的调试输出

    // 创建并显示裁剪对话框
    ClipDialog dialog(this);
    dialog.setWindowTitle("地理数据裁剪工具");

    // 模态显示，等待用户操作
    if (dialog.exec() == QDialog::Accepted) {
        // 获取用户输入
        QString inputPath = dialog.getInputPath();
        QString extent = dialog.getExtent();
        QString outputPath = dialog.getOutputPath();

        // 验证输入
        if (inputPath.isEmpty() || outputPath.isEmpty()) {
            QMessageBox::warning(this, "参数错误", "输入/输出路径不能为空");
            return;
        }

        // 解析裁剪范围
        bool ok;
        QStringList coords = extent.split(',');
        if (coords.size() != 4) {
            QMessageBox::warning(this, "参数错误", "裁剪范围格式应为 XMin,YMin,XMax,YMax");
            return;
        }
        double xMin = coords[0].toDouble(&ok);
        double yMin = coords[1].toDouble(&ok);
        double xMax = coords[2].toDouble(&ok);
        double yMax = coords[3].toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "参数错误", "裁剪范围必须为数字");
            return;
        }

        // 执行实际裁剪操作（示例）
        /*if (performClip(inputPath, xMin, yMin, xMax, yMax, outputPath)) {
            QMessageBox::information(this, "成功", "裁剪操作完成");
        }
        else {
            QMessageBox::critical(this, "失败", "裁剪过程中出错");
        }*/
    }
    else {
        qDebug() << "用户取消了裁剪操作";
    }
}

void QtWidgetsApplication1::on_actionOpen_triggered() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "打开SHP文件", "D:\GISERR\graduate_design\resourse", "Shapefile (*.shp);;All Files (*)");

    if (filePath.isEmpty()) return;

    // 清空旧数据
    m_polygons.clear();
    m_lines.clear();
    m_points.clear();

        // 打开SHP文件
    GDALDataset* poDS = (GDALDataset*)GDALOpenEx(
        filePath.toUtf8().constData(), GDAL_OF_VECTOR, NULL, NULL, NULL);

    if (poDS == nullptr) {
        QMessageBox::critical(this, "错误", "无法打开文件！");
        return;
    }

    // 获取图层
    OGRLayer* poLayer = poDS->GetLayer(0);
    if (poLayer == nullptr) {
        GDALClose(poDS);
        QMessageBox::critical(this, "错误", "无法获取图层！");
            return;
        }

    // 获取空间参考信息（用于坐标转换）
    OGRSpatialReference* poSRS = poLayer->GetSpatialRef();
    if (poSRS == nullptr) {
        poSRS = new OGRSpatialReference();
        poSRS->SetWellKnownGeogCS("WGS84");  // 默认使用WGS84
    }

    // 重置数据范围
    m_minX = m_minY = 1e30;
    m_maxX = m_maxY = -1e30;

    // 遍历要素
    OGRFeature* poFeature;//定义一个要素集
    //相当于一个迭代器
    while ((poFeature = poLayer->GetNextFeature()) != nullptr) {
        OGRGeometry* poGeometry = poFeature->GetGeometryRef();
        if (poGeometry == nullptr) {
            OGRFeature::DestroyFeature(poFeature);
            continue;
        }

        // 根据几何类型处理
        switch (poGeometry->getGeometryType()) {
        case wkbPoint: {
            OGRPoint* poPoint = (OGRPoint*)poGeometry;
            double x = poPoint->getX();
            double y = poPoint->getY();
            m_points.append(QPointF(x, y));

            // 更新范围
            m_minX = qMin(m_minX, x);
            m_minY = qMin(m_minY, y);
            m_maxX = qMax(m_maxX, x);
            m_maxY = qMax(m_maxY, y);
            break;
        }
        case wkbLineString: {
            OGRLineString* poLine = (OGRLineString*)poGeometry;
            QLineF line;
            for (int i = 0; i < poLine->getNumPoints(); i++) {
                double x = poLine->getX(i);
                double y = poLine->getY(i);
                if (i == 0) {
                    line.setP1(QPointF(x, y));
                }
                else {
                    line.setP2(QPointF(x, y));
                    m_lines.append(line);
                    line.setP1(line.p2());
                }
                m_minX = qMin(m_minX, x);
                m_minY = qMin(m_minY, y);
                m_maxX = qMax(m_maxX, x);
                m_maxY = qMax(m_maxY, y);
            }
            break;
        }
        case wkbPolygon: {
            OGRPolygon* poPolygon = (OGRPolygon*)poGeometry;
            OGRLinearRing* poExterior = poPolygon->getExteriorRing();
            QPolygonF polygon;
            for (int i = 0; i < poExterior->getNumPoints(); i++) {
                double x = poExterior->getX(i);
                double y = poExterior->getY(i);
                polygon.append(QPointF(x, y));
                m_minX = qMin(m_minX, x);
                m_minY = qMin(m_minY, y);
                m_maxX = qMax(m_maxX, x);
                m_maxY = qMax(m_maxY, y);
            }
            m_polygons.append(polygon);
            break;
        }
        default:
            break;
        }

        OGRFeature::DestroyFeature(poFeature);
    }

        // 释放资源
    GDALClose(poDS);

    // 刷新界面
    update();
}

// 绘制事件
void QtWidgetsApplication1::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    if (m_polygons.isEmpty() && m_lines.isEmpty() && m_points.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 计算坐标转换比例（将地理坐标映射到窗口）
    double viewWidth = width() - 20;
    double viewHeight = height() - 20;
    double scaleX = viewWidth / (m_maxX - m_minX);
    double scaleY = viewHeight / (m_maxY - m_minY);
    double offsetX = 10 - m_minX * scaleX;
    double offsetY = 10 - m_minY * scaleY;

    // 绘制多边形
    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    painter.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
    for (const QPolygonF& poly : m_polygons) {
        QPolygonF transformed;
        for (const QPointF& point : poly) {
            transformed.append(QPointF(point.x() * scaleX + offsetX,
                point.y() * scaleY + offsetY));
        }
        painter.drawPolygon(transformed);
    }

    // 绘制线
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    for (const QLineF& line : m_lines) {
        QLineF transformed(
            line.x1() * scaleX + offsetX, line.y1() * scaleY + offsetY,
            line.x2() * scaleX + offsetX, line.y2() * scaleY + offsetY
        );
        painter.drawLine(transformed);
    }

    // 绘制点
    painter.setPen(QPen(Qt::green, 2, Qt::SolidLine));
    for (const QPointF& point : m_points) {
        QPointF transformed(point.x() * scaleX + offsetX, point.y() * scaleY + offsetY);
        painter.drawPoint(transformed);
    }
}

void QtWidgetsApplication1::on_actionSave_triggered()
{
    // 保存文件的操作
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "", "所有文件 (*.*)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "提示", "保存文件: " + fileName);
    }
}

void QtWidgetsApplication1::on_actionExit_triggered()
{
    // 退出程序
    close();
}

void QtWidgetsApplication1::on_actionAbout_triggered()
{
    // 关于对话框
    QMessageBox::about(this, "关于", "QtWidgetsApplication1\n版本 1.0");
}