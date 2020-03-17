#include "lib/ballistics/ballistics.h"
#include "InputWindow.h"


InputWindow::InputWindow(int w, int h, const char* title):Fl_Window(w,h,title){
 
	// Solution pointers. Must be initialized to NULL so we don't accidentally try to free them later. 
	gsln=NULL;
	mem1=NULL;
	mem2=NULL;
	mem3=NULL;
	mem4=NULL;
	mem5=NULL;
	mem6=NULL;
	mem7=NULL;
	mem8=NULL;

	// A (double*) pointer to the actual solution data.  Initialize to 
	// Null so we can make sure to free any memory it has after a solution
	// is closed.
	Solution=NULL;
	Smem1=NULL;
	Smem2=NULL;
	Smem3=NULL;
	Smem4=NULL;
	Smem5=NULL;
	Smem6=NULL;
	Smem7=NULL;
	Smem8=NULL;

	
	// Start drawing the widgets on screen.
   begin();
	
	// Draw any general purpose borders or labels.
	  Fl_Box* GBox = new Fl_Box(250,80,140,95,"Drag Function");
	  GBox->box(FL_BORDER_BOX);
	  GBox->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE);   
   
	// Create and place the Drag Function inputs on the form.
	  inG1 = new Fl_Round_Button(270,105,30,15,"G1");
	  inG2 = new Fl_Round_Button(270,125,30,15,"G2");
	  inG5 = new Fl_Round_Button(270,145,30,15,"G5");
	  inG6 = new Fl_Round_Button(330,105,30,15,"G6");
	  inG7 = new Fl_Round_Button(330,125,30,15,"G7");
	  inG8 = new Fl_Round_Button(330,145,30,15,"G8");
	  
	  inG1->type(FL_RADIO_BUTTON);
	  inG2->type(FL_RADIO_BUTTON);
	  inG5->type(FL_RADIO_BUTTON);
	  inG6->type(FL_RADIO_BUTTON);
	  inG7->type(FL_RADIO_BUTTON);
	  inG8->type(FL_RADIO_BUTTON);
	  
	  inG1->set();
	  
	
	
	// Create and place the input fields on the form.
	  inName = new Fl_Input(55,45,335,20,"Name  ");
	  inBC = new Fl_Float_Input(10,80,50,20," Drag Coefficient");
	  inWeight=new Fl_Int_Input(10,105,50,20," Projectile Weight (grains)");
	  inMV = new Fl_Int_Input(10,130,50,20," Initial Velocity (ft/s)");
	  inZero = new Fl_Int_Input(10,155,50,20," Zero Range (yds)");
	  inSH = new Fl_Float_Input(10,180,50,20," Sight Height Over Bore (in)");
	  inAngle = new Fl_Int_Input(10,205,50,20," Shooting Angle (deg)");
	  inVwind = new Fl_Int_Input(10,230,50,20," Wind Velocity (mi/hr)");
	  inAwind = new Fl_Int_Input(10,255,50,20," Wind Angle (0-360 deg)");
	  
	  inName->align(FL_ALIGN_LEFT);
	  inBC->align(FL_ALIGN_RIGHT);
	  inWeight->align(FL_ALIGN_RIGHT);
	  inMV->align(FL_ALIGN_RIGHT);
	  inZero->align(FL_ALIGN_RIGHT);
	  inAngle->align(FL_ALIGN_RIGHT);
	  inVwind->align(FL_ALIGN_RIGHT);
	  inAwind->align(FL_ALIGN_RIGHT);
	  inSH->align(FL_ALIGN_RIGHT);
	  
	  inName->value("308 Win Match, 168gr Sierra Match King");
	  inBC->value("0.465");
	  inWeight->value("168");
	  inMV->value("2650");
	  inZero->value("200");
	  inAngle->value("0");
	  inVwind->value("0");
	  inAwind->value("0");
	  inSH->value("1.6");
	  
	  
	// Put a checkbox to enable or disable atmospheric corrections.
	  ckWeather = new Fl_Check_Button(10,290,20,20,"Enable Atmospheric Corrections");
	  ckWeather->align(FL_ALIGN_RIGHT);
	  ckWeather->callback(cb_ckWeather,this);
	  
	// Now the atmospheric inputs.
	  inAltitude = new Fl_Int_Input(10,315,50,20," Altitude (ft)");
	  inTemp = new Fl_Int_Input(10,340,50,20," Temperature (F)");
	  inPressure = new Fl_Float_Input(10,365,50,20," Barometric Pressure (in Hg)");
	  inHumidity = new Fl_Int_Input(10,390,50,20," Relative Humidity (%)");
		
	  inTemp->align(FL_ALIGN_RIGHT);
	  inPressure->align(FL_ALIGN_RIGHT);
	  inAltitude->align(FL_ALIGN_RIGHT);
	  inHumidity->align(FL_ALIGN_RIGHT);
	  
	// Set the default to standard atmosphere.
	  inTemp->value("59");
	  inAltitude->value("0");
	  inPressure->value("29.53");
	  inHumidity->value("78");
	  

	// Disable the atmospheric conditions at first since many users may not want to use it.
	  ckWeather->activate();
	  inTemp->deactivate();
	  inAltitude->deactivate();
	  inPressure->deactivate();
	  inHumidity->deactivate();
		
	// Initialize the output window display.
	  oStatus = new Fl_Multiline_Output(10,420,380,70,"Solution Status");
	  oStatus->align(FL_ALIGN_LEFT | FL_ALIGN_TOP |  FL_ALIGN_INSIDE);
	  oStatus->value("GNU Ballistic Computer:  Waiting for input.");
	
	// Create and place buttons on the form.
      btSolve = new Fl_Button( 280, 380, 50, 30, "So&lve");
	  btReset = new Fl_Button( 340, 380, 50, 30, "&Reset");
	  
      btSolve->callback(cb_Solve,this);
	  btReset->callback(cb_Clear, this);

	  
	  // Declare the menu bar variables
	  // m for menu, mc for menu choice.
	  menu = new Fl_Menu_Bar(0,0,400,30, "MENU");
	  
	  
		  Fl_Menu_Item m_File = {"&File",0,0,0,FL_SUBMENU};
			Fl_Menu_Item mc_New = {"&New...",0,(Fl_Callback*)cb_mNew,this};
			Fl_Menu_Item mc_Open = {"&Open...",0,(Fl_Callback*)cb_Open,this};
			Fl_Menu_Item mc_Save = {"&Save...",0,(Fl_Callback*)cb_Save,this};
			Fl_Menu_Item mc_AdvExport = {"E&xport Data..",0,(Fl_Callback*)cb_Nothing,this};
			Fl_Menu_Item mc_Quit = {"&Quit",0,(Fl_Callback*)cb_Quit,this};
	   
		  Fl_Menu_Item m_Analysis = {"&Analysis",0,0,0, FL_SUBMENU};
			Fl_Menu_Item mc_RangeTable = {"&Range Table",0,(Fl_Callback*)cb_RangeTable,this};
			Fl_Menu_Item mc_QuickPlot = {"Quick &Plot",0,(Fl_Callback*)cb_Plot, this};
			Fl_Menu_Item mc_RangeCard = {"&Range Card",0,(Fl_Callback*)cb_Nothing};
			Fl_Menu_Item mc_ClickChart = {"&Click Chart",0,(Fl_Callback*)cb_Nothing,this,FL_MENU_DIVIDER};
			Fl_Menu_Item mc_MVA = {"&Advanced Analysis",0,(Fl_Callback*)cb_Nothing,this};


		  Fl_Menu_Item  m_Tools = {"&Tools",0,0,0, FL_SUBMENU};
			Fl_Menu_Item mc_OptimizePBR = {"&Optimize PBR",0,(Fl_Callback*)cb_PBR,this};
			Fl_Menu_Item mc_CalcBC = {"&Calculate Drag Coefficient",0,(Fl_Callback*)cb_Nothing,this,FL_MENU_DIVIDER};	
		    Fl_Menu_Item mc_Options = {"&Options",0,(Fl_Callback*)cb_Options,this};
	
		  Fl_Menu_Item  m_Storage = {"&Solution Memory",0,0,0, FL_SUBMENU};
			Fl_Menu_Item mc_Store1 = {"Store to Memory 1 ",0,(Fl_Callback*)cb_Store1,this};
			Fl_Menu_Item mc_Store2 = {"Store to Memory 2 ",0,(Fl_Callback*)cb_Store2,this,FL_MENU_DIVIDER};
			Fl_Menu_Item mc_Store3 = {"Store to Memory 3 ",0,(Fl_Callback*)cb_Store3,this};
			Fl_Menu_Item mc_Store4 = {"Store to Memory 4 ",0,(Fl_Callback*)cb_Store4,this};
			Fl_Menu_Item mc_Store5 = {"Store to Memory 5 ",0,(Fl_Callback*)cb_Store5,this};
			Fl_Menu_Item mc_Store6 = {"Store to Memory 6 ",0,(Fl_Callback*)cb_Store6,this};
			Fl_Menu_Item mc_Store7 = {"Store to Memory 7 ",0,(Fl_Callback*)cb_Store7,this};
			Fl_Menu_Item mc_Store8 = {"Store to Memory 8 ",0,(Fl_Callback*)cb_Store8,this};

	      Fl_Menu_Item m_Help = {"&Help",0,0,0, FL_SUBMENU};
			Fl_Menu_Item mc_Help = {"&Help",0,(Fl_Callback*)cb_HelpWindow};
			Fl_Menu_Item mc_License = {"&License",0,(Fl_Callback*)cb_LicenseWindow};
			Fl_Menu_Item mc_About = {"&About",0,(Fl_Callback*)cb_AboutWindow};


	   // Set initial condition for menu items.
	  m_Analyze.deactivate();
	  m_Aids.deactivate();
	  
	  //mc_Export.deactivate(); // Data exports from the RangeTable window.
	  
	  // These are planned features, but haven't been done yet.
	  ////// TODO //////
	  m_Storage.deactivate();
	  mc_RangeCard.deactivate();
	  mc_ClickChart.deactivate();
	  mc_CalcBC.deactivate();
	  m_Solution.deactivate();
	  mc_MVA.deactivate();
	  mc_AdvExport.deactivate();
	  m_Analysis.deactivate();
	  mc_Options.deactivate();


	#define MANALYSIS 7
	#define MSTORAGE 19
	Fl_Menu_Item menuitems[] = {
	  m_File, //0
		mc_New,
		mc_Open,
		mc_Save,
		mc_AdvExport,
		mc_Quit,
		{0},//6
	  m_Analysis, //7
		mc_RangeTable,
		mc_QuickPlot,
		mc_RangeCard,
		mc_ClickChart,
	    mc_MVA, 
		{0}, // 13
	  m_Tools,
	    mc_OptimizePBR,
		mc_CalcBC,
		mc_Options,
		{0},
	  m_Storage, 
	    mc_Store1, //20
		mc_Store2, //21
		mc_Store3,
		mc_Store4,
		mc_Store5,
		mc_Store6,
		mc_Store7,
		mc_Store8,
		{0},
	  m_Help, //23
		mc_Help,
		mc_License,
		mc_About,
		{0}, //27
	  {0}
	  };
		
		
	menu->copy(menuitems);

   end();
   show();
}


