#pragma once
#include "stdafx.h"
#include <QString>
#include <QVector>
//#include "ogrsf_frmts.h" // GDAL/OGRͷ�ļ������ڴ���Shapefile
template<typename T>
class OGRLayer {

};
class FileReader {
private:
    // ˽�й��캯������ֹ�ⲿʵ����
    FileReader() = default;
    ~FileReader() = default;

    // ��ֹ�����͸�ֵ
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

public:
    // ��ȡ����ʵ���ľ�̬����
    static FileReader* instance();

    // �������ò��������أ�ʵ��FileReader()->xxx()�﷨
    FileReader* operator()();

    // ���Ĺ��ܣ���ȡShapefile�ļ�
    bool readShapefile();
};