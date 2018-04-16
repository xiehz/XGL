#pragma once
using namespace System;

namespace XGLEvent {
 public	ref class XGLCreator
	{
	public:
		XGLCreator();
		virtual ~XGLCreator();
	public:
		intptr_t createXGL();
	};
}


