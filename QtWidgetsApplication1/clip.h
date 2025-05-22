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
        setWindowTitle("裁剪工具");
        setMinimumWidth(400);

        // 创建界面元素
        createWidgets();
        setupLayout();
        connectSignals();
    }
    // 获取裁剪对象路径
    QString getInputPath() const {
        return inputPathEdit->text();
    }

    // 获取裁剪范围（格式：XMin,YMin,XMax,YMax）
    QString getExtent() const {
        return extentEdit->text();
    }

    // 获取输出位置路径
    QString getOutputPath() const {
        return outputPathEdit->text();
    }
private slots:
    void browseInput() {
        QString filePath = QFileDialog::getOpenFileName(
            this, "选择裁剪对象", "", "所有文件 (*.*)");
        if (!filePath.isEmpty()) {
            inputPathEdit->setText(filePath);
        }
    }

    void browseOutput() {
        QString filePath = QFileDialog::getSaveFileName(
            this, "选择输出位置", "", "所有文件 (*.*)");
        if (!filePath.isEmpty()) {
            outputPathEdit->setText(filePath);
        }
    }
    void browseExtent() {
        QString filePath = QFileDialog::getOpenFileName(
            this, "选择裁剪范围", "", "Shapefile (*.shp);;All Files (*.*)");
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
            QMessageBox::warning(this, "参数错误", "请填写所有字段");
            return;
        }

        // 调用裁剪函数
        if (clip(input, extent, output)) {
            QMessageBox::information(this, "成功", "裁剪完成");
            accept(); // 关闭对话框
        }
        else {
            QMessageBox::critical(this, "失败", "裁剪过程中出错");
        }
        }
        catch (const char* msg) { // 捕获异常
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

    // 添加以下成员变量声明
    QPushButton* browseInputBtn;
    QPushButton* browseExtentBtn;
    QPushButton* browseOutputBtn;
    QPushButton* clipBtn;
    QPushButton* cancelBtn;

  void createWidgets() {
    // 输入文件选择
    inputPathEdit = new QLineEdit(this);
    browseInputBtn = new QPushButton("浏览...", this);  // 改为类成员变量

    // 裁剪范围输入
    extentPathEdit = new QLineEdit(this);
    browseExtentBtn = new QPushButton("浏览...", this);  // 改为类成员变量

    // 输出文件选择
    outputPathEdit = new QLineEdit(this);
    browseOutputBtn = new QPushButton("浏览...", this);  // 改为类成员变量

    // 按钮
    clipBtn = new QPushButton("裁剪", this);  // 改为类成员变量
    cancelBtn = new QPushButton("取消", this);  // 改为类成员变量

    // 连接信号槽
    connect(browseInputBtn, &QPushButton::clicked, this, &ClipDialog::browseInput);
    connect(browseExtentBtn, &QPushButton::clicked, this, &ClipDialog::browseExtent);
    connect(browseOutputBtn, &QPushButton::clicked, this, &ClipDialog::browseOutput);
    connect(clipBtn, &QPushButton::clicked, this, &ClipDialog::performClip);
    connect(cancelBtn, &QPushButton::clicked, this, &ClipDialog::reject);
}

void setupLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 输入文件行
    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(new QLabel("裁剪对象:", this));
    inputLayout->addWidget(inputPathEdit);
    inputLayout->addWidget(browseInputBtn);
    mainLayout->addLayout(inputLayout);

    // 裁剪范围行
    QHBoxLayout* extentLayout = new QHBoxLayout;
    extentLayout->addWidget(new QLabel("裁剪范围:", this));
    extentLayout->addWidget(extentPathEdit);
    extentLayout->addWidget(browseExtentBtn);
    mainLayout->addLayout(extentLayout);

    // 输出文件行
    QHBoxLayout* outputLayout = new QHBoxLayout;
    outputLayout->addWidget(new QLabel("输出位置:", this));
    outputLayout->addWidget(outputPathEdit);
    outputLayout->addWidget(browseOutputBtn);  // 取消注释
    mainLayout->addLayout(outputLayout);

    // 按钮行
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(clipBtn);  // 取消注释
    buttonLayout->addWidget(cancelBtn);  // 取消注释
    mainLayout->addLayout(buttonLayout);
}

    void connectSignals() {
        // 信号槽已在createWidgets中连接
    }

    // 裁剪函数实现
    bool clip(const QString& inputPath, const QString& extent, const QString& outputPath) {
        // 解析裁剪范围（格式: XMin,YMin,XMax,YMax）
        QStringList coords = extent.split(',');
        if (coords.size() != 4) {
            qDebug() << "裁剪范围格式错误";
            return false;
        }

        double xMin = coords[0].toDouble();
        double yMin = coords[1].toDouble();
        double xMax = coords[2].toDouble();
        double yMax = coords[3].toDouble();

        // 实际裁剪逻辑（这里用GDAL示例）
        try {
            // 示例：使用GDAL进行栅格裁剪
            // GDALAllRegister();
            // GDALDataset* srcDS = (GDALDataset*)GDALOpen(inputPath.toUtf8(), GA_ReadOnly);
            // ... 裁剪操作 ...
            // GDALClose(srcDS);

            qDebug() << "执行裁剪:" << inputPath << "->" << outputPath;
            qDebug() << "裁剪范围:" << xMin << yMin << xMax << yMax;

            // 模拟成功
            return true;
        }
        catch (const std::exception& e) {
            qDebug() << "裁剪失败:" << e.what();
            return false;
        }
    }
};