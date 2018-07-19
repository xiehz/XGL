#pragma once
#include "GL\glew.h"
#include "xgl\Vec3f"
#include "XGLModel.h"
#include "XLight.h"
#include "TutorialInterface.h"

namespace XGLModel {

	class PointLightShader : public TutorialInterface
	{
	public:
		PointLightShader(int n);
		~PointLightShader();
	public:
		// 通过 TutorialInterface 继承
		virtual void initUniform() override;
		void updateSampler(int pos, int diffuse, int normal);
		void activeLights();
		void updateScreen(unsigned int width, unsigned int height);
		unsigned int num() { return N; }
		void updateLight(const TagPointLight& pl, const unsigned int index, float materialIntensity, float shineness);
		void updateMT(const XGL::Matrixf& mv, const XGL::Matrixf& pers);
	private:
		const unsigned int N;
		GLint g_tex_pos;
		GLint g_tex_diffuse;
		GLint g_tex_normal;
		struct TagPointLightLocation
		{
			GLint Color;
			GLint AmbientIntensity ;
			GLint DiffuseIntensity ;
			GLint Eposition ;
			struct TagAttenuationLocation
			{
				GLint Constant;
				GLint Linear;
				GLint Exp;
			}Attenuation;
		};

		TagPointLightLocation* m_pointLocation;
		GLint g_shineness;
		GLint g_materialIntensity;
		GLint  g_mv;
		GLint  g_normal;
		GLint  g_mvp;
		GLint  g_N;
		GLint g_screensize;

		// 通过 TutorialInterface 继承
		virtual void initGL() override;
		virtual void draw() override;
	};


}

