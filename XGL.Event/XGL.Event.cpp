// XGL.Event.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "XGL.Event.h"


// ���ǵ���������һ��ʾ��
XGLEVENT_API int nXGLEvent=0;

// ���ǵ���������һ��ʾ����
XGLEVENT_API int fnXGLEvent(void)
{
    return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� XGL.Event.h
CXGLEvent::CXGLEvent()
{
    return;
}
