INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir) -I$(srcdir)/../shared/config -I$(srcdir)/../shared/packet -I$(srcdir)/../sample


noinst_PROGRAMS = srdgame-realmserver

srdgame_realmserver_SOURCES  = realmserver.h \
			       realmserver.cpp \
			       realmsocketbase.h \
			       realmsocketbase.cpp \
			       realmsocket.h \
			       realmsocket.cpp \
			       realmsocketl.h \
			       realmsocketl.cpp \
			       realmsocketw.h \
			       realmsocketw.cpp \
			       realmworker.h \
			       realmworker.cpp \
			       realmmgr.h \
			       realmmgr.cpp \
			       main.cpp

srdgame_realmserver_LDADD = -llog -lnetwork -lthread -lconfig -lpacket -lpthread -ldl
srdgame_realmserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/network -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config -L$(srcdir)/../shared/packet
