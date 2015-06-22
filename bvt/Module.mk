################ Source files ##########################################

bvt/SRCS	:= $(wildcard bvt/bvt*.cc)
bvt/BVTS	:= $(bvt/SRCS:.cc=)
bvt/OBJS	:= $(addprefix $O,$(bvt/SRCS:.cc=.o))
ifdef BUILD_STATIC
bvt/LIBS	:= ${LIBA}
else
bvt/LIBS	:= -L$(abspath $O.) -l${NAME}
endif
ifdef BUILD_SHARED
bvt/LIBS	:= -Wl,--rpath=$(abspath $O.) ${bvt/LIBS}
endif
ifdef NOLIBSTDCPP
bvt/LIBS	+= ${STAL_LIBS} -lm
endif
bvt/DEPS	:= ${bvt/OBJS:.o=.d} $Obvt/stdtest.d $Obvt/bench.d

################ Compilation ###########################################

.PHONY:	bvt/all bvt/run bvt/clean bvt/check

bvt/all:	${bvt/BVTS}

# The correct output of a bvt is stored in bvtXX.std
# When the bvt runs, its output is compared to .std
#
bvt/run:	${bvt/BVTS}
	@echo "Running build verification tests:"
	@for i in ${bvt/BVTS}; do \
	    echo "Running $$i"; \
	    ./$$i < $$i.cc &> $$i.out; \
	    diff $$i.std $$i.out && rm -f $$i.out; \
	done

${bvt/BVTS}: bvt/%: $Obvt/%.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

bvt/bench:	$Obvt/bench.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

################ Maintenance ###########################################

clean:	bvt/clean
bvt/clean:
	@if [ -d $Obvt ]; then\
	    rm -f ${bvt/BVTS} ${bvt/OBJS} ${bvt/DEPS} bvt/bench $Obvt/bench.o $Obvt/stdtest.o;\
	    rmdir $Obvt;\
	fi
ifndef CROSS_COMPILE
check:		bvt/run
else
$(warning Cross compiling; cannot run check. Will build check script to run in target environment.)
check:		${bvt/BVTS}
endif
bvt/check:	check

${bvt/OBJS} $Obvt/stdtest.o $Obvt/bench.o: Makefile bvt/Module.mk Config.mk ${NAME}/config.h

-include ${bvt/DEPS}
