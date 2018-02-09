//------------------------------------------------------------------------------
/*
	Copyright (c) 2012, 2013 Skywell Labs Inc.
	Copyright (c) 2017-2018 TrueChain Foundation.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef SKYWELL_NODESTORE_ENCODEDBLOB_H_INCLUDED
#define SKYWELL_NODESTORE_ENCODEDBLOB_H_INCLUDED

#include <data/nodestore/NodeObject.h>
#include <beast/module/core/memory/MemoryBlock.h>
#include <beast/utility/noexcept.h>
#include <cstddef>

namespace truechain {
namespace NodeStore {

/** Utility for producing flattened node objects.
    @note This defines the database format of a NodeObject!
*/
//  TODO Make allocator aware and use short_alloc
struct EncodedBlob
{
public:
    void prepare (NodeObject::Ptr const& object);
    void const* getKey () const noexcept { return m_key; }
    std::size_t getSize () const noexcept { return m_size; }
    void const* getData () const noexcept { return m_data.getData (); }

private:
    void const* m_key;
    beast::MemoryBlock m_data;
    std::size_t m_size;
};

}
}

#endif
