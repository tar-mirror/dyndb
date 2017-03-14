VERSION=0.5.2
PACKAGE=dyndb
CLASS=data
PACKAGE_VERSION=$(PACKAGE)-$(VERSION)

prefix=/usr/local

HEADERS=typesize.h seek.h

INCLUDES=-I.
CC=gcc
L=./link.sh
C=./compile.sh
W=./compilew.sh
GZIP=gzip --force --best
CLEANFILES+=distcheck.tmp distcheck.tmp2
PACKAGE_VERSION=$(PACKAGE)-$(VERSION)
INCLUDES=-DPACKAGE_VERSION=\"$(PACKAGE)-$(VERSION)\" \
 -DPACKAGE=\"$(PACKAGE)\" -DVERSION=\"$(VERSION)\"



libbyte_a_SOURCES=str_rchr.c fmt_ulong.c byte_copy.c str_len.c \
byte_chr.c byte_cr.c str_diff.c str_chr.c scan_long.c scan_sign.c \
scan_ulong.c fmt_xlong.c 
libbyte_a_OBJECTS=$(libbyte_a_SOURCES:.c=.o)
 
libunix_a_SOURCES=stralloc_catb.c stralloc_plus.c gen_alloc_lus.c \
gen_alloc_ady.c stralloc_opyb.c stralloc_eady.c stralloc_opys.c \
error.c error_str.c alloc_re.c alloc.c env_get.c stralloc_pend.c \
gen_alloc_end.c stralloc_copy.c stralloc_free.c stralloc_cats.c \
buffer_0.c buffer_put.c buffer_1.c getln.c buffer_get.c buffer.c \
open_trunc.c open_read.c open_readwri.c 
libunix_a_OBJECTS=$(libunix_a_SOURCES:.c=.o)
 
libuomisc_a_SOURCES=timing.c bailout.c buffer_2.c buffer_put.c \
uogetopt.c bailout_name.c 
libuomisc_a_OBJECTS=$(libuomisc_a_SOURCES:.c=.o)
 
libdyndb_a_SOURCES=dyndb_look.c dyndb_hash.c dyndb_read.c \
dyndb_fdisk.c dyndb_walk.c dyndb_create.c dyndb_write.c dyndb_init.c \
dyndb_enter.c dyndb_tdisk.c dyndb_delete.c dyndb_fwalk.c dyndb_seek.c 
libdyndb_a_OBJECTS=$(libdyndb_a_SOURCES:.c=.o)
 

LIBRARIES += libbyte.a libunix.a libuomisc.a libdyndb.a
 
TARGETS += dyndbtree dyndbstats dyndbdump dyndbfdump dyndbmake \
dyndbget dyndbadd dyndbdel dyndb_intro.7 dyndbdel.1 dyndbadd.1 \
dyndbget.1 libdyndb.3 dyndbmake.1
 
CLEANFILES += fsync.o fsync.lib gettimeofday.lib inet_aton.o \
inet_aton.lib socket.lib socklen_t.h socklen_t.stamp ssize_t.h \
ssize_t.stamp typesize2.h typesize2.stamp have_futimes.h iopause.h \
utimes.o utimes.lib vfork.o vfork.lib version.tmac dyndb_intro.html.t \
dyndbdel.html.t dyndbadd.html.t dyndbget.html.t libdyndb.html.t \
dyndbmake.html.t install.html.t INSTALL.t dyndb_intro.7 \
dyndb_intro.7.t dyndbdel.1 dyndbdel.1.t dyndbadd.1 dyndbadd.1.t \
dyndbget.1 dyndbget.1.t libdyndb.3 libdyndb.3.t dyndbmake.1 \
dyndbmake.1.t *.stamp
 
MAINTAINERCLEAN_FILES += dyndb_intro.html dyndbdel.html dyndbadd.html \
dyndbget.html libdyndb.html dyndbmake.html install.html INSTALL
 

