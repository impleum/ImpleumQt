// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
       boost::assign::map_list_of
        (  0,     uint256("02a8be139ec629b13df22e7abc7f9ad5239df39efaf2f5bf3ab5e4d102425dbe") )
        (  2,     uint256("49c9ce21a7916a4026e782a4728ba626b02cc6caf35615c4d7c9400ad22b5282") ) // Premine
        ( 35,     uint256("477d36da0993b3a5e279fd0eba7ab4825b4ff54f0c3e55b8df4e7e6c1afe6939") )
        ( 2500,   uint256("49a2d1719097b5d9ec81d89627eaa71dfefb158cb0bc0ac58051d5ca0089dd98") )
        ( 4000,   uint256("bd4c0a8c11431012f1b59be225b5913a1f06e1225e85a10216f2be5db1b4c0f1") )
        ( 34000,  uint256("e490b0d5eda1874bdb3ce5e2567e3b51b26bd73c05d9e9c83f614d634093a8a8") )
        ( 40000,  uint256("95c59e88378c0a1fed38f0797a49d3e7bb63ab9079b56a86c8eedaa570cfd672") )
        ( 71000,  uint256("298ee8172927a727ba26820d53ba491fc2025f275bd25513203e643593849501") )
        ( 100000,  uint256("54cdf03ca463e416bc1c759bf9e6e5367f06288c47805b421ff33f731a9ffcd3") )
        ( 150000,  uint256("8c8b2a0746e2e8f277807d5af79926f129fd1ea15397d27359aaa9e9eee104e9") )
        ( 200000,  uint256("a8c3901f1752cea4defdea41ee94221eb9b43c8836b995eb1bb873538b7d18b4") )
        ( 215001,  uint256("04c2b9fe7e52e0c6d54fbdf5018fcda8709457b1c12b0dc8eae185b2018de19a") )
        ( 250000,  uint256("36bca99e22d680d6bb4b2dd7b844e1a939925d5bebe320f1d9f5c7c8adb87882") );
 
    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
