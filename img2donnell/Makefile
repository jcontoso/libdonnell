CC ?= cc
SOURCES = $(wildcard src/*.c)
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags libpng)
LIBS = $(shell $(PKGCONFIG) --libs libpng)
PACKAGE = img2donnell

all: ${PACKAGE}

${PACKAGE}: $(SOURCES)
	$(CC) $(^) -o ${PACKAGE} ${CFLAGS} ${LIBS}
	
clean:
	rm -f ${PACKAGE}
		
uninstall:
	rm -f /usr/local/bin/${PACKAGE}
	
install: all
	install -Dm0755 ${PACKAGE} /usr/local/bin/${PACKAGE}
	
.PHONY: clean install uninstall
