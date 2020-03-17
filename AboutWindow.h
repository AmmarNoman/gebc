#ifndef __ABOUTWINDOW
#define __ABOUTWINDOW

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

#include <stdio.h>
#include <stdlib.h>

class AboutWindow : public Fl_Window {
	public:
	AboutWindow(void);
	~AboutWindow(void);
	
	private:
	Fl_Text_Display* lic;
	Fl_Text_Buffer* buf;

};

#endif