InputWindow::~InputWindow(){

	// Free any memory we are using.
	if (Solution!=NULL) {free(Solution);Solution=NULL;}
	if (Smem1!=NULL) {  free(Smem1);  Smem1=NULL; }
	if (Smem2!=NULL) {  free(Smem2);  Smem2=NULL; }
	if (Smem3!=NULL) {  free(Smem3);  Smem3=NULL; }
	if (Smem4!=NULL) {  free(Smem4);  Smem4=NULL; }
	if (Smem5!=NULL) {  free(Smem5);  Smem5=NULL; }
	if (Smem6!=NULL) {  free(Smem6);  Smem6=NULL; }
	if (Smem7!=NULL) {  free(Smem7);  Smem7=NULL; }
	if (Smem8!=NULL) {  free(Smem8);  Smem8=NULL; }

	if (gsln!=NULL)  {  gsln->~GBCSolution(); delete gsln;  gsln=NULL;}
	if (mem1!=NULL) { mem1->~GBCSolution(); delete mem1;  mem1=NULL;}
	if (mem2!=NULL) { mem2->~GBCSolution(); delete mem2;  mem2=NULL;}
	if (mem3!=NULL) { mem3->~GBCSolution(); delete mem3;  mem3=NULL;}
	if (mem4!=NULL) { mem4->~GBCSolution(); delete mem4;  mem4=NULL;}
	if (mem5!=NULL) { mem5->~GBCSolution(); delete mem5;  mem5=NULL;}
	if (mem6!=NULL) { mem6->~GBCSolution(); delete mem6;  mem6=NULL;}
	if (mem7!=NULL) { mem7->~GBCSolution(); delete mem7;  mem7=NULL;}
	if (mem8!=NULL) { mem8->~GBCSolution(); delete mem8;  mem8=NULL;}

	delete inG1;
	delete inG2;
	delete inG5;
	delete inG6;
	delete inG7;
	delete inG8;
	delete inName; 
	delete inBC; 
	delete inWeight;
	delete inMV;
	delete inZero; 
	delete inSH;
	delete inAngle; 
	delete inVwind; 
	delete inAwind;	  
	delete ckWeather;
	delete inAltitude;
	delete inTemp;
	delete inPressure;
	delete inHumidity;
	delete oStatus;
	delete btSolve;
	delete btReset;
	
}
void InputWindow::cb_ckWeather(Fl_Widget* o, void* v){
	InputWindow* T=(InputWindow*)v;
	
	if (T->ckWeather->value()==1){
		T->inTemp->activate();
		T->inPressure->activate();
		T->inAltitude->activate();
		T->inHumidity->activate();
	}
	else if (T->ckWeather->value()==0){
		T->inTemp->deactivate();
		T->inPressure->deactivate();
		T->inAltitude->deactivate();
		T->inHumidity->deactivate();
	}
	
}


