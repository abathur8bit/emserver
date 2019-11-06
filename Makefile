#********************************************************************
#       Copyright (c) 2000, Ant Works Software
#	http://www.antws.com
#	Author: Lee Patterson (lee@antws.com)
#
#       file name      : $Source: /usr/local/cvsroot/easyemud/server1/Makefile,v $ 
#       file version   : $Revision: 1.3 $ 
#       date changed   : $Date: 2006/04/19 04:53:32 $ 
#       last change by : $Author: lpatterson $ 
#
#*********************************************************************

#when adding a new source file, you have to rebuild the dependancy
#easiest way is to 'make clean all' to rebuild everything,
#or 'rm makefile.depend'

TERM = dumb
VERSION   = '"0.01.00"'
TITLE     = '"EasyeMud Server"'

PROGRAM      := emserver
PROGRAM_DBG  := em_dbg

#//TODO: remove unneeded Cross files

PROGRAM_SRCS = \
	area.cpp \
	clienthandler.cpp \
	emscript.cpp \
	emserver.cpp \
	exparse.cpp \
	mudobject.cpp \
	player.cpp \
	room.cpp \
	smallbasic.cpp \
	world.cpp \
	../common/stringtokenizer.cpp \
	main.cpp
#
#-----------------------------------------------------
#

ALL_SRCS    = $(PROGRAM_SRCS)

#
# Convert the source files into object files
#
PROGRAM_OBJS     := $(PROGRAM_SRCS:.cpp=.o)
PROGRAM_OBJS_DBG := $(PROGRAM_SRCS:.cpp=_dbg.o)

#
# Include and library paths used in source
#
INCLUDES := -I../../.. -I. -I.. 
LIBS_DBG := -L../../../ssobjects -lssobjects_dbg -lpthread
LIBS     := -L../../../ssobjects -lssobjects     -lpthread

#
# Build options
#
#WARNINGS = -W -Wall -Woverloaded-virtual -Weffc++
WARNINGS = -W -Wall -Woverloaded-virtual 
CCFLAGS_DBG := $(INCLUDES) $(WARNINGS) -g -fPIC  -D_REENTRANT -DDEBUG  -DUSE_NCURSES -DVERSION='"$(VERSION)"' -DTITLE='"$(TITLE)"'
CCFLAGS     := $(INCLUDES) $(WARNINGS) -fPIC -O2 -D_REENTRANT -DNDEBUG -DUSE_NCURSES -DVERSION='"$(VERSION)"' -DTITLE='"$(TITLE)"'

#
# Compiler used
#
CC := g++

#
# Targets
#
none         : debug
all          : debug release
release      : tags $(PROGRAM)
debug        : tags $(PROGRAM_DBG)
makefile.depend : depend

#
# Build commands
#
%.o : %.cpp
	@echo "Compiling release $<"
	@$(CC) -c $(CCFLAGS) $< -o $@

%_dbg.o : %.cpp
	@echo "Compiling debug $<"
	@$(CC) -c $(CCFLAGS_DBG) $< -o $@

$(PROGRAM): $(PROGRAM)
	@echo "*** Building release $(PROGRAM)"
	@$(CC) -o $(PROGRAM) $(PROGRAM_OBJS) $(LIBS) -lpthread

$(PROGRAM_DBG): $(PROGRAM_OBJS_DBG)
	@echo "*** Building debug $(PROGRAM_DBG)"
	@$(CC) -o $(PROGRAM_DBG) $(PROGRAM_OBJS_DBG) $(LIBS_DBG) 

tidy :
	@echo "Removing object files"
	@rm -f *.o

clean : 
	@echo "Removing all intermediate files, leaving depenancy"
	@rm -f  $(SERVER) $(SERVER_OBJS) $(SERVER_DBG) $(SERVER_OBJS_DBG) \
		$(PROGRAM) $(PROGRAM_OBJS) $(PROGRAM_DBG) $(PROGRAM_OBJS_DBG) \
		core TAGS *.log

cleanall : 
	@echo "Removing all intermediate files"
	@rm -f  $(SERVER) $(SERVER_OBJS) $(SERVER_DBG) $(SERVER_OBJS_DBG) \
		$(PROGRAM) $(PROGRAM_OBJS) $(PROGRAM_DBG) $(PROGRAM_OBJS_DBG) \
		core makefile.depend TAGS *.log *~

tags : 
	@echo Starting build at:
	@date
	@echo "Building tags file"
	@etags *.cpp *.h

depend: 
	@if [ ! -f makefile.depend ]; then \
	echo "Building dependancies"; \
	rm -f makefile.depend; \
	for i in $(ALL_SRCS) ; \
	do \
	$(CC) -DUSE_NCURSES $(INCLUDES) -MM -c $$i >> makefile.depend ; \
	$(CC) -DUSE_NCURSES $(INCLUDES) -MM -c $$i | sed 's/\.o/_dbg\.o/' >> makefile.depend ; \
	done; \
	fi

-include makefile.depend
