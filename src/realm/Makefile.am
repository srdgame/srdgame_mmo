INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir) -I$(srcdir)/../shared/config


noinst_PROGRAMS = srdgame-realmserver

srdgame_realmserver_SOURCES  = realmserver.h \
			       realmserver.cpp \
			       realmsocket.h \
			       realmsocket.cpp \
			       realmworker.h \
			       realmworker.cpp \
			       realmmgr.h \
			       realmmgr.cpp \
			       main.cpp

srdgame_realmserver_LDADD = -llog -lnetwork -lthread -lconfig -lpthread
srdgame_realmserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/network -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config
