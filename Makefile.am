debug=false

SUBDIRS = ./lib/ballistics/
bin_PROGRAMS = gebc

# Install the executable one directory up
gebcprogdir=../

# Define the sources.
gebc_SOURCES = \
	InputWindow.cpp \
	PlotWindow.cpp \
	RangeWindow.cpp \
	LicenseWindow.cpp \
	AboutWindow.cpp \
	PBRWindow.cpp \
	GBCSolution.cpp \
	GNU_Ballistics.cpp \
	InputWindow.h \
	PlotWindow.h \
	RangeWindow.h \
	LicenseWindow.h \
	AboutWindow.h \
	PBRWindow.h \
	GBCSolution.h 

gebcdir = ./lib/ballistics

AM_CFLAGS= -O3
AM_CXXFLAGS = -O3
AM_LDFLAGS= -L./lib/ballistics 
LDADD= -lhpdf -lballistics
