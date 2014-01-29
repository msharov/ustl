// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "uexception.h"
#include <stdlib.h>

/// Just like malloc, but throws on failure.
void* tmalloc (size_t n) throw (ustl::bad_alloc) __attribute__((malloc));
/// Just like free, but doesn't crash when given a NULL.
inline void nfree (void* p) noexcept { if (p) free (p); }

#if WITHOUT_LIBSTDCPP

namespace std {
    typedef void (*new_handler)(void);

    new_handler set_new_handler(new_handler hdl) noexcept;
#if HAVE_CPP11
    new_handler get_new_handler(void) noexcept;
#endif
}

//
// These are replaceable signatures:
//  - normal single new and delete (no arguments, throw @c bad_alloc on error)
//  - normal array new and delete (same)
//  - @c nothrow single new and delete (take a @c nothrow argument, return
//    @c NULL on error)
//  - @c nothrow array new and delete (same)
//
//  Placement new and delete signatures (take a memory address argument,
//  does nothing) may not be replaced by a user's program.
//
inline void* operator new (size_t n) throw (std::bad_alloc)	{ return (tmalloc (n)); }
inline void* operator new[] (size_t n) throw (std::bad_alloc)	{ return (tmalloc (n)); }
inline void  operator delete (void* p)				{ nfree (p); }
inline void  operator delete[] (void* p)			{ nfree (p); }

// Default placement versions of operator new.
inline void* operator new (size_t, void* p) noexcept	{ return (p); }
inline void* operator new[] (size_t, void* p) noexcept	{ return (p); }

// Default placement versions of operator delete.
inline void  operator delete  (void*, void*) noexcept	{ }
inline void  operator delete[](void*, void*) noexcept	{ }

#else
#include <new>
#endif	// WITHOUT_LIBSTDCPP
