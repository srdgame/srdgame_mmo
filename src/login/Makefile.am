INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir) -I$(srcdir)/../shared/config -I$(srcdir)/../shared/packet


noinst_PROGRAMS = srdgame-loginserver

srdgame_loginserver_SOURCES  = loginserver.h \
			       loginserver.cpp \
			       loginsocket.h \
			       loginsocket.cpp \
			       loginworker.h \
			       loginworker.cpp \
			       loginmgr.h \
			       loginmgr.cpp \
			       main.cpp

srdgame_loginserver_LDADD = -llog -lnetwork -lthread -lconfig -lpacket -lpthread
srdgame_loginserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/network -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config -L$(srcdir)/../shared/packet
