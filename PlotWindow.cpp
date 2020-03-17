#include "PlotWindow.h"

PlotWindow::PlotWindow(int w, int h, GBCSolution* Gsln, GBCSolution* GMEM1, GBCSolution* GMEM2) : Fl_Window(w,h,"Solution Visualization Tools"){
	
	// Store a pointer to the calling solution.
	gsln=Gsln;
	mem1=GMEM1;
	mem2=GMEM2;
	
	x_click=0;
	y_click=0;
	
	begin();
	this->size_range(0,0,1600,1600);
	
	// Put a simple menu, for saving or closing the window.
	menu = new Fl_Menu_Bar(0,0,w,25,"MENU");

	Fl_Menu_Item items[]= {
	    {"&File",0,0,0,FL_SUBMENU},
		 {"Save as..",0,0,0,FL_SUBMENU | FL_MENU_INACTIVE},
		    {"PNG (.png)",0,(Fl_Callback*)cb_nothing,this,0},
			{"JPEG (.jpg)",0,(Fl_Callback*)cb_nothing,this,0},
			{"Bitmap (.bmp)",0,(Fl_Callback*)cb_nothing,this,0},
			{"Adobe PDF (.pdf)",0,(Fl_Callback*)cb_nothing,this,0},
			{0},
		  {"&Close",0,(Fl_Callback*)cb_Close,this,0},
		  {0},			
		{0}
	};
	
	menu->copy(items);
	menu->down_box(FL_NO_BOX);

	// Put in a range slider for selecting the chart's range.	
	sl_x = new Fl_Value_Slider(150,40,350,30,"Range (yds)");
	sl_x->align(FL_ALIGN_RIGHT);
	sl_x->value(200);
	sl_x->minimum(200);
	sl_x->maximum(gsln->MaxRows());
	sl_x->type(FL_HOR_NICE_SLIDER);
	sl_x->callback(cb_slUpdate,this);
	sl_x->precision(0);
	sl_x->step(10);
	
	//Fl_Button* btPlot = new Fl_Button(600,40,70,30,"UPDATE");
	//btPlot->callback(cb_btPlot,this);
	
	btMem1 = new Fl_Check_Button(590,30,20,20,"Memory 1");
	btMem2 = new Fl_Check_Button(590,50,20,20,"Memory 2");
	btMem1->align(FL_ALIGN_RIGHT);
	btMem2->align(FL_ALIGN_RIGHT);
	btMem1->labelcolor(FL_RED);
	btMem2->labelcolor(FL_DARK_GREEN);
	btMem1->callback(cb_Mem1,this);
	btMem2->callback(cb_Mem2,this);
		
	if (mem1==NULL) btMem1->deactivate();
	if (mem2==NULL) btMem2->deactivate();
	
	if (mem1!=NULL) btMem1->tooltip(mem1->Name());
	if (mem2!=NULL) btMem2->tooltip(mem2->Name());
	
	rbPlotPath = new Fl_Round_Button(20,30,30,20,"Path (inches)");
	rbPlotVelocity = new Fl_Round_Button(20,50,30,20,"Velocity");
	rbPlotEnergy = new Fl_Round_Button(20,70,30,20,"Energy");
	
	rbPlotPath->type(FL_RADIO_BUTTON);
	rbPlotVelocity->type(FL_RADIO_BUTTON);
	rbPlotEnergy->type(FL_RADIO_BUTTON);
	
	rbPlotPath->callback(cb_btPlot,this);
	rbPlotVelocity->callback(cb_btPlot,this);
	rbPlotEnergy->callback(cb_btPlot,this);
	
	rbPlotPath->value(1);
	
	rbPlotPath->align(FL_ALIGN_RIGHT);
	rbPlotVelocity->align(FL_ALIGN_RIGHT);
	rbPlotEnergy->align(FL_ALIGN_RIGHT);
	
	end();
	show();
	
	
}

