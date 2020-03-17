#include "RangeWindow.h"

RangeWindow::RangeWindow(int w, int h, GBCSolution* inSLN):Fl_Window(w,h,inSLN->Name()){
	begin();
	menu = new Fl_Menu_Bar(0,0,w,30,"MENU");
	Sln=inSLN;
	
	Fl_Menu_Item items[]= {
	    {"&File",0,0,0,FL_SUBMENU},
		  {"Export Data",0,0,0,FL_SUBMENU},
		    {"Comma delimited (.csv)",0,(Fl_Callback*)cb_ExportCSV,this,0},
			{"Excel Spreadsheet (.xls)",0,(Fl_Callback*)cb_ExportExcel,this,0},
			{"OpenOffice Spreadsheet (.ods)",0,(Fl_Callback*)cb_ExportOO,this,0},
			{"HTML Table (.html)",0,(Fl_Callback*)cb_ExportHTML,this,0},
			{"SQL Insert Statement (.sql)",0,(Fl_Callback*)cb_ExportSQL,this,0},
			{"XML File (.xml)",0,(Fl_Callback*)cb_ExportXML,this,0},
			{"PDF Document (.pdf)",0,(Fl_Callback*)cb_ExportPDF,this,0},
			{0},
		  {"&Close",0,(Fl_Callback*)cb_Close,this,0},
		  {0},			
		{"&Minimum Range",0,0,0,FL_SUBMENU},
		  {"0",0,(Fl_Callback*)cb_Min0,this,FL_MENU_RADIO|FL_MENU_VALUE},
		  {"50",0,(Fl_Callback*)cb_Min50,this,FL_MENU_RADIO},
		  {"100",0,(Fl_Callback*)cb_Min100,this,FL_MENU_RADIO},
		  {"250",0,(Fl_Callback*)cb_Min250,this,FL_MENU_RADIO},
		  {"500",0,(Fl_Callback*)cb_Min500,this,FL_MENU_RADIO},
		  {"750",0,(Fl_Callback*)cb_Min750,this,FL_MENU_RADIO},
		  {"1000",0,(Fl_Callback*)cb_Min1000,this,FL_MENU_RADIO},
		  {"2000",0,(Fl_Callback*)cb_Min2000,this,FL_MENU_RADIO},
		  {0},
		{"&Maximum Range",0,0,0,FL_SUBMENU},
		  {"100",0,(Fl_Callback*)cb_Max100,this,FL_MENU_RADIO},
		  {"250",0,(Fl_Callback*)cb_Max250,this,FL_MENU_RADIO},
		  {"500",0,(Fl_Callback*)cb_Max500,this,FL_MENU_RADIO},
		  {"750",0,(Fl_Callback*)cb_Max750,this,FL_MENU_RADIO},
		  {"1000",0,(Fl_Callback*)cb_Max1000,this,FL_MENU_RADIO|FL_MENU_VALUE},
		  {"1760",0,(Fl_Callback*)cb_Max1760,this,FL_MENU_RADIO},
		  {"2000",0,(Fl_Callback*)cb_Max2000,this,FL_MENU_RADIO},
		  {"MAX",0,(Fl_Callback*)cb_MaxMax,this,FL_MENU_RADIO},
		  {0},
		{"&Step Size",0,0,0,FL_SUBMENU},
		  {"1",0,(Fl_Callback*)cb_Step1,this,FL_MENU_RADIO},
		  {"5",0,(Fl_Callback*)cb_Step5,this,FL_MENU_RADIO},
		  {"10",0,(Fl_Callback*)cb_Step10,this,FL_MENU_RADIO|FL_MENU_VALUE},
		  {"25",0,(Fl_Callback*)cb_Step25,this,FL_MENU_RADIO},
		  {"50",0,(Fl_Callback*)cb_Step50,this,FL_MENU_RADIO},
		  {"100",0,(Fl_Callback*)cb_Step100,this,FL_MENU_RADIO},
		  {"250",0,(Fl_Callback*)cb_Step250,this,FL_MENU_RADIO},
		  {0},		  
		{0}
	};
	
	menu->copy(items);
		  
	tbl = new Fl_Browser(5,35,w-10,h-40,"Hello");	

	
	colwidths = (int*)malloc(10*sizeof(int));
	colwidths[0]=70; // Range (yds)
	colwidths[1]=70; // Drop (in)
	colwidths[2]=70; // Drop (MOA)
	colwidths[3]=70; // Vel (ft/s)
	colwidths[4]=70; // Energy (ft-lb)
	colwidths[5]=70; // Winddrift (in)
	colwidths[6]=70; // Windage (ft)
	colwidths[7]=70; // Time (s)
	colwidths[8]=0;	
	
	tbl->column_widths(colwidths);
	tbl->column_char('\t');
	
	min=0;
	max=1000;
	step=10;
	
	GenTable();
		
	end();
	show();
	
}

