#include <functional>
#include "XGLInterop.h"
#include "MessageHelper.h"
#include"XGLInterface\XGLInterface.h"


using namespace XGLWrapper;


XGLInterop::XGLInterop(intptr_t view, XGLControl ^ control)
{
	this->view = view;
	this->control = control;
	using namespace std::placeholders;
	getXGLInterface()->setEventHandler(std::bind<int>(&XGLInterop::handleViewMsg,this,_1,_2));
}

XGLInterop::~XGLInterop()
{

}

int XGLInterop::sendMessage(int id, String ^ msg)
{
	std::string values;
	ConvertToString(msg, values);
	return getXGLInterface()->onMessage(id,values);
}

XGLInterface::XGLInterface * XGLInterop::getXGLInterface()
{
	return reinterpret_cast<XGLInterface::XGLInterface*>(view);
}

int XGLInterop::handleViewMsg(int id, std::string& msg)
{
	String^ values = System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr)(char *)msg.c_str());
	return this->control->OnViewMsg(id,values);
}
