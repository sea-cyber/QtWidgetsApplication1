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
    setWindowTitle("SHP�ļ��鿴��");
    createMenus();  // �����˵���
    GDALAllRegister();
    OGRRegisterAll();
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    QMessageBox::information(nullptr, "tips", " frame will be closed��", QMessageBox::Ok);
    //delete ui;
}

// �����˵���
void QtWidgetsApplication1::createMenus()
{
    // ����"�ļ�"�˵�
    QMenu* fileMenu = menuBar()->addMenu("file");

    // ��Ӳ˵���    
    QAction* newAction = new QAction("new", this);
    QAction* openAction = new QAction("open", this);
    QAction* saveAction = new QAction("save", this);
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();  // ��ӷָ���

    QAction* exitAction = new QAction("exit", this);
    fileMenu->addAction(exitAction);

    // ����"����"�˵�
    QMenu* helpMenu = menuBar()->addMenu("help");



    // ��Ӳ˵���
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
    // �����źźͲ�
    connect(newAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionNew_triggered);
    connect(openAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionOpen_triggered);
    connect(saveAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionSave_triggered);
    connect(exitAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionExit_triggered);
    connect(aboutAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionAbout_triggered);
    connect(clipAction, &QAction::triggered, this, &QtWidgetsApplication1::on_clipAction_triggered);
}

// �˵����Ӧ�Ĳۺ���ʵ��
void QtWidgetsApplication1::on_actionNew_triggered()
{
    // �½��ļ��Ĳ���
    QMessageBox::information(this, "tips", "new file");
}
// �˵����Ӧ�Ĳۺ���ʵ��
void QtWidgetsApplication1::on_clipAction_triggered()
{
    qDebug() << "�ü����ܴ���"; // ʹ��Qt�ĵ������

    // ��������ʾ�ü��Ի���
    ClipDialog dialog(this);
    dialog.setWindowTitle("�������ݲü�����");

    // ģ̬��ʾ���ȴ��û�����
    if (dialog.exec() == QDialog::Accepted) {
        // ��ȡ�û�����
        QString inputPath = dialog.getInputPath();
        QString extent = dialog.getExtent();
        QString outputPath = dialog.getOutputPath();

        // ��֤����
        if (inputPath.isEmpty() || outputPath.isEmpty()) {
            QMessageBox::warning(this, "��������", "����/���·������Ϊ��");
            return;
        }

        // �����ü���Χ
        bool ok;
        QStringList coords = extent.split(',');
        if (coords.size() != 4) {
            QMessageBox::warning(this, "��������", "�ü���Χ��ʽӦΪ XMin,YMin,XMax,YMax");
            return;
        }
        double xMin = coords[0].toDouble(&ok);
        double yMin = coords[1].toDouble(&ok);
        double xMax = coords[2].toDouble(&ok);
        double yMax = coords[3].toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "��������", "�ü���Χ����Ϊ����");
            return;
        }

        // ִ��ʵ�ʲü�������ʾ����
        /*if (performClip(inputPath, xMin, yMin, xMax, yMax, outputPath)) {
            QMessageBox::information(this, "�ɹ�", "�ü��������");
        }
        else {
            QMessageBox::critical(this, "ʧ��", "�ü������г���");
        }*/
    }
    else {
        qDebug() << "�û�ȡ���˲ü�����";
    }
}

void QtWidgetsApplication1::on_actionOpen_triggered() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "��SHP�ļ�", "D:\GISERR\graduate_design\resourse", "Shapefile (*.shp);;All Files (*)");

    if (filePath.isEmpty()) return;

    // ��վ�����
    m_polygons.clear();
    m_lines.clear();
    m_points.clear();

        // ��SHP�ļ�
    GDALDataset* poDS = (GDALDataset*)GDALOpenEx(
        filePath.toUtf8().constData(), GDAL_OF_VECTOR, NULL, NULL, NULL);

    if (poDS == nullptr) {
        QMessageBox::critical(this, "����", "�޷����ļ���");
        return;
    }

    // ��ȡͼ��
    OGRLayer* poLayer = poDS->GetLayer(0);
    if (poLayer == nullptr) {
        GDALClose(poDS);
        QMessageBox::critical(this, "����", "�޷���ȡͼ�㣡");
            return;
        }

    // ��ȡ�ռ�ο���Ϣ����������ת����
    OGRSpatialReference* poSRS = poLayer->GetSpatialRef();
    if (poSRS == nullptr) {
        poSRS = new OGRSpatialReference();
        poSRS->SetWellKnownGeogCS("WGS84");  // Ĭ��ʹ��WGS84
    }

    // �������ݷ�Χ
    m_minX = m_minY = 1e30;
    m_maxX = m_maxY = -1e30;

    // ����Ҫ��
    OGRFeature* poFeature;//����һ��Ҫ�ؼ�
    //�൱��һ��������
    while ((poFeature = poLayer->GetNextFeature()) != nullptr) {
        OGRGeometry* poGeometry = poFeature->GetGeometryRef();
        if (poGeometry == nullptr) {
            OGRFeature::DestroyFeature(poFeature);
            continue;
        }

        // ���ݼ������ʹ���
        switch (poGeometry->getGeometryType()) {
        case wkbPoint: {
            OGRPoint* poPoint = (OGRPoint*)poGeometry;
            double x = poPoint->getX();
            double y = poPoint->getY();
            m_points.append(QPointF(x, y));

            // ���·�Χ
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

        // �ͷ���Դ
    GDALClose(poDS);

    // ˢ�½���
    update();
}

// �����¼�
void QtWidgetsApplication1::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    if (m_polygons.isEmpty() && m_lines.isEmpty() && m_points.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // ��������ת������������������ӳ�䵽���ڣ�
    double viewWidth = width() - 20;
    double viewHeight = height() - 20;
    double scaleX = viewWidth / (m_maxX - m_minX);
    double scaleY = viewHeight / (m_maxY - m_minY);
    double offsetX = 10 - m_minX * scaleX;
    double offsetY = 10 - m_minY * scaleY;

    // ���ƶ����
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

    // ������
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    for (const QLineF& line : m_lines) {
        QLineF transformed(
            line.x1() * scaleX + offsetX, line.y1() * scaleY + offsetY,
            line.x2() * scaleX + offsetX, line.y2() * scaleY + offsetY
        );
        painter.drawLine(transformed);
    }

    // ���Ƶ�
    painter.setPen(QPen(Qt::green, 2, Qt::SolidLine));
    for (const QPointF& point : m_points) {
        QPointF transformed(point.x() * scaleX + offsetX, point.y() * scaleY + offsetY);
        painter.drawPoint(transformed);
    }
}

void QtWidgetsApplication1::on_actionSave_triggered()
{
    // �����ļ��Ĳ���
    QString fileName = QFileDialog::getSaveFileName(this, "�����ļ�", "", "�����ļ� (*.*)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "��ʾ", "�����ļ�: " + fileName);
    }
}

void QtWidgetsApplication1::on_actionExit_triggered()
{
    // �˳�����
    close();
}

void QtWidgetsApplication1::on_actionAbout_triggered()
{
    // ���ڶԻ���
    QMessageBox::about(this, "����", "QtWidgetsApplication1\n�汾 1.0");
}