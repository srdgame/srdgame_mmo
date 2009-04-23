AUTOMAKE_OPTION=foreign

INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir) -I$(srcdir)/../shared/packet -I$(srcdir)/../shared/util -I$(srcdir)/../shared/crypto -I$(srcdir)/../shared/sample -I$(srcdir)/../shared/game

lib_LTLIBRARIES = libsample.la

libsample_la_SOURCES = defs.h \
					   login.h \
					   login.cpp \
					   char.h \
					   char.cpp \
					   crypto.h \
					   crypto.cpp \
					   map.h \
					   map.cpp \
					   share.h \
					   share.cpp \
					   fromdispatch.h \
					   fromdispatch.cpp \
					   todispatch.h \
					   todispatch.cpp \
					   packetdb.h \
					   packetdb.cpp \
					   main.cpp


