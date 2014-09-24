# A makefile to buid ff.

#Copyright 2014 Lauri Gustafsson, Kalle Korhonen and Julius Heino
#
#This file is part of Fedora Fighters.
#
#    Fedora Fighters is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Fedora Fighters is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Fedora Fighters, see COPYING. If not, see <http://www.gnu.org/licenses/>.
#

SRCS=main.cpp gfxutil.cpp stage.cpp game_data.cpp util.cpp player.cpp config.cpp game.cpp
TARGET=fedf
CFLAGS=
INCLUDES=-I./
LFLAGS=
LIBS=-lSDL -lSDL_image
CC=g++
STRIP=strip
lin:TARGET=fedf
lin:CFLAGS=-O2 -fomit-frame-pointer
lin:INCLUDES=-I./
lin:LFLAGS=
lin:LIBS=-lSDL -lSDL_image
lin:CC=g++
lin:STRIP=strip
lindbhack:TARGET=fedf-dbhack-windowed
lindbhack:CFLAGS=-O2 -fomit-frame-pointer -DDBHACK -DNOFS
lindbhack:INCLUDES=-I./
lindbhack:LFLAGS=
lindbhack:LIBS=-lSDL -lSDL_image
lindbhack:CC=g++
lindbhack:STRIP=strip
linwin:TARGET=fedf-windowed
linwin:CFLAGS=-O2 -fomit-frame-pointer -DNOFS
linwin:INCLUDES=-I./
linwin:LFLAGS=
linwin:LIBS=-lSDL -lSDL_image
linwin:CC=g++
linwin:STRIP=strip
debug:TARGET=fedfd
debug:CFLAGS=-g -DDEBUG
win32:TARGET=fedf.exe
win32:CFLAGS=-O3 -fomit-frame-pointer -funroll-loops
win32:INCLUDES=-I/opt/win32/include -I./ -I/opt/win32/include/SDL
win32:LFLAGS=-L/opt/win32/lib
win32:LIBS=-lmingw32 -lSDLmain -lSDL -lSDL_image
win32:CC=i586-mingw32msvc-g++
win32:STRIP=i586-mingw32msvc-strip
win32dbhack:TARGET=fedf-dbhack-windowed.exe
win32dbhack:CFLAGS=-O3 -fomit-frame-pointer -funroll-loops -DDBHACK -DNOFS
win32dbhack:INCLUDES=-I/opt/win32/include -I./ -I/opt/win32/include/SDL
win32dbhack:LFLAGS=-L/opt/win32/lib
win32dbhack:LIBS=-lmingw32 -lSDLmain -lSDL -lSDL_image
win32dbhack:CC=i586-mingw32msvc-g++
win32dbhack:STRIP=i586-mingw32msvc-strip
win32win:TARGET=fedf-windowed.exe
win32win:CFLAGS=-O3 -fomit-frame-pointer -funroll-loops -DNOFS
win32win:INCLUDES=-I/opt/win32/include -I./ -I/opt/win32/include/SDL
win32win:LFLAGS=-L/opt/win32/lib
win32win:LIBS=-lmingw32 -lSDLmain -lSDL -lSDL_image
win32win:CC=i586-mingw32msvc-g++
win32win:STRIP=i586-mingw32msvc-strip
OBJS=$(SRCS:.cpp=.o)

lin: $(TARGET)
	$(STRIP) --strip-all $(TARGET)
	@echo "Build finished."
	
lindbhack: $(TARGET)
	$(STRIP) --strip-all $(TARGET)
	@echo "Build finished."
	
linwin: $(TARGET)
	$(STRIP) --strip-all $(TARGET)
	@echo "Build finished."

win32: $(TARGET)
	$(STRIP) --strip-all $(TARGET)
	upx $(TARGET)
	cp /opt/win32/bin/SDL.dll ./
	cp /opt/win32/bin/SDL_image.dll ./
	cp /opt/win32/bin/zlib1.dll ./
	cp /opt/win32/bin/libpng*.dll ./
	cp /opt/win32/bin/SDL_mixer.dll ./
	@echo "Finished."

win32dbhack: $(TARGET)
	$(STRIP) --strip-all $(TARGET)
	upx $(TARGET)
	cp /opt/win32/bin/SDL.dll ./
	cp /opt/win32/bin/SDL_image.dll ./
	cp /opt/win32/bin/zlib1.dll ./
	cp /opt/win32/bin/libpng*.dll ./
	cp /opt/win32/bin/SDL_mixer.dll ./
	@echo "Finished."

win32win: $(TARGET)
	$(STRIP) --strip-all $(TARGET)
	upx $(TARGET)
	cp /opt/win32/bin/SDL.dll ./
	cp /opt/win32/bin/SDL_image.dll ./
	cp /opt/win32/bin/zlib1.dll ./
	cp /opt/win32/bin/libpng*.dll ./
	cp /opt/win32/bin/SDL_mixer.dll ./
	@echo "Finished."

debug: $(TARGET)
	@echo "Debug build finished."

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
clean:
	rm ./*.o ./*~ -f
