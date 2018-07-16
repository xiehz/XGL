// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XGLMODEL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XGLMODEL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef  XGLMODEL_H
#define XGLMODEL_H


#ifdef XGLMODEL_EXPORTS
#define XGLMODEL_API __declspec(dllexport)
#else
#define XGLMODEL_API __declspec(dllimport)
#endif

// �����Ǵ� XGLModel.dll ������
class XGLMODEL_API CXGLModel {
public:
	CXGLModel(void);
	// TODO:  �ڴ�������ķ�����
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