void InputWindow::cb_Solve(Fl_Widget* o, void* vd) {

	// If we have an old solution, clean up its memory areas now.
	// Disable the analysis menu.
	DisableMenu(vd);
	InputWindow* T = (InputWindow*)vd;
	
	// Make some local pointers to use in the function, before we assign the solution
	// to the currently selected "working" solution on the UI.
	GBCSolution* lsln = NULL;
	double* lSolution = NULL;

	double bc=-1; 		// The ballistic coefficient for the projectile.
	double v=-1; 		// Intial velocity, in ft/s
	double sh=-1; 		// The Sight height over bore, in inches.
	double angle=-1; 	// The shooting angle (uphill / downhill), in degrees.
	double zero=-1; 	// The zero range of the rifle, in yards.
	double windspeed=-1; // The wind speed in miles per hour.
	double windangle=-1; // The wind angle (0=headwind, 90=right to left, 180=tailwind, 270/-90=left to right)
	int df=0;
	int numRows=0;
	char txt1[1024];

	double zeroangle=-1; // The bore / sight angle.
	
	bc = atof(T->inBC->value());
	v = atof(T->inMV->value());
	sh = atof(T->inSH->value());
	angle=atof(T->inAngle->value());
	zero=atof(T->inZero->value());
	windspeed=atof(T->inVwind->value());
	windangle=atof(T->inAwind->value());
	
	if (T->inG1->value()==1) df=G1;
	else if (T->inG2->value()==1) df=G2;
	else if (T->inG5->value()==1) df=G5;
	else if (T->inG6->value()==1) df=G6;
	else if (T->inG7->value()==1) df=G7;
	else if (T->inG8->value()==1) df=G8;
	
	// If the Enable Weather checkbox is activated, we correct the 
	// Ballistic coefficient for the non-std weather conditions.
	if (T->ckWeather->value()==1){
	
		double Altitude = atof(T->inAltitude->value());
		double Barometer = atof(T->inPressure->value());
		double Temperature = atof(T->inTemp->value());
		double RH = atof(T->inHumidity->value())/100;
	
		bc = AtmCorrect(bc, Altitude,Barometer,Temperature,RH);
	}
		
	// Find the zero angle of the bore relative to the sighting system.
	zeroangle=ZeroAngle(df,bc,v,sh,zero,0);
	
	// Generate a solution using the GNU Ballistics library call.
	numRows = SolveAll(df,bc,v,sh,angle,zeroangle,windspeed,windangle,&(lSolution));
	
	// If we get a valid solution, store it as a new GBC Solution.
	// T->gsln was cleaned up at the beginning of this function,
	// so we know it points to NULL already.
	if (numRows>0 && lSolution!=NULL){ // Solution is valid with no errors.
		
		// Store some general data about the load and weather conditions in the 2048 extra bytes.
		lsln = new GBCSolution(
							lSolution, 
							T->inName->value(),
							atof(T->inBC->value()), 
							atof(T->inSH->value()), 
							atoi(T->inWeight->value()), 
							atoi(T->inMV->value()), 
							atoi(T->inAngle->value()), 
							atoi(T->inZero->value()), 
							atoi(T->inVwind->value()),
							atoi(T->inAwind->value()), 
							atoi(T->inTemp->value()),
							atoi(T->inHumidity->value()), 
							atof(T->inPressure->value()), 
							atoi(T->inAltitude->value()),
							(int)lSolution[10*__BCOMP_MAXRANGE__+1],
							T->ckWeather->value(),
							df
							);
		
		// Update the maximum valid range of the solution.
		// We do this separately because I'm too lazy to update the GBCSolution() constructor,
		// but we need this data to pass into the RangeWindow for maximum distances.
		lsln->MaxRows(numRows);
		
		if (lsln == NULL){
		// Inform the user of our failure.
		sprintf(txt1,"GNU Ballistics Computer:  Solution failed!  Unknown error.");
		T->oStatus->value(txt1);
		return;
		}
	
		// Otherwise we assume success, and inform the user of our success.
		sprintf(txt1,"GNU Ballistics Computer:  Solution Valid.\nSolution's maximum valid range is %d yards\nUse the options in the 'Solution' menu to view results.",numRows-2);
		T->oStatus->value(txt1);
		EnableMenu((InputWindow*) vd);	
		
		// Now we need to figure out which solution the user wanted to store this as, and
		// assign our local pointers to the user's requested solution.
		if (T->gsln != NULL){
			//if (T->gsln->sln != NULL) printf("398: T->gsln->sln = %p",T->gsln->sln); //free(T->gsln->sln);
			delete(T->gsln);
		}

		T->gsln=lsln;
		
		

	
	}

	return;	

}

