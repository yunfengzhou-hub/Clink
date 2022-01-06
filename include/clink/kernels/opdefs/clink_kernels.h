/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// MLIR op definitions for clink kernels.
// This file declares the 'clink' dialect as well as the operators in
// the clink library.

#ifndef CLINK_KERNELS_OPDEFS_CLINK_KERNELS_H_
#define CLINK_KERNELS_OPDEFS_CLINK_KERNELS_H_

#include "mlir/IR/Dialect.h"
#include "mlir/IR/DialectImplementation.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"

using namespace mlir;

namespace clink {

// Dialect for clink operations.
class ClinkDialect : public Dialect {
public:
  static StringRef getDialectNamespace() { return "clink"; }
  explicit ClinkDialect(MLIRContext *context);

  mlir::Type parseType(mlir::DialectAsmParser &parser) const override;
  void printType(mlir::Type type,
                 mlir::DialectAsmPrinter &printer) const override;
};

} // namespace clink

#define GET_OP_CLASSES
#include "clink/kernels/opdefs/clink_kernels.h.inc"

#endif // CLINK_KERNELS_OPDEFS_CLINK_KERNELS_H_
