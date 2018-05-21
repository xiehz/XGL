#include "stdafx.h"
#include "XGL\ViewGL.h"
#include "XGLModel\ModelGL.h"
#include "XGLModel\TutorialFactory.h"
#include "XGL\TrackballCamera.h"
#include "XGL\OrbitCamera.h"
#include "xgl\XEventHandler.h"
#include "xgl\XOrbitCamera.h"
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
	camera = nullptr;
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
	camera = new XOrbitCamera();
	model = XGLModel::TutorialFactory::getInstance().getClass(name);
	model->bindHandle(xgl->getHandle());

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0, 0.0, 2), Vec3f(0, 0.0, 0.0f), Vec3f(0, 1, 0));

	model->setViewMatrix(orbit->getMatrix().ptr());
	float width = 1.0f, height = 1.0f;

	model->project(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, 1, 100.0f);

	dynamic_cast<ControllerGL*>(xgl)->setup(model, viewgl, camera);
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

	if(camera !=nullptr)
	delete camera;
	camera = nullptr;

	return 1;
}

int XGLView::XGLViewer::onMessage(int id, std::string &msg)
{
	return sendViewMsg(id, msg);
}




