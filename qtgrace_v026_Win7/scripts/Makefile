#####################################################
# Makefile for the scripts of QtGrace               #
#####################################################
# You should not change anything here.              #
#####################################################

TOP=..

include $(TOP)/Make.conf

SCRIPTS =	Remove_Const_Offset.com \
		Remove_Linear_Offset.com \
		Remove_Sin_Offset.com

all : dummy

tests : dummy

clean : dummy

distclean : dummy

devclean : dummy

install : $(SCRIPTS)
	$(MKINSTALLDIRS) $(DESTDIR)$(GRACE_HOME)/scripts
	for i in $(SCRIPTS); do $(INSTALL_DATA) $$i $(DESTDIR)$(GRACE_HOME)/scripts; done

links : dummy

dummy :
