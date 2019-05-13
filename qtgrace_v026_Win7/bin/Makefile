#####################################################
# Makefile for binaries of QtGrace                  #
#####################################################
# You should not change anything here.              #
#####################################################

TOP=..

include $(TOP)/Make.conf

TARGET=	qtgrace

all :

clean :
	$(RM) $(TARGET)

install : qtgrace
	$(MKINSTALLDIRS) $(DESTDIR)$(GRACE_HOME)
	$(MKINSTALLDIRS) $(DESTDIR)$(PREFIX)/bin
	$(INSTALL_PROGRAM) qtgrace $(DESTDIR)$(PREFIX)/bin/

