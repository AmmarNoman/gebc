#ifndef __LICENSEWINDOW
#define __LICENSEWINDOW

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

class LicenseWindow : public Fl_Window {
	public:
	LicenseWindow(void);
	~LicenseWindow(void);
	
	private:
	Fl_Text_Buffer* buf;
	Fl_Text_Display* lic;
	

};

#endif
