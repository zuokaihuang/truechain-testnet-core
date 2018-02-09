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

#ifndef SKYWELL_APP_PATHS_SKYWELLSTATE_H_INCLUDED
#define SKYWELL_APP_PATHS_SKYWELLSTATE_H_INCLUDED

#include <transaction/book/Types.h>
#include <protocol/STAmount.h>
#include <cstdint>
#include <beast/cxx14/memory.h> // <memory>

namespace truechain {

//
// A truechain line's state.
// - Isolate ledger entry format.
//

class SkywellState
{
public:
    typedef std::shared_ptr <SkywellState> pointer;

public:
    SkywellState () = delete;

    virtual ~SkywellState () { }

    static SkywellState::pointer makeItem (
        Account const& accountID, STLedgerEntry::ref ledgerEntry);

    LedgerEntryType getType ()
    {
        return ltSKYWELL_STATE;
    }

    Account const& getAccountID () const
    {
        return  mViewLowest ? mLowID : mHighID;
    }

    Account const& getAccountIDPeer () const
    {
        return !mViewLowest ? mLowID : mHighID;
    }

    // True, Provided auth to peer.
    bool getAuth () const
    {
        return mFlags & (mViewLowest ? lsfLowAuth : lsfHighAuth);
    }

    bool getAuthPeer () const
    {
        return mFlags & (!mViewLowest ? lsfLowAuth : lsfHighAuth);
    }

    bool getNoSkywell () const
    {
        return mFlags & (mViewLowest ? lsfLowNoSkywell : lsfHighNoSkywell);
    }

    bool getNoSkywellPeer () const
    {
        return mFlags & (!mViewLowest ? lsfLowNoSkywell : lsfHighNoSkywell);
    }

    /** Have we set the freeze flag on our peer */
    bool getFreeze () const
    {
        return mFlags & (mViewLowest ? lsfLowFreeze : lsfHighFreeze);
    }

    /** Has the peer set the freeze flag on us */
    bool getFreezePeer () const
    {
        return mFlags & (!mViewLowest ? lsfLowFreeze : lsfHighFreeze);
    }

    STAmount const& getBalance () const
    {
        return mBalance;
    }

    STAmount const& getLimit () const
    {
        return  mViewLowest ? mLowLimit : mHighLimit;
    }

    STAmount const& getLimitPeer () const
    {
        return !mViewLowest ? mLowLimit : mHighLimit;
    }

    std::uint32_t getQualityIn () const
    {
        return ((std::uint32_t) (mViewLowest ? mLowQualityIn : mHighQualityIn));
    }

    std::uint32_t getQualityOut () const
    {
        return ((std::uint32_t) (mViewLowest ? mLowQualityOut : mHighQualityOut));
    }

    STLedgerEntry::pointer getSLE ()
    {
        return mLedgerEntry;
    }

    const STLedgerEntry& peekSLE () const
    {
        return *mLedgerEntry;
    }

    STLedgerEntry& peekSLE ()
    {
        return *mLedgerEntry;
    }

    Json::Value getJson (int);

    Blob getRaw () const;

private:
    SkywellState (
        STLedgerEntry::ref ledgerEntry,
        Account const& viewAccount);

private:
    STLedgerEntry::pointer  mLedgerEntry;

    bool                            mViewLowest;

    std::uint32_t                   mFlags;

    STAmount const&                 mLowLimit;
    STAmount const&                 mHighLimit;

    Account const&                  mLowID;
    Account const&                  mHighID;

    std::uint64_t                   mLowQualityIn;
    std::uint64_t                   mLowQualityOut;
    std::uint64_t                   mHighQualityIn;
    std::uint64_t                   mHighQualityOut;

    STAmount                        mBalance;
};

std::vector <SkywellState::pointer>
getSkywellStateItems (
    Account const& accountID,
    Ledger::ref ledger);

} // truechain

#endif
