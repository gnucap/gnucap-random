GNUCAP_CONF = $(shell which gnucap-conf$(SUFFIX))

include Make.override

ifneq ($(GNUCAP_CONF),)
    CXX = $(shell $(GNUCAP_CONF) --cxx)
    GNUCAP_CPPFLAGS = $(shell $(GNUCAP_CONF) --cppflags) -DADD_VERSION -DPIC
    GNUCAP_CXXFLAGS = $(shell $(GNUCAP_CONF) --cxxflags)
	 GNUCAP_LIBDIR   = $(shell $(GNUCAP_CONF) --libdir)
else
    $(info no gnucap-conf, this will not work.)
    $(info please install gnucap...)
endif

GNUCAP_CXXFLAGS+= -fPIC -shared

CLEANFILES = f_random.so
MODULES = f_random.so

all: f_random.so

LIBS=-lgsl -lgslcblas -lm
f_random.so : f_random.cc
	$(CXX) $(CXXFLAGS) $(GNUCAP_CXXFLAGS) $(CPPFLAGS) $(GNUCAP_CPPFLAGS) -o $@ f_random.cc $(LIBS)

GNUCAP_PKGLIBDIR = $(GNUCAP_LIBDIR)/gnucap

install : $(MODULES)
	install -d $(DESTDIR)$(GNUCAP_PKGLIBDIR)
	install $(MODULES) $(DESTDIR)$(GNUCAP_PKGLIBDIR)

uninstall :
	(cd $(DESTDIR)$(GNUCAP_PKGLIBDIR) ; rm $(MODULES))

clean :
	rm -f $(CLEANFILES)

# not yet. please consider in packaging
check:
	@:

distclean: clean
	rm Make.override

Make.override:
	[ -e $@ ] || echo "# here you may override settings" > $@
