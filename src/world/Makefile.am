INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir) -I$(srcdir)/../shared/config -I$(srcdir)/../shared/packet -I$(srcdir)/../shared/game -I$(srcdir)/../shared/database -I$(srcdir)/../shared/sample


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
				   npc.h \
				   player.h \
				   player.cpp \
				   map.h \
				   map.cpp \
				   mapmgr.h \
				   mapmgr.cpp \
				   mapconf.h \
				   mapconf.cpp \
				   worldauth.h \
				   worldauth.cpp \
			       main.cpp

srdgame_worldserver_LDADD = -llog -lnetwork -lthread -lconfig -lpacket -lgame -lmysqlclient -ldatabase -lsample -lpthread -ldl -lz -lrt
srdgame_worldserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/network -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config -L$(srcdir)/../shared/packet -L$(srcdir)/../shared/game -L$(srcdir)/../shared/database -L$(srcdir)/../shared/sample
