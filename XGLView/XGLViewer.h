#pragma once
#include <string>
#include "XGLView.h"
#include "XGLInterface\XGLInterface.h"
namespace XGL {
	class ViewGL;
	class Camera;
	class XEventHandler;
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

		// Í¨¹ý XGLInterface ¼Ì³Ð
		virtual int onMessage(int id, std::string &msg);
	};
}



