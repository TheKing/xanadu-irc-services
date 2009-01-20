# Makefile for language module

MAKEARGS = 'CFLAGS=${CFLAGS}' 'CC=${CC}' 'ANOPELIBS=${ANOPELIBS}' \
	   'LDFLAGS=${LDFLAGS}' 'BINDEST=${BINDEST}' 'INSTALL=${INSTALL}' \
           'INCLUDEDIR=${INCLUDEDIR}' 'RM=${RM}' 'CP=${CP}' \
     	   'TOUCH=${TOUCH}' 'SHELL=${SHELL}' 'DATDEST=${DATDEST}' \
   	   'RUNGROUP=${RUNGROUP}' 'USE_MODULES=${USE_MODULES}'

LANGOBJS = en_us
LANGSRCS = en_us.l

LANGCOMP = ./langcomp
#LANGCOMP = ./langcomp -w


all: $(LANGOBJS)

distclean: clean spotless

install: all
	test -d $(DATDEST)/languages || mkdir $(DATDEST)/languages
	@if [ "$(RUNGROUP)" ] ; then \
		echo chgrp -R $(RUNGROUP) $(DATDEST)/languages ; \
		chgrp -R $(RUNGROUP) $(DATDEST)/languages ; \
		echo chmod -R g+rw $(DATDEST)/languages ; \
		chmod -R g+rw $(DATDEST)/languages ; \
		$(CP) $(LANGOBJS) $(DATDEST)/languages ; \
		echo chgrp -R $(RUNGROUP) $(DATDEST)/languages ; \
		chgrp -R $(RUNGROUP) $(DATDEST)/languages/* ; \
		echo chmod -R g+rw $(DATDEST)/languages/* ; \
		chmod -R g+rw $(DATDEST)/languages/* ; \
	fi
	@if [ ! "$(RUNGROUP)" ] ; then \
		chmod 700 $(DATDEST)/languages/ ; \
		$(CP) $(LANGOBJS) $(DATDEST)/languages ; \
		chmod 600 $(DATDEST)/languages/* ; \
	fi
	
clean:
	rm -f $(LANGOBJS) langcomp language.h

spotless: clean
	rm -f index


en_us:    en_us.l    langcomp index
	./langcomp $@.l

langcomp: langcomp.c
	$(CC) $(CFLAGS) langcomp.c -o $@


language.h: index Makefile
	@perl -e <index >$@ 'print STDERR "Generating language.h... "; $$i=0; while (<>) { chop; printf "#define %-32s %d\n", $$_, $$i++; } print "\n#define NUM_STRINGS $$i\n"; print STDERR "$$i strings\n";'
	cp language.h ../include

index: en_us.l
	grep '^[A-Z]' en_us.l >index
