#include "stdafx.h"
#include "XGLInterface\XGLInterface.h"
#include "MessageHelper.h"
#include "XGLInterop.h"
#include "XGLControl.h"

void XGLWrapper::XGLControl::BindXGLInterface(intptr_t viewer)
{
	xgl = reinterpret_cast<XGLInterface::XGLInterface*>(viewer);
	xgl->bindWindow((HWND) this->Handle.ToInt64());

	interop = new XGLInterop(viewer,this);
}

XGLWrapper::XGLControl::!XGLControl()
{
	if (xgl != nullptr)
	{
		delete xgl;
		xgl = nullptr;
	}
	if (interop != nullptr)
	{
		delete interop;
		interop = nullptr;
	}
}

void XGLWrapper::XGLControl::Render() {
	if (xgl != nullptr)
	{
		xgl->render();
	}
}


void XGLWrapper::XGLControl::Setup(String^ name)
{
	std::string cname;
	ConvertToString(name, cname);
	if (xgl != nullptr)
		xgl->setup(cname);
}

int XGLWrapper::XGLControl::Close()
{
	if (xgl != nullptr)
		return xgl->close();
	return 0;
}

int XGLWrapper::XGLControl::SendMsg(int id, String ^ msg)
{
	if (xgl != nullptr && interop != nullptr)
	{
		return interop->sendMessage(id,msg);
	}
	return 0;
}

void XGLWrapper::XGLControl::WndProc(System::Windows::Forms::Message % m)
{
	if (xgl != nullptr && xgl ->isSetup())
	{
		if (!dispatchMessage(m))
		{
			__super::WndProc(m);
		}
	}
	else {
		__super::WndProc(m);
	}
}

intptr_t XGLWrapper::XGLControl::getInstance()
{
	return (intptr_t)xgl;
}

bool XGLWrapper::XGLControl::dispatchMessage(System::Windows::Forms::Message % m)
{
	HWND hwnd = (HWND) (this->Handle.ToInt64());
	WPARAM wParam = m.WParam.ToInt64();
	LPARAM lParam = m.LParam.ToInt64();

	switch (m.Msg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		xgl->paint();
		break;
	case WM_LBUTTONDOWN:
		xgl->lButtonDown(wParam,GetX(lParam),GetY(lParam));
		break;
	case WM_MBUTTONDOWN:
		xgl->mButtonDown(wParam, GetX(lParam), GetY(lParam));
		break;
	case WM_MBUTTONUP:
		xgl->mButtonUp(wParam, GetX(lParam), GetY(lParam));
		break;
	case WM_LBUTTONUP:
		xgl->lButtonUp(wParam, GetX(lParam), GetY(lParam));
		break;
	case WM_RBUTTONDOWN:
		xgl->rButtonDown(wParam, GetX(lParam), GetY(lParam));
		break;
	case WM_RBUTTONUP:
		xgl->rButtonUp(wParam, GetX(lParam), GetY(lParam));
		break;
	case WM_MOUSEWHEEL:
		xgl->mouseWheel(wParam, GET_WHEEL_DELTA_WPARAM(wParam),GetX(lParam), GetY(lParam));
		break;
	case WM_MOUSEMOVE:
		xgl->mouseMove(wParam, GetX(lParam), GetY(lParam));
		break;
	case WM_SIZE:
		xgl->size(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_ERASEBKGND:
		return true;
	case WM_XGL_MODEL:
	{
		String^ values = System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)lParam);
		OnViewMsg(int(m.WParam), values);
	}
		break;
	default:
		break;
	}
	return false;
}
