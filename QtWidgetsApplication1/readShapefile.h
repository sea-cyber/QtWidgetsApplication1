#pragma once
#include "stdafx.h"
#include <QString>
#include <QVector>
//#include "ogrsf_frmts.h" // GDAL/OGR头文件，用于处理Shapefile
template<typename T>
class OGRLayer {

};
class FileReader {
private:
    // 私有构造函数，防止外部实例化
    FileReader() = default;
    ~FileReader() = default;

    // 禁止拷贝和赋值
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

public:
    // 获取单例实例的静态方法
    static FileReader* instance();

    // 函数调用操作符重载，实现FileReader()->xxx()语法
    FileReader* operator()();

    // 核心功能：读取Shapefile文件
    bool readShapefile();
};