#ifndef __RANGEWINDOW
#define __RANGEWINDOW

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_File_Chooser.H>

#include <stdlib.h>
#include <stdio.h>

#include "GBCSolution.h"
#include "hpdf.h"

class RangeWindow : public Fl_Window {

   public:
      RangeWindow(int w, int h, GBCSolution* inSLN);
      ~RangeWindow();
	  int* colwidths;
	  int min,max,step,maxvalid;
	  Fl_Menu_Bar* menu;
	  Fl_Browser* tbl;
	  GBCSolution* Sln;
		
   private:
	static void cb_Nothing(Fl_Widget* , void* );
	static void cb_Min0(Fl_Widget* , void*);
	static void cb_Min50(Fl_Widget* , void*);
	static void cb_Min100(Fl_Widget* , void*);
	static void cb_Min250(Fl_Widget* , void*);
	static void cb_Min500(Fl_Widget* , void*);
	static void cb_Min750(Fl_Widget* , void*);
	static void cb_Min1000(Fl_Widget* , void*);
	static void cb_Min2000(Fl_Widget* , void*);

	static void cb_Max100(Fl_Widget* , void*);
	static void cb_Max250(Fl_Widget* , void*);
	static void cb_Max500(Fl_Widget* , void*);
	static void cb_Max750(Fl_Widget* , void*);
	static void cb_Max1000(Fl_Widget* , void*);
	static void cb_Max1760(Fl_Widget* , void*);
	static void cb_Max2000(Fl_Widget* , void*);
	static void cb_MaxMax(Fl_Widget* , void*);

	static void cb_Step1(Fl_Widget* , void*);
	static void cb_Step5(Fl_Widget* , void*);
	static void cb_Step10(Fl_Widget* , void*);
	static void cb_Step25(Fl_Widget* , void*);
	static void cb_Step50(Fl_Widget* , void*);
	static void cb_Step100(Fl_Widget* , void*);
	static void cb_Step250(Fl_Widget* , void*);
	
	static void cb_ExportCSV(Fl_Widget*, void*);
	static void cb_ExportExcel(Fl_Widget*, void*);
	static void cb_ExportHTML(Fl_Widget*, void*);
	static void cb_ExportSQL(Fl_Widget*, void*);
	static void cb_ExportXML(Fl_Widget*, void*);
	static void cb_ExportOO(Fl_Widget*, void*);
	static void cb_ExportPDF(Fl_Widget*, void*);
	static void cb_Close(Fl_Widget*, void*);
		  
		  
	
	void GenTable(void);

};

#endif