void InputWindow::EnableMenu(void* v){
		InputWindow* T = (InputWindow*)v;
		
		// Enable the Analyze menu.  This function is a crude way to do it, but it works.
		// I'm looking for a better way to do this, but have yet to find one.
		Fl_Menu_Item* mp;
		mp=(Fl_Menu_Item*)&(T->menu->menu()[MANALYSIS]);
		mp->activate();
		mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE]);
		mp->activate();		
		
		T->redraw();
}

void InputWindow::DisableMenu(void* v){
		InputWindow* T = (InputWindow*)v;
		// Disable the Analyze menu.  This function is a crude way to do it.
		Fl_Menu_Item* mp;
		mp=(Fl_Menu_Item*)&(T->menu->menu()[MANALYSIS]);
		mp->deactivate();
		mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE]);
		mp->deactivate();		
		
		T->redraw();
}


// While I get the program running, some buttons will
// do nothing.  For that they get to point to this callback.
void InputWindow::cb_Nothing(Fl_Widget*, void* v) {
	;
}
void InputWindow::cb_Quit(Fl_Widget*, void* v) {

	InputWindow* T = (InputWindow*)v;
	T->~InputWindow();
}


// We seem to have a memory leak here??
// When the use clicks "solve" and "reset" and "solve" again, there is an extra 460 kb lost.
void InputWindow::cb_Clear(Fl_Widget* o, void* v) {

	  InputWindow* T=(InputWindow*)v;
	  
	  // When the "Reset" button is clicked, we want to 
	  // clear out the old solution and restore std. conditions.
	  DisableMenu(v);
	  T->inBC->value("0.465");
	  T->inMV->value("2650");
	  T->inZero->value("200");
	  T->inAngle->value("0");
	  T->inSH->value("1.6");
	  T->inVwind->value("0");
	  T->inAwind->value("0");
	  T->inTemp->value("59");
	  T->inWeight->value("168");
	  T->inAltitude->value("0");
	  T->inPressure->value("29.53");
	  T->inHumidity->value("78");
	  T->inName->value("308 Win Match, 168gr Sierra Match King");
	  T->ckWeather->clear();
	  T->cb_ckWeather(o,v);
	  T->oStatus->value("GNU Ballistic Computer:  Solution Reset");
	  
	  T->inG1->value(1);
	  T->inG2->value(0);
	  T->inG5->value(0);
	  T->inG6->value(0);
	  T->inG7->value(0);
	  T->inG8->value(0);
	  
 
	  // Free up any memory we have been using.
	  
	  	// Free any memory we are using.
	if (T->Solution!=NULL) {free(T->Solution); T->Solution=NULL;}
	if (T->Smem1!=NULL) {  free(T->Smem1);  T->Smem1=NULL; }
	if (T->Smem2!=NULL) {  free(T->Smem2);  T->Smem2=NULL; }
	if (T->Smem3!=NULL) {  free(T->Smem3);  T->Smem3=NULL; }
	if (T->Smem4!=NULL) {  free(T->Smem4);  T->Smem4=NULL; }
	if (T->Smem5!=NULL) {  free(T->Smem5);  T->Smem5=NULL; }
	if (T->Smem6!=NULL) {  free(T->Smem6);  T->Smem6=NULL; }
	if (T->Smem7!=NULL) {  free(T->Smem7);  T->Smem7=NULL; }
	if (T->Smem8!=NULL) {  free(T->Smem8);  T->Smem8=NULL; }

	if (T->gsln!=NULL)  {  T->gsln->~GBCSolution(); delete T->gsln;  T->gsln=NULL;}
	if (T->mem1!=NULL) { T->mem1->~GBCSolution(); delete T->mem1;  T->mem1=NULL;}
	if (T->mem2!=NULL) { T->mem2->~GBCSolution(); delete T->mem2;  T->mem2=NULL;}
	if (T->mem3!=NULL) { T->mem3->~GBCSolution(); delete T->mem3;  T->mem3=NULL;}
	if (T->mem4!=NULL) { T->mem4->~GBCSolution(); delete T->mem4;  T->mem4=NULL;}
	if (T->mem5!=NULL) { T->mem5->~GBCSolution(); delete T->mem5;  T->mem5=NULL;}
	if (T->mem6!=NULL) { T->mem6->~GBCSolution(); delete T->mem6;  T->mem6=NULL;}
	if (T->mem7!=NULL) { T->mem7->~GBCSolution(); delete T->mem7;  T->mem7=NULL;}
	if (T->mem8!=NULL) { T->mem8->~GBCSolution(); delete T->mem8;  T->mem8=NULL;}
	

	// Turn the memory labels back to black to show they are null.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+1]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+2]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+3]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+4]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+5]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+6]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+7]);  mp->labelcolor(FL_BLACK);
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+8]);  mp->labelcolor(FL_BLACK);	

}

