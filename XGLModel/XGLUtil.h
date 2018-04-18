#pragma once
#include <string>


namespace XGLModel {

#define XGLERROR(error) errorReport(__FILE__, __LINE__, error);

	extern bool readShaderFile(const char* pFileName, std::string& outFile);
	extern void errorReport(const char* pFileName, unsigned int line, const char* pError);

}