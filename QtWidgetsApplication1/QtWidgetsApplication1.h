#pragma execution_character_set("utf-8")
#pragma once
#include "gdal_priv.h"
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#pragma once
//#include <QtWidgets/QMainWindow>
//#include "ui_QtWidgetsApplication1.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = nullptr);
    ~QtWidgetsApplication1();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_clipAction_triggered();
protected:
    void paintEvent(QPaintEvent* event) override;  // �ػ��¼�
private:

private:
    Ui::QtWidgetsApplication1Class ui;
    void createMenus();  // �����˵���
    QVector<QPolygonF> m_polygons;    // �洢���������
    QVector<QLineF> m_lines;         // �洢������
    QVector<QPointF> m_points;       // �洢������
    double m_minX, m_minY, m_maxX, m_maxY;  // ���ݷ�Χ
};