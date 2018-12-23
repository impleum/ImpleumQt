// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

void MineGenesis(CBlock genesis){
    // This will figure out a valid hash and Nonce if you're creating a different genesis block:
    uint256 hashTarget = CBigNum().SetCompact(Params().ProofOfWorkLimit().GetCompact()).getuint256();
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    uint256 newhash = genesis.GetHash();
    uint256 besthash;
    memset(&besthash,0xFF,32);
    while (newhash > hashTarget) {
        ++genesis.nNonce;
        if (genesis.nNonce == 0){
            printf("NONCE WRAPPED, incrementing time");
            ++genesis.nTime;
        }
    newhash = genesis.GetHash();
    if(newhash < besthash){
        besthash=newhash;
        printf("New best: %s\n", newhash.GetHex().c_str());
    }
    }
    printf("Found Genesis, Nonce: %ld, Hash: %s\n", genesis.nNonce, genesis.GetHash().GetHex().c_str());
    printf("Gensis Hash Merkle: %s\n", genesis.hashMerkleRoot.ToString().c_str());
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//
// Impleum Main
//

class CImpleumMainParams : public CChainParams {
public:
    CImpleumMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x51;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x41;
        pchMessageStart[3] = 0x31;
        //vAlertPubKey = ParseHex("0486bce1bac0d543f104cbff2bd23680056a3b9ea05e1137d2ff90eeb5e08472eb500322593a2cb06fbf8297d7beb6cd30cb90f98153b5b7cce1493749e41e0284");
        nDefaultPort = 16171;
        nRPCPort = 16172;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        //CBlock(hash=000001faef25dec4fbcf906e6242621df2c183bf232f263d0ba5b101911e4563, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90, nTime=1393221600, nBits=1e0fffff, nNonce=164482, vtx=1, vchBlockSig=)
        //  Coinbase(hash=12630d16a9, nTime=1393221600, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //    CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a24323020466562203230313420426974636f696e2041544d7320636f6d6520746f20555341)
        //    CTxOut(empty)
        //  vMerkleTree: 12630d16a9
        const char* pszTimestamp = "https://cryptocrimson.com/news/apple-payment-request-api-ripple-interledger-protocol";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(63) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1523364655, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1523364655;
        genesis.nBits    = 0x1e0fffff;
        genesis.nNonce   = 2380297;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("02a8be139ec629b13df22e7abc7f9ad5239df39efaf2f5bf3ab5e4d102425dbe"));
        assert(genesis.hashMerkleRoot == uint256("bd3233dd8d4e7ce3ee8097f4002b4f9303000a5109e02a402d41d2faf74eb244"));

        vSeeds.push_back(CDNSSeedData("Seednode1", "impleum.com"));
        vSeeds.push_back(CDNSSeedData("Seednode2", "masterpool.pw"));
        vSeeds.push_back(CDNSSeedData("Seednode3", "109.108.77.134"));
        vSeeds.push_back(CDNSSeedData("Seednode4", "62.80.181.141"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 102);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 125);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, (63+128));
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xC2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0xDD).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 100000;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::IMPLEUMMAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CImpleumMainParams impleumMainParams;

//
// Impleum Testnet
//

class CImpleumTestNetParams : public CImpleumMainParams {
public:
    CImpleumTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x51;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x41;
        pchMessageStart[3] = 0x31;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        vAlertPubKey = ParseHex("0471dc165db490094d35cde15b1f5d755fa6ad6f2b5ed0f340e3f17f57389c3c2af113a8cbcc885bde73305a553b5640c83021128008ddf882e856336269080496");
        nDefaultPort = 16271;
        nRPCPort = 16272;

        strDataDir = "impleumtestnet";
        // Modify the testnet genesis block so the timestamp is valid for a later start.
        unsigned int tempComp = bnProofOfWorkLimit.GetCompact();
        genesis.nBits  = tempComp;//520159231;
        genesis.nNonce = 1503601;
        genesis.nTime    = 152336701;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("dab06c78af72a1fb8a17581c631ec2d3df60a8029142962de5448b992b47467e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("Seednode1", "texplorer.impleum.com"));
        vSeeds.push_back(CDNSSeedData("Seednode2", "faucet.impleum.com"));
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 102); // Impleum test net start with i
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 65 + 128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nLastPOWBlock = 100000;
    }
    virtual Network NetworkID() const { return CChainParams::IMPLEUMTEST; }
};
static CImpleumTestNetParams impleumTestParams;

//
// Impleum Regression test
//
class CImpleumRegTestParams : public CImpleumTestNetParams {
public:
    CImpleumRegTestParams() {
        pchMessageStart[0] = 0xcd;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xef;
        bnProofOfWorkLimit = CBigNum(~uint256("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
        genesis.nTime = 1520158227;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1503601;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 16371;
        nRPCPort = 16372;
        strDataDir = "regtest";
//        MineGenesis(genesis);
        assert(hashGenesisBlock == uint256("2be11e67483fb6eabb3fbd0316246ecb85a04c16d2407e0c2cb4f9a77d87b363"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::IMPLEUMREGTEST; }
};
static CImpleumRegTestParams impleumRegTestParams;

static CChainParams *pCurrentParams = &impleumMainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::IMPLEUMMAIN:
            pCurrentParams = &impleumMainParams;
            break;
        case CChainParams::IMPLEUMTEST:
            pCurrentParams = &impleumTestParams;
            break;
        case CChainParams::IMPLEUMREGTEST:
            pCurrentParams = &impleumRegTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::IMPLEUMREGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::IMPLEUMTEST);
    } else
        SelectParams(CChainParams::IMPLEUMMAIN);

    return true;
}
