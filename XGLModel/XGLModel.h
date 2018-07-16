// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 XGLMODEL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// XGLMODEL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef  XGLMODEL_H
#define XGLMODEL_H


#ifdef XGLMODEL_EXPORTS
#define XGLMODEL_API __declspec(dllexport)
#else
#define XGLMODEL_API __declspec(dllimport)
#endif

// 此类是从 XGLModel.dll 导出的
class XGLMODEL_API CXGLModel {
public:
	CXGLModel(void);
	// TODO:  在此添加您的方法。
};

extern XGLMODEL_API int nXGLModel;

XGLMODEL_API int fnXGLModel(void);
#endif //  XGLMODEL_H

#define INVALID_MATERIAL 0xFFFFFFFF

#define INDEX_BUFFER 0    
#define POS_VB       1
#define NORMAL_VB    2
#define TEXCOORD_VB  3    
#define WORLD_MAT_VB 4

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define WORLD_LOCATION 3

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

#define GLCheckError() (glGetError() == GL_NO_ERROR)

#define COLOR_TEXTURE_UNIT 0x84C0
