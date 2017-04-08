-include Config.mk

ifeq (${V},1)
Q :=
q := \#
else
Q := @
q :=
endif

################ Source files ##########################################

SRCS	:= $(wildcard *.cc)
INCS	:= $(wildcard *.h)
OBJS	:= $(addprefix $O,$(SRCS:.cc=.o))
DEPS	:= ${OBJS:.o=.d}
MKDEPS	:= Makefile Config.mk config.h $O.d
ONAME	:= $(notdir $(abspath $O))

################ Compilation ###########################################

.PHONY: all clean html check distclean maintainer-clean

ALLTGTS	:= ${MKDEPS}
all:	${ALLTGTS}

ifdef BUILD_SHARED
SLIBL	:= $O$(call slib_lnk,${NAME})
SLIBS	:= $O$(call slib_son,${NAME})
SLIBT	:= $O$(call slib_tgt,${NAME})
SLINKS	:= ${SLIBL}
ifneq (${SLIBS},${SLIBT})
SLINKS	+= ${SLIBS}
endif
ALLTGTS	+= ${SLIBT} ${SLINKS}

all:	${SLIBT} ${SLINKS}
${SLIBT}:	${OBJS}
	@${q}echo "Linking $(notdir $@) ..."
	${Q}${LD} -fPIC ${LDFLAGS} $(call slib_flags,$(subst $O,,${SLIBS})) -o $@ $^ ${LIBS}
${SLINKS}:	${SLIBT}
	${Q}(cd $(dir $@); rm -f $(notdir $@); ln -s $(notdir $<) $(notdir $@))

endif
ifdef BUILD_STATIC
LIBA	:= $Olib${NAME}.a
ALLTGTS	+= ${LIBA}

all:	${LIBA}
${LIBA}:	${OBJS}
	@${q}echo "Linking $@ ..."
	${Q}rm -f $@
	${Q}${AR} qc $@ ${OBJS}
	${Q}${RANLIB} $@
endif

$O%.o:	%.cc
	@${q}echo "    Compiling $< ..."
	${Q}${CXX} ${CXXFLAGS} -MMD -MT "$(<:.cc=.s) $@" -o $@ -c $<

%.s:	%.cc
	@${q}echo "    Compiling $< to assembly ..."
	${Q}${CXX} ${CXXFLAGS} -S -o $@ -c $<

include test/Module.mk

################ Installation ##########################################

.PHONY:	install uninstall install-incs uninstall-incs

####### Install headers

ifdef INCDIR	# These ifdefs allow cold bootstrap to work correctly
LIDIR	:= ${INCDIR}/${NAME}
INCSI	:= $(addprefix ${LIDIR}/,$(filter-out ${NAME}.h,${INCS}))
RINCI	:= ${LIDIR}.h

install:	install-incs
install-incs: ${INCSI} ${RINCI}
${INCSI}: ${LIDIR}/%.h: %.h
	@${q}echo "Installing $@ ..."
	${Q}${INSTALLDATA} $< $@
${RINCI}: ${NAME}.h
	@${q}echo "Installing $@ ..."
	${Q}${INSTALLDATA} $< $@
uninstall:	uninstall-incs
uninstall-incs:
	${Q}if [ -d ${LIDIR} -o -f ${RINCI} ]; then\
	    echo "Removing ${LIDIR}/ and ${RINCI} ...";\
	    rm -f ${INCSI} ${RINCI};\
	    ${RMPATH} ${LIDIR};\
	fi
endif

####### Install libraries (shared and/or static)

ifdef LIBDIR
ifdef BUILD_SHARED
LIBTI	:= ${LIBDIR}/$(notdir ${SLIBT})
LIBLI	:= $(addprefix ${LIBDIR}/,$(notdir ${SLINKS}))
install:	${LIBTI} ${LIBLI}
${LIBTI}:	${SLIBT}
	@${q}echo "Installing $@ ..."
	${Q}${INSTALLLIB} $< $@
${LIBLI}: ${LIBTI}
	${Q}(cd ${LIBDIR}; rm -f $@; ln -s $(notdir $<) $(notdir $@))
endif
ifdef BUILD_STATIC
LIBAI	:= ${LIBDIR}/$(notdir ${LIBA})
install:	${LIBAI}
${LIBAI}:	${LIBA}
	@${q}echo "Installing $@ ..."
	${Q}${INSTALLLIB} $< $@
endif

uninstall:
	@${q}echo "Removing library from ${LIBDIR} ..."
	${Q}rm -f ${LIBTI} ${LIBLI} ${LIBSI} ${LIBAI}
endif

################ Maintenance ###########################################

clean:
	${Q}if [ -h ${ONAME} ]; then\
	    rm -f ${OBJS} ${DEPS} ${SLIBT} ${SLINKS} ${LIBA} $O.d ${ONAME};\
	    ${RMPATH} ${BUILDDIR} > /dev/null 2>&1 || true;\
	fi

html:	${SRCS} ${INCS} ${NAME}doc.in
	${Q}${DOXYGEN} ${NAME}doc.in

distclean:	clean
	${Q}rm -f Config.mk config.h config.status

maintainer-clean: distclean
	${Q}if [ -d docs/html ]; then rm -f docs/html/*; rmdir docs/html; fi

$O.d:	${BUILDDIR}/.d
	${Q}[ -h ${ONAME} ] || ln -sf ${BUILDDIR} ${ONAME}
${BUILDDIR}/.d:	Makefile
	${Q}mkdir -p ${BUILDDIR} && touch ${BUILDDIR}/.d

${OBJS}:		${MKDEPS}
Config.mk:		Config.mk.in
config.h:		config.h.in
Config.mk config.h:	configure
	@if [ -x config.status ]; then			\
	    echo "Reconfiguring ..."; ./config.status;	\
	else						\
	    echo "Running configure ..."; ./configure;	\
	fi

-include ${DEPS}
