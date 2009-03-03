INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir)


noinst_PROGRAMS = srdgame-realmserver

srdgame_realmserver_SOURCES  = \
	main.cpp

srdgame_realmserver_LDADD = -llog -lthread -lconfig -lpthread
srdgame_realmserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config
