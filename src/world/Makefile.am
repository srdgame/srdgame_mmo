INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir) -I$(srcdir)/../shared/config -I$(srcdir)/../shared/packet


noinst_PROGRAMS = srdgame-worldserver

srdgame_worldserver_SOURCES  = worldserver.h \
			       worldserver.cpp \
			       worldsocketbase.h \
			       worldsocketbase.cpp \
			       worldsocket.h \
			       worldsocket.cpp \
			       intersocket.h \
			       intersocket.cpp \
			       worldworker.h \
			       worldworker.cpp \
			       worldmgr.h \
			       worldmgr.cpp \
			       main.cpp

srdgame_worldserver_LDADD = -llog -lnetwork -lthread -lconfig -lpacket -lpthread -ldl
srdgame_worldserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/network -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config -L$(srcdir)/../shared/packet
