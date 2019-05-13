#####################################################
# Makefile for documentation of Grace               #
#####################################################
# You should not change anything here.              #
#####################################################

TOP=.

include $(TOP)/Make.conf

DOCS =	\
	CHANGES \
	COPYRIGHT \
	DEVELOPERS \
	LICENSE \
	README_GRACE_5.1.22.TXT \
	README_QTGRACE.txt \
	gracerc.user \
	gracerc \
	qt_grace_ger.qm \
	qt_grace_ger.ts

SUBDIRS = src scripts doc examples fonts templates bin

all : src/pars.c
	(cd src ; qmake -unix src.pro)
	for subdir in $(SUBDIRS); do \
		echo "Making $$@ in $$subdir"; \
		$(MAKE) DESTDIR=$(DESTDIR) -C $$subdir $@ ; \
	done;

clean :
	for subdir in $(SUBDIRS); do \
		echo "Cleaning $$subdir"; \
		$(MAKE) DESTDIR=$(DESTDIR) -C $$subdir $@ ; \
	done;

install : all
	for subdir in $(SUBDIRS); do \
		echo "Making $$@ in $$subdir"; \
		$(MAKE) DESTDIR=$(DESTDIR) -C $$subdir $@ ; \
	done;
	$(MKINSTALLDIRS) $(DESTDIR)$(DOC_HOME)
	for file in $(DOCS); do \
		$(INSTALL_DATA) $$file $(DESTDIR)$(DOC_HOME); \
	done


src/pars.c: src/pars.yacc
	bison -y -t $< -o $@

