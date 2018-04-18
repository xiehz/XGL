#include "stdafx.h"
#include "iostream"
#include <fstream>
#include "XGLUtil.h"

bool XGLModel::readShaderFile(const char * pFileName, std::string & outFile)
{
	std::ifstream f(pFileName);
	bool ret = false;
	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		XGLERROR(pFileName);
	}

	return ret;
}

void XGLModel::errorReport(const char * pFileName, unsigned int line, const char * pError)
{
#ifdef WIN32
	char msg[1000];
	_snprintf_s(msg, sizeof(msg), "%s:%d: %s", pFileName, line, pError);
	MessageBoxA(NULL, msg, NULL, 0);
#else
	fprintf(stderr, "%s:%d: %s\n", pFileName, line, pError);
#endif    
}
