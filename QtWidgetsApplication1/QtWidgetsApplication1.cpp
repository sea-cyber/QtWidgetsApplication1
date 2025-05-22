#include "stdafx.h"
#include "QtWidgetsApplication1.h"
//#include"readShapefile.h"
//#include "QtWidgetsApplication1.h"
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
    // 打开文件的操作
    QString fileName = QFileDialog::getOpenFileName(this, "open file", "", "all (*.*)");
    if (!fileName.isEmpty()) {

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in.setCodec("UTF-8"); // 设置编码，确保中文正常显示

            QString content = in.readAll(); // 读取全部内容

            // 打印到调试输出
            qDebug() << "File content:" << content;

            // 或者显示在消息框中(适合小文件)
            QMessageBox::information(this, "File Content", content);
                
            file.close();
        }
        else {
            // 打开失败时显示错误信息
            QMessageBox::warning(this, "Error", "Failed to open file: " + file.errorString());
        }
    }
        //FileReader::instance()->readShapefile();
        QMessageBox::information(this, "tips", "open file: " + fileName);
    
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