// Using the "New" Menu button does the same thing as the reset button.
void InputWindow::cb_mNew(Fl_Widget* o, void* v){
    DisableMenu(v);
	InputWindow* T=(InputWindow*)v;
	T->cb_Clear(o,v);
	T->oStatus->value("GNU Ballistic Computer:  Solution cleared. \nNow begin new Solution");
}

// Don't remember where these came from... I'm not sure they're used at all in fact.
// I've commented them out for now.  The next release will have them removed if no
// break-downs are found due to removal.  I think they are from when I first started
// making the program, but didn't really know what to do yet for FLTK.
//void InputWindow::cb_quit(Fl_Widget* o, void* v) {
//   ( (InputWindow*)v )->cb_quit_i();
//}
//void InputWindow::cb_quit_i() {
//    hide();
//}
//void InputWindow::cb_mOpen(Fl_Widget* o, void* v) {
//	DisableMenu(v);
//   InputWindow* T = (InputWindow*)v;
//	T->oStatus->value("Select file to load...");	
//}


void InputWindow::cb_RangeTable(Fl_Widget*, void* v){
	InputWindow* T = (InputWindow*)v;
	
	// Spawn a new RangeWindow using the current solution to populate it.
	if (T->gsln != NULL && T->gsln->sln != NULL){
		new RangeWindow(600,400,T->gsln);
	}
	else {
		T->oStatus->value("Error creating Range Window.  Unknown Error.");
	}
	
}		

