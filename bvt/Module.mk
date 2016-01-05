################ Source files ##########################################

bvt/SRCS	:= $(wildcard bvt/bvt*.cc)
bvt/BVTS	:= $(addprefix $O,$(bvt/SRCS:.cc=))
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
bvt/OUTS	:= $(addprefix $O,$(bvt/SRCS:.cc=.out))

################ Compilation ###########################################

.PHONY:	bvt/all bvt/run bvt/clean bvt/check

bvt/all:	${bvt/BVTS}

# The correct output of a bvt is stored in bvtXX.std
# When the bvt runs, its output is compared to .std
#
bvt/run:	${bvt/BVTS}
	@echo "Running build verification tests:"
	@for i in ${bvt/BVTS}; do	\
	    BVT="bvt/$$(basename $$i)";	\
	    echo "Running $$i";		\
	    ./$$i < $$BVT.cc > $$i.out 2>&1;	\
	    diff $$BVT.std $$i.out && rm -f $$i.out; \
	done

${bvt/BVTS}: $Obvt/%: $Obvt/%.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

$Obvt/bench:	$Obvt/bench.o $Obvt/stdtest.o ${ALLTGTS}
	@echo "Linking $@ ..."
	@${LD} ${LDFLAGS} -o $@ $< $Obvt/stdtest.o ${bvt/LIBS}

$Obvt/.d:	$O.d
	@mkdir $Obvt
	@touch $Obvt/.d

################ Maintenance ###########################################

clean:	bvt/clean
bvt/clean:
	@if [ -d $Obvt ]; then\
	    rm -f ${bvt/BVTS} ${bvt/OBJS} ${bvt/DEPS} ${bvt/OUTS} $Obvt/bench $Obvt/bench.o $Obvt/stdtest.o $Obvt/.d;\
	    rmdir $Obvt;\
	fi
check:		bvt/run
bvt/check:	check

${bvt/OBJS} $Obvt/stdtest.o $Obvt/bench.o:	${MKDEPS} $Obvt/.d

-include ${bvt/DEPS}
