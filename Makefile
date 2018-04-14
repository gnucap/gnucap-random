GNUCAP_CONF = $(shell which gnucap-conf$(SUFFIX))
PACKAGE_NAME = gnucap-random

include Make.override

ifneq ($(GNUCAP_CONF),)
    CXX = $(shell $(GNUCAP_CONF) --cxx)
    GNUCAP_CPPFLAGS = $(shell $(GNUCAP_CONF) --cppflags) -DADD_VERSION -DPIC
    GNUCAP_CXXFLAGS = $(shell $(GNUCAP_CONF) --cxxflags)
	 GNUCAP_PKGLIBDIR   = $(shell $(GNUCAP_CONF) --pkglibdir)
# TODO complete gnucap-conf
	 GNUCAP_PREFIX   = $(shell $(GNUCAP_CONF) --exec-prefix)# BUG, should be prefix!
	 GNUCAP_DOCDIR = $(GNUCAP_PREFIX)/share/doc
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

EXAMPLES = randomtest.gc

install : $(MODULES) $(EXAMPLES) README
	install -d $(DESTDIR)$(GNUCAP_PKGLIBDIR)
	install $(MODULES) $(DESTDIR)$(GNUCAP_PKGLIBDIR)

	install -d $(DESTDIR)$(GNUCAP_DOCDIR)/$(PACKAGE_NAME)
	install README $(DESTDIR)$(GNUCAP_DOCDIR)/$(PACKAGE_NAME)

	install -d $(DESTDIR)$(GNUCAP_DOCDIR)/$(PACKAGE_NAME)/examples
	install $(EXAMPLES) $(DESTDIR)$(GNUCAP_DOCDIR)/$(PACKAGE_NAME)/examples

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
