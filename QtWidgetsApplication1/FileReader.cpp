#include "readShapefile.h"
#include "stdafx.h"
// ʵ�־�̬���������ص���ʵ��
FileReader* FileReader::instance() {
    static FileReader instance;  // C++11���Զ��̰߳�ȫ
    return &instance;
}

// ʵ��()����������
FileReader* FileReader::operator()() {
    return this;
}

// ʵ�ֶ�ȡ������ģ�⣩
bool FileReader::readShapefile() {
    // ʵ��ʵ����Ҫ����GDAL��
    return true;
}