#pragma once
#include "GL\glew.h"
#include "TutorialFactory.h"
#include "xgl/Matrixf"
#include "xgl\Vec3f"
#include "XGLModel.h"
#include "XLight.h"
namespace XGLModel {

	class DirLightShader : public TutorialInterface
	{
	public:
		DirLightShader();
		~DirLightShader();
	public:
		// 通过 TutorialInterface 继承
		virtual void initUniform() override;
		void updateLight(const TagDirectionLight& pl,float materialIntensity, float shineness);
		void updateMT(const XGL::Matrixf& mv, const XGL::Matrixf& pers );
		void updateScreen(unsigned int width, unsigned int height);
		void updateSampler(int pos, int diffuse, int normal);
	private:
		struct TagDirectionLocation
		{
			GLint Color;
			GLint AmbientIntensity;
			GLint DiffuseIntensity;
			GLint Direction;
		};

		TagDirectionLocation m_lightLocation;

		GLint g_shineness;
		GLint g_materialIntensity;
		GLint  g_mv;
		GLint  g_normal;
		GLint  g_mvp;

		GLint  g_tex_pos;
		GLint  g_tex_normal;
		GLint  g_tex_diffuse;

		GLint g_screensize;

		// 通过 TutorialInterface 继承
		virtual void initGL() override;
		virtual void draw() override;
	};

}

