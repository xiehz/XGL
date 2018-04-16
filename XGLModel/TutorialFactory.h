#pragma once
#include <string>
#include <map>
#include "TutorialInterface.h"

namespace XGLModel {
	class ModelGL;
	typedef TutorialInterface* (*CREATEPROC)();

#define REGISTER(classname) \
	classname* objectCreator##classname()\
	{\
		return new classname;\
	}\
    RegisterAction global_Register##classname##creator(                        \
        #classname,(CREATEPROC)objectCreator##classname);


	class XGLMODEL_API TutorialFactory
	{
	private:
		TutorialFactory();
		TutorialFactory(TutorialFactory&);
	public:
		virtual ~TutorialFactory();
		static TutorialFactory& getInstance() {
			static TutorialFactory factory;
			return factory;
		}
	public:
		ModelGL* getClass(const std::string& t);
		void registerCreateProc(const std::string& name, CREATEPROC cp);
		void unRegisterCreateProc(const std::string& name);
	private:
		std::map<std::string, CREATEPROC> cp;

	};
	class RegisterAction {
	public:
		RegisterAction(std::string name, CREATEPROC cp)
		{
			TutorialFactory::getInstance().registerCreateProc(name, cp);
		}
	};

}