RangeWindow::~RangeWindow(){
	delete tbl;
	delete menu;
	free(colwidths);


}


void RangeWindow::GenTable(){
	
	
	double r,p,m,v,e,wi,wm,t;
	tbl->clear();
	
	int MIN=this->min;
	int MAX=this->max;
	int STEP=this->step;

		

	int num=Sln->MaxRows();
	

	// printf("NUM: %d",num);

	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
	
	char* str = (char*)malloc(1024*sizeof(char));
	
	// Print some general information about the load.
	sprintf(str,"@b%s",Sln->Name()); tbl->add(str);
	sprintf(str,"Drag Coefficient: %.3f   Projectile Weight: %d grains",Sln->BC(), Sln->Weight()); tbl->add(str);
	sprintf(str,"Initial Velocity: %d (ft/s)   Zero Range: %d yards   Shooting Angle: %d degrees",Sln->MuzzleVelocity(), Sln->ZeroRange(), Sln->ShootingAngle()); tbl->add(str);
	sprintf(str,"Wind Velocity:  %d mph    Wind Direction: %d degrees",Sln->WindSpeed(), Sln->WindAngle()); tbl->add(str);
	sprintf(str,"Altitude: %d feet   Barometer: %.2f in-Hg  Temperature: %d F   Relative Humidity: %d%%",Sln->Altitude(), Sln->Pressure(), Sln->Temp(), Sln->Humidity()); tbl->add(str);
	
	tbl->add("");
	
	tbl->add("@b@cRange\t@b@cDrop\t@b@cDrop\t@b@cVelocity\t@b@cEnergy\t@b@cWind Drift\t@b@cWindage\t@b@cTime",0);
	tbl->add("@c(yards)\t@c(inches)\t@c(MOA)\t@c(ft/s)\t@c(ft-lb)\t@c(inches)\t@c(MOA)\t@c(s)",0);
		
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=Sln->GetRange(n);
		p=Sln->GetPath(n);
		m=Sln->GetMOA(n);
		v=Sln->GetVelocity(n);
		wi=Sln->GetWindage(n);
		wm=Sln->GetWindageMOA(n);
		t=Sln->GetTime(n);
		e=Sln->Weight()*v*v/450436;
		
		sprintf(str,"@c%.0f\t@c%.2f\t@c%.2f\t@c%.0f\t@c%.0f\t@c%.2f\t@c%.2f\t@c%.2f",r,p,m,v,e,wi,wm,t);
		tbl->add(str,0);
	}
	
	
}




void RangeWindow::cb_Nothing(Fl_Widget* t, void* v){
	//exit(0);
}


void RangeWindow::cb_Min0(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=0;
	T->GenTable();
}
void RangeWindow::cb_Min50(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=50;
	T->GenTable();
}
void RangeWindow::cb_Min100(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=100;
	T->GenTable();
}
void RangeWindow::cb_Min250(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=250;
	T->GenTable();
}
void RangeWindow::cb_Min500(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=500;
	T->GenTable();
}
void RangeWindow::cb_Min750(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=750;
	T->GenTable();
}
void RangeWindow::cb_Min1000(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=1000;
	T->GenTable();
}
void RangeWindow::cb_Min2000(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->min=2000;
	T->GenTable();
}


void RangeWindow::cb_Max100(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=100;
	T->GenTable();
}
void RangeWindow::cb_Max250(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=250;
	T->GenTable();
}
void RangeWindow::cb_Max500(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=500;
	T->GenTable();
}
void RangeWindow::cb_Max750(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=750;
	T->GenTable();
}
void RangeWindow::cb_Max1000(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=1000;
	T->GenTable();
}
void RangeWindow::cb_Max1760(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=1760;
	T->GenTable();
}
void RangeWindow::cb_Max2000(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->max=2000;
	T->GenTable();
}
void RangeWindow::cb_MaxMax(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	int mr = T->Sln->MaxRows();
	T->max=mr;
	T->GenTable();
}


