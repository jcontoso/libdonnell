# COMPILER OPTIONS
CC ?= cc
PKGCONFIG = $(shell which pkg-config)
DEPS = fribidi fontconfig freetype2 libpng
CFLAGS = -g $(shell $(PKGCONFIG) --cflags $(DEPS)) -fvisibility=hidden -fPIC -shared -Iinclude -Iassets
LIBS = $(shell $(PKGCONFIG) --libs $(DEPS)) -ldl -lunistring

# FILE AND PROJECT NAMES
NAME = donnell
LIBTARGET = lib$(NAME).so 
PCTARGET = $(NAME).pc

# FILES
SOURCES = $(wildcard src/*.c)

# PATHS
PREFIX = /usr
PKGCONFIG_PATHS = $(shell  $(PKGCONFIG) --variable pc_path pkg-config)
PKGCONFIG_PATHS_LIST := $(subst :, ,$(PKGCONFIG_PATHS:v%=%))
PKGCONFIG_PATH = $(word 1,$(PKGCONFIG_PATHS_LIST))

# VERSION INFORMATION
MAJOR_VERSION = 0
MINOR_VERSION = 0
VERSION_CFLAGS = -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION)

# PKGCONFIG FILE
define PCFILE
prefix=$(PREFIX)
exec_prefix=$${prefix}
includedir=$${prefix}/include
libdir=$${prefix}/lib

Name: $(TARGET)
Description: A simple C library for drawing graphics
Version: $(MAJOR_VERSION).$(MINOR_VERSION)
Cflags: -I$${includedir}
Libs: -L$${libdir} -ldonnell
endef

# EXAMPLES
EXAMPLES_SOURCES = examples/example.c examples/example-titlebar.c
EXAMPLES = $(basename $(EXAMPLES_SOURCES))
EXAMPLE_CFLAGS = -g $(shell $(PKGCONFIG) --cflags $(NAME))
EXAMPLE_LIBS = $(shell $(PKGCONFIG) --libs $(NAME))

EXAMPLES_X11_SOURCES = examples/example-titlebar-x11.c examples/example-x11.c
EXAMPLES_X11 = $(basename $(EXAMPLES_X11_SOURCES))
EXAMPLE_X11_CFLAGS = -g $(shell $(PKGCONFIG) --cflags $(NAME) x11)
EXAMPLE_X11_LIBS = $(shell $(PKGCONFIG) --libs $(NAME) x11)

# RULES
all: $(LIBTARGET) $(PCTARGET) 

clean:
	rm -f $(OBJECTS) $(LIBTARGET) $(PCTARGET)
	rm -f $(EXAMPLES) $(EXAMPLES_X11)
	rm -f donnell.pc
	rm -f example.png
	rm -f examples/example2.png	
	rm -f examples/example.png
	rm -f example2.png
	rm -f examples/titlebar.png
	rm -f titlebar.png
		
uninstall:
	rm -f $(word 1,$(PKGCONFIG_PATHS_LIST))/$(PCTARGET)
	rm -f $(PREFIX)/include/donnell.h
	rm -f $(PREFIX)/lib/$(LIBTARGET)

install: all $(PKGCONFIG_PATH)
	install $(PCTARGET) $(PKGCONFIG_PATH)/$(PCTARGET)
	install include/donnell.h $(PREFIX)/include/donnell.h
	install $(LIBTARGET) $(PREFIX)/lib/$(LIBTARGET)
	
examples: $(EXAMPLES) $(EXAMPLES_X11)
	
$(LIBTARGET): $(SOURCES)
	$(CC) $(^) -o ${LIBTARGET} ${CFLAGS} ${VERSION_CFLAGS} ${LIBS}

$(PCTARGET):
	$(file > $@,$(PCFILE))
    
$(EXAMPLES): %: %.c
	$(CC) $< -o $@ ${EXAMPLE_CFLAGS} ${EXAMPLE_LIBS}

$(EXAMPLES_X11): %: %.c
	$(CC) $< -o $@ ${EXAMPLE_X11_CFLAGS} ${EXAMPLE_X11_LIBS}

$(PKGCONFIG_PATH):
	mkdir -p $(PKGCONFIG_PATH)
        
.PHONY: clean uninstall install examples
