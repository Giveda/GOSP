#
# Copyright (C) 2019  明心  <imleizhang@qq.com>
# All rights reserved.
# 
# This program is an open-source software; and it is distributed in the hope 
# that it will be useful, but WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
# PURPOSE. 
# This program is not a free software; so you can not redistribute it and/or 
# modify it without my authorization. If you need use it for any 
# commercial purpose, you should first get commercial authorization from
# me, otherwise there will be your's credit and legal risks. 
#
#

FILES = src example
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
	$(MAKE) $@ -C $$i; \
	fi \
	done
	@rm -rf moc_obj
	@rm -f  Makefile
