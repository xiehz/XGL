#pragma once
#include "stdafx.h"

namespace XGLWrapper {

	inline int GetX(unsigned int lp)
	{
		return (int)(short)LOWORD(lp);
	}

	inline int GetY(unsigned int lp)
	{
		return (int)(short)HIWORD(lp);
	}

	void ConvertToString(System::String^ str, std::string& text)
	{
		char* p = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
		text = p;
		System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)p);
	}
}


