s/ \?@INLINE_OPTS@/ -fvisibility-inlines-hidden -fno-threadsafe-statics -fno-enforce-eh-specs/g
s/ \?@libgcc_eh@//g
s/#undef \(HAVE_LONG_LONG\)/#define \1 1/g
s/#undef \(SIZE_OF_LONG_LONG\)/#define \1 8/g
s/#undef \(HAVE_INT64_T\)/#define \1 1/g
s/#undef \(HAVE_VECTOR_EXTENSIONS\)/#define \1 1/g
s/@BYTE_ORDER@/LITTLE_ENDIAN/g
s/#undef \(RETSIGTYPE\)/#define \1 void/g
s/#undef const/\/\* #define const \*\//g
s/#undef inline/\/\* #define inline __inline \*\//g
s/#undef off_t/\/\* typedef long off_t; \*\//g
s/#undef size_t/\/\* typedef long size_t; \*\//g
s/#undef \(SIZE_OF_CHAR\)/#define \1 1/g
s/#undef \(SIZE_OF_SHORT\)/#define \1 2/g
s/#undef \(SIZE_OF_INT\)/#define \1 4/g
s/#undef \(SIZE_OF_LONG\)/#define \1 4/g
s/#undef \(SIZE_OF_POINTER\)/#define \1 4/g
s/#undef \(SIZE_OF_SIZE_T\)/#define \1 4/g
s/#undef \(LSTAT_FOLLOWS_SLASHED_SYMLINK\)/#define \1 1/g
s/ \?@PROCESSOR_OPTS@/ -O3 -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=neon-fp16 -mvectorize-with-neon-quad/g
# s/#undef \(WANT_NAME_DEMANGLING\)/#define \1 1/g
s/#define \(WANT_MMX\) 1/#undef \1/g
s/#undef \(WANT_ALWAYS_INLINE\)/#define \1 1/g
s/#define \(WITHOUT_LIBSTDCPP\) 1/#undef \1/g
s/\(NOLIBSTDCPP\)/#\1/g
