#pragma once
#include <functional>
#include <string>
#include "stdafx.h"
#include "Controller.h"
namespace XGLInterface {

	typedef std::function<int(int,std::string&)> XGLDelegate;

	class XGLInterface {

	public:
		XGLInterface() {
			xgl = nullptr;
			is_Setup = false;
			deleg = 0;
		}
		virtual ~XGLInterface()
		{
			if (xgl != nullptr)
			{
				xgl->close();
				delete xgl;
				xgl = nullptr;
			}
		}

	public:
		virtual void bindWindow(HWND  handle) { 
			xgl->setHandle(handle); 
		}
		void setEventHandler(const XGLDelegate&  deleg) { this->deleg = deleg; }

	public:
		bool isSetup() { return is_Setup; }
		virtual int paint() { return xgl->paint(); }
		virtual int render() { return xgl->render(); }
		virtual int onMessage(int id,std::string& msg) = 0;
		virtual int sendViewMsg(int id, std::string& msg) {return this->deleg(id,msg); }
		virtual void setup(std::string name) = 0;
		virtual int close() = 0;
		virtual int lButtonDown(WPARAM state, int x, int y) { return xgl->lButtonDown((WPARAM)state,x,y); }
		virtual int lButtonUp(WPARAM state, int x, int y) { return xgl->lButtonUp((WPARAM)state, x, y); }
		virtual int rButtonDown(WPARAM state, int x, int y) { return xgl->rButtonDown((WPARAM)state, x, y); }
		virtual int rButtonUp(WPARAM state, int x, int y) { return xgl->rButtonUp((WPARAM)state, x, y); }
		virtual int mouseMove(WPARAM state, int x, int y) { return xgl->mouseMove((WPARAM)state, x, y); }
		virtual int mouseWheel(int state, int delta, int x, int y) { return xgl->mouseWheel(state, delta,x, y); }
		virtual int size( WPARAM wParam,int w, int h) { return xgl->size((WPARAM)wParam,w,h); }

	protected:
		Controller* xgl;
		bool is_Setup;
	private:
		XGLDelegate  deleg;
	};
}
