#include "stdafx.h"
#include "QtWidgetsApplication1.h"
//#include"readShapefile.h"
//#include "QtWidgetsApplication1.h"
#define GLFW_INCLUDE_NONE
#include <QMessageBox>
#include <QFileDialog>

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    createMenus();  // 创建菜单栏
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    QMessageBox::information(nullptr, "tips", " frame will be closed！", QMessageBox::Ok);
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
    // 连接信号和槽
    connect(newAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionNew_triggered);
    connect(openAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionOpen_triggered);
    connect(saveAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionSave_triggered);
    connect(exitAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionExit_triggered);
    connect(aboutAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionAbout_triggered);
}

// 菜单项对应的槽函数实现
void QtWidgetsApplication1::on_actionNew_triggered()
{
    // 新建文件的操作
    QMessageBox::information(this, "tips", "new file");
}

void QtWidgetsApplication1::on_actionOpen_triggered()
{
	GDALAllRegister(); // 注册所有驱动程序
    OGRRegisterAll();
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("打开SHP文件");
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("ESRI Shapefiles (*.shp);;所有文件 (*)");
    if (fileDialog.exec()) {//
        QString filePath = fileDialog.selectedFiles().first();

        // 显示加载状态
        statusBar()->showMessage("正在加载: " + filePath);

        // 打开SHP文件
        GDALDataset* dataset = (GDALDataset*)GDALOpenEx(
            filePath.toUtf8().constData(),
            GDAL_OF_VECTOR,
            nullptr, nullptr, nullptr
        );

        if (dataset == nullptr) {
            QMessageBox::critical(this, "错误", "无法打开文件: " + filePath);
            statusBar()->showMessage("加载失败");
            return;
        }

        // 清空现有数据
        //clearPreviousData();

        // 处理SHP数据
        //processShapefile(dataset);

        // 释放资源
        GDALClose(dataset);

        // 显示完成状态
        statusBar()->showMessage("加载完成: " + filePath);
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