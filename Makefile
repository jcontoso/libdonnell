CC ?= cc
PKGCONFIG = $(shell which pkg-config)

DEPS = fribidi fontconfig freetype2
CFLAGS = $(shell $(PKGCONFIG) --cflags $(DEPS)) -fvisibility=hidden -fPIC -shared -Iinclude
LIBS = $(shell $(PKGCONFIG) --libs $(DEPS)) -ldl

NAME = donnell
LIBTARGET = lib$(NAME).so 
PCTARGET = $(NAME).pc
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
OBJECTS = $(SOURCES:.c=.o)

PREFIX = /usr
MAJOR_VERSION = 0
MINOR_VERSION = 0

define PCFILE
prefix=$(PREFIX)
exec_prefix=$${prefix}
includedir=$${prefix}/include
libdir=$${prefix}/lib

Name: $(TARGET)
Description: A simple C library for drawing graphics
Version: $(MAJOR_VERSION).$(MINOR_VERSION)
Requires: $(DEPS)
Cflags: -I$${includedir}
Libs: -L$${libdir} -ldonnell
endef

all: $(LIBTARGET) $(PCTARGET)

clean:
	rm -f $(OBJECTS) $(LIBTARGET) $(PCTARGET)

uninstall:
	rm -f /usr/share/pkgconfig/$(PCTARGET)
	rm -f $(PREFIX)/include/donnell.h

install: all
	install -Dm0644 $(PCTARGET) /usr/share/pkgconfig/$(PCTARGET)
	install -Dm0644 include/donnell.h $(PREFIX)/include/donnell.h
	install -Dm0644 $(LIBTARGET) $(PREFIX)/lib/$(LIBTARGET)
	
$(LIBTARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) ${LIBS} -o $(LIBTARGET) $(OBJECTS)

$(PCTARGET):
	$(file > $@,$(PCFILE))
    
.PHONY: clean uninstall install
