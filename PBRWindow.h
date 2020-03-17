#ifndef __PBRWINDOW
#define __PBRWINDOW


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>

#include "InputWindow.h"
#include "lib/ballistics/ballistics.h"

#include <stdio.h>
#include <stdlib.h>


class PBRWindow : public Fl_Window {
   public:
      PBRWindow( InputWindow* );
      ~PBRWindow();
	  
   private:
        InputWindow* inW;
	 Fl_Int_Input* VI;
	 Fl_Float_Input* CD;
	 Fl_Float_Input* SH;
	 Fl_Float_Input* VitalSize;
	 Fl_Button* Calc;
	 
	 Fl_Multiline_Output* oStatus;
	 
	 static void cb_calc(Fl_Widget*, void*);

};
#endif


