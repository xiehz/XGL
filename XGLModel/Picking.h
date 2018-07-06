#pragma once
#include "xgl/Matrixf"
#include "TutorialFactory.h"

namespace XGLModel {

	struct PixelInfo
	{
		unsigned int ObjectID;
		unsigned int DrawID;
		unsigned int PrimitiveID;

		PixelInfo() {
			ObjectID = 0;
			DrawID = 0;
			PrimitiveID = 0;
		}
		PixelInfo operator =(const PixelInfo& info)
		{
			ObjectID = info.ObjectID;
			DrawID = info.DrawID;
			PrimitiveID = info.PrimitiveID;
		}
	};

	class Picking : public TutorialInterface
	{
	public:
		Picking();
		~Picking();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;
		void setCamera(const Matrixf& m);
		const XGLModel::PixelInfo& readPixel(unsigned int x, unsigned int y);
	public:
		GLuint m_fbo;
		GLuint m_colorbuffer;
		GLuint m_depthbuffer;
	public:
		GLuint g_mv;
		GLuint g_pers;
		GLint g_objectindex;
		GLint g_drawcallindex;
		XGLModel::PixelInfo m_pixel;

	};



}