all: systype $(TARGETS) $(EXTRA_TARGETS)
 
have_futimes.h: have_func.sh compile.sh link.sh
	sh have_func.sh futimes "" "futimes(0,0)" "" "" >$@.t && mv $@.t \
$@
iopause.h: iopause.sh compile.sh link.sh
	sh iopause.sh >iopause.t && mv iopause.t $@
libbyte.a: $(libbyte_a_OBJECTS) makelib.sh
	./makelib.sh $@ $(libbyte_a_OBJECTS)
libunix.a: $(libunix_a_OBJECTS) makelib.sh
	./makelib.sh $@ $(libunix_a_OBJECTS)
libuomisc.a: $(libuomisc_a_OBJECTS) makelib.sh
	./makelib.sh $@ $(libuomisc_a_OBJECTS)
libdyndb.a: $(libdyndb_a_OBJECTS) makelib.sh
	./makelib.sh $@ $(libdyndb_a_OBJECTS)
dyndbtree:  dyndbtree.o libdyndb.a libuomisc.a libunix.a libbyte.a \
link.sh
	$L dyndbtree  dyndbtree.o libdyndb.a libuomisc.a libunix.a libbyte.a \

dyndbstats:  dyndbstats.o libdyndb.a libuomisc.a libunix.a libbyte.a \
gettimeofday.lib link.sh
	$L dyndbstats  dyndbstats.o libdyndb.a libuomisc.a libunix.a libbyte.a \
`cat gettimeofday.lib` 
dyndbdump:  dyndbdump.o libdyndb.a libuomisc.a libunix.a libbyte.a \
link.sh
	$L dyndbdump  dyndbdump.o libdyndb.a libuomisc.a libunix.a libbyte.a \

dyndbfdump:  dyndbfdump.o libdyndb.a libuomisc.a libunix.a libbyte.a \
link.sh
	$L dyndbfdump  dyndbfdump.o libdyndb.a libuomisc.a libunix.a libbyte.a \

dyndbmake:  dyndbmake.o libdyndb.a libuomisc.a libunix.a libbyte.a \
fsync.lib link.sh
	$L dyndbmake  dyndbmake.o libdyndb.a libuomisc.a libunix.a libbyte.a \
`cat fsync.lib` 
dyndbget:  dyndbget.o libdyndb.a libuomisc.a libunix.a libbyte.a \
link.sh
	$L dyndbget  dyndbget.o libdyndb.a libuomisc.a libunix.a libbyte.a \

dyndbadd:  dyndbadd.o libdyndb.a libuomisc.a libunix.a libbyte.a \
fsync.lib link.sh
	$L dyndbadd  dyndbadd.o libdyndb.a libuomisc.a libunix.a libbyte.a \
`cat fsync.lib` 
dyndbdel:  dyndbdel.o libdyndb.a libuomisc.a libunix.a libbyte.a \
fsync.lib link.sh
	$L dyndbdel  dyndbdel.o libdyndb.a libuomisc.a libunix.a libbyte.a \
`cat fsync.lib` 
dyndb_intro.html: dyndb_intro.7.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac dyndb_intro.7.in \
>dyndb_intro.html.t && mv dyndb_intro.html.t \
dyndb_intro.html
dyndbdel.html: dyndbdel.1.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac dyndbdel.1.in \
>dyndbdel.html.t && mv dyndbdel.html.t dyndbdel.html
dyndbadd.html: dyndbadd.1.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac dyndbadd.1.in \
>dyndbadd.html.t && mv dyndbadd.html.t dyndbadd.html
dyndbget.html: dyndbget.1.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac dyndbget.1.in \
>dyndbget.html.t && mv dyndbget.html.t dyndbget.html
libdyndb.html: libdyndb.3.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac libdyndb.3.in \
>libdyndb.html.t && mv libdyndb.html.t libdyndb.html
dyndbmake.html: dyndbmake.1.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac dyndbmake.1.in \
>dyndbmake.html.t && mv dyndbmake.html.t dyndbmake.html
install.html: install.0.in groff.tmac version.tmac
	groff -mwww -Thtml -P-l groff.tmac version.tmac install.0.in \