PlotWindow::~PlotWindow(void){

	delete menu;
	delete sl_x;
	delete rbPlotPath;
	delete rbPlotVelocity;
	delete rbPlotEnergy;
	delete btMem1;
	delete btMem2;
	

}


void PlotWindow::draw(void){
	
	// Let the window draw itself first.
	Fl_Window::draw();
	
	if (rbPlotPath->value()==1) PlotPath();
	else if (rbPlotEnergy->value()==1) PlotEnergy();
	else if (rbPlotVelocity->value()==1) PlotVelocity();

}

int PlotWindow::handle(int evt){

	if (evt==FL_PUSH && Fl::event_button1()){
		x_click=Fl::event_x();
		y_click=Fl::event_y();
		this->redraw();
			
	}
	

	
	return Fl_Window::handle(evt);

}



void PlotWindow::PlotPath(void){

	int w = this->w();
	int h = this->h();
	
	int xmin = 5;
	int xmax = w-5;
	int ymin = 100;
	int ymax = h-5;
	
	int y0 = (ymax-ymin)/2 + ymin;
	
	
	// The gridlines look best with 15-25 graduations in x, so we set some basic defaults here.
	x_ticks=25;
	if (sl_x->value() > 500) x_ticks=50;
	if (sl_x->value() > 1000) x_ticks=100;
	if (sl_x->value() > 2000) x_ticks=250;
	if (sl_x->value() > 4000) x_ticks=500;
	

	// We need to find the max and min y-values to determine our y scale and y-ticks
	double _miny=0;
	double _maxy=0;
	

		
	for (int e=0;e<sl_x->value();e++){
		if (gsln->GetPath(e) > _maxy) _maxy=gsln->GetPath(e);
		if (gsln->GetPath(e) < _miny) _miny=gsln->GetPath(e);
	}

	int x_range=(int)sl_x->value();
	double y_range=0;
	
	if (fabs(_miny) > fabs(_maxy)) y_range = fabs(2.5*_miny);
	else y_range = fabs(2.5*_maxy);
	
	double x_scale = (double)(xmax-xmin) / (double)x_range;
	double y_scale = (double)(ymax-ymin) / (double)y_range;
	


	
	// Now do our custom drawing.
	fl_draw_box(FL_FLAT_BOX,xmin,ymin,w-10,h-ymin-5,FL_WHITE);

	// Draw the x-axis.
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID,2);
	fl_line(xmin,y0,xmax,y0);
	
	// The y-axis looks best with about 7-12 ticks, so we scale it here reasonably.
	y_ticks=1;
	if (y_range > 10) y_ticks=2;
	if (y_range > 20) y_ticks=6;
	if (y_range > 40) y_ticks=12;
	if (y_range > 80) y_ticks=24;
	if (y_range > 200) y_ticks=48;
	if (y_range > 400) y_ticks=100;
	if (y_range > 800) y_ticks=250;
	if (y_range > 1600) y_ticks=500;

	double px;
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID,1);
	char txt[64];
	int r=0;
	double txtw=0;
	// Draw X gridlines every x_ticks yds and label them.
	for (px=xmin;px<xmax;px+=((double)x_ticks*x_scale)){
		//fl_line((int)px,y_bottom,(int)px,y_top);
		fl_line_style(FL_DOT,1);
		fl_line((int)px,(int)ymin,(int)px,(int)ymax);
		sprintf(txt,"%d",r*x_ticks);
		if (r%2==0 && r>0) {
			txtw=fl_width(txt);
			fl_line_style(FL_SOLID,1);
			fl_line((int)px,y0,(int)px,y0+8);
			fl_draw(txt,(int)((int)px-(txtw/2)),y0+10+fl_height());
		}
		r++;
	}
	
	// Draw Y hashmarks every 1 y_tick going up.
	double py;
	r=0;
	for (py=y0;py>=ymin;py-=((double)y_ticks*y_scale)){
		if (r>0){
			fl_line(xmin,(int)py,xmax,(int)py);
			sprintf(txt,"+%d\"",r*y_ticks);
			fl_draw(txt,xmin+10,(int)(py+fl_height()/2));
		}
		r++;
	}
	
	// Draw Y hashmarks every 1 y_tick going down
	r=0;
	for (py=y0;py<=ymax;py+=((double)y_ticks*y_scale)){
		if (r>0){
			fl_line(xmin,(int)py,xmax,(int)py);
			sprintf(txt,"-%d\"",r*y_ticks);
			if (py+fl_height()/2<ymax)	fl_draw(txt,xmin+10,(int)(py+fl_height()/2));
		}
		r++;
	}
	
	int x1,x2;
	double y1,y2;
	
	// For the pixel locations to plot.
	int px1, px2, py1,py2;
	
	x1=(int)this->gsln->GetRange(0);
	y1=this->gsln->GetPath(0);
		
	int m=this->gsln->MaxRows();
	
	fl_color(FL_BLUE);
	fl_line_style(FL_SOLID,1);
	
	for (int n=1;n<x_range && n<m ;n++){
		x2=(int)this->gsln->GetRange(n);
		y2=this->gsln->GetPath(n);
		
		// Translate the x,y values into scaled pixels.
		px1=(int)(xmin+x_scale*(double)x1);
		px2=(int)(xmin+x_scale*(double)x2);
		
		py1=(int)((y0)-(double)y_scale*(double)y1);
		py2=(int)((y0)-(double)y_scale*(double)y2);
	
		if (py2>=ymax) break;
		if (px2>=xmax) break;
		
		// Plot the points.
		fl_line(px1,py1,px2,py2);
		
		x1=x2;
		y1=y2;
		

	}
	
	if (btMem1->value()==1){
	
		x1=(int)this->mem1->GetRange(0);
		y1=this->mem1->GetPath(0);
		
		int m=this->gsln->MaxRows();
	
		fl_color(FL_RED);
		fl_line_style(FL_SOLID,1);
		
		for (int n=1;n<x_range && n<m ;n++){
			x2=(int)this->mem1->GetRange(n);
			y2=this->mem1->GetPath(n);
			
			// Translate the x,y values into scaled pixels.
			px1=(int)(xmin+x_scale*(double)x1);
			px2=(int)(xmin+x_scale*(double)x2);
			
			py1=(int)((y0)-(double)y_scale*(double)y1);
			py2=(int)((y0)-(double)y_scale*(double)y2);
		
			if (py2>=ymax) break;
			if (px2>=xmax) break;
			
			// Plot the points.
			if (px1>xmin && px1<xmax && py1<ymax && py1>ymin)
				fl_line(px1,py1,px2,py2);
			
			x1=x2;
			y1=y2;
			

		}
	}
	
	
	if (btMem2->value()==1){
	
		x1=(int)this->mem2->GetRange(0);
		y1=this->mem2->GetPath(0);
		
		int m=this->gsln->MaxRows();
	
		fl_color(FL_GREEN);
		fl_line_style(FL_SOLID,1);
		
		for (int n=1;n<x_range && n<m ;n++){
			x2=(int)this->mem2->GetRange(n);
			y2=this->mem2->GetPath(n);
			
			// Translate the x,y values into scaled pixels.
			px1=(int)(xmin+x_scale*(double)x1);
			px2=(int)(xmin+x_scale*(double)x2);
			
			py1=(int)((y0)-(double)y_scale*(double)y1);
			py2=(int)((y0)-(double)y_scale*(double)y2);
		
			if (py2>=ymax) break;
			if (px2>=xmax) break;
			
			// Plot the points.
			if (px1>xmin && px1<xmax && py1<ymax && py1>ymin)
				fl_line(px1,py1,px2,py2);
			
			x1=x2;
			y1=y2;
			

		}
	}
	
	
	if (x_click>xmin && x_click<xmax && y_click>ymin && y_click<ymax){
		// If the user clicks, show the coordinates they clicked on.
		double click_label_x= (x_click-xmin)/x_scale;
		double click_label_y=(y0-y_click)/y_scale;
		char lbl_point[20];
		sprintf(lbl_point,"(%.0f,%.2f)",click_label_x, click_label_y);
		fl_color(FL_BLACK);
		fl_draw(lbl_point,x_click+5, y_click-5);
		fl_rectf(x_click-2, y_click-2,4,4);
	}
	
	
	
	// Draw the frame around it last to make it all pretty.
	fl_frame("aaaa",5,100,w-10,h-105);
	
	return;



}

