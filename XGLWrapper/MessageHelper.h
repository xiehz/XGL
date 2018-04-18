#pragma once
#include "stdafx.h"

namespace XGLWrapper {

	extern void ConvertToString(System::String^ str, std::string& text);

	inline int GetX(unsigned int lp)
	{
		return (int)(short)LOWORD(lp);
	}

	inline int GetY(unsigned int lp)
	{
		return (int)(short)HIWORD(lp);
	}

	inline void ConvertToString(System::String^ str, std::string& text)
	{
		char* p = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
		text = p;
		System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)p);
	}
}


