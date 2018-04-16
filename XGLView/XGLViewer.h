#pragma once
#include <string>
#include "XGLView.h"
#include "XGLInterface\XGLInterface.h"
namespace XGL {
	class ViewGL;
	class Camera;
}

namespace XGLModel {
	class ModelGL;
}
namespace XGLView {
	class ControllerGL;

	class XGLVIEW_API XGLViewer : public XGLInterface::XGLInterface
	{
	public:
		XGLViewer();
		virtual ~XGLViewer();
	public:
		virtual void setup(std::string name)override;
		virtual int close()override;
	protected:
		XGLModel::ModelGL* model;
		XGL::ViewGL* viewgl;
		XGL::Camera* camera;
	};
}