void PlotWindow::PlotEnergy(void){


	int w = this->w();
	int h = this->h();
	
	int xmin = 5;
	int xmax = w-5;
	int ymin = 100;
	int ymax = h-5;
	
	int y0 = (ymax)-50;
	
	
	// The gridlines look best with 15-25 graduations in x, so we set some basic defaults here.
	x_ticks=25;
	if (sl_x->value() > 500) x_ticks=50;
	if (sl_x->value() > 1000) x_ticks=100;
	if (sl_x->value() > 2000) x_ticks=250;
	if (sl_x->value() > 4000) x_ticks=500;
	

	// We need to find the max y-value to determine our y scale and y-ticks

	double _maxy=0;
	double _miny=0;
		
	for (int e=1;e<sl_x->value();e++){
		if (gsln->GetPath(e) > _maxy) _maxy=gsln->GetEnergy(e);
	}

	int x_range=(int)sl_x->value();
	double y_range=0;
	
	if (fabs(_miny) > fabs(_maxy)) y_range = fabs(1.5*_miny);
	else y_range = fabs(1.5*_maxy);
	
	double x_scale = (double)(xmax-xmin) / (double)x_range;
	double y_scale = (double)(ymax-ymin) / (double)y_range;
	
	// Now do our custom drawing.
	fl_draw_box(FL_FLAT_BOX,xmin,ymin,w-10,h-ymin-5,FL_WHITE);

	
	// Draw the x-axis.
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID,2);
	fl_line(xmin,y0,xmax,y0);
	
	// The y-axis looks best with about 7-12 ticks, so we scale it here reasonably.
	y_ticks=1;
	if (y_range > 10) y_ticks=2;
	if (y_range > 20) y_ticks=6;
	if (y_range > 40) y_ticks=12;
	if (y_range > 80) y_ticks=24;
	if (y_range > 200) y_ticks=48;
	if (y_range > 400) y_ticks=100;
	if (y_range > 800) y_ticks=250;
	if (y_range > 1600) y_ticks=500;

	double px;
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID,1);
	char txt[64];
	int r=0;
	double txtw=0;
	// Draw X gridlines every x_ticks yds and label them.
	for (px=xmin;px<xmax;px+=((double)x_ticks*x_scale)){
		//fl_line((int)px,y_bottom,(int)px,y_top);
		fl_line_style(FL_DOT,1);
		fl_line((int)px,(int)ymin,(int)px,(int)y0);
		sprintf(txt,"%d",r*x_ticks);
		if (r%2==0 && r>0) {
			txtw=fl_width(txt);
			fl_line_style(FL_SOLID,1);
			fl_line((int)px,y0,(int)px,y0+8);
			fl_draw(txt,(int)((int)px-(txtw/2)),y0+10+fl_height());
		}
		r++;
	}
	
	// Draw Y hashmarks every 1 y_tick going up.
	double py;
	r=0;
	for (py=y0;py>=ymin;py-=((double)y_ticks*y_scale)){
		if (r>0){
			fl_line(xmin,(int)py,xmax,(int)py);
			sprintf(txt,"+%d ft-lb",r*y_ticks);
			fl_draw(txt,xmin+10,(int)(py+fl_height()/2));
		}
		r++;
	}
	
	
	int x1,x2;
	double y1,y2;
	
	// For the pixel locations to plot.
	int px1, px2, py1,py2;
	
	x1=(int)this->gsln->GetRange(0);
	y1=this->gsln->GetEnergy(0);
		
	int m=this->gsln->MaxRows();
	
	fl_color(FL_BLUE);
	fl_line_style(FL_SOLID,1);
	
	for (int n=1;n<x_range && n<m ;n++){
		x2=(int)this->gsln->GetRange(n);
		y2=this->gsln->GetEnergy(n);
		
		// Translate the x,y values into scaled pixels.
		px1=(int)(xmin+x_scale*(double)x1);
		px2=(int)(xmin+x_scale*(double)x2);
		
		py1=(int)((y0)-(double)y_scale*(double)y1);
		py2=(int)((y0)-(double)y_scale*(double)y2);
	
		if (py2>=ymax) break;
		if (px2>=xmax) break;
		
		// Plot the points.
		fl_line(px1,py1,px2,py2);
		
		x1=x2;
		y1=y2;
		

	}
	
	
	if (btMem1->value()==1){
	
		x1=(int)this->mem1->GetRange(0);
		y1=this->mem1->GetEnergy(0);
			
		int m=this->mem1->MaxRows();
		
		fl_color(FL_RED);
		fl_line_style(FL_SOLID,1);
		
		for (int n=1;n<x_range && n<m ;n++){
			x2=(int)this->mem1->GetRange(n);
			y2=this->mem1->GetEnergy(n);
			
			// Translate the x,y values into scaled pixels.
			px1=(int)(xmin+x_scale*(double)x1);
			px2=(int)(xmin+x_scale*(double)x2);
			
			py1=(int)((y0)-(double)y_scale*(double)y1);
			py2=(int)((y0)-(double)y_scale*(double)y2);
		
			if (py2>=ymax) break;
			if (px2>=xmax) break;
			
			// Plot the points.
			if (px1>xmin && px1<xmax && py1<ymax && py1>ymin) fl_line(px1,py1,px2,py2);
			
			x1=x2;
			y1=y2;
			

		}
	}

	
	if (btMem2->value()==1){
	
		x1=(int)this->mem2->GetRange(0);
		y1=this->mem2->GetEnergy(0);
			
		int m=this->mem2->MaxRows();
		
		fl_color(FL_GREEN);
		fl_line_style(FL_SOLID,1);
		
		for (int n=1;n<x_range && n<m ;n++){
			x2=(int)this->mem2->GetRange(n);
			y2=this->mem2->GetEnergy(n);
			
			// Translate the x,y values into scaled pixels.
			px1=(int)(xmin+x_scale*(double)x1);
			px2=(int)(xmin+x_scale*(double)x2);
			
			py1=(int)((y0)-(double)y_scale*(double)y1);
			py2=(int)((y0)-(double)y_scale*(double)y2);
		
			if (py2>=ymax) break;
			if (px2>=xmax) break;
			
			// Plot the points.
			if (px1>xmin && px1<xmax && py1<ymax && py1>ymin) fl_line(px1,py1,px2,py2);
			
			x1=x2;
			y1=y2;
			

		}
	}
	
	
	if (x_click>xmin && x_click<xmax && y_click>ymin && y_click<ymax){
		// If the user clicks, show the coordinates they clicked on.
		double click_label_x= (x_click-xmin)/x_scale;
		double click_label_y=(y0-y_click)/y_scale;
		char lbl_point[20];
		sprintf(lbl_point,"(%.0f,%.2f)",click_label_x, click_label_y);
		fl_color(FL_BLACK);
		fl_draw(lbl_point,x_click+5, y_click-5);
		fl_rectf(x_click-2, y_click-2,4,4);
	}
	
	
	// Draw the frame around it last to make it all pretty.
	fl_frame("aaaa",5,100,w-10,h-105);
	
	return;
	


}