>install.html.t && mv install.html.t install.html
INSTALL: install.0.in groff.tmac version.tmac
	groff -man -Tascii groff.tmac version.tmac install.0.in |col -b \
>INSTALL.t && mv INSTALL.t INSTALL
dyndb_intro.7: dyndb_intro.7.in groff.tmac version.tmac
	cat groff.tmac version.tmac dyndb_intro.7.in \
 | sed 's/^.section/.SH/' >dyndb_intro.7.t \
 && mv dyndb_intro.7.t dyndb_intro.7
dyndbdel.1: dyndbdel.1.in groff.tmac version.tmac
	cat groff.tmac version.tmac dyndbdel.1.in \
 | sed 's/^.section/.SH/' >dyndbdel.1.t \
 && mv dyndbdel.1.t dyndbdel.1
dyndbadd.1: dyndbadd.1.in groff.tmac version.tmac
	cat groff.tmac version.tmac dyndbadd.1.in \
 | sed 's/^.section/.SH/' >dyndbadd.1.t \
 && mv dyndbadd.1.t dyndbadd.1
dyndbget.1: dyndbget.1.in groff.tmac version.tmac
	cat groff.tmac version.tmac dyndbget.1.in \
 | sed 's/^.section/.SH/' >dyndbget.1.t \
 && mv dyndbget.1.t dyndbget.1
libdyndb.3: libdyndb.3.in groff.tmac version.tmac
	cat groff.tmac version.tmac libdyndb.3.in \
 | sed 's/^.section/.SH/' >libdyndb.3.t \
 && mv libdyndb.3.t libdyndb.3
dyndbmake.1: dyndbmake.1.in groff.tmac version.tmac
	cat groff.tmac version.tmac dyndbmake.1.in \
 | sed 's/^.section/.SH/' >dyndbmake.1.t \
 && mv dyndbmake.1.t dyndbmake.1
version.tmac: version.tmac.stamp
version.tmac.stamp: Makefile
	(echo .ds PA $(PACKAGE) ;\
	 echo .ds VE $(VERSION) ;\
	 echo .ds CL $(CLASS) ;\
	) >version.tmac.t
	cmp version.tmac version.tmac.t \
	 || cp version.tmac.t version.tmac
	touch $@ && rm version.tmac.t
html: dyndb_intro.html dyndbdel.html dyndbadd.html dyndbget.html \
libdyndb.html dyndbmake.html install.html INSTALL $(EXTRA_HTML)

 
byte_chr.o: byte_chr.c byte.h compilew.sh
	$W  -c $<
buffer_put.o: buffer_put.c buffer.h str.h byte.h error.h compile.sh
	$C  -c $<
stralloc_copy.o: stralloc_copy.c stralloc.h gen_alloc.h compilew.sh
	$W  -c $<
dyndbfdump.o: dyndbfdump.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h fmt.h buffer.h bailout.h str.h uogetopt.h compilew.sh
	$W  -c $<
bailout.o: bailout.c error.h buffer.h bailout.h fmt.h compilew.sh
	$W  -c $<
str_len.o: str_len.c str.h compilew.sh
	$W  -c $<
fmt_xlong.o: fmt_xlong.c gen_fmt.h fmt.h compilew.sh
	$W  -c $<
gen_alloc_ady.o: gen_alloc_ady.c alloc.h gen_alloci.h compilew.sh
	$W  -c $<
byte_cr.o: byte_cr.c byte.h compilew.sh
	$W  -c $<
dyndb_init.o: dyndb_init.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
scan_ulong.o: scan_ulong.c gen_scan.h scan.h case.h compilew.sh
	$W  -c $<
timing.o: timing.c timing.h compilew.sh
	$W  -c $<
