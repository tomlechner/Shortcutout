#### Shortcutout/Makefile


#LAXDIR= ../lax
LAXDIR= ./laxkit/lax
LAXIDIR=$(LAXDIR)/interfaces
LAXOBJDIR=$(LAXDIR)

LD=g++

EXTRA_LAX_CFLAGS= -I/usr/include/GraphicsMagick
EXTRA_LAX_LIBS= -lGraphicsMagick++ -lGraphicsMagick

LDFLAGS= -L/usr/X11R6/lib -L$(LAXDIR) -L$(LAXIDIR) -lX11 -lm -lcairo `pkg-config --libs freetype2` `imlib2-config --libs` -lfontconfig -lXext \
                -lXi -lXft -lXrandr -lcrypto -lharfbuzz -lsqlite3 $(OPTIONALLIBS) $(EXTRA_LAX_LIBS)
DEBUGFLAGS= -g -gdwarf-2
CPPFLAGS= $(EXTRA_LAX_LIBS) $(DEBUGFLAGS) -I$(LAXDIR)/.. -std=c++11 `pkg-config --cflags freetype2`


all: shortcuts

typethis: lax laxinterface typethis.o keyboardwindow.o keys.cc
	$(LD) $(CPPFLAGS) typethis.o keyboardwindow.o -llaxinterfaces -llaxkit $(LDFLAGS) -o $@

shortcuts: lax laxinterface shortcuts.o 
	$(LD) $(CPPFLAGS) shortcuts.o -llaxinterfaces -llaxkit $(LDFLAGS) -o $@




#-----------------------------------------------


lax:
	cd $(LAXDIR) && $(MAKE)

laxinterface:
	cd $(LAXDIR)/interfaces && $(MAKE)



#-----------------------------------------------

.PHONY: clean all
clean:
	rm -f *.o
	rm -f typethis
	rm -f shortcuts
