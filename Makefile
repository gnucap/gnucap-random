GNUCAP_CONF = $(shell which gnucap-conf$(SUFFIX))

include Make.override

ifneq ($(GNUCAP_CONF),)
    CXX = $(shell $(GNUCAP_CONF) --cxx)
    GNUCAP_CPPFLAGS = $(shell $(GNUCAP_CONF) --cppflags) -DADD_VERSION -DPIC
    GNUCAP_CXXFLAGS = $(shell $(GNUCAP_CONF) --cxxflags)
	 GNUCAP_LIBDIR   = $(shell $(GNUCAP_CONF) --libdir)
else
    $(info no gnucap-conf, this might not work)
    CXX = g++
    GNUCAP_CXXFLAGS = \
        -g -O0 \
        -Wall -Wextra \
        -Wswitch-enum -Wundef -Wpointer-arith -Woverloaded-virtual \
        -Wcast-qual -Wcast-align -Wpacked -Wshadow -Wconversion \
        -Winit-self -Wmissing-include-dirs -Winvalid-pch \
        -Wvolatile-register-var -Wstack-protector \
        -Wlogical-op -Wvla -Woverlength-strings -Wsign-conversion
    GNUCAP_CPPFLAGS = \
        -DHAVE_LIBREADLINE \
        -DUNIX \
        -DTRACE_UNTESTED
	 GNUCAP_LIBDIR=/usr/share/gnucap
endif

GNUCAP_CXXFLAGS+= -fPIC -shared

CLEANFILES = f_random.so
MODULES = f_random.so

all: f_random.so

LIBS=-lgsl -lgslcblas -lm
f_random.so : f_random.cc
	$(CXX) $(CXXFLAGS) $(GNUCAP_CXXFLAGS) $(CPPFLAGS) $(GNUCAP_CPPFLAGS) -o $@ f_random.cc $(LIBS)

install : $(MODULES)
	install -d $(DESTDIR)$(GNUCAP_LIBDIR)
	install $(MODULES) $(DESTDIR)$(GNUCAP_LIBDIR)

uninstall :
	(cd $(DESTDIR)$(GNUCAP_LIBDIR) ; rm $(MODULES))

clean :
	rm -f $(CLEANFILES)

distclean: clean
	rm Make.override

Make.override:
	[ -e $@ ] || echo "# here you may override settings" > $@
