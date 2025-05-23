#pragma execution_character_set("utf-8")

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include<iostream>
class ClipDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit ClipDialog(QWidget* parent = nullptr) : QDialog(parent) {
        setWindowTitle("�ü�����");
        setMinimumWidth(400);

        // ��������Ԫ��
        createWidgets();
        setupLayout();
        connectSignals();
    }
    // ��ȡ�ü�����·��
    QString getInputPath() const {
        return inputPathEdit->text();
    }

    // ��ȡ�ü���Χ����ʽ��XMin,YMin,XMax,YMax��
    QString getExtent() const {
        return extentEdit->text();
    }

    // ��ȡ���λ��·��
    QString getOutputPath() const {
        return outputPathEdit->text();
    }
private slots:
    void browseInput() {
        QString filePath = QFileDialog::getOpenFileName(
            this, "ѡ��ü�����", "", "�����ļ� (*.*)");
        if (!filePath.isEmpty()) {
            inputPathEdit->setText(filePath);
        }
    }

    void browseOutput() {
        QString filePath = QFileDialog::getSaveFileName(
            this, "ѡ�����λ��", "", "�����ļ� (*.*)");
        if (!filePath.isEmpty()) {
            outputPathEdit->setText(filePath);
        }
    }
    void browseExtent() {
        QString filePath = QFileDialog::getOpenFileName(
            this, "ѡ��ü���Χ", "", "Shapefile (*.shp);;All Files (*.*)");
        if (!filePath.isEmpty()) {
            extentPathEdit->setText(filePath);
        }
    }
    void performClip() {
        try {
            QString input = inputPathEdit->text().trimmed();
        QString extent = extentEdit->text().trimmed();
        QString output = outputPathEdit->text().trimmed();
       
      

        if (input.isEmpty() || extent.isEmpty() || output.isEmpty()) {
            QMessageBox::warning(this, "��������", "����д�����ֶ�");
            return;
        }

        // ���òü�����
        if (clip(input, extent, output)) {
            QMessageBox::information(this, "�ɹ�", "�ü����");
            accept(); // �رնԻ���
        }
        else {
            QMessageBox::critical(this, "ʧ��", "�ü������г���");
        }
        }
        catch (const char* msg) { // �����쳣
            std::cout << msg << endl;
        };
    }

private:
private:
    QLineEdit* inputPathEdit;
    QLineEdit* extentEdit;
    //QLineEdit* inputPathEdit; 

    //QLineEdit* inputPathEdit;
    QLineEdit* extentPathEdit;
    QLineEdit* outputPathEdit;

    // ������³�Ա��������
    QPushButton* browseInputBtn;
    QPushButton* browseExtentBtn;
    QPushButton* browseOutputBtn;
    QPushButton* clipBtn;
    QPushButton* cancelBtn;

  void createWidgets() {
    // �����ļ�ѡ��
    inputPathEdit = new QLineEdit(this);
    browseInputBtn = new QPushButton("���...", this);  // ��Ϊ���Ա����

    // �ü���Χ����
    extentPathEdit = new QLineEdit(this);
    browseExtentBtn = new QPushButton("���...", this);  // ��Ϊ���Ա����

    // ����ļ�ѡ��
    outputPathEdit = new QLineEdit(this);
    browseOutputBtn = new QPushButton("���...", this);  // ��Ϊ���Ա����

    // ��ť
    clipBtn = new QPushButton("�ü�", this);  // ��Ϊ���Ա����
    cancelBtn = new QPushButton("ȡ��", this);  // ��Ϊ���Ա����

    // �����źŲ�
    connect(browseInputBtn, &QPushButton::clicked, this, &ClipDialog::browseInput);
    connect(browseExtentBtn, &QPushButton::clicked, this, &ClipDialog::browseExtent);
    connect(browseOutputBtn, &QPushButton::clicked, this, &ClipDialog::browseOutput);
    connect(clipBtn, &QPushButton::clicked, this, &ClipDialog::performClip);
    connect(cancelBtn, &QPushButton::clicked, this, &ClipDialog::reject);
}

void setupLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // �����ļ���
    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("�ü�����:", this));
    inputLayout->addWidget(inputPathEdit);
    inputLayout->addWidget(browseInputBtn);
    mainLayout->addLayout(inputLayout);

    // �ü���Χ��
    QHBoxLayout* extentLayout = new QHBoxLayout;
    extentLayout->addWidget(new QLabel("�ü���Χ:", this));
    extentLayout->addWidget(extentPathEdit);
    extentLayout->addWidget(browseExtentBtn);
    mainLayout->addLayout(extentLayout);

    // ����ļ���
    QHBoxLayout* outputLayout = new QHBoxLayout;
    outputLayout->addWidget(new QLabel("���λ��:", this));
    outputLayout->addWidget(outputPathEdit);
    outputLayout->addWidget(browseOutputBtn);  // ȡ��ע��
    mainLayout->addLayout(outputLayout);

    // ��ť��
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(clipBtn);  // ȡ��ע��
    buttonLayout->addWidget(cancelBtn);  // ȡ��ע��
    mainLayout->addLayout(buttonLayout);
}

    void connectSignals() {
        // �źŲ�����createWidgets������
    }

    // �ü�����ʵ��
    bool clip(const QString& inputPath, const QString& extent, const QString& outputPath) {
        // �����ü���Χ����ʽ: XMin,YMin,XMax,YMax��
        QStringList coords = extent.split(',');
        if (coords.size() != 4) {
            qDebug() << "�ü���Χ��ʽ����";
            return false;
        }

        double xMin = coords[0].toDouble();
        double yMin = coords[1].toDouble();
        double xMax = coords[2].toDouble();
        double yMax = coords[3].toDouble();

        // ʵ�ʲü��߼���������GDALʾ����
        try {
            // ʾ����ʹ��GDAL����դ��ü�
            // GDALAllRegister();
            // GDALDataset* srcDS = (GDALDataset*)GDALOpen(inputPath.toUtf8(), GA_ReadOnly);
            // ... �ü����� ...
            // GDALClose(srcDS);

            qDebug() << "ִ�вü�:" << inputPath << "->" << outputPath;
            qDebug() << "�ü���Χ:" << xMin << yMin << xMax << yMax;

            // ģ��ɹ�
            return true;
        }
        catch (const std::exception& e) {
            qDebug() << "�ü�ʧ��:" << e.what();
            return false;
        }
    }
};