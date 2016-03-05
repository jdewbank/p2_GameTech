noinst_HEADERS = Application.h Ball.h PlayingField.h Paddle.h BaseApplication.h PhysicsWorld.h SoundPlayer.h Scoreboard.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS =  -I$(top_srcdir)
assignment2_CPPFLAGS += -I/usr/include/SDL
assignment2_SOURCES = Application.cpp Ball.cpp PlayingField.cpp Paddle.cpp BaseApplication.cpp PhysicsWorld.cpp SoundPlayer.cpp Scoreboard.cpp
assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system
assignment2_LDFLAGS += -lSDL -lSDL_mixer

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
