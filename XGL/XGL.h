// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XGL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XGL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef  XGL_H
#define XGL_H

#ifdef XGL_EXPORTS
#define XGL_API __declspec(dllexport)
#else
#define XGL_API __declspec(dllimport)
#endif

// �����Ǵ� XGL.dll ������
class XGL_API CXGL {
public:
	CXGL(void);
	// TODO:  �ڴ�������ķ�����
};

extern XGL_API int nXGL;

XGL_API int fnXGL(void);

#endif // ! XGL_H



