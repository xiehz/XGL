#include "stdafx.h"
#include "xgl/error.h"
#include "xgl/Matrixf"
#include "XTexture.h"
#include "XRandomTexture.h"
#include "Billboarding.h"
#include "ParticleSystem.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 1000.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

namespace XGLModel {
	REGISTER(ParticleSystem)
}

struct Particle
{
	float Type;
	float PosX;
	float PosY;
	float PosZ;

	float VelX;
	float VelY;
	float VelZ;
	float LifetimeMillis;
};
long long GetCurrentTimeMillis()
{
#ifdef WIN32    
	return GetTickCount();
#else
	timeval t;
	gettimeofday(&t, NULL);

	long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
	return ret;
#endif    
}

XGLModel::ParticleSystem::ParticleSystem()
{
	m_pTex = 0;
	m_pRandom = 0;
	m_currVB = 0;
	m_currTFB = 1;
	m_pBillboarding = 0;
	m_isFirst = true;
	m_time = 0;
}


XGLModel::ParticleSystem::~ParticleSystem()
{
	if (m_pTex)
		delete m_pTex;
	m_pTex = 0;
	if (m_pRandom)
		delete m_pRandom;
	m_pRandom = 0;
	if (m_pBillboarding)
		delete m_pBillboarding;
	m_pBillboarding = 0;
}

void XGLModel::ParticleSystem::initGL()
{
	m_pBillboarding = dynamic_cast<Billboarding*>(TutorialFactory::getInstance().getClass("Billboarding"));
	m_pBillboarding->initShader();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	m_pTex = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/fireworks_red.jpg");
	m_pTex->Load();

	m_pRandom = new XRandomTexture();
	m_pRandom->Init(MAX_PARTICLES);
	
	Particle Particles[MAX_PARTICLES];
	memset(Particles, 0, sizeof(Particles));

	Particles[0].Type = PARTICLE_TYPE_LAUNCHER;
	Particles[0].PosX = 0;
	Particles[0].PosY = 0;
	Particles[0].PosZ = 1.0;

	Particles[0].VelX = 0;
	Particles[0].VelY = 0.01f;
	Particles[0].VelZ = 0;

	Particles[0].LifetimeMillis = 0.0f;

	glGenTransformFeedbacks(2, m_transformFeedback);
	glGenBuffers(2, m_particleBuffer);

	for (int i = 0; i < 2; ++i)
	{
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
		int size = sizeof(Particles);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
	}
	m_currentTimeMillis = GetCurrentTimeMillis();
}

void XGLModel::ParticleSystem::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	long long TimeNowMillis = GetCurrentTimeMillis();
	unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
	m_currentTimeMillis = TimeNowMillis;

	glUseProgram(program);
	m_time += DeltaTimeMillis;
	UpdateParticles(DeltaTimeMillis);

	glUseProgram(m_pBillboarding->program);
	RenderParticles();

	m_currVB = m_currTFB;
	m_currTFB = (m_currTFB + 1) & 0x1;
}

void XGLModel::ParticleSystem::initUniform()
{
	if(!InitParticle())
	{
		XGLERROR("get uniform failed");
	}

	m_pBillboarding->initUniform();
	m_pBillboarding->setBillboardSize(0.01f);
}


void XGLModel::ParticleSystem::UpdateParticles(int DeltaTimeMillis)
{
	glUniform1f(m_timeLocation, m_time);
	glUniform1f(m_deltaTimeMillisLocation,DeltaTimeMillis);
	glUniform1i(m_randomTextureLocation, 1);
	glUniform1f(m_launcherLifetimeLocation, 100.0f);
	glUniform1i(m_randomTextureLocation, 1);
	glUniform1f(m_shellLifetimeLocation, 10000.0f);
	glUniform1f(m_secondaryShellLifetimeLocation, 2500.0f);

	m_pRandom->Bind(GL_TEXTURE1);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);                          // type
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);         // position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);        // velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);          // lifetime

	glBeginTransformFeedback(GL_POINTS);

	if (m_isFirst) {
		glDrawArrays(GL_POINTS, 0, 1);

		m_isFirst = false;
	}
	else {
		glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
	}

	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}


void XGLModel::ParticleSystem::RenderParticles()
{
	Matrixf& mv = camera->getInverseMatrix();
	const XGL::Vec3f& trans = mv.getTrans();
	Matrixf transMat = Matrixf::identity();
	transMat.setTrans(trans);
	Matrixf test = projectMatrix;
	test.preMult(transMat);

	glUniform3f(m_pBillboarding->g_eye, trans.x(), trans.y(), trans.z());
	glUniformMatrix4fv(m_pBillboarding->g_pers, 1, GL_FALSE, test.ptr());
	glUniform1i(m_pBillboarding->g_sampler2d, 0);
	m_pTex->Bind(GL_TEXTURE0);

	glDisable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);
		Particle Particles[MAX_PARTICLES];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(Particles), Particles);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);  // position

	glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

	glDisableVertexAttribArray(0);
}

bool XGLModel::ParticleSystem::InitParticle()
{
	const GLchar* Varyings[4];
	Varyings[0] = "Type1";
	Varyings[1] = "Position1";
	Varyings[2] = "Velocity1";
	Varyings[3] = "Age1";

	glTransformFeedbackVaryings(program, 4, Varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(program);
//	getErrorInformation(GetLastError());

	m_deltaTimeMillisLocation = glGetUniformLocation(program,"gDeltaTimeMillis");
	m_randomTextureLocation = glGetUniformLocation(program, "gRandomTexture");
	m_timeLocation = glGetUniformLocation(program, "gTime");
	m_launcherLifetimeLocation = glGetUniformLocation(program, "gLauncherLifetime");
	m_shellLifetimeLocation = glGetUniformLocation(program, "gShellLifetime");
	m_secondaryShellLifetimeLocation = glGetUniformLocation(program, "gSecondaryShellLifetime");

	if (m_deltaTimeMillisLocation < 0 ||
		m_timeLocation <0 ||
		m_randomTextureLocation < 0||
		m_launcherLifetimeLocation < 0||
		m_shellLifetimeLocation <0||
		m_secondaryShellLifetimeLocation <0) {
		return false;
	}

	return true;
}