void RangeWindow::cb_Step1(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=1;
	T->GenTable();
}
void RangeWindow::cb_Step5(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=5;
	T->GenTable();
}
void RangeWindow::cb_Step10(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=10;
	T->GenTable();
}
void RangeWindow::cb_Step25(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=25;
	T->GenTable();
}
void RangeWindow::cb_Step50(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=50;
	T->GenTable();
}
void RangeWindow::cb_Step100(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=100;
	T->GenTable();
}
void RangeWindow::cb_Step250(Fl_Widget* t, void* v){
	RangeWindow* T=(RangeWindow*)v;
	T->step=250;
	T->GenTable();
}


void RangeWindow::cb_ExportCSV(Fl_Widget* f, void* vtt){

	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	fname = fl_file_chooser("Please select a location and filename to export the CSV data.","*.csv","OutputData.csv",0);
	if (fname==NULL) return;

	
	FILE* ofile = fopen(fname,"w");
	
	// Print the output to a file.
	double r,p,m,v,e,wi,wm,t;
	
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;

	fprintf(ofile, "Range,Drop,Drop,Velocity,Energy,Wind Drift,Windage,Time");
	fprintf(ofile, "\n(yards),(inches),(MOA),(ft/s),(ft-lb),(inches),(MOA),(s)");
	
	int num=T->Sln->MaxRows();
	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
	
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;
		
		fprintf(ofile,"\n%.0f,%.2f,%.2f,%.0f,%.2f,%.2f,%.2f,%.2f",r,p,m,v,e,wi,wm,t);
		
	}
	
	fclose(ofile);
	
 
}
void RangeWindow::cb_ExportExcel(Fl_Widget* f, void* vtt){

	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	fname = fl_file_chooser("Please select a location and filename to export the Excel data.","*.xls","OutputData.xls",0);
	if (fname==NULL) return;

	FILE* ofile = fopen(fname,"w");
	
	// Print the output to a file.
	double r,p,m,v,e,wi,wm,t;
	
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;

	fprintf(ofile, "\n<html><head><title>$s</title></head>");
	fprintf(ofile, "\n\n<table><tr>");
	fprintf(ofile, "<td>Range</td>\
					<td>Drop</td>\
					<td>Drop</td>\
					<td>Velocity</td>\
					<td>Energy</td>\
					<td>Winddrift</td>\
					<td>Windage</td>\
					<td>Time</td></tr>"
					);
					
	fprintf(ofile,"\n<tr>");
	fprintf(ofile, "<td>(yards)</td>\
					<td>(inches)</td>\
					<td>(MOA)</td>\
					<td>(ft/s)</td>\
					<td>(ft-lb)</td>\
					<td>(inches)</td>\
					<td>(MOA)</td>\
					<td>(s)</td>"
					);
					
	int num=T->Sln->MaxRows();
	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
		
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;
		
		fprintf(ofile,"\n<tr>\
						<td>%.0f</td>\
						<td>%.2f</td>\
						<td>%.2f</td>\
						<td>%.0f</td>\
						<td>%.2f</td>\
						<td>%.2f</td>\
						<td>%.2f</td>\
						<td>%.2f</td></tr>",
						r,p,m,v,e,wi,wm,t);
	}
	
	fprintf(ofile,"\n</table></html>");
	fclose(ofile);



	
	
}


void RangeWindow::cb_ExportOO(Fl_Widget* f, void* vtt){

	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	
	fname = fl_file_chooser("Please select a location and filename to export the OpenOffice spreadsheet.","*.ods","OutputData.ods",0);
	if (fname==NULL) return;

	FILE* ofile = fopen(fname,"w");
	
	// Print the output to a file.
	double r,p,m,v,e,wi,wm,t;
	
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;

	fprintf(ofile, "\n<html><head><title>%s</title></head>","TO DO ");
	fprintf(ofile, "\n\n<table><tr>");
	fprintf(ofile, "\n<td>Range</td>\
					\n<td>Drop</td>\
					\n<td>Drop</td>\
					\n<td>Velocity</td>\
					\n<td>Energy</td>\
					\n<td>Winddrift</td>\
					\n<td>Windage</td>\
					\n<td>Time</td></tr>"
					);
					
	fprintf(ofile,"\n\n<tr>");
	fprintf(ofile, "\n<td>(yards)</td>\
					\n<td>(inches)</td>\
					\n<td>(MOA)</td>\
					\n<td>(ft/s)</td>\
					\n<td>(ft-lb)</td>\
					\n<td>(inches)</td>\
					\n<td>(MOA)</td>\
					\n<td>(s)</td>"
					);
					
	int num=T->Sln->MaxRows();
	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
	

	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;
		
		fprintf(ofile,"\n<tr>\
						\n<td>%.0f</td>\
						\n<td>%.2f</td>\
						\n<td>%.2f</td>\
						\n<td>%.0f</td>\
						\n<td>%.2f</td>\
						\n<td>%.2f</td>\
						\n<td>%.2f</td>\
						\n<td>%.2f</td></tr>",
						r,p,m,v,e,wi,wm,t);
	}
	
	fprintf(ofile,"\n</table></html>");
	fclose(ofile);
	
}



