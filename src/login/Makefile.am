INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir) -I$(srcdir)/../shared/config -I$(srcdir)/../shared/packet -I$(srcdir)/../shared/database -I$(srcdir)/../sample


noinst_PROGRAMS = srdgame-loginserver

srdgame_loginserver_SOURCES  = loginserver.h \
			       loginserver.cpp \
				   loginsocketbase.h \
				   loginsocketbase.cpp \
			       loginsocket.h \
			       loginsocket.cpp \
				   intersocket.h \
				   intersocket.cpp \
			       loginworker.h \
			       loginworker.cpp \
			       loginmgr.h \
			       loginmgr.cpp \
				   loginauth.h \
				   loginauth.cpp \
				   accountmgr.h \
				   accountmgr.cpp \
				   security.h \
				   security.cpp \
				   userinfo.h \
				   userinfo.cpp \
				   sample/rosql.h \
				   sample/rosql.cpp \
			       main.cpp

srdgame_loginserver_LDADD = -llog -lnetwork -lthread -lconfig -lpacket -ldatabase -lpthread -ldl -lmysqlclient
srdgame_loginserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/network -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config -L$(srcdir)/../shared/packet -L$(srcdir)/../shared/database
