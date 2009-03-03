INCLUDES = $(SRDGAME_INCLUDES) -I$(srcdir)/../shared/log -I$(srcdir)/../shared/network -I$(srcdir)/../shared/thread -I$(srcdir)/../shared/util -I$(srcdir)


noinst_PROGRAMS = srdgame-loginserver

srdgame_loginserver_SOURCES  = \
	main.cpp

srdgame_loginserver_LDADD = -llog -lthread -lconfig -lpthread
srdgame_loginserver_LDFLAGS = -L$(srcdir)/../shared/log -L$(srcdir)/../shared/thread -L$(srcdir)/../shared/config
