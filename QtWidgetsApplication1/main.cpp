#include "stdafx.h"
#include "QtWidgetsApplication1.h"
#include <QtWidgets/QApplication>
#include "gdal_priv.h"
int main(int argc, char *argv[])
{
    GDALAllRegister();
    QApplication a(argc, argv);
    QFont font("΢���ź�", 10);  // ��������Ϊ΢���źڣ��ֺ� 10
    a.setFont(font);
    QtWidgetsApplication1 w;
    w.show();
    return a.exec();
}
