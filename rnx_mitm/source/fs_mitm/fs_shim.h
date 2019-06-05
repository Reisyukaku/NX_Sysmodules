/**
 * @file fs_shim.h
 * @brief Filesystem Services (fs) IPC wrapper. To be merged into libnx, eventually.
 * @author SciresM
 * @copyright libnx Authors
 */
#pragma once
#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Missing fsp-srv commands. */
Result fsOpenBisStorageFwd(Service* s, FsStorage* out, u32 PartitionId);
Result fsOpenDataStorageByCurrentProcessFwd(Service* s, FsStorage* out);
Result fsOpenDataStorageByDataIdFwd(Service* s, FsStorageId storage_id, u64 data_id, FsStorage* out);
Result fsOpenFileSystemWithPatchFwd(Service* s, FsFileSystem* out, u64 titleId, FsFileSystemType fsType);
Result fsOpenFileSystemWithIdFwd(Service* s, FsFileSystem* out, u64 titleId, FsFileSystemType fsType, const char* contentPath);
Result fsOpenSaveDataFileSystemFwd(Service* s, FsFileSystem* out, u8 inval, FsSave *save);

/* Missing FS File commands. */
Result fsFileOperateRange(FsFile* f, u32 op_id, u64 off, u64 len, FsRangeInfo *out);

/* Missing FS Storage commands. */
Result fsStorageOperateRange(FsStorage* s, u32 op_id, u64 off, u64 len, FsRangeInfo *out);

#ifdef __cplusplus
}
#endif