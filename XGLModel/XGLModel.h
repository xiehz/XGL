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
