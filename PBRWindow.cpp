#include "PBRWindow.h"

PBRWindow::PBRWindow(InputWindow* IW):Fl_Window(250,300,"Optimize Point Blank Range") {

	inW = IW;
	if (inW==NULL) return;
	
	begin();

	CD = new Fl_Float_Input(30,20,60,25,"Drag Coefficient");
	VI = new Fl_Int_Input(30,50,60,25,"Initial Velocity (ft/s)");
	SH = new Fl_Float_Input(30,80,60,25,"Sight Height (inches)");
	VitalSize = new Fl_Float_Input(30,110,60,25,"Vital Zone Size (inches)");
	
	VI->value(IW->inMV->value());
	VitalSize->value("3.0");
	CD->value(IW->inBC->value());
	SH->value(IW->inSH->value());
	
	VI->align(FL_ALIGN_RIGHT);
	VitalSize->align(FL_ALIGN_RIGHT);
	CD->align(FL_ALIGN_RIGHT);
	SH->align(FL_ALIGN_RIGHT);
	
	Calc = new Fl_Button(65,150,120,25,"Optimize PBR");
	Calc->callback(cb_calc, this);
	
	
	oStatus = new Fl_Multiline_Output(10,190,230,100,"");
	oStatus->value("Waiting for input...");
	
	end();
	show();
	
}

PBRWindow::~PBRWindow(void){

	delete CD;
	delete VI;
	delete SH;
	delete VitalSize;
	delete Calc;
	delete oStatus;

}


void PBRWindow::cb_calc(Fl_Widget* o, void* v){

	PBRWindow* T = (PBRWindow*)v;
	int result[5];
	int df=0;
	
	
	// Use the InputWindow selected drag function since I forgot
	// to put a selector on this window, and most people won't want
	// to change it anyway.
	if (T->inW->inG1->value()==1) df=G1;
	if (T->inW->inG2->value()==1) df=G2;
	if (T->inW->inG5->value()==1) df=G5;
	if (T->inW->inG6->value()==1) df=G6;
	if (T->inW->inG7->value()==1) df=G7;
	if (T->inW->inG8->value()==1) df=G8;
	
	
	double cd = atof(T->CD->value());
	double mv = atof(T->VI->value());
	double sh = atof(T->SH->value());
	double vs = atof(T->VitalSize->value());
	
	pbr(df,cd,mv, sh, vs,(int*)&result);
	char txt[255];
	sprintf(txt,"Near Zero:  %d yards\nFar Zero: %d yards\nMinimum PBR: %d yards\nMaximum PBR: %d yards\nSight-in at 100yds: %.2f\" High",result[0], result[1], result[2],result[3],float((float)result[4]/(float)100));
	T->oStatus->value(txt);
	
}
