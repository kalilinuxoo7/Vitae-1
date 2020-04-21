#ifndef VITAE_ZVITTRACKER_H
#define VITAE_ZVITTRACKER_H

#include "zerocoin.h"
#include "witness.h"
#include "sync.h"
#include <list>

class CDeterministicMint;
class CzVITWallet;

class CzVITTracker
{
private:
    bool fInitialized;
    std::string strWalletFile;
    std::map<uint256, CMintMeta> mapSerialHashes;
    std::map<uint256, uint256> mapPendingSpends; //serialhash, txid of spend
    std::map<uint256, std::unique_ptr<CoinWitnessData> > mapStakeCache; //serialhash, witness value, height
    bool UpdateStatusInternal(const std::set<uint256>& setMempool, CMintMeta& mint);
public:
    CzVITTracker(std::string strWalletFile);
    ~CzVITTracker();
    void Add(const CDeterministicMint& dMint, bool isNew = false, bool isArchived = false, CzVITWallet* zPIVWallet = NULL);
    void Add(const CZerocoinMint& mint, bool isNew = false, bool isArchived = false);
    bool Archive(CMintMeta& meta);
    bool HasPubcoin(const CBigNum& bnValue) const;
    bool HasPubcoinHash(const uint256& hashPubcoin) const;
    bool HasSerial(const CBigNum& bnSerial) const;
    bool HasSerialHash(const uint256& hashSerial) const;
    bool HasMintTx(const uint256& txid);
    bool IsEmpty() const { return mapSerialHashes.empty(); }
    void Init();
    CMintMeta Get(const uint256& hashSerial);
    CMintMeta GetMetaFromPubcoin(const uint256& hashPubcoin);
    bool GetMetaFromStakeHash(const uint256& hashStake, CMintMeta& meta) const;
    CAmount GetBalance(bool fConfirmedOnly, bool fUnconfirmedOnly) const;
    std::vector<uint256> GetSerialHashes();
    mutable CCriticalSection cs_spendcache;
    CoinWitnessData* GetSpendCache(const uint256& hashStake) EXCLUSIVE_LOCKS_REQUIRED(cs_spendcache);
    bool ClearSpendCache() EXCLUSIVE_LOCKS_REQUIRED(cs_spendcache);
    std::vector<CMintMeta> GetMints(bool fConfirmedOnly) const;
    CAmount GetUnconfirmedBalance() const;
    std::set<CMintMeta> ListMints(bool fUnusedOnly, bool fMatureOnly, bool fUpdateStatus, bool fWrongSeed = false, bool fExcludeV1 = false);
    void RemovePending(const uint256& txid);
    void SetPubcoinUsed(const uint256& hashPubcoin, const uint256& txid);
    void SetPubcoinNotUsed(const uint256& hashPubcoin);
    bool UnArchive(const uint256& hashPubcoin, bool isDeterministic);
    bool UpdateZerocoinMint(const CZerocoinMint& mint);
    bool UpdateState(const CMintMeta& meta);
    void Clear();
};

#endif //VITAE_ZVITTRACKER_H
