#ifndef __INPUTWINDOW
#define __INPUTWINDOW

#include "lib/ballistics/ballistics.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Help_Dialog.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>

#include <stdlib.h>
#include <stdio.h>

#include "GBCSolution.h"
#include "RangeWindow.h"
#include "AboutWindow.h"
#include "LicenseWindow.h"
#include "PlotWindow.h"

class InputWindow : public Fl_Window{
 
   public:
      InputWindow(int w, int h, const char* title );
      ~InputWindow();
      Fl_Button* btSolve;
	  Fl_Button* btReset;
	  Fl_Button* btStdAtmosphere;
	  Fl_Button* btShowChart;
	  Fl_Button* btPlot;
	  Fl_Button* btCompare;
      Fl_Button* btSaveSolution;
      Fl_Button* btLoadSolution;
	  
	  double* Solution;
	  double* Smem1;
	  double* Smem2;
	  double* Smem3;
	  double* Smem4;
	  double* Smem5;
	  double* Smem6;
	  double* Smem7;
	  double* Smem8;
	  
	  GBCSolution* gsln;
	  GBCSolution* mem1;
	  GBCSolution* mem2;
  	  GBCSolution* mem3;
	  GBCSolution* mem4;
	  GBCSolution* mem5;
	  GBCSolution* mem6;
  	  GBCSolution* mem7;
	  GBCSolution* mem8;	  
	  
	  int useSolution;
	 
	  // Drag function selection pointers.
	  Fl_Round_Button* inG1;
	  Fl_Round_Button* inG2;
	  Fl_Round_Button* inG5;
	  Fl_Round_Button* inG6;
	  Fl_Round_Button* inG7;
	  Fl_Round_Button* inG8;
	  
	  Fl_Check_Button* ckWeather;
	  Fl_Input* inName;
	  Fl_Int_Input* inWeight;
      Fl_Float_Input* inBC;
	  Fl_Float_Input* inSH;
	  Fl_Int_Input* inMV;
      Fl_Int_Input* inAngle;
	  Fl_Int_Input* inZero;
      Fl_Int_Input* inVwind;
	  Fl_Int_Input* inAwind;
	  Fl_Int_Input* inTemp; Fl_Float_Input* inPressure;
	  Fl_Int_Input* inHumidity;
	  Fl_Int_Input* inAltitude;
	  
	  Fl_Menu_Bar *menu;
	  Fl_Menu_Item *pmenuitems;
	  
	  // Menu Item Declarations.
	  		Fl_Menu_Item m_File;
			Fl_Menu_Item mc_New; 
			Fl_Menu_Item mc_Open; 
			Fl_Menu_Item mc_Save; 
			Fl_Menu_Item mc_Export; 
			Fl_Menu_Item mc_Quit; 
		   
		  Fl_Menu_Item m_Edit;
			Fl_Menu_Item mc_Copy;
			Fl_Menu_Item mc_Paste;
			Fl_Menu_Item mc_Options; 
	   
	   	  Fl_Menu_Item m_Analyze; 
			Fl_Menu_Item mc_Table;
			Fl_Menu_Item mc_Plot; 
			Fl_Menu_Item mc_Compare;
			
	   	  Fl_Menu_Item m_Aids;
			Fl_Menu_Item mc_RangeCard; 
			Fl_Menu_Item mc_ClickChart; 
			Fl_Menu_Item mc_RangeTable;
			
		  Fl_Menu_Item m_Solution; 
			Fl_Menu_Item mc_Solve;
			Fl_Menu_Item mc_Data;
			Fl_Menu_Item mc_Advanced;
	   
	      Fl_Menu_Item m_Help; 
			Fl_Menu_Item mc_Help;
			Fl_Menu_Item mc_License;
			Fl_Menu_Item mc_About;
			Fl_Menu_Item mc_Bug;
			
	  Fl_Multiline_Output* oStatus;
	  
	  // Fl_Menu_Item mc_Store1,mc_Store2, mc_Store3, mc_Store4, mc_Store5;
		  
	  // FL Outputs
      Fl_Output* out;
    
   private:
      static void cb_Solve(Fl_Widget*, void*);
      //inline void cb_copy_i();
      //static void cb_quit(Fl_Widget*, void*);
      //inline void cb_quit_i();

	  static void cb_Quit(Fl_Widget*, void*);
	  static void cb_Nothing(Fl_Widget*, void*);
	  static void cb_ckWeather(Fl_Widget*, void*);
	  static void cb_Clear(Fl_Widget*, void*);
	  static void cb_mNew(Fl_Widget*, void*);
	  static void EnableMenu(void*);
	  static void DisableMenu(void*);
	  static void cb_RangeTable(Fl_Widget*, void* );
	  static void cb_Paste(Fl_Widget*, void* v);
	  static void cb_Copy(Fl_Widget*, void* v);
	  static void cb_HelpWindow(Fl_Widget*, void*v);
	  static void cb_LicenseWindow(Fl_Widget*, void*v);
	  static void cb_AboutWindow(Fl_Widget*,void*v);
	  static void cb_Save(Fl_Widget*, void*);
	  static void cb_Open(Fl_Widget*, void*);
	  static void cb_Plot(Fl_Widget*, void*);
	  static void cb_Options(Fl_Widget*, void*);
	  
	  static void cb_Store1(Fl_Widget*, void*);
	  static void cb_Store2(Fl_Widget*, void*);
	  static void cb_Store3(Fl_Widget*, void*);
	  static void cb_Store4(Fl_Widget*, void*);
	  static void cb_Store5(Fl_Widget*, void*);
	  static void cb_Store6(Fl_Widget*, void*);
	  static void cb_Store7(Fl_Widget*, void*);
	  static void cb_Store8(Fl_Widget*, void*);	  
	  
	  
	  static void cb_PBR(Fl_Widget*, void*);
	  
	  
	  void Paste(void);
	  void Copy(void);
	  
	  


};


#include "PBRWindow.h"

#endif

