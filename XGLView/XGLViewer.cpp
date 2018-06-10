#include "stdafx.h"
#include "XGL\ViewGL.h"
#include "XGLModel\ModelGL.h"
#include "XGLModel\TutorialFactory.h"
#include "XGL\TrackballCamera.h"
#include "XGL\OrbitCamera.h"
#include "ControllerGL.h"
#include "XGLViewer.h"

using namespace XGL;
using namespace XGLModel;
using  namespace XGLView;

XGLViewer::XGLViewer()
{
	xgl = new ControllerGL();
	viewgl = nullptr;
	model = nullptr;
}


XGLViewer::~XGLViewer()
{
	this->close();

	delete xgl;
	xgl = nullptr;
}

void XGLView::XGLViewer::setup(std::string name)
{
	this->close();

	viewgl = new ViewGL();
	model = XGLModel::TutorialFactory::getInstance().getClass(name);
	model->bindHandle(xgl->getHandle());

	dynamic_cast<ControllerGL*>(xgl)->setup(model, viewgl);
	is_Setup =xgl->create();
}

int XGLView::XGLViewer::close()
{
	is_Setup = false;
	xgl->close();

	if(viewgl!=nullptr)
	delete viewgl;
	viewgl = nullptr;

	if (model != nullptr)
		delete model;
	model = nullptr;

	return 1;
}

int XGLView::XGLViewer::onMessage(int id, std::string &msg)
{
	return sendViewMsg(id, msg);
}




