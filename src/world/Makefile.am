INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir)


noinst_PROGRAMS = srdgame-worldserver

srdgame_worldserver_SOURCES  = \
	main.cpp

srdgame_worldserver_LDADD = -llog -lthread -lconfig -lpthread
srdgame_worldserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config