dyndb_seek.o: dyndb_seek.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
alloc_re.o: alloc_re.c alloc.h byte.h compilew.sh
	$W  -c $<
dyndbmake.o: dyndbmake.c /usr/include/errno.h buffer.h stralloc.h \
  gen_alloc.h dyndb.h int32.h typesize.h typesize2.h uint32.h bailout.h \
  fmt.h uogetopt.h str.h attributes.h compilew.sh
	$W  -c $<
getln.o: getln.c byte.h getln.h buffer.h stralloc.h gen_alloc.h compilew.sh
	$W  -c $<
stralloc_opyb.o: stralloc_opyb.c stralloc.h gen_alloc.h byte.h compilew.sh
	$W  -c $<
scan_long.o: scan_long.c gen_scan.h scan.h case.h compilew.sh
	$W  -c $<
alloc.o: alloc.c alloc.h error.h compilew.sh
	$W  -c $<
fmt_ulong.o: fmt_ulong.c gen_fmt.h fmt.h compilew.sh
	$W  -c $<
buffer.o: buffer.c buffer.h compile.sh
	$C  -c $<
scan_sign.o: scan_sign.c scan.h compilew.sh
	$W  -c $<
dyndbtree.o: dyndbtree.c dyndb.h int32.h typesize.h typesize2.h uint32.h \
  bailout.h uogetopt.h compilew.sh
	$W  -c $<
buffer_get.o: buffer_get.c buffer.h byte.h error.h compile.sh
	$C  -c $<
str_diff.o: str_diff.c str.h compilew.sh
	$W  -c $<
dyndb_delete.o: dyndb_delete.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
dyndbdel.o: dyndbdel.c error.h buffer.h stralloc.h gen_alloc.h dyndb.h \
  int32.h typesize.h typesize2.h uint32.h bailout.h open.h str.h \
  uogetopt.h compilew.sh
	$W  -c $<
buffer_0.o: buffer_0.c readwrite.h buffer.h compile.sh
	$C  -c $<
stralloc_pend.o: stralloc_pend.c stralloc.h gen_alloc.h gen_alloci.h compilew.sh
	$W  -c $<
stralloc_eady.o: stralloc_eady.c stralloc.h gen_alloc.h gen_alloci.h compilew.sh
	$W  -c $<
dyndb_fdisk.o: dyndb_fdisk.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
stralloc_free.o: stralloc_free.c stralloc.h gen_alloc.h alloc.h compilew.sh
	$W  -c $<
byte_copy.o: byte_copy.c byte.h compilew.sh
	$W  -c $<
dyndb_look.o: dyndb_look.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
buffer_2.o: buffer_2.c readwrite.h buffer.h compile.sh
	$C  -c $<
uogetopt.o: uogetopt.c readwrite.h str.h env.h scan.h uogetopt.h \
  attributes.h compilew.sh
	$W  -c $<
dyndb_read.o: dyndb_read.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
stralloc_opys.o: stralloc_opys.c str.h stralloc.h gen_alloc.h compilew.sh
	$W  -c $<
open_readwri.o: open_readwri.c open.h compilew.sh
	$W  -c $<
stralloc_plus.o: stralloc_plus.c stralloc.h gen_alloc.h gen_alloci.h compilew.sh
	$W  -c $<
dyndbdump.o: dyndbdump.c dyndb.h int32.h typesize.h typesize2.h uint32.h \
  fmt.h buffer.h bailout.h uogetopt.h str.h compilew.sh
	$W  -c $<
dyndb_enter.o: dyndb_enter.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
dyndb_hash.o: dyndb_hash.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
gen_alloc_end.o: gen_alloc_end.c gen_alloci.h byte.h compilew.sh
	$W  -c $<
stralloc_catb.o: stralloc_catb.c stralloc.h gen_alloc.h byte.h compilew.sh
	$W  -c $<
