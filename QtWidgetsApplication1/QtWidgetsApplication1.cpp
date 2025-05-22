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
    createMenus();  // �����˵���
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    QMessageBox::information(nullptr, "tips", " frame will be closed��", QMessageBox::Ok);
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
    // �����źźͲ�
    connect(newAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionNew_triggered);
    connect(openAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionOpen_triggered);
    connect(saveAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionSave_triggered);
    connect(exitAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionExit_triggered);
    connect(aboutAction, &QAction::triggered, this, &QtWidgetsApplication1::on_actionAbout_triggered);
}

// �˵����Ӧ�Ĳۺ���ʵ��
void QtWidgetsApplication1::on_actionNew_triggered()
{
    // �½��ļ��Ĳ���
    QMessageBox::information(this, "tips", "new file");
}

void QtWidgetsApplication1::on_actionOpen_triggered()
{
	GDALAllRegister(); // ע��������������
    OGRRegisterAll();
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("��SHP�ļ�");
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setNameFilter("ESRI Shapefiles (*.shp);;�����ļ� (*)");
    if (fileDialog.exec()) {//
        QString filePath = fileDialog.selectedFiles().first();

        // ��ʾ����״̬
        statusBar()->showMessage("���ڼ���: " + filePath);

        // ��SHP�ļ�
        GDALDataset* dataset = (GDALDataset*)GDALOpenEx(
            filePath.toUtf8().constData(),
            GDAL_OF_VECTOR,
            nullptr, nullptr, nullptr
        );

        if (dataset == nullptr) {
            QMessageBox::critical(this, "����", "�޷����ļ�: " + filePath);
            statusBar()->showMessage("����ʧ��");
            return;
        }

        // �����������
        //clearPreviousData();

        // ����SHP����
        //processShapefile(dataset);

        // �ͷ���Դ
        GDALClose(dataset);

        // ��ʾ���״̬
        statusBar()->showMessage("�������: " + filePath);
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