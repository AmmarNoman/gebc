#include "AboutWindow.h"

AboutWindow::AboutWindow(void) : Fl_Window(600,400,"About GNU Ballistics Computer"){
	
	begin();
	lic = new Fl_Text_Display(5,5,590,390);
	buf = new Fl_Text_Buffer(36000);
	
	buf->text("GNU Exterior Ballistics Computer v1.00\n\
Created by:   Derek R. Yates\n\
Released on:  February, 2008\n\
Built using:  Mingw32 / GCC compiler\n\
\n\
Note from the Creator:\n\
GNU Exterior Ballistics Computer was created as an open-sourced\n\
alternative to the multitude of commercial ballistics computers.\n\
As an avid shooter, I have purchased several commercial packages\n\
over the years, but never found exactly what I wanted in any of them.\n\
I have created this program, and released it under the GNU GPL\n\
License in order to create what I am looking for in a program, \n\
and to give you a base on which to build and improve what I've \n\
started.  The unique features I have included are a multitude of\n\
data export features, making it easy to load your data into a variety\n\
of analysis software, from OpenOffice to Microsoft Excel, and also in\n\
Adobe PDF format.  Never before has so much ballistics information been\n\
available so freely.\n\
\n\
This program was written in C++, and uses the excellent GNU Exterior\n\
ballistics library (which I wrote previously for personal use).  The \n\
solutions are based on the three degrees of freedom differential equations\n\
and solved numerically to get a very accurate solution.  In general the\n\
range data is accurate to within 0.1 yards, and the drop data is accurate\n\
to within a fraction of an inch.  Standard drag models from G1-G8 are supported\n\
making this program very flexible for advanced ballistics use.\n\
\n\
The GUI was created using FLTK, to allow both Windows and Linux users\n\
to build the source.  PDF creation is done using the LibHaru library, which\n\
is highly recommended should you ever need a C++ library for PDF creation.\n\
\n\
I hope you have as much fun using this program as I have had creating it,\n\
and wish you the best shooting.\n");

	lic->buffer(buf);
	
	end();
	show();
	
	
}

AboutWindow::~AboutWindow(void){
  delete lic;
  delete buf;
}

