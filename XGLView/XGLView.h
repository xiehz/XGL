// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XGLVIEW_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XGLVIEW_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XGLVIEW_EXPORTS
#define XGLVIEW_API __declspec(dllexport)
#else
#define XGLVIEW_API __declspec(dllimport)
#endif

// �����Ǵ� XGLView.dll ������
class XGLVIEW_API CXGLView {
public:
	CXGLView(void);
	// TODO:  �ڴ�������ķ�����
};

extern XGLVIEW_API int nXGLView;

XGLVIEW_API int fnXGLView(void);