void RangeWindow::cb_ExportHTML(Fl_Widget* f , void* vtt){
	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	fname = fl_file_chooser("Please select a location and filename to export the HTML data.","*.html","OutputData.html",0);
	if (fname==NULL) return;

	FILE* ofile = fopen(fname,"w");
	
	// Print the output to a file.
	double r,p,m,v,e,wi,wm,t;
	
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;

	
	fprintf(ofile, "\n<html><head><title>%s</title></head>","");
	fprintf(ofile, "\n\n<table width=560 bgcolor=black><tr>");
	fprintf(ofile, "\n<td width=70 bgcolor=white align=center><b>Range</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Drop</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Drop</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Velocity</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Energy</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Winddrift</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Windage</b></td>\
					\n<td width=70 bgcolor=white align=center><b>Time</b></td></tr>"
					);
					
	fprintf(ofile,"\n\n<tr>");
	fprintf(ofile, "\n<td width=70 bgcolor=white  align=center><b>(yards)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(inches)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(MOA)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(ft/s)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(ft-lb)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(inches)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(MOA)</b></td>\
					\n<td width=70 bgcolor=white align=center><b>(s)</b></td>"
					);
					
	int num=T->Sln->MaxRows();
	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
	
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;
		
		e=v*v/450436;
		
		fprintf(ofile,"\n\n<tr>\
						\n<td width=70 bgcolor=white align=center>%.0f</td>\
						\n<td width=70 bgcolor=white align=center>%.2f</td>\
						\n<td width=70 bgcolor=white align=center>%.2f</td>\
						\n<td width=70 bgcolor=white align=center>%.0f</td>\
						\n<td width=70 bgcolor=white align=center>%.2f</td>\
						\n<td width=70 bgcolor=white align=center>%.2f</td>\
						\n<td width=70 bgcolor=white align=center>%.2f</td>\
						\n<td width=70 bgcolor=white align=center>%.2f</td></tr>",
						r,p,m,v,e,wi,wm,t);
	}
	
	fprintf(ofile,"\n</table></html>");
	fclose(ofile);


}
void RangeWindow::cb_ExportSQL(Fl_Widget* f, void* vtt){

	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	fname = fl_file_chooser("Please select a location and filename to export the SQL File.","*.sql","OutputData.sql",0);
	if (fname==NULL) return;

	FILE* ofile = fopen(fname,"w");
	
	// Print the output to a file.
	double r,p,m,v,e,wi,wm,t;
	
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;

	int num=T->Sln->MaxRows();
	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
			
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;

		fprintf(ofile,"\nINSERT INTO BallisticData {range, drop, elevation, velocity, energy, winddrift, windage, time} VALUES\
						{%.0f,\
						%.2f, \
						%.2f,\
						%.0f,\
						%.2f,\
						%.2f,\
						%.2f,\
						%.2f};",
						r,p,m,v,e,wi,wm,t);
	}
	
	
	
	fclose(ofile);
	


}
void RangeWindow::cb_ExportXML(Fl_Widget* f, void* vtt){


	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	fname = fl_file_chooser("Please select a location and filename to export the XML File.","*.xml","OutputData.xml",0);
	if (fname==NULL) return;

	FILE* ofile = fopen(fname,"w");
	
	// Print the output to a file.
	double r,p,m,v,e,wi,wm,t;
	
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;

	fprintf(ofile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	

	fprintf(ofile, "\n<SolutionTitle>%s</SolutionTitle>","TEST");
	fprintf(ofile, "\n\n<SolutionData>");
					
	int num=T->Sln->MaxRows();
	if (MAX>num) MAX=num;
	if (MIN>MAX) MIN=MAX;
		
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;

		fprintf(ofile,"\n<SolutionElement>\
						<Range>%.0f</Range>\
						<Drop>%.2f</Drop>\
						<Elevation>%.2f</Elevation>\
						<Velocity>%.0f</Velocity>\
						<Energy>%.2f</Energy>\
						<Winddrift>%.2f</Winddrift>\
						<Windage>%.2f</Windage>\
						<Time>%.2f</td></Time></SolutionElement>",
						r,p,m,v,e,wi,wm,t);
	}
	
	fprintf(ofile,"\n</SolutionData>");
	
	
	fclose(ofile);
	
	
}

