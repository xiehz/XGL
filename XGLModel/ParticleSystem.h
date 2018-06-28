#pragma once

#include "TutorialFactory.h"


namespace XGLModel {

	class XTexture;
	class XRandomTexture;
	class Billboarding;
	class ParticleSystem : public TutorialInterface
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		// Í¨¹ý TutorialInterface ¼Ì³Ð
		virtual void initGL() override;
		virtual void draw() override;
		virtual void initUniform() override;

		void Render(int DeltaTimeMillis);

		void UpdateParticles(int DeltaTimeMillis);

		void RenderParticles();

		bool InitParticle();

	private:
		GLuint g_eye;
		GLuint g_pers;
		XTexture* m_pTex;
		XRandomTexture* m_pRandom;
		GLuint m_particleBuffer[2];
		GLuint m_transformFeedback[2];
		long m_time;
		bool m_isFirst;
		unsigned int m_currVB;
		unsigned int m_currTFB;
		Billboarding* m_pBillboarding;
		long long m_currentTimeMillis;
	private:
		GLuint m_deltaTimeMillisLocation;
		GLuint m_randomTextureLocation;
		GLuint m_timeLocation;
		GLuint m_launcherLifetimeLocation;
		GLuint m_shellLifetimeLocation;
		GLuint m_secondaryShellLifetimeLocation;
	};


}