// Implement the paste function.  This I did actually figure out how to do.
// The Cut function isn't working yet, but Ctrl-C and Ctrl-V are fortunately
// built into FLTK, so they both work.
void InputWindow::cb_Paste(Fl_Widget* r, void* v){
	InputWindow* T = (InputWindow*)v;
	T->Paste();		
}

void InputWindow::cb_Copy(Fl_Widget* r, void* v){
	InputWindow* T = (InputWindow*)v;
	T->Copy();
}

void InputWindow::Paste(){
	Fl::paste(*this,1);
}
	
void InputWindow::Copy(){
	

	//int len = Fl::event_length();
	//Fl::copy(Fl::event_text(),len,1);
	//printf("\nLENGTH: %d		Text: %s",len,Fl::event_text());
	
	// 2nd try?
	// this->handle(FL_CTRL + 'c');
	
	// Can't figure out how to do this??  very strange...
	
}

void InputWindow::cb_HelpWindow(Fl_Widget* r, void* vtt){
	// Spawn a new help window, this will show whatever is in HelpFile.html
	Fl_Help_Dialog* hd = new Fl_Help_Dialog();
	hd->load("HelpFile.html");
	hd->show();
}

void InputWindow::cb_LicenseWindow(Fl_Widget* r, void*v){

	// The LicenseWindow has all the license hard-coded into it, 
	// so we just need to spawn it and forget about it.
	// I don't think this will make a memory leak since it will
	// call it's own destructor when it exits.
	new LicenseWindow();
}

void InputWindow::cb_AboutWindow(Fl_Widget*,void*v){
	// Spawn a new AboutWindow.
	new AboutWindow();
	
}