void RangeWindow::cb_Close(Fl_Widget* f, void* v){
	RangeWindow* T = (RangeWindow*)v;
	T->~RangeWindow();
}

	

void RangeWindow::cb_ExportPDF(Fl_Widget*, void* vtt){

	RangeWindow* T = (RangeWindow*)vtt;
	
	char* fname=NULL;
	fname = fl_file_chooser("Please select a location and filename to save the PDF Document.","*.pdf","OutputData.pdf ",0);
	if (fname==NULL) return;


    const char *page_title = T->Sln->Name();
    HPDF_Doc  pdf;
    HPDF_Page page;
    HPDF_Font def_font;
    HPDF_REAL tw;
    HPDF_REAL height;
    HPDF_REAL width;
    pdf = HPDF_New (NULL, NULL);

    /* Add a new page object. */
    page = HPDF_AddPage (pdf);
    height = HPDF_Page_GetHeight (page);
    width = HPDF_Page_GetWidth (page);
	
    /* Print the title of the page (with positioning center). */
    def_font = HPDF_GetFont (pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize (page, def_font, 16);
    tw = HPDF_Page_TextWidth (page, page_title);
    HPDF_Page_BeginText (page);
    HPDF_Page_TextOut (page, (width - tw) / 2, height - 50, page_title);
    HPDF_Page_EndText (page);
	
	HPDF_Page_SetFontAndSize(page,def_font,8);
	tw = HPDF_Page_TextWidth(page,"GNU Exterior Ballistics, v1.05");
	HPDF_Page_BeginText(page);
	HPDF_Page_TextOut (page, (width-tw)/2, height-62, "GNU Exterior Ballistics, v1.05");
	HPDF_Page_EndText(page);
	

    /* output Row Data. */
	int margin=50;
	int colsize=62;
	int rowheight=10;
	int footer=50;
		
	HPDF_Page_SetFontAndSize (page, def_font, 10);
    HPDF_Page_BeginText (page);
	
	// First print the weather condition data and such.
	char* str = (char*)malloc(1024*sizeof(char));
	// Print some general information about the load.
	sprintf(str,"Drag Coefficient: %.3f   Projectile Weight: %d grains",T->Sln->BC(), T->Sln->Weight());
	HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*0, str);
	sprintf(str,"Initial Velocity: %d (ft/s)   Zero Range: %d yards   Shooting Angle: %d degrees",T->Sln->MuzzleVelocity(), T->Sln->ZeroRange(), T->Sln->ShootingAngle());
	HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*1, str);
	sprintf(str,"Wind Velocity:  %d mph    Wind Direction: %d degrees",T->Sln->WindSpeed(), T->Sln->WindAngle());
	HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*2, str);
	sprintf(str,"Altitude: %d feet   Barometer: %.2f in-Hg  Temperature: %d F   Relative Humidity: %d%%",T->Sln->Altitude(), T->Sln->Pressure(), T->Sln->Temp(), T->Sln->Humidity());
	HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*3, str);

	
    HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*5, "Range");
	HPDF_Page_TextOut(page,margin+1*colsize,height-80- rowheight*5, "Drop");
	HPDF_Page_TextOut(page,margin+2*colsize,height-80- rowheight*5,"Elevation");
	HPDF_Page_TextOut(page,margin+3*colsize,height-80- rowheight*5,"Velocity");
	HPDF_Page_TextOut(page,margin+4*colsize,height-80- rowheight*5,"Energy");
	HPDF_Page_TextOut(page,margin+5*colsize,height-80- rowheight*5,"Wind Drift");
	HPDF_Page_TextOut(page,margin+6*colsize,height-80- rowheight*5,"Windage");
	HPDF_Page_TextOut(page,margin+7*colsize,height-80- rowheight*5,"Time");
	
	HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*6, "(yards)");
	HPDF_Page_TextOut(page,margin+1*colsize,height-80- rowheight*6, "(inches)");
	HPDF_Page_TextOut(page,margin+2*colsize,height-80- rowheight*6,"(MOA)");
	HPDF_Page_TextOut(page,margin+3*colsize,height-80- rowheight*6,"(ft/s)");
	HPDF_Page_TextOut(page,margin+4*colsize,height-80- rowheight*6,"(ft-lb)");
	HPDF_Page_TextOut(page,margin+5*colsize,height-80- rowheight*6,"(inches)");
	HPDF_Page_TextOut(page,margin+6*colsize,height-80- rowheight*6,"(MOA)");
	HPDF_Page_TextOut(page,margin+7*colsize,height-80- rowheight*6,"(seconds)");
	
	
    HPDF_Page_EndText (page);



	// Print the rows iteratively
	double r,p,m,v,e,wi,wm,t;
	int MIN=T->min;
	int MAX=T->max;
	int STEP=T->step;
	int row=8;
	HPDF_Page_SetFontAndSize (page, def_font, 8);
	
	for (int n=MIN;n<=MAX;n=n+STEP){
		r=T->Sln->GetRange(n);
		p=T->Sln->GetPath(n);
		m=T->Sln->GetMOA(n);
		v=T->Sln->GetVelocity(n);
		e=0;
		wi=T->Sln->GetWindage(n);
		wm=T->Sln->GetWindageMOA(n);
		t=T->Sln->GetTime(n);
		e=T->Sln->Weight()*v*v/450436;

		// Print the row.
		HPDF_Page_BeginText (page);
		sprintf(str,"%.0f",r);	HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.2f",p);	HPDF_Page_TextOut (page,margin+1*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.2f",m);	HPDF_Page_TextOut (page,margin+2*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.0f",v);	HPDF_Page_TextOut (page,margin+3*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.0f",e);	HPDF_Page_TextOut (page,margin+4*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.2f",wi);	HPDF_Page_TextOut (page,margin+5*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.2f",wm);	HPDF_Page_TextOut (page,margin+6*colsize, height - 80 - rowheight*row,str);
		sprintf(str,"%.2f",t);	HPDF_Page_TextOut (page,margin+7*colsize, height - 80 - rowheight*row,str);
		HPDF_Page_EndText (page);
		row++;
		
		if ((height-80-rowheight*row)<footer){
		
			// Insert a new page, we have come to the end of this one.
			page = HPDF_AddPage (pdf);
			HPDF_Page_SetFontAndSize (page, def_font, 10);
			HPDF_Page_BeginText (page);
			HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*2, "Range");
			HPDF_Page_TextOut(page,margin+1*colsize,height-80- rowheight*2, "Drop");
			HPDF_Page_TextOut(page,margin+2*colsize,height-80- rowheight*2,"Elevation");
			HPDF_Page_TextOut(page,margin+3*colsize,height-80- rowheight*2,"Velocity");
			HPDF_Page_TextOut(page,margin+4*colsize,height-80- rowheight*2,"Energy");
			HPDF_Page_TextOut(page,margin+5*colsize,height-80- rowheight*2,"Wind Drift");
			HPDF_Page_TextOut(page,margin+6*colsize,height-80- rowheight*2,"Windage");
			HPDF_Page_TextOut(page,margin+7*colsize,height-80- rowheight*2,"Time");
			
			HPDF_Page_TextOut (page,margin+0*colsize, height - 80 - rowheight*3, "(yards)");
			HPDF_Page_TextOut(page,margin+1*colsize,height-80- rowheight*3, "(inches)");
			HPDF_Page_TextOut(page,margin+2*colsize,height-80- rowheight*3,"(MOA)");
			HPDF_Page_TextOut(page,margin+3*colsize,height-80- rowheight*3,"(ft/s)");
			HPDF_Page_TextOut(page,margin+4*colsize,height-80- rowheight*3,"(ft-lb)");
			HPDF_Page_TextOut(page,margin+5*colsize,height-80- rowheight*3,"(inches)");
			HPDF_Page_TextOut(page,margin+6*colsize,height-80- rowheight*3,"(MOA)");
			HPDF_Page_TextOut(page,margin+7*colsize,height-80- rowheight*3,"(seconds)");
	
			HPDF_Page_EndText (page);

			HPDF_Page_SetFontAndSize (page, def_font, 8);
			row=5;
		}
	

	}

	free(str);
	
    HPDF_SaveToFile (pdf, fname);



    /* clean up */
    HPDF_Free (pdf);


}


		  