dyndb_fwalk.o: dyndb_fwalk.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
dyndbadd.o: dyndbadd.c error.h buffer.h stralloc.h gen_alloc.h dyndb.h \
  int32.h typesize.h typesize2.h uint32.h bailout.h open.h str.h \
  uogetopt.h attributes.h compilew.sh
	$W  -c $<
dyndb_create.o: dyndb_create.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
env_get.o: env_get.c env.h compilew.sh
	$W  -c $<
dyndb_walk.o: dyndb_walk.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
str_chr.o: str_chr.c str.h compilew.sh
	$W  -c $<
error_str.o: error_str.c error.h compilew.sh
	$W  -c $<
gen_alloc_lus.o: gen_alloc_lus.c gen_alloci.h compilew.sh
	$W  -c $<
str_rchr.o: str_rchr.c str.h compilew.sh
	$W  -c $<
bailout_name.o: bailout_name.c bailout.h compilew.sh
	$W  -c $<
dyndb_write.o: dyndb_write.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
stralloc_cats.o: stralloc_cats.c str.h stralloc.h gen_alloc.h compilew.sh
	$W  -c $<
buffer_1.o: buffer_1.c readwrite.h buffer.h compile.sh
	$C  -c $<
error.o: error.c error.h compilew.sh
	$W  -c $<
open_read.o: open_read.c open.h compilew.sh
	$W  -c $<
dyndbget.o: dyndbget.c error.h buffer.h stralloc.h gen_alloc.h str.h \
  dyndb.h int32.h typesize.h typesize2.h uint32.h bailout.h uogetopt.h compilew.sh
	$W  -c $<
dyndb_tdisk.o: dyndb_tdisk.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h compilew.sh
	$W  -c $<
dyndbstats.o: dyndbstats.c dyndb.h int32.h typesize.h typesize2.h \
  uint32.h timing.h bailout.h uogetopt.h compilew.sh
	$W  -c $<
open_trunc.o: open_trunc.c open.h compilew.sh
	$W  -c $<
typesize2.h: typesize.stamp
typesize.stamp: typesize.sh compile.sh link.sh
	CC="$(CC)" sh typesize.sh >typesize.t
	@if cmp typesize.t typesize2.h 2>/dev/null >/dev/null ; then : ; else mv typesize.t typesize2.h ; fi
	rm -f typesize.t
	touch $@

gettimeofday.lib: gettimeofday.sh compile.sh link.sh
	sh gettimeofday.sh >gettimeofday.t && mv gettimeofday.t $@

fsync.lib: fsync.sh compile.sh link.sh
	sh fsync.sh >fsync.t && mv fsync.t $@

.PRECIOUS: Makefile
remake: clean
	rm -f compile.sh link.sh compilew.sh
packageinfo: 
	@echo version="$(VERSION)"
	@echo package="$(PACKAGE)"
	@echo class="$(CLASS)"
compile.sh: 
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'CFLAGS="$${CFLAGS:-$$_CFLAGS_OG}"' ; \
	echo 'CFLAGS="$${CFLAGS:--O2}"' ; \
	echo 'DEFS="$${DEFS:--${DEFS}}"' ; \
	echo 'INCLUDES="$${INCLUDES:--${INCLUDES}}"' ; \
	echo 'CC="$${CC:-gcc}"' ; \
	echo 'exec $$CC $(DEFS) $(INCLUDES) $$CFLAGS $${1+"$$@"}' ; \
	) > compile.t && chmod 755 compile.t
	@mv compile.t $@
compilew.sh: 
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'CFLAGS="$${CFLAGS:-$$_CFLAGS_OWG}"' ; \
	echo 'CFLAGS="$${CFLAGS:--O2 -Wall}"' ; \
	echo 'DEFS="$${DEFS:--${DEFS}}"' ; \
	echo 'INCLUDES="$${INCLUDES:--${INCLUDES}}"' ; \
	echo 'CC="$${CC:-gcc}"' ; \
	echo 'exec $$CC $(DEFS) $(INCLUDES) $$CFLAGS $${1+"$$@"}' ; \
	) > compilew.t && chmod 755 compilew.t
	@mv compilew.t compilew.sh
