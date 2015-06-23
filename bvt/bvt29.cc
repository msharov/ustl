// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "stdtest.h"
#if HAVE_CPP11

static void Print (const char* tname, bool tvalue)
{
    //static const char bv[2][8] = { "false", "true " };
    //cout.format ("%s = %s\n", bv[tvalue], tname);
    cout << tvalue << "\t= " << tname << endl;
}
#define PRT(test)	Print(#test, test::value)
#define PRT2(t1,t2)	Print(#t1 "," #t2, t1,t2::value)
#define PRT3(t1,t2,t3)	Print(#t1 "," #t2 "," #t3, t1,t2,t3::value)

namespace {
class TTA {};

class TTB : public TTA {};

class Polymorphic {
public:
    virtual ~Polymorphic (void);
    virtual void Function (void) = 0;
    static int s_var;
};
}

POD_CLASS (TTA);

HAS_MEMBER_FUNCTION (Function, void (O::*)(void));
HAS_MEMBER_FUNCTION (read, void (O::*)(istream&));
HAS_STATIC_MEMBER_VARIABLE (int, s_var);
HAS_STATIC_MEMBER_VARIABLE (void, s_invalid);

enum TestEnum { e_Val1, e_Val2 };

static void TestTypeTraits (void)
{
    cout.format ("----------------------------------------------------------------------\n"
	    " Testing type traits\n"
	    "----------------------------------------------------------------------\n");

    //----------------------------------------------------------------------
    // Primary type categories
    //----------------------------------------------------------------------
    PRT (is_void<void>);
    PRT (is_void<int>);
    PRT (is_void<void*>);
    PRT (is_void<TTA>);

    PRT (is_integral<char>);
    PRT (is_integral<const char>);
    PRT (is_integral<volatile char>);
    PRT (is_integral<const volatile char>);
    PRT (is_integral<signed char>);
    PRT (is_integral<unsigned char>);
    PRT (is_integral<short>);
    PRT (is_integral<unsigned short>);
    PRT (is_integral<int>);
    PRT (is_integral<unsigned int>);
    PRT (is_integral<long>);
    PRT (is_integral<unsigned long>);
#if HAVE_LONG_LONG
    PRT (is_integral<long long>);
    PRT (is_integral<unsigned long long>);
#else
    Print ("is_integral<long long>", true);
    Print ("is_integral<unsigned long long>", true);
#endif
    PRT (is_integral<float>);
    PRT (is_integral<void*>);
    PRT (is_integral<TTA>);
    PRT (is_integral<TTA*>);

    PRT (is_floating_point<float>);
    PRT (is_floating_point<double>);
    PRT (is_floating_point<int>);

    PRT (is_array<int>);
    PRT (is_array<int[4]>);
    PRT (is_array<int[2][4]>);
    PRT (is_array<const char[]>);

    PRT (is_pointer<long>);
    PRT (is_pointer<void*>);
    PRT (is_pointer<TTA*>);

    PRT (is_lvalue_reference<long>);
    PRT (is_lvalue_reference<int&>);
    PRT (is_lvalue_reference<void*&>);
    PRT (is_lvalue_reference<volatile float&>);

    PRT (is_rvalue_reference<long>);
    PRT (is_rvalue_reference<int&&>);
    PRT (is_rvalue_reference<void*&>);
    PRT (is_rvalue_reference<volatile float&&>);

    PRT (is_reference<long>);
    PRT (is_reference<int&>);
    PRT (is_reference<void*&>);
    PRT (is_reference<volatile float&>);

    PRT (is_member_object_pointer<void>);
    PRT (is_member_function_pointer<int>);
    PRT (is_member_function_pointer<void (TTA::*)(void)>);

    PRT (is_enum<TestEnum>);
    PRT (is_enum<TTA>);
    PRT (is_enum<int>);

    PRT (is_class<TTA>);
    PRT (is_class<int>);

    PRT (is_function<int>);
    PRT (is_function<void (*)(int)>);
    PRT (is_function<decltype(Print)>);
    PRT (is_function<void (TTA::*)(void)>);

    //----------------------------------------------------------------------
    // Composite type categories
    //----------------------------------------------------------------------
    PRT (is_arithmetic<int>);
    PRT (is_arithmetic<volatile float>);
    PRT (is_arithmetic<const char>);
    PRT (is_arithmetic<TTA>);
    PRT (is_arithmetic<TestEnum>);

    PRT (is_floating_point<int>);
    PRT (is_floating_point<float>);
    PRT (is_floating_point<double>);
    PRT (is_floating_point<TTA>);

    PRT (is_object<int>);
    PRT (is_object<float>);
    PRT (is_object<TTA>);
    PRT (is_object<TestEnum>);

    PRT (is_scalar<volatile int>);
    PRT (is_scalar<float>);
    PRT (is_scalar<TTA>);
    PRT (is_scalar<TestEnum>);

    PRT (is_compound<const int>);
    PRT (is_compound<float>);
    PRT (is_compound<TTA>);
    PRT (is_compound<TestEnum>);

    PRT (is_member_pointer<void (TTA::*)(void)>);
    PRT (is_member_pointer<void*>);

    //----------------------------------------------------------------------
    // Type properties
    //----------------------------------------------------------------------
    PRT (is_const<int>);
    PRT (is_const<const int>);
    PRT (is_const<const volatile int>);

    PRT (is_volatile<float>);
    PRT (is_volatile<const float>);
    PRT (is_volatile<const volatile float>);

    PRT (is_pod<int>);
    PRT (is_pod<int[15]>);
    PRT (is_pod<TTA>);
    PRT (is_pod<Polymorphic>);

    PRT (is_empty<int>);
    PRT (is_empty<TTA>);
    PRT (is_empty<TTB>);

    PRT (has_trivial_constructor<TTA>);
    PRT (has_trivial_constructor<Polymorphic>);

    PRT (has_trivial_copy<TTA>);
    PRT (has_trivial_copy<Polymorphic>);

    PRT (has_trivial_assign<TTA>);
    PRT (has_trivial_assign<Polymorphic>);

    PRT (has_trivial_destructor<TTA>);
    PRT (has_trivial_destructor<Polymorphic>);

    PRT (is_signed<int>);
    PRT (is_signed<unsigned int>);
    PRT (is_signed<float>);

    PRT (is_unsigned<int>);
    PRT (is_unsigned<unsigned int>);
    PRT (is_unsigned<float>);

    PRT (sizeof(char) == alignment_of<char>);
    PRT (sizeof(short) == alignment_of<short>);
    PRT (sizeof(int) == alignment_of<int>);
    PRT (sizeof(void*) == alignment_of<void*>);
    PRT (sizeof(double) == alignment_of<double>);
    PRT (sizeof(char) == alignment_of<TTA>);

    PRT (0 == rank<int>);
    PRT (1 == rank<int[1]>);
    PRT (2 == rank<int[][2]>);
    PRT (3 == rank<int[1][2][3]>);

    PRT (0 == extent<int>);
    PRT (1 == extent<int[1]>);
    PRT (1 == extent<int[1][2]>);
    PRT2 (2 == extent<int[][2],1>);
    PRT (1 == extent<int[1][2][3]>);
    PRT2 (2 == extent<int[1][2][3],1>);
    PRT2 (3 == extent<int[1][2][3],2>);

    //----------------------------------------------------------------------
    // Type relations and modifications
    //----------------------------------------------------------------------
    PRT2 (is_same<int,int>);
    PRT2 (is_same<int,unsigned int>);

    PRT2 (is_base_of<int,unsigned int>);
    PRT2 (is_base_of<TTA,TTB>);
    PRT2 (is_base_of<TTA,Polymorphic>);

    PRT2 (is_convertible<int,unsigned int>);
    PRT2 (is_convertible<TTA,TTA>);
    PRT2 (is_convertible<TTA*,TTB*>);
    PRT2 (is_convertible<TTB*,TTA*>);

    PRT2 (is_same<remove_const<const int>::type, int>);
    PRT2 (is_same<remove_const<const volatile int>::type, int>);

    PRT2 (is_same<remove_volatile<const int>::type, int>);
    PRT2 (is_same<remove_volatile<const volatile int>::type, int>);

    PRT2 (is_same<add_const<int>::type, volatile int>);
    PRT2 (is_same<add_const<volatile int>::type, const volatile int>);

    PRT2 (is_same<add_volatile<int>::type, volatile int>);
    PRT2 (is_same<add_volatile<const int>::type, const volatile int>);

    PRT2 (is_same<add_cv<int>::type, const volatile int>);

    PRT2 (is_same<remove_pointer<int*>::type, int>);
    PRT2 (is_same<remove_reference<const int&>::type, const int>);
    PRT2 (is_same<remove_reference<const volatile int&>::type, const volatile int>);
    PRT2 (is_same<add_pointer<int>::type, int*>);
    PRT2 (is_same<add_lvalue_reference<int volatile>::type, volatile int&>);
    PRT2 (is_same<add_rvalue_reference<int>::type, int&&>);

    PRT2 (is_same<remove_extent<int>::type,int>);
    PRT2 (is_same<remove_extent<int[2][3]>::type,int[3]>);
    PRT2 (is_same<remove_extent<int[][3]>::type,int[3]>);
    PRT2 (is_same<remove_extent<int[3]>::type,int>);
    PRT2 (is_same<remove_all_extents<int>::type,int>);
    PRT2 (is_same<remove_all_extents<int[2][3]>::type,int>);
    PRT2 (is_same<remove_all_extents<int[3]>::type,int>);

    PRT (has_member_function_Function<TTB>);
    PRT (has_member_function_Function<Polymorphic>);
    PRT (has_member_function_read<Polymorphic>);
    PRT (has_member_function_read<vector<int>>);
    PRT (has_static_member_variable_s_var<Polymorphic>);
    PRT (has_static_member_variable_s_invalid<Polymorphic>);

#define ALIGNED_STORAGE_TEST(size,grain)	\
    cout.format ("aligned_storage<" #size "," #grain ">::type:\tsizeof %zu,\talignof %zu\n", sizeof(aligned_storage<size,grain>::type), alignof(aligned_storage<size,grain>::type))
    ALIGNED_STORAGE_TEST(1,1);
    ALIGNED_STORAGE_TEST(2,1);
    ALIGNED_STORAGE_TEST(2,2);
    ALIGNED_STORAGE_TEST(2,4);
    ALIGNED_STORAGE_TEST(5,4);
    ALIGNED_STORAGE_TEST(16,8);
    ALIGNED_STORAGE_TEST(17,16);
#undef ALIGNED_STORAGE_TEST
}

#else // if !HAVE_CPP11

static void TestTypeTraits (void)
{
    memblock outbuf;
    try {
      outbuf.read_file ("bvt/bvt29.std");
    } catch (...) {
      outbuf.read_file ("bvt29.std");
    }
    cout << outbuf;
}

#endif

StdBvtMain (TestTypeTraits)
