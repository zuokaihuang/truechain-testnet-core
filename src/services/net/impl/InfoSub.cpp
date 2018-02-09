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

#include <BeastConfig.h>
#include <services/net/InfoSub.h>
#include <atomic>

namespace truechain {

// This is the primary interface into the "client" portion of the program.
// Code that wants to do normal operations on the network such as
// creating and monitoring accounts, creating transactions, and so on
// should use this interface. The RPC code will primarily be a light wrapper
// over this code.

// Eventually, it will check the node's operating mode (synched, unsynched,
// etectera) and defer to the correct means of processing. The current
// code assumes this node is synched (and will continue to do so until
// there's a functional network.

//------------------------------------------------------------------------------

InfoSub::Source::Source (char const* name, Stoppable& parent)
    : Stoppable (name, parent)
{
}

//------------------------------------------------------------------------------

InfoSub::InfoSub (Source& source, Consumer consumer)
    : m_consumer (consumer)
    , m_source (source)
{
    static std::atomic <int> s_seq_id (0);
    mSeq = ++s_seq_id;
}

InfoSub::~InfoSub ()
{
    m_source.unsubTransactions (mSeq);
    m_source.unsubRTTransactions (mSeq);
    m_source.unsubLedger (mSeq);
    m_source.unsubServer (mSeq);

    // Use the internal unsubscribe so that it won't call
    // back to us and modify its own parameter
    if (! mSubAccountInfo_t.empty ())
        m_source.unsubAccountInternal
            (mSeq, mSubAccountInfo_t, true);

    if (! mSubAccountInfo_t.empty ())
        m_source.unsubAccountInternal
            (mSeq, mSubAccountInfo_f, false);
}

Resource::Consumer& InfoSub::getConsumer()
{
    return m_consumer;
}

void InfoSub::send (
    Json::Value const& jvObj, std::string const& sObj, bool broadcast)
{
    send (jvObj, broadcast);
}

std::uint64_t InfoSub::getSeq ()
{
    return mSeq;
}

void InfoSub::onSendEmpty ()
{
}

void InfoSub::insertSubAccountInfo (SkywellAddress addr, bool rt)
{
    ScopedLockType sl (mLock);

    (rt ? mSubAccountInfo_t : mSubAccountInfo_f).insert (addr);
}

void InfoSub::deleteSubAccountInfo (SkywellAddress addr, bool rt)
{
    ScopedLockType sl (mLock);

    (rt ? mSubAccountInfo_t : mSubAccountInfo_f).erase (addr);
}

void InfoSub::clearPathRequest ()
{
    mPathRequest.reset ();
}

void InfoSub::setPathRequest (const std::shared_ptr<PathRequest>& req)
{
    mPathRequest = req;
}

const std::shared_ptr<PathRequest>& InfoSub::getPathRequest ()
{
    return mPathRequest;
}

} // truechain
