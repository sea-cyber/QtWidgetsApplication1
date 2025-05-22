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

private:
    Ui::QtWidgetsApplication1Class ui;
    void createMenus();  // ´´½¨²Ëµ¥À¸
};