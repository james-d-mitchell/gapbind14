#
# gapbind14: Seamless operability between C++14 and GAP
#
# This file is part of the build system of a GAP kernel extension.
# Requires automake.
#
ACLOCAL_AMFLAGS = -I cnf/m4

AM_CXXFLAGS = -I$(top_srcdir)/../../

BINARCHDIR = bin/$(GAPARCH)
GAPINSTALLLIB = $(BINARCHDIR)/gapbind14.so

lib_LTLIBRARIES = gapbind14.la

gapbind14_la_SOURCES =  src/demo.cpp
gapbind14_la_SOURCES += src/gapbind14.cpp
gapbind14_la_SOURCES += src/to_gap.cpp
gapbind14_la_CPPFLAGS = $(GAP_CPPFLAGS) -DCONFIG_H
gapbind14_la_CXXFLAGS = $(GAP_CXXFLAGS) -std=c++14 
gapbind14_la_LDFLAGS = -module -avoid-version
if SYS_IS_CYGWIN
gapbind14_la_LDFLAGS += -no-undefined -version-info 0:0:0 
gapbind14_la_LDFLAGS += -Wl,$(GAPROOT)/bin/$(GAPARCH)/gap.dll
endif

all-local: $(GAPINSTALLLIB)

$(GAPINSTALLLIB): gapbind14.la
	$(mkdir_p) $(BINARCHDIR)
if SYS_IS_CYGWIN
	cp .libs/gapbind14.dll $(GAPINSTALLLIB)
else
	cp .libs/gapbind14.so $(GAPINSTALLLIB)
endif

clean-local:
	rm -f $(GAPINSTALLLIB)

distclean-local:
	rm -rf bin/*

doc:
	($(GAPROOT)/bin/gap.sh -A makedoc.g)
