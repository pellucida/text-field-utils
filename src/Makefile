

SUBDIRS	= str strprintf strvec strsplit collect uncollect
SUBDIRS	+= fld fldvec fldparse  project
.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

MAKEFLAGS=GLIBC_PRINTF=-DGLIBC_PRINTF STR_PRINTF_DIR=glibc_printf _GNU_SOURCE=-D_GNU_SOURCE
MAKEFLAGS=GLIBC_PRINTF=-DGLIBC_PRINTF STR_PRINTF_DIR=non_glibc_printf _GNU_SOURCE=-D_GNU_SOURCE

$(SUBDIRS):
	$(MAKE) -C $@ 

distclean clean:	
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	