void InputWindow::cb_Save(Fl_Widget*, void*v){

	// If we save the whole solution its like 4mb, so just save the parameters
	// and we can regenerate the solution after we reload the parameters.
	InputWindow* T = (InputWindow*)v;
		
	if (T->gsln != NULL){



		// Get the file name we want to save the data as, using the FLTK standard file picker function.
		char* fname=NULL;
		fname = fl_file_chooser("Please select a location to save the data.","*.gbc","OutputData.gbc",0);
		if (fname==NULL) return;
		
		// Open the file for binary writing.
		FILE* ofile = fopen(fname,"wb");
		
		// Write the current solution class to the file, then close it.
		int wrote;
		wrote=fwrite(T->gsln,1,sizeof(GBCSolution),ofile);
		fclose(ofile);

		// Update the status display to show our success.

		if (wrote == sizeof(GBCSolution)){
			T->oStatus->value("GNU Ballistic Computer:  File saved successfully.");
		}
		else {
			T->oStatus->value("GNU Ballistic Computer:  There was an error writing the file.  Please check the path and try again.");
		}
	}
	else {
		T->oStatus->value("GNU Ballistic Computer:  Can not save, since the solution \nis not yet created.  Please create the solution using the \n'Solve' button before you save it.");
	}
	
	return;

}


void InputWindow::cb_Open(Fl_Widget* o, void*v){

	
	InputWindow* T = (InputWindow*)v;
	
	// A solution for locally working with gsln (hence the name lsln)
	GBCSolution* lsln = NULL;
	
	// Get the file name to load 
	char* fname=NULL;
	fname = fl_file_chooser("Please select a file to open.","*.gbc","",0);
	if (fname==NULL) {
		T->oStatus->value("GNU Ballistic Computer:  Error opening file!");
		return;
	}
	
	// and open it for binary read.
	FILE* ofile = fopen(fname,"rb");
	

	
	// Free an old solution if there is one, because we want to
	// write over it.
	if (T->gsln != NULL) {
		free(T->gsln);
		T->gsln=NULL;
	}
	
	// Allocate some space for the loaded solution.
	if (lsln != NULL) {
		free(lsln);
		lsln=NULL;
	}

	// Assign some new memory for lsln.
	lsln = (GBCSolution*)malloc(sizeof(GBCSolution));
	
	if (lsln==NULL){
		T->oStatus->value("GNU Ballistic Computer:  Memory allocation error.");
		return;
	}
	
	// Read the GBCSolution class from the file and load it into program
	// memory.  check to make sure we read the right amount of data.
	int read;	
	read=fread(lsln,1,sizeof(GBCSolution),ofile);
	fclose(ofile);
	
	if (read!=sizeof(GBCSolution)){
		T->oStatus->value("GNU Ballistic Computer:  Error reading file data!\nPlease ensure the file is a valid GNUBC solution.");
		if (lsln!=NULL) {
			free(lsln);
			lsln=NULL;
		}

		if (T->gsln!=NULL) {
			free(T->gsln);
			T->gsln=NULL;
		}
		
		return;
	}
			

	// Set the parent's solution pointer to the local one we just imported.
	T->gsln=lsln;

	// Load the solution data into the INPUT fields in the InputWindow.
	char buff[1024];
	T->inName->value(lsln->Name());
	sprintf(buff,"%.3f",lsln->BC());  T->inBC->value(buff);
	sprintf(buff,"%.2f",lsln->SightHeight());  T->inSH->value(buff);
	sprintf(buff,"%d",lsln->MuzzleVelocity());  T->inMV->value(buff);
	sprintf(buff,"%d",lsln->ShootingAngle());  T->inAngle->value(buff);
	sprintf(buff,"%d",lsln->ZeroRange());  T->inZero->value(buff);
	sprintf(buff,"%d",lsln->WindSpeed());  T->inVwind->value(buff);
	sprintf(buff,"%d",lsln->WindAngle());  T->inAwind->value(buff);
	sprintf(buff,"%d",lsln->Temp());  T->inTemp->value(buff);
	sprintf(buff,"%.2f",lsln->Pressure());  T->inPressure->value(buff);
	sprintf(buff,"%d",lsln->Humidity());  T->inHumidity->value(buff);
	sprintf(buff,"%d",lsln->Altitude());  T->inAltitude->value(buff);	
	sprintf(buff,"%d",lsln->Weight());  T->inWeight->value(buff);
	T->ckWeather->value(lsln->UseWeather());
	T->cb_ckWeather(o,v);
	
	T->inG1->value(0);
	T->inG2->value(0);
	T->inG5->value(0);
	T->inG6->value(0);
	T->inG7->value(0);
	T->inG8->value(0);
	
	
	if (lsln->df==G1) T->inG1->set();
	if (lsln->df==G2) T->inG2->set();
	if (lsln->df==G5) T->inG5->set();
	if (lsln->df==G6) T->inG6->set();
	if (lsln->df==G7) T->inG7->set();
	if (lsln->df==G8) T->inG8->set();

	// We don't actually have a valid solution, so make sure to set the solution
	// double* pointer to NULL before we call solve.
	T->gsln->sln=NULL;
	
	// And now solve the loaded parameters.
	DisableMenu(T);
	T->cb_Solve(o,v);
	
	// Finally inform the user of our success.
	T->oStatus->value("GNU Ballistics Computer:  File loaded successfully.");
	
	return;

}


