#include "stdafx.h"
#include "TutorialFactory.h"

using namespace XGLModel;
TutorialFactory::TutorialFactory()
{
}


TutorialFactory::~TutorialFactory()
{
}

ModelGL * TutorialFactory::getClass(const std::string & t)
{
	std::map<std::string, CREATEPROC>::iterator iter = this->cp.find(t);
	if (iter == this->cp.end())
		return  new ModelGL();
	else {
		return cp[t]();
	}
}

void TutorialFactory::registerCreateProc(const std::string & name, CREATEPROC cp)
{
	this->cp[name] = cp;
}

void TutorialFactory::unRegisterCreateProc(const std::string & name)
{
	std::map<std::string, CREATEPROC>::iterator iter = this->cp.find(name);
	if (iter != cp.end())
	{
		iter = cp.erase(iter);
	}
}