void PlotWindow::PlotVelocity(void){



	int w = this->w();
	int h = this->h();
	
	int xmin = 5;
	int xmax = w-5;
	int ymin = 100;
	int ymax = h-5;
	
	int y0 = (ymax)-50;
	
	
	// The gridlines look best with 15-25 graduations in x, so we set some basic defaults here.
	x_ticks=25;
	if (sl_x->value() > 500) x_ticks=50;
	if (sl_x->value() > 1000) x_ticks=100;
	if (sl_x->value() > 2000) x_ticks=250;
	if (sl_x->value() > 4000) x_ticks=500;
	

	// We need to find the max y-value to determine our y scale and y-ticks

	double _maxy=0;
	double _miny=0;
		
	for (int e=1;e<sl_x->value();e++){
		if (gsln->GetPath(e) > _maxy) _maxy=gsln->GetVelocity(e);
	}

	int x_range=(int)sl_x->value();
	double y_range=0;
	
	if (fabs(_miny) > fabs(_maxy)) y_range = fabs(1.5*_miny);
	else y_range = fabs(1.5*_maxy);
	
	double x_scale = (double)(xmax-xmin) / (double)x_range;
	double y_scale = (double)(ymax-ymin) / (double)y_range;
	
	// Now do our custom drawing.
	fl_draw_box(FL_FLAT_BOX,xmin,ymin,w-10,h-ymin-5,FL_WHITE);

	
	// Draw the x-axis.
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID,2);
	fl_line(xmin,y0,xmax,y0);
	
	// The y-axis looks best with about 7-12 ticks, so we scale it here reasonably.
	y_ticks=1;
	if (y_range > 10) y_ticks=2;
	if (y_range > 20) y_ticks=6;
	if (y_range > 40) y_ticks=12;
	if (y_range > 80) y_ticks=24;
	if (y_range > 200) y_ticks=48;
	if (y_range > 400) y_ticks=100;
	if (y_range > 800) y_ticks=250;
	if (y_range > 1600) y_ticks=500;

	double px;
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID,1);
	char txt[64];
	int r=0;
	double txtw=0;
	// Draw X gridlines every x_ticks yds and label them.
	for (px=xmin;px<xmax;px+=((double)x_ticks*x_scale)){
		//fl_line((int)px,y_bottom,(int)px,y_top);
		fl_line_style(FL_DOT,1);
		fl_line((int)px,(int)ymin,(int)px,(int)y0);
		sprintf(txt,"%d",r*x_ticks);
		if (r%2==0 && r>0) {
			txtw=fl_width(txt);
			fl_line_style(FL_SOLID,1);
			fl_line((int)px,y0,(int)px,y0+8);
			fl_draw(txt,(int)((int)px-(txtw/2)),y0+10+fl_height());
		}
		r++;
	}
	
	// Draw Y hashmarks every 1 y_tick going up.
	double py;
	r=0;
	for (py=y0;py>=ymin;py-=((double)y_ticks*y_scale)){
		if (r>0){
			fl_line(xmin,(int)py,xmax,(int)py);
			sprintf(txt,"+%d ft/s",r*y_ticks);
			fl_draw(txt,xmin+10,(int)(py+fl_height()/2));
		}
		r++;
	}
	
	
	int x1,x2;
	double y1,y2;
	
	// For the pixel locations to plot.
	int px1, px2, py1,py2;
	
	x1=(int)this->gsln->GetRange(0);
	y1=this->gsln->GetVelocity(0);
		
	int m=this->gsln->MaxRows();
	
	fl_color(FL_BLUE);
	fl_line_style(FL_SOLID,1);
	
	for (int n=1;n<x_range && n<m ;n++){
		x2=(int)this->gsln->GetRange(n);
		y2=this->gsln->GetVelocity(n);
		
		// Translate the x,y values into scaled pixels.
		px1=(int)(xmin+x_scale*(double)x1);
		px2=(int)(xmin+x_scale*(double)x2);
		
		py1=(int)((y0)-(double)y_scale*(double)y1);
		py2=(int)((y0)-(double)y_scale*(double)y2);
	
		if (py2>=ymax) break;
		if (px2>=xmax) break;
		
		// Plot the points.
		fl_line(px1,py1,px2,py2);
		
		x1=x2;
		y1=y2;
		

	}
	
	if (btMem1->value()==1){
		x1=(int)this->mem1->GetRange(0);
		y1=this->mem1->GetVelocity(0);
			
		int m=this->mem1->MaxRows();
		
		fl_color(FL_RED);
		fl_line_style(FL_SOLID,1);
		
		for (int n=1;n<x_range && n<m ;n++){
			x2=(int)this->mem1->GetRange(n);
			y2=this->mem1->GetVelocity(n);
			
			// Translate the x,y values into scaled pixels.
			px1=(int)(xmin+x_scale*(double)x1);
			px2=(int)(xmin+x_scale*(double)x2);
			
			py1=(int)((y0)-(double)y_scale*(double)y1);
			py2=(int)((y0)-(double)y_scale*(double)y2);
		
			if (py2>=ymax) break;
			if (px2>=xmax) break;
			
			// Plot the points.
			if (px1>xmin && px1<xmax && py1<ymax && py1>ymin) fl_line(px1,py1,px2,py2);
			
			x1=x2;
			y1=y2;
			

		}
		
	}
	
	
	if (btMem2->value()==1){
		x1=(int)this->mem2->GetRange(0);
		y1=this->mem2->GetVelocity(0);
			
		int m=this->mem2->MaxRows();
		
		fl_color(FL_GREEN);
		fl_line_style(FL_SOLID,1);
		
		for (int n=1;n<x_range && n<m ;n++){
			x2=(int)this->mem2->GetRange(n);
			y2=this->mem2->GetVelocity(n);
			
			// Translate the x,y values into scaled pixels.
			px1=(int)(xmin+x_scale*(double)x1);
			px2=(int)(xmin+x_scale*(double)x2);
			
			py1=(int)((y0)-(double)y_scale*(double)y1);
			py2=(int)((y0)-(double)y_scale*(double)y2);
		
			if (py2>=ymax) break;
			if (px2>=xmax) break;
			
			// Plot the points.
			if (px1>xmin && px1<xmax && py1<ymax && py1>ymin) fl_line(px1,py1,px2,py2);
			
			x1=x2;
			y1=y2;
			

		}
		
	}
	
	
	if (x_click>xmin && x_click<xmax && y_click>ymin && y_click<ymax){
		// If the user clicks, show the coordinates they clicked on.
		double click_label_x= (x_click-xmin)/x_scale;
		double click_label_y=(y0-y_click)/y_scale;
		char lbl_point[20];
		sprintf(lbl_point,"(%.0f,%.2f)",click_label_x, click_label_y);
		fl_color(FL_BLACK);
		fl_draw(lbl_point,x_click+5, y_click-5);
		fl_rectf(x_click-2, y_click-2,4,4);
	}
	
	
	
	
	// Draw the frame around it last to make it all pretty.
	fl_frame("aaaa",5,100,w-10,h-105);
	
	return;
	
	
	
	
}

void PlotWindow::cb_btPlot(Fl_Widget* o, void* v){
	PlotWindow* T = (PlotWindow*)v;
	
	T->damage(255);
	T->draw();


}

void PlotWindow::cb_nothing(Fl_Widget* o, void* v){

	;
}

void PlotWindow::cb_Close(Fl_Widget* o, void* v){
	PlotWindow* T = (PlotWindow*)v;
	T->~PlotWindow();
}

void PlotWindow::cb_slUpdate(Fl_Widget* o, void* v){

	PlotWindow* T = (PlotWindow*)v;
	T->damage(255);
	T->draw();
}

void PlotWindow::cb_Mem1(Fl_Widget* o, void* v){

	PlotWindow* T = (PlotWindow*)v;
	T->damage(255);
	T->draw();

}


void PlotWindow::cb_Mem2(Fl_Widget* o, void* v){

	PlotWindow* T = (PlotWindow*)v;
	T->redraw();

}
	
