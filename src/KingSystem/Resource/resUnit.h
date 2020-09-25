#pragma once

#include <KingSystem/Utils/StrTreeMap.h>
#include <hostio/seadHostIONode.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>
#include <prim/seadTypedBitFlag.h>
#include <resource/seadResourceMgr.h>
#include <thread/seadAtomic.h>
#include "KingSystem/Resource/resHandle.h"
#include "KingSystem/Utils/Thread/Event.h"
#include "KingSystem/Utils/Thread/Task.h"
#include "KingSystem/Utils/Types.h"

namespace sead {
class ArchiveRes;
class DirectResource;
class FileDevice;
}  // namespace sead

namespace ksys::res {

class Cache;
class Handle;
class LoadRequest;
class OverlayArena;
class ResourceUnit;

class ResourceUnitMapNode : public util::StrTreeMapNode {
public:
    explicit ResourceUnitMapNode(ResourceUnit* unit) : mUnit(unit) {}
    ~ResourceUnitMapNode() override { ; }
    void erase_() override { util::StrTreeMapNode::erase_(); }

    ResourceUnit* getUnit() const { return mUnit; }
    util::StrTreeMapKey& key() { return mKey; }
    const util::StrTreeMapKey& key() const { return mKey; }

private:
    ResourceUnit* mUnit = nullptr;
};

class ResourceUnit : public sead::hostio::Node {
    SEAD_RTTI_BASE(ResourceUnit)
public:
    enum class Status {
        _0 = 0,
        _1 = 1,
        _2 = 2,
        _3 = 3,
        _4 = 4,
        _5 = 5,
        _6 = 6,
        _7 = 7,
        _8 = 8,
        _9 = 9,
        _10 = 10,
        _11 = 11,
        _12 = 12,
        _13 = 13,
        _14 = 14,
        _15 = 15,
    };

    struct InitArg {
        bool set_flag_1;
        bool set_flag_2;
        bool set_flag_4;
        sead::Atomic<bool> load_req_field_26;
        sead::Atomic<bool> load_req_field_28;
        Handle* handle;
        Cache* cache;
        void* _18;
        sead::Heap* heap;
        LoadRequest* load_req;
        OverlayArena* arena;
        u32 alloc_size;
        sead::SafeString path;
    };
    KSYS_CHECK_SIZE_NX150(InitArg, 0x50);

    explicit ResourceUnit(const InitArg& arg);
    ResourceUnit();
    virtual ~ResourceUnit();

    bool init(const InitArg& arg);

    void unloadArchiveRes();

    void attachHandle(Handle* handle);

    s32 getRefCount() const;

    Status getStatus() const;
    void updateStatus();

    bool isTask1NotQueued() const;
    bool isStatus0() const;
    bool isTask1ActiveOrStatus7() const;
    bool isStatus1() const;
    bool isStatus9_12_15() const;
    bool isParseOk() const;
    bool isStatusFlag8000Set() const;
    bool isLinkedToResourceMgr() const;
    size_t getHeapSize() const;
    sead::Heap* getHeap() const;

    bool isLinkedToCache() const;
    void setIsLinkedToCache(bool linked);
    const auto& getCacheKey() const { return mMapNode.key(); }
    void removeFromCache();

    bool removeTask3FromQueue();

    static size_t getArenaUnitListNodeOffset() {
        return offsetof(ResourceUnit, mArenaUnitListNode);
    }

    static size_t getArenaUnitListNode2Offset() {
        return offsetof(ResourceUnit, mArenaUnitListNode2);
    }

    static size_t getResMgrUnitListNodeOffset() {
        return offsetof(ResourceUnit, mResMgrUnitListNode);
    }

private:
    friend class Handle;

    enum class CacheFlag : u8 {
        IsLinkedToCache = 0x1,
    };

    enum class Flag : u8 {
        _1 = 0x1,
        _2 = 0x2,
        _4 = 0x4,
    };

    enum class StatusFlag {
        HasHeap = 0x1,
        BufferSizeIsNonZero = 0x10,
        LoadFromArchive = 0x20,
        LoadReqField24IsTrue = 0x40,
        FailedMaybe = 0x100,
        FileSizeIsZero = 0x200,
        FileSizeExceedsAllocSize = 0x800,
        _1000 = 0x1000,
        FileOrResInstanceTooLargeForHeap = 0x2000,
        LoadFailed = 0x4000,
        NeedToIncrementRefCount = 0x8000,
        _20000 = 0x20000,
        _40000 = 0x40000,
        _80000 = 0x80000,
    };

    sead::TypedBitFlag<CacheFlag> mCacheFlags;
    sead::TypedBitFlag<Flag> mFlags;
    sead::TypedBitFlag<StatusFlag> mStatusFlags;
    Status mStatus = Status::_0;
    OverlayArena* mArena = nullptr;
    OverlayArena* mArena1 = nullptr;
    OverlayArena* mArena2 = nullptr;
    OverlayArena* mLoadReqArena = nullptr;
    sead::DirectResource* mResource = nullptr;
    Handle mArchiveResHandle;
    sead::ArchiveRes* mArchiveRes = nullptr;
    void* mLoadReqField68 = nullptr;
    Cache* mCache = nullptr;
    sead::FileDevice* mFileDevice = nullptr;
    u32 mLoadReqAllocSize = 0;
    u32 mAllocSize = 0;
    u32 mInfoAllocSize = 0;
    sead::Atomic<s32> mRefCount;
    sead::ListNode mArenaUnitListNode;
    sead::ListNode mArenaUnitListNode2;
    sead::ListNode mResMgrUnitListNode;
    util::Task mTask1;
    util::Task mTask2;
    util::Task mTask3;
    ResourceUnitMapNode mMapNode{this};
    sead::Atomic<s32> mCounter;
    util::Event mEvent{nullptr,
                       sead::IDisposer::HeapNullOption::DoNotAppendDisposerIfNoHeapSpecified, true};
    sead::ResourceMgr::LoadArg mLoadArg;
    sead::FixedSafeString<128> mPath;
    sead::Heap* mHeap = nullptr;
};
KSYS_CHECK_SIZE_NX150(ResourceUnit, 0x468);

}  // namespace ksys::res
