FILES = $(wildcard ./*)
MAKE = make 

all:
	@for i in $(FILES); do \
	if [ -d $$i ]; then \
	$(MAKE) $@ -C $$i; \
	fi \
	done

clean:
	@for i in $(FILES); do \
	if [ -d $$i ]; then \
	$(MAKE) $@ -C $$i; \
	fi \
	done

strip:
	@for i in $(FILES); do \
	if [ -d $$i ]; then \
	$(MAKE) $@ -C $$i; \
	fi \
	done

distclean:
	@for i in $(FILES); do \
	if [ -d $$i ]; then \
	svn delete --force  $$i/$$i; \
	$(MAKE) $@ -C $$i; \
	fi \
	done
