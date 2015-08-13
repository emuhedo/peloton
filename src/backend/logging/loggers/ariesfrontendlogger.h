/*-------------------------------------------------------------------------
 *
 * ariesfrontendlogger.h
 * file description
 *
 * Copyright(c) 2015, CMU
 *
 * /peloton/src/backend/logging/ariesfrontendlogger.h
 *
 *-------------------------------------------------------------------------
 */

#pragma once

#include "backend/logging/frontendlogger.h"
#include "backend/logging/records/tuplerecord.h"
#include "backend/concurrency/transaction.h"

#include <fcntl.h>

namespace peloton {
namespace logging {

static std::vector<LogRecord*> aries_global_queue;

//===--------------------------------------------------------------------===//
// Aries Frontend Logger 
//===--------------------------------------------------------------------===//

class AriesFrontendLogger : public FrontendLogger{

  public:

    AriesFrontendLogger(void);

   ~AriesFrontendLogger(void);

    void MainLoop(void);

    void CollectLogRecord(void);

    void Flush(void);

    //===--------------------------------------------------------------------===//
    // Recovery 
    //===--------------------------------------------------------------------===//

    void Recovery(void);

  private:

    LogRecordType GetNextLogRecordType(void);

    size_t LogFileSize();

    size_t GetNextFrameSize(void);

    size_t GetLogRecordCount() const;

    bool ReadTupleRecordHeader(TupleRecord& tupleRecord);

    storage::Tuple* ReadTupleRecordBody(catalog::Schema* schema);

    storage::DataTable* GetTable(TupleRecord tupleRecord);

    storage::TileGroup* GetTileGroup(oid_t tile_group_id);

    void RedoInsert(concurrency::Transaction* txn);
    void RedoDelete(concurrency::Transaction* txn);
    void RedoUpdate(concurrency::Transaction* txn);

    void InsertTuple(concurrency::Transaction* txn);
    void DeleteTuple(concurrency::Transaction* txn);
    void UpdateTuple(concurrency::Transaction* txn);

    //===--------------------------------------------------------------------===//
    // Member Variables
    //===--------------------------------------------------------------------===//

    // FIXME :: Hard coded file name
    std::string filename = "/home/parallels/git/peloton/build/aries_log.txt";

    // FIXME :: Hard coded global_queue size
    oid_t aries_global_queue_size = 1;

    // File pointer and descriptor
    FILE* logFile;

    int logFileFd;

    // permit reading and writing by the owner, and to permit reading
    // only by group members and others.
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

};

}  // namespace logging
}  // namespace peloton
