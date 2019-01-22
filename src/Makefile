

SUBDIRS	= str strprintf strvec strsplit collect
SUBDIRS	+= fld fldvec fldparse  project
.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

MAKEFLAGS=GLIBC_PRINTF=-DGLIBC_PRINTF STR_PRINTF_DIR=glibc_printf _GNU_SOURCE=-D_GNU_SOURCE

$(SUBDIRS):
	$(MAKE) -C $@ 

clean:	
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	
