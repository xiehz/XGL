#include "stdafx.h"
#include "XGLView\XGLViewer.h"
#include "XGLCreator.h"

using namespace XGLEvent;

XGLCreator::XGLCreator()
{
}


XGLCreator::~XGLCreator()
{
}

intptr_t XGLCreator::createXGL()
{
	return (intptr_t)(new XGLView::XGLViewer());
}
