// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� XGLEVENT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// XGLEVENT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef XGLEVENT_EXPORTS
#define XGLEVENT_API __declspec(dllexport)
#else
#define XGLEVENT_API __declspec(dllimport)
#endif

// �����Ǵ� XGL.Event.dll ������
class XGLEVENT_API CXGLEvent {
public:
	CXGLEvent(void);
	// TODO:  �ڴ�������ķ�����
};

extern XGLEVENT_API int nXGLEvent;

XGLEVENT_API int fnXGLEvent(void);
