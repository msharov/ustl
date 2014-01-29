// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "unew.h"

namespace {
std::new_handler allocation_fail_handler = 0;

void default_allocation_fail_handler(void) { throw ustl::bad_alloc(); }

}

std::new_handler std::set_new_handler(std::new_handler hdl) noexcept
{
    std::new_handler out = allocation_fail_handler;
    allocation_fail_handler = hdl;
    return out;
}

#ifdef HAVE_CPP11
std::new_handler std::get_new_handler(void) noexcept
{
    return allocation_fail_handler;
}
#endif

void* tmalloc (size_t n) throw (ustl::bad_alloc)
{
    while (true) {
        void* p = malloc (n);
        if (p)
            return p;

        if (allocation_fail_handler)
            allocation_fail_handler();
        else
            default_allocation_fail_handler();
    }
}
