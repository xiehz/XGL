// XGLModel.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "XGLModel.h"
#include <XGL\Quaternion.h>

// 这是导出变量的一个示例
XGLMODEL_API int nXGLModel=0;

// 这是导出函数的一个示例。
XGLMODEL_API int fnXGLModel(void)
{
	
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 XGLModel.h
CXGLModel::CXGLModel()
{
    return;
}
