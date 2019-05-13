#####################################################
# Makefile for the templates of QtGrace             #
#####################################################
# You should not change anything here.              #
#####################################################

TOP=..

include $(TOP)/Make.conf

TEMPLATES = Default.agr \
	QtGrace_std_bin_format_qtgrace.fmt \
	QtGrace_std_bin_format_wav.fmt

all : dummy

tests : dummy

clean : dummy

distclean : dummy

devclean : dummy

install : $(TEMPLATES)
	$(MKINSTALLDIRS) $(DESTDIR)$(GRACE_HOME)/templates
 	for file in $(TEMPLATES); do \
 		echo "Installing $$file"; \
 		$(INSTALL_DATA) $$file $(DESTDIR)$(GRACE_HOME)/templates; \
 	done;

links : dummy

dummy :
