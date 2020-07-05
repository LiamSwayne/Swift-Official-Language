#ifndef SWIFT_SERIALIZATION_MODULE_SUMMARY_FILE_H
#define SWIFT_SERIALIZATION_MODULE_SUMMARY_FILE_H

#include "swift/SIL/ModuleSummary.h"
#include "llvm/Bitcode/RecordLayout.h"
#include "llvm/Support/MemoryBuffer.h"
#include <memory>

namespace swift {

namespace modulesummary {

using llvm::BCArray;
using llvm::BCBlob;
using llvm::BCFixed;
using llvm::BCGenericRecordLayout;
using llvm::BCRecordLayout;
using llvm::BCVBR;

const unsigned char MODULE_SUMMARY_SIGNATURE[] = {'M', 'O', 'D', 'S'};

enum BlockID {
  MODULE_SUMMARY_ID = llvm::bitc::FIRST_APPLICATION_BLOCKID,

  FUNCTION_SUMMARY_ID,
};

namespace module_summary {
enum {
  MODULE_METADATA,
};

using MetadataLayout = BCRecordLayout<
  MODULE_METADATA,
  BCBlob // Module name
>;
};

namespace function_summary {
enum {
  METADATA,
  CALL_GRAPH_EDGE,
};

using MetadataLayout = BCRecordLayout<METADATA,
                                      BCVBR<16>,  // Function GUID
                                      BCFixed<1>, // live
                                      BCBlob      // Name string
                                      >;
using CallGraphEdgeLayout =
    BCRecordLayout<CALL_GRAPH_EDGE,
                   BCFixed<32>, // FunctionSummary::Edge::Kind
                   BCVBR<16>,   // Target Function GUID
                   BCVBR<16>    // Table GUID
                   >;
} // namespace function_summary

bool emitModuleSummaryIndex(const ModuleSummaryIndex &index,
                            DiagnosticEngine &diags, StringRef path);

bool loadModuleSummaryIndex(llvm::MemoryBufferRef inputBuffer,
                            ModuleSummaryIndex &moduleSummary);
} // namespace modulesummary
} // namespace swift

#endif
