// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "mistream.h"
#include "memblock.h"
#include "ualgo.h"
#include "umemory.h"
#include "fstream.h"
#include <errno.h>

namespace ustl {

memblock::memblock (void) noexcept		: memlink (), m_Capacity (0) { }
memblock::memblock (const void* p, size_type n) : memlink (), m_Capacity (0) { assign (p, n); }
memblock::memblock (size_type n)		: memlink (), m_Capacity (0) { resize (n); }
memblock::memblock (const cmemlink& b)		: memlink (), m_Capacity (0) { assign (b); }
memblock::memblock (const memlink& b)		: memlink (), m_Capacity (0) { assign (b); }
memblock::memblock (const memblock& b)		: memlink (), m_Capacity (0) { assign (b); }
memblock::~memblock (void) noexcept		{ deallocate(); }

void memblock::unlink (void) noexcept
{
    m_Capacity = 0;
    memlink::unlink();
}

/// resizes the block to \p newSize bytes, reallocating if necessary.
void memblock::resize (size_type newSize, bool bExact)
{
    if (m_Capacity < newSize + minimumFreeCapacity())
	reserve (newSize, bExact);
    memlink::resize (newSize);
}

/// Frees internal data.
void memblock::deallocate (void) noexcept
{
    if (m_Capacity) {
	assert (cdata() && "Internal error: space allocated, but the pointer is NULL");
	assert (data() && "Internal error: read-only block is marked as allocated space");
	free (data());
    }
    unlink();
}

/// Assumes control of the memory block \p p of size \p n.
/// The block assigned using this function will be freed in the destructor.
void memblock::manage (void* p, size_type n) noexcept
{
    assert (p || !n);
    assert (!m_Capacity && "Already managing something. deallocate or unlink first.");
    link (p, n);
    m_Capacity = n;
}

/// "Instantiate" a linked block by allocating and copying the linked data.
void memblock::copy_link (void)
{
    const pointer p (begin());
    const size_t sz (size());
    if (is_linked())
	unlink();
    assign (p, sz);
}
 
/// Copies data from \p p, \p n.
void memblock::assign (const void* p, size_type n)
{
    assert ((p != (const void*) cdata() || size() == n) && "Self-assignment can not resize");
    resize (n);
    copy_n (const_pointer(p), n, begin());
}

/// \brief Reallocates internal block to hold at least \p newSize bytes.
///
/// Additional memory may be allocated, but for efficiency it is a very
/// good idea to call reserve before doing byte-by-byte edit operations.
/// The block size as returned by size() is not altered. reserve will not
/// reduce allocated memory. If you think you are wasting space, call
/// deallocate and start over. To avoid wasting space, use the block for
/// only one purpose, and try to get that purpose to use similar amounts
/// of memory on each iteration.
///
void memblock::reserve (size_type newSize, bool bExact)
{
    if ((newSize += minimumFreeCapacity()) <= m_Capacity)
	return;
    pointer oldBlock (is_linked() ? NULL : data());
    const size_t alignedSize (NextPow2 (newSize));
    if (!bExact)
	newSize = alignedSize;
    pointer newBlock = (pointer) realloc (oldBlock, newSize);
    if (!newBlock)
	throw bad_alloc (newSize);
    if (!oldBlock && (cdata() != NULL))
	copy_n (cdata(), min (size() + 1, newSize), newBlock);
    link (newBlock, size());
    m_Capacity = newSize;
}

/// Reduces capacity to match size
void memblock::shrink_to_fit (void)
{
    if (is_linked())
	return;
    pointer newBlock = (pointer) realloc (begin(), size());
    if (!newBlock && size())
	throw bad_alloc (size());
    m_Capacity = size();
    memlink::relink (newBlock, size());
}

/// Shifts the data in the linked block from \p start to \p start + \p n.
memblock::iterator memblock::insert (const_iterator start, size_type n)
{
    const uoff_t ip = start - begin();
    assert (ip <= size());
    resize (size() + n, false);
    memlink::insert (iat(ip), n);
    return (iat (ip));
}

/// Shifts the data in the linked block from \p start + \p n to \p start.
memblock::iterator memblock::erase (const_iterator start, size_type n)
{
    const uoff_t ep = start - begin();
    assert (ep + n <= size());
    reserve (size() - n);
    iterator iep = iat(ep);
    memlink::erase (iep, n);
    memlink::resize (size() - n);
    return (iep);
}

/// Reads the object from stream \p s
void memblock::read (istream& is)
{
    written_size_type n = 0;
    is >> n;
    if (!is.verify_remaining ("read", "ustl::memblock", n))
	return;
    resize (n);
    is.read (data(), writable_size());
    is.align (stream_align_of (n));
}

/// Reads the entire file \p "filename".
void memblock::read_file (const char* filename)
{
    fstream f;
    f.exceptions (fstream::allbadbits);
    f.open (filename, fstream::in);
    const off_t fsize (f.size());
    reserve (fsize);
    f.read (data(), fsize);
    f.close();
    resize (fsize);
}

memblock::size_type memblock::minimumFreeCapacity (void) const noexcept { return (0); }

} // namespace ustl
