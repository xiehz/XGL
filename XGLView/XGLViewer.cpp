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
	camera = new OrbitCamera();
	model = XGLModel::TutorialFactory::getInstance().getClass(name);
	model->bindHandle(xgl->getHandle());

	camera->lookat(Vector3(0, 0, 2), Vector3(0, 0, 0), Vector3(0, 1, 0));
	model->setViewMatrix(camera->getViewMatrix());
	float width = 2.f, height = 2.f;
	camera->project(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, 1.0f,3.0f);
	model->setProject(camera->getProjectMatrix());

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




