///////////////////////////////////////////////////////////////////////////////
// Log.cpp
// =======
// It prints out any log messages to file or dialog box.
// Log class is a singleton class which is contructed by calling
// Log::getInstance() (lazy initialization), and is destructed automatically
// when the application is terminated.
//
// In order to log, use Win::log() function with appropriate formats.
// For example, Win::log(L"My number: %d\n", 123).
// It is similar to printf() function of C standard libirary.
//
// The template of the log dialog window is defined in log.rc and logResource.h
// You must include both resource file with this source codes.
// The dialog window cannot be closed by user once it is created. But it will be
// destroyed when the application terminated.
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2006-07-14
// UPDATED: 2008-05-07
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <cstdarg>
#include <cwchar>
#include <sstream>
#include <iomanip>
#include "Log.h"
using namespace XGL;


const char* LOG_FILE = "log.txt";

BOOL CALLBACK logDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);



///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////
Log::Log() : logMode()
{
    // open log file
    logFile.open(LOG_FILE, std::ios::out);
    if(logFile.fail())
        return;

    // first put starting date and time
    logFile << "===== Log started at "
            << getDate() << ", "
            << getTime() << L". =====\n\n"
            << std::flush;
}



///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
Log::~Log()
{
    // close opened file
    logFile << "\n\n===== END OF LOG =====\n";
    logFile.close();
}



///////////////////////////////////////////////////////////////////////////////
// instantiate a singleton instance if not exist
///////////////////////////////////////////////////////////////////////////////
Log& Log::getInstance()
{
    static Log self;
    return self;
}



///////////////////////////////////////////////////////////////////////////////
// add message to log
///////////////////////////////////////////////////////////////////////////////
void Log::put(const std::string& message)
{
	// put time first and append message
	logFile << getTime() << L"  "
		<< message
		<< L"\n"
		<< std::flush;
}




///////////////////////////////////////////////////////////////////////////////
// get system date as a string
///////////////////////////////////////////////////////////////////////////////
const std::string Log::getDate()
{
    std::stringstream ss;

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

	ss << std::setfill('0');
	ss << sysTime.wYear << "-" << std::setw(2)
        << sysTime.wMonth << "-" << std::setw(2)
        << sysTime.wDay;

    return ss.str();
}



///////////////////////////////////////////////////////////////////////////////
// get system time as a string
///////////////////////////////////////////////////////////////////////////////
const std::string Log::getTime()
{
	std::stringstream ss;

    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);

	ss << std::setfill('0');
	ss << sysTime.wHour << ":" << std::setw(2)
        << sysTime.wMinute << ":" << std::setw(2)
        << sysTime.wSecond;

    return ss.str();
}



void XGL::log(const char *format, ...)
{
    char buffer[LOG_MAX_STRING];

    // do the formating
    va_list valist;
    va_start(valist, format);
    _vsnprintf_s(buffer, LOG_MAX_STRING, format, valist);
    va_end(valist);

    Log::getInstance().put(buffer);
}



void XGL::log(const std::string& str)
{
    Log::getInstance().put(str);
}
