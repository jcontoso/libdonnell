# COMPILER OPTIONS
CC ?= cc
PKGCONFIG = $(shell which pkg-config)
DEPS = fribidi fontconfig freetype2
CFLAGS = -g $(shell $(PKGCONFIG) --cflags $(DEPS)) -fvisibility=hidden -fPIC -shared -Iinclude
LIBS = $(shell $(PKGCONFIG) --libs $(DEPS)) -ldl

# FILE AND PROJECT NAMES
NAME = donnell
LIBTARGET = lib$(NAME).so 
PCTARGET = $(NAME).pc

# FILES
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)

# PATHS
PREFIX = /usr
PKGCONFIG_PATHS = $(shell  $(PKGCONFIG) --variable pc_path pkg-config)
PKGCONFIG_PATHS_LIST := $(subst :, ,$(PKGCONFIG_PATHS:v%=%))
PKGCONFIG_PATH = $(word 1,$(PKGCONFIG_PATHS_LIST))

# VERSION INFORMATION
MAJOR_VERSION = 0
MINOR_VERSION = 0

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
EXAMPLE_SOURCES = $(wildcard examples/*.c)
EXAMPLES = $(basename $(EXAMPLE_SOURCES))
EXAMPLE_CFLAGS = $(shell $(PKGCONFIG) --cflags $(NAME))
EXAMPLE_LIBS = $(shell $(PKGCONFIG) --libs $(NAME))

# RULES
all: $(LIBTARGET) $(PCTARGET) $(PKGCONFIG_PATH)

clean:
	rm -f $(OBJECTS) $(LIBTARGET) $(PCTARGET)
	rm -f $(EXAMPLES)
	rm -f examples/test.bmp
	rm -f donnell.pc
	rm -f test.bmp

uninstall:
	rm -f $(word 1,$(PKGCONFIG_PATHS_LIST))/$(PCTARGET)
	rm -f $(PREFIX)/include/donnell.h
	rm -f $(PREFIX)/lib/$(LIBTARGET)

install: all
	install $(PCTARGET) $(PKGCONFIG_PATH)/$(PCTARGET)
	install include/donnell.h $(PREFIX)/include/donnell.h
	install $(LIBTARGET) $(PREFIX)/lib/$(LIBTARGET)
	
examples: $(EXAMPLES)
	
$(LIBTARGET): $(SOURCES)
	$(CC) $(^) -o ${LIBTARGET} ${CFLAGS} ${LIBS}

$(PCTARGET):
	$(file > $@,$(PCFILE))
    
$(EXAMPLES): %: %.c
	$(CC) $< -o $@ ${EXAMPLE_CFLAGS} ${EXAMPLE_LIBS}
        
$(PKGCONFIG_PATH):
	mkdir -p $(PKGCONFIG_PATH)
        
.PHONY: clean uninstall install examples