void InputWindow::cb_Plot(Fl_Widget* o, void* v){

	InputWindow* T = (InputWindow*)v;

	GBCSolution* sln = T->gsln;
	GBCSolution* lmem1 = T->mem1;
	GBCSolution* lmem2 = T->mem2;
	
	new PlotWindow(700,500,sln,lmem1,lmem2);

}


void InputWindow::cb_Store1(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 1");
	
	if (T->mem1 != NULL) {
		if (T->mem1->sln!=NULL) free (T->mem1->sln);
		delete(T->mem1);
	}
	
	if (T->Smem1!= NULL) free(T->Smem1);
	
	T->mem1=T->gsln;
	T->Smem1=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label red to show it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+1]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}
void InputWindow::cb_Store2(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 2");

	if (T->mem2 != NULL) {
		if (T->mem2->sln!=NULL) free(T->mem2->sln);
		delete(T->mem2);
	}
	
	if (T->Smem2!= NULL) free(T->Smem2);
	
	T->mem2=T->gsln;
	T->Smem2=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+2]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}

void InputWindow::cb_Store3(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 3");

	if (T->mem3 != NULL) {
		if (T->mem3->sln!=NULL) free(T->mem3->sln);
		delete(T->mem3);
	}
	
	if (T->Smem3!= NULL) free(T->Smem3);
	
	T->mem3=T->gsln;
	T->Smem3=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+3]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}

void InputWindow::cb_Store4(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 4");

	if (T->mem4 != NULL) {
		if (T->mem4->sln!=NULL) free(T->mem4->sln);
		delete(T->mem4);
	}
	
	if (T->Smem4!= NULL) free(T->Smem4);
	
	T->mem4=T->gsln;
	T->Smem4=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+4]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}

void InputWindow::cb_Store5(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 5");

	if (T->mem5 != NULL) {
		if (T->mem5->sln!=NULL) free(T->mem5->sln);
		delete(T->mem5);
	}
	
	if (T->Smem5!= NULL) free(T->Smem5);
	
	T->mem5=T->gsln;
	T->Smem5=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+5]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}

void InputWindow::cb_Store6(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 6");

	if (T->mem6 != NULL) {
		if (T->mem6->sln!=NULL) free(T->mem6->sln);
		delete(T->mem6);
	}
	
	if (T->Smem6!= NULL) free(T->Smem6);
	
	T->mem6=T->gsln;
	T->Smem6=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+6]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}

void InputWindow::cb_Store7(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 7");

	if (T->mem7 != NULL) {
		if (T->mem7->sln!=NULL) free(T->mem7->sln);
		delete(T->mem7);
	}
	
	if (T->Smem7!= NULL) free(T->Smem7);
	
	T->mem7=T->gsln;
	T->Smem7=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+7]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}

void InputWindow::cb_Store8(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	T->oStatus->value("GNU Ballistics Computer: Stored to Memory 8");

	if (T->mem8 != NULL) {
		if (T->mem8->sln!=NULL) free(T->mem8->sln);
		delete(T->mem8);
	}
	
	if (T->Smem8!= NULL) free(T->Smem8);
	
	T->mem8=T->gsln;
	T->Smem8=T->Solution;
	
	T->gsln=NULL;
	T->Solution=NULL;
	
	// Turn the label blue to indicate it's been used.
	Fl_Menu_Item* mp;
	mp=(Fl_Menu_Item*)&(T->menu->menu()[MSTORAGE+8]);
	mp->labelcolor(FL_RED);
	
	cb_Solve(o,v);

}


void InputWindow::cb_PBR(Fl_Widget* o, void* v){
	InputWindow* T = (InputWindow*)v;
	new PBRWindow(T);

}

void InputWindow::cb_Options(Fl_Widget* o, void* v){
	//InputWindow* T = (InputWindow*)v;
	//new OptionsWindow();
}

