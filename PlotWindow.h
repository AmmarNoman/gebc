#ifndef __PLOTWINDOW
#define __PLOTWINDOW

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.h>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GBCSolution.h"

class PlotWindow : public Fl_Window {
	public:
	PlotWindow(int, int, GBCSolution*, GBCSolution*, GBCSolution*);
	~PlotWindow(void);
	GBCSolution* gsln;
	GBCSolution* mem1;
	GBCSolution* mem2;
	

	
	private:
	virtual void draw(void);
	int cwidth;
	int cheight;
	int x_ticks;
	int y_ticks;
	
	void PlotPath(void);
	void PlotEnergy(void);
	void PlotVelocity(void);
	
	int x_click;
	int y_click;
	
	Fl_Round_Button* rbPlotPath;
	Fl_Round_Button* rbPlotVelocity;
	Fl_Round_Button* rbPlotEnergy;
	
	Fl_Check_Button* btMem1;
	Fl_Check_Button* btMem2;
	
	Fl_Value_Slider* sl_x;
	
	Fl_Menu_Bar* menu;
	
	virtual int handle(int);
	static void cb_nothing(Fl_Widget* o, void* v);
	static void cb_Close(Fl_Widget* o, void* v);
	static void cb_btPlot(Fl_Widget* o, void* v);
	static void cb_slUpdate(Fl_Widget* o, void* v);
	static void cb_Mem1(Fl_Widget* o, void* v);
	static void cb_Mem2(Fl_Widget* o, void* v);
	

};

#endif
