#pragma once
#include "stdafx.h"
#include <msclr/gcroot.h>
#include <string>
#include "XGLControl.h"

namespace XGLWrapper {

	public  class XGLInterop
	{
	public:
		XGLInterop(intptr_t view, XGLControl^ control );
		virtual ~XGLInterop();
	public:
		int sendMessage(int id,String^ msg);
	protected:
		XGLInterface::XGLInterface* getXGLInterface();
	private:
		int handleViewMsg(int id, std::string& msg);
		intptr_t view;
		msclr::gcroot<XGLControl^> control;
	};

}


