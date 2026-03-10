CFLAGS += -I../..
ifeq ($(OS),Windows_NT)
	LDFLAGS += -s -lopengl32 -lgdi32
	TARGET = stickit-windows.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS += -framework OpenGL -framework Cocoa
		TARGET = stickit-macos
	else ifeq ($(UNAME_S),Linux)
		LDFLAGS += -s -lGLU -lGL -lX11
		TARGET = stickit-linux
	endif
endif

$(TARGET): main.c tigr/tigr.c
	gcc $^ -O3 -s -o $@ $(CFLAGS) $(LDFLAGS)

$(TARGET)-debug: main.c tigr/tigr.c
	gcc $^ -g -o $@ $(CFLAGS) $(LDFLAGS)

dev: main.c tigr/tigr.c
	tcc $^ -Os -o $@ $(CFLAGS) $(LDFLAGS) && ./dev

clang-dbg: main.c tigr/tigr.c
	clang $^ -O3 -s -o $@ $(CFLAGS) $(LDFLAGS)