link.sh:
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'LDFLAGS="$${LDFLAGS:-$$_CFLAGS_G}"' ; \
	echo 'LDFLAGS="$${LDFLAGS:--g}"' ; \
	echo 'CC="$${CC:-gcc}"' ; \
	echo 'exec $$CC $$LDFLAGS -o $${1+"$$@"} ' ; \
	) > link.t && chmod 755 link.t
	@mv link.t link.sh
makelib.sh:
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'RANLIB="$${RANLIB:-ranlib}"' ; \
	echo 'AR="$${AR:-ar}"' ; \
	echo '$$AR cru $${1+"$$@"} ' ; \
	echo 'exec $$RANLIB $$1' ; \
	) > makelib.t && chmod 755 makelib.t
	@mv makelib.t makelib.sh
maintainer-clean: distclean
	rm -f Makefile $(MAINTAINERCLEANFILES)
distclean: clean
	rm -f $(DISTCLEANFILES) dummy.tmp # to avoid "no arguments" error
clean spotless: mostlyclean
	rm -f $(LIBRARIES) $(TARGETS) link.sh compile.sh compilew.sh makelib.sh
	rm -rf deps
mostlyclean:
	rm -f *.o $(TARGETS) $(CLEANFILES)

NEWS-check:
	@if sed 1q NEWS | grep -e "$(VERSION)" > /dev/null; then :; else \
		echo "NEWS not updated; not releasing" 1>&2; \
		exit 1;                                   \
	fi
	@if grep -e "$(VERSION)" ChangeLog > /dev/null; then :; else \
		echo "ChangeLog not updated; not releasing" 1>&2; \
		exit 1;                                   \
	fi
bk-dist: NEWS-check
	bk tag `echo "Release-$(VERSION)" | sed 's/\./-/g'`
	$(MAKE) dist
gpg-dist: $(DIST)
	gpg -b --armor $(DIST)
DIST=$(PACKAGE)-$(VERSION).tar.gz

dist: $(DIST)
$(DIST): $(DISTFILES)
	rm -f $@
	package

force:	

systype: typesize2.h $(EXTRA_SYSTYPE)
	rm -f $@.t
	@echo  >>$@.t
	@echo "PACKAGE: $(PACKAGE) $(VERSION)" >>$@.t
	@echo "REALNAME: $(REALNAME)" >>$@.t
	@echo "uname-a:" `uname -a` >>$@.t
	@echo "uname-s:" `uname -s` >>$@.t
	@echo "uname-r:" `uname -r` >>$@.t
	@echo "uname-v:" `uname -v` >>$@.t
	@echo "uname-m:" `uname -m` >>$@.t
	cat typesize2.h >>$@.t
	if test "x$(EXTRA_SYSTYPE)" = x ; then true ; else \
	  for i in $(EXTRA_SYSTYPE) dummy ; do \
	    if test "x$$i" = "xdummy" ; then true ; else \
	      grep systype-info $$i >> $@.t ; \
	    fi \
	  done \
	fi
	mv $@.t $@

# that's not the "right" dependency, but that way it works even if
# uolib is absent.
LICENSES: Makefile.pre LICENSES.HEAD LICENSES.LOCAL
	( cat LICENSES.HEAD ; \
	for i in *.[ch] ; do \
		grep -h -s "^$$i:" /home/uwe/work/uolib/LICENSES.REPO \
		|| grep -h -s "^$$i:" ./LICENSES.LOCAL \
		|| grep -h -s "^$$i:" ./LICENSES \
		|| echo "$$i: unknown" ; \
	done ; \
	) >>$@.t
	mv $@.t $@

check: docheck check.exp
	sh ./docheck 2>&1 | cat -v | tee check.out | diff -u check.exp -
