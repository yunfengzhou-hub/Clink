/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Op Definitions                                                             *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifdef GET_OP_LIST
#undef GET_OP_LIST

::clink::OneHotEncoderLoadOp,
::clink::SquareAddF64Op,
::clink::SquareF64Op
#endif  // GET_OP_LIST

#ifdef GET_OP_CLASSES
#undef GET_OP_CLASSES


//===----------------------------------------------------------------------===//
// Local Utility Method Definitions
//===----------------------------------------------------------------------===//

namespace clink {

static ::mlir::LogicalResult __mlir_ods_local_type_constraint_clink_kernels0(
    ::mlir::Operation *op, ::mlir::Type type, ::llvm::StringRef valueKind,
    unsigned valueIndex) {
  if (!((type.isF64()))) {
    return op->emitOpError(valueKind) << " #" << valueIndex
        << " must be 64-bit float, but got " << type;
  }
  return ::mlir::success();
}

static ::mlir::LogicalResult __mlir_ods_local_type_constraint_clink_kernels1(
    ::mlir::Operation *op, ::mlir::Type type, ::llvm::StringRef valueKind,
    unsigned valueIndex) {
    std::cout << __LINE__ << std::endl;
    std::cout << __LINE__ << (type == NULL) << std::endl;
    // std::cout << __LINE__ << type.isa<clink::ModelType>() << std::endl;
  if (!((type.isa<clink::ModelType>()))) {
    std::cout << __LINE__ << std::endl;
    return op->emitOpError(valueKind) << " #" << valueIndex
        << " must be !clink.onehotencoder type, but got " << type;
    std::cout << __LINE__ << std::endl;
  }
    std::cout << __LINE__ << std::endl;
  return ::mlir::success();
}
} // namespace clink
namespace clink {

//===----------------------------------------------------------------------===//
// ::clink::OneHotEncoderLoadOp definitions
//===----------------------------------------------------------------------===//

OneHotEncoderLoadOpAdaptor::OneHotEncoderLoadOpAdaptor(::mlir::ValueRange values, ::mlir::DictionaryAttr attrs, ::mlir::RegionRange regions)  : odsOperands(values), odsAttrs(attrs), odsRegions(regions) {

}

OneHotEncoderLoadOpAdaptor::OneHotEncoderLoadOpAdaptor(OneHotEncoderLoadOp &op)  : odsOperands(op->getOperands()), odsAttrs(op->getAttrDictionary()), odsRegions(op->getRegions()) {

}

::mlir::ValueRange OneHotEncoderLoadOpAdaptor::getOperands() {
  return odsOperands;
}
std::pair<unsigned, unsigned> OneHotEncoderLoadOpAdaptor::getODSOperandIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::ValueRange OneHotEncoderLoadOpAdaptor::getODSOperands(unsigned index) {
  auto valueRange = getODSOperandIndexAndLength(index);
  return {std::next(odsOperands.begin(), valueRange.first),
           std::next(odsOperands.begin(), valueRange.first + valueRange.second)};
}
::mlir::DictionaryAttr OneHotEncoderLoadOpAdaptor::getAttributes() {
  return odsAttrs;
}
::mlir::LogicalResult OneHotEncoderLoadOpAdaptor::verify(::mlir::Location loc) {
  return ::mlir::success();
}


std::pair<unsigned, unsigned> OneHotEncoderLoadOp::getODSOperandIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::Operation::operand_range OneHotEncoderLoadOp::getODSOperands(unsigned index) {
  auto valueRange = getODSOperandIndexAndLength(index);
  return {std::next(getOperation()->operand_begin(), valueRange.first),
           std::next(getOperation()->operand_begin(), valueRange.first + valueRange.second)};
}
std::pair<unsigned, unsigned> OneHotEncoderLoadOp::getODSResultIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::Operation::result_range OneHotEncoderLoadOp::getODSResults(unsigned index) {
  auto valueRange = getODSResultIndexAndLength(index);
  return {std::next(getOperation()->result_begin(), valueRange.first),
           std::next(getOperation()->result_begin(), valueRange.first + valueRange.second)};
}
void OneHotEncoderLoadOp::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::Type resultType0, ::mlir::Value odsArg_0) {
  odsState.addOperands(odsArg_0);
  odsState.addTypes(resultType0);
}
void OneHotEncoderLoadOp::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::Value odsArg_0) {
  odsState.addOperands(odsArg_0);

        ::llvm::SmallVector<::mlir::Type, 2> inferredReturnTypes;
        if (::mlir::succeeded(OneHotEncoderLoadOp::inferReturnTypes(odsBuilder.getContext(),
                      odsState.location, odsState.operands,
                      odsState.attributes.getDictionary(odsState.getContext()),
                      /*regions=*/{}, inferredReturnTypes)))
          odsState.addTypes(inferredReturnTypes);
        else
          ::llvm::report_fatal_error("Failed to infer result type(s).");
}
void OneHotEncoderLoadOp::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::TypeRange resultTypes, ::mlir::Value odsArg_0) {
  odsState.addOperands(odsArg_0);
  assert(resultTypes.size() == 1u && "mismatched number of results");
  odsState.addTypes(resultTypes);
}
void OneHotEncoderLoadOp::build(::mlir::OpBuilder &, ::mlir::OperationState &odsState, ::mlir::TypeRange resultTypes, ::mlir::ValueRange operands, ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  assert(operands.size() == 1u && "mismatched number of parameters");
  odsState.addOperands(operands);
  odsState.addAttributes(attributes);
  assert(resultTypes.size() == 1u && "mismatched number of return types");
  odsState.addTypes(resultTypes);
}
void OneHotEncoderLoadOp::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::ValueRange operands, ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  assert(operands.size() == 1u && "mismatched number of parameters");
  odsState.addOperands(operands);
  odsState.addAttributes(attributes);

    ::mlir::SmallVector<::mlir::Type, 2> inferredReturnTypes;
    if (::mlir::succeeded(OneHotEncoderLoadOp::inferReturnTypes(odsBuilder.getContext(),
                  odsState.location, operands,
                  odsState.attributes.getDictionary(odsState.getContext()),
                  /*regions=*/{}, inferredReturnTypes))) {  assert(inferredReturnTypes.size() == 1u && "mismatched number of return types");
      odsState.addTypes(inferredReturnTypes);
    } else
      ::llvm::report_fatal_error("Failed to infer result type(s).");
}
::mlir::LogicalResult OneHotEncoderLoadOp::verify() {
    std::cout << __LINE__ << std::endl;
  {
    unsigned index = 0; (void)index;
    auto valueGroup0 = getODSOperands(0);

    for (auto v : valueGroup0) {
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels0(*this, v.getType(), "operand", index++)))
        return ::mlir::failure();
    }
  }
    std::cout << __LINE__ << std::endl;
  {
    unsigned index = 0; (void)index;
    auto valueGroup0 = getODSResults(0);

    std::cout << __LINE__ << std::endl;
    for (auto v : valueGroup0) {
    std::cout << __LINE__ << std::endl;
    std::cout << __LINE__ << getODSResultIndexAndLength(0).first << getODSResultIndexAndLength(0).second << std::endl;
    std::cout << __LINE__ << typeid(v).name() << std::endl;
    // std::cout << __LINE__ << v.isa<clink::ModelType>() << std::endl;
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels1(*this, v.getType(), "result", index++)))
        return ::mlir::failure();
    std::cout << __LINE__ << std::endl;
    }
  }
    std::cout << __LINE__ << std::endl;
  return ::mlir::success();
}
::mlir::LogicalResult OneHotEncoderLoadOp::inferReturnTypes(::mlir::MLIRContext *context, ::llvm::Optional<::mlir::Location> location, ::mlir::ValueRange operands, ::mlir::DictionaryAttr attributes, ::mlir::RegionRange regions, ::llvm::SmallVectorImpl<::mlir::Type>&inferredReturnTypes) {
  inferredReturnTypes.resize(1);
  ::mlir::Builder odsBuilder(context);
  inferredReturnTypes[0] = odsBuilder.getType<clink::ModelType>();
  return ::mlir::success();
}
::mlir::ParseResult OneHotEncoderLoadOp::parse(::mlir::OpAsmParser &parser, ::mlir::OperationState &result) {
  ::mlir::SmallVector<::mlir::OpAsmParser::OperandType, 4> allOperands;
  ::llvm::SMLoc allOperandLoc = parser.getCurrentLocation();
  if (parser.parseOperandList(allOperands))
    return ::mlir::failure();
  if (parser.parseOptionalAttrDict(result.attributes))
    return ::mlir::failure();
  ::mlir::Type odsBuildableType0 = parser.getBuilder().getType<clink::ModelType>();
  ::mlir::Type odsBuildableType1 = parser.getBuilder().getF64Type();
  result.addTypes(odsBuildableType0);
  if (parser.resolveOperands(allOperands, odsBuildableType1, allOperandLoc, result.operands))
    return ::mlir::failure();
  return ::mlir::success();
}
void OneHotEncoderLoadOp::print(::mlir::OpAsmPrinter &_odsPrinter) {
  _odsPrinter << ' ';
  _odsPrinter << getOperation()->getOperands();
  _odsPrinter.printOptionalAttrDict((*this)->getAttrs(), /*elidedAttrs=*/{});
}
} // namespace clink
DEFINE_EXPLICIT_TYPE_ID(::clink::OneHotEncoderLoadOp)

namespace clink {

//===----------------------------------------------------------------------===//
// ::clink::SquareAddF64Op definitions
//===----------------------------------------------------------------------===//

SquareAddF64OpAdaptor::SquareAddF64OpAdaptor(::mlir::ValueRange values, ::mlir::DictionaryAttr attrs, ::mlir::RegionRange regions)  : odsOperands(values), odsAttrs(attrs), odsRegions(regions) {

}

SquareAddF64OpAdaptor::SquareAddF64OpAdaptor(SquareAddF64Op &op)  : odsOperands(op->getOperands()), odsAttrs(op->getAttrDictionary()), odsRegions(op->getRegions()) {

}

::mlir::ValueRange SquareAddF64OpAdaptor::getOperands() {
  return odsOperands;
}
std::pair<unsigned, unsigned> SquareAddF64OpAdaptor::getODSOperandIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::ValueRange SquareAddF64OpAdaptor::getODSOperands(unsigned index) {
  auto valueRange = getODSOperandIndexAndLength(index);
  return {std::next(odsOperands.begin(), valueRange.first),
           std::next(odsOperands.begin(), valueRange.first + valueRange.second)};
}
::mlir::DictionaryAttr SquareAddF64OpAdaptor::getAttributes() {
  return odsAttrs;
}
::mlir::LogicalResult SquareAddF64OpAdaptor::verify(::mlir::Location loc) {
  return ::mlir::success();
}


std::pair<unsigned, unsigned> SquareAddF64Op::getODSOperandIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::Operation::operand_range SquareAddF64Op::getODSOperands(unsigned index) {
  auto valueRange = getODSOperandIndexAndLength(index);
  return {std::next(getOperation()->operand_begin(), valueRange.first),
           std::next(getOperation()->operand_begin(), valueRange.first + valueRange.second)};
}
std::pair<unsigned, unsigned> SquareAddF64Op::getODSResultIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::Operation::result_range SquareAddF64Op::getODSResults(unsigned index) {
  auto valueRange = getODSResultIndexAndLength(index);
  return {std::next(getOperation()->result_begin(), valueRange.first),
           std::next(getOperation()->result_begin(), valueRange.first + valueRange.second)};
}
void SquareAddF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::Type resultType0, ::mlir::Value odsArg_0, ::mlir::Value odsArg_1) {
  odsState.addOperands(odsArg_0);
  odsState.addOperands(odsArg_1);
  odsState.addTypes(resultType0);
}
void SquareAddF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::Value odsArg_0, ::mlir::Value odsArg_1) {
  odsState.addOperands(odsArg_0);
  odsState.addOperands(odsArg_1);

        ::llvm::SmallVector<::mlir::Type, 2> inferredReturnTypes;
        if (::mlir::succeeded(SquareAddF64Op::inferReturnTypes(odsBuilder.getContext(),
                      odsState.location, odsState.operands,
                      odsState.attributes.getDictionary(odsState.getContext()),
                      /*regions=*/{}, inferredReturnTypes)))
          odsState.addTypes(inferredReturnTypes);
        else
          ::llvm::report_fatal_error("Failed to infer result type(s).");
}
void SquareAddF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::TypeRange resultTypes, ::mlir::Value odsArg_0, ::mlir::Value odsArg_1) {
  odsState.addOperands(odsArg_0);
  odsState.addOperands(odsArg_1);
  assert(resultTypes.size() == 1u && "mismatched number of results");
  odsState.addTypes(resultTypes);
}
void SquareAddF64Op::build(::mlir::OpBuilder &, ::mlir::OperationState &odsState, ::mlir::TypeRange resultTypes, ::mlir::ValueRange operands, ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  assert(operands.size() == 2u && "mismatched number of parameters");
  odsState.addOperands(operands);
  odsState.addAttributes(attributes);
  assert(resultTypes.size() == 1u && "mismatched number of return types");
  odsState.addTypes(resultTypes);
}
void SquareAddF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::ValueRange operands, ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  assert(operands.size() == 2u && "mismatched number of parameters");
  odsState.addOperands(operands);
  odsState.addAttributes(attributes);

    ::mlir::SmallVector<::mlir::Type, 2> inferredReturnTypes;
    if (::mlir::succeeded(SquareAddF64Op::inferReturnTypes(odsBuilder.getContext(),
                  odsState.location, operands,
                  odsState.attributes.getDictionary(odsState.getContext()),
                  /*regions=*/{}, inferredReturnTypes))) {  assert(inferredReturnTypes.size() == 1u && "mismatched number of return types");
      odsState.addTypes(inferredReturnTypes);
    } else
      ::llvm::report_fatal_error("Failed to infer result type(s).");
}
::mlir::LogicalResult SquareAddF64Op::verify() {
  {
    unsigned index = 0; (void)index;
    auto valueGroup0 = getODSOperands(0);

    for (auto v : valueGroup0) {
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels0(*this, v.getType(), "operand", index++)))
        return ::mlir::failure();
    }
    auto valueGroup1 = getODSOperands(1);

    for (auto v : valueGroup1) {
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels0(*this, v.getType(), "operand", index++)))
        return ::mlir::failure();
    }
  }
  {
    unsigned index = 0; (void)index;
    auto valueGroup0 = getODSResults(0);

    for (auto v : valueGroup0) {
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels0(*this, v.getType(), "result", index++)))
        return ::mlir::failure();
    }
  }
  return ::mlir::success();
}
::mlir::LogicalResult SquareAddF64Op::inferReturnTypes(::mlir::MLIRContext *context, ::llvm::Optional<::mlir::Location> location, ::mlir::ValueRange operands, ::mlir::DictionaryAttr attributes, ::mlir::RegionRange regions, ::llvm::SmallVectorImpl<::mlir::Type>&inferredReturnTypes) {
  inferredReturnTypes.resize(1);
  ::mlir::Builder odsBuilder(context);
  inferredReturnTypes[0] = odsBuilder.getF64Type();
  return ::mlir::success();
}
::mlir::ParseResult SquareAddF64Op::parse(::mlir::OpAsmParser &parser, ::mlir::OperationState &result) {
  ::mlir::SmallVector<::mlir::OpAsmParser::OperandType, 4> allOperands;
  ::llvm::SMLoc allOperandLoc = parser.getCurrentLocation();
  if (parser.parseOperandList(allOperands))
    return ::mlir::failure();
  if (parser.parseOptionalAttrDict(result.attributes))
    return ::mlir::failure();
  ::mlir::Type odsBuildableType0 = parser.getBuilder().getF64Type();
  result.addTypes(odsBuildableType0);
  if (parser.resolveOperands(allOperands, ::llvm::concat<const Type>(::llvm::ArrayRef<::mlir::Type>(odsBuildableType0), ::llvm::ArrayRef<::mlir::Type>(odsBuildableType0)), allOperandLoc, result.operands))
    return ::mlir::failure();
  return ::mlir::success();
}
void SquareAddF64Op::print(::mlir::OpAsmPrinter &_odsPrinter) {
  _odsPrinter << ' ';
  _odsPrinter << getOperation()->getOperands();
  _odsPrinter.printOptionalAttrDict((*this)->getAttrs(), /*elidedAttrs=*/{});
}
} // namespace clink
DEFINE_EXPLICIT_TYPE_ID(::clink::SquareAddF64Op)

namespace clink {

//===----------------------------------------------------------------------===//
// ::clink::SquareF64Op definitions
//===----------------------------------------------------------------------===//

SquareF64OpAdaptor::SquareF64OpAdaptor(::mlir::ValueRange values, ::mlir::DictionaryAttr attrs, ::mlir::RegionRange regions)  : odsOperands(values), odsAttrs(attrs), odsRegions(regions) {

}

SquareF64OpAdaptor::SquareF64OpAdaptor(SquareF64Op &op)  : odsOperands(op->getOperands()), odsAttrs(op->getAttrDictionary()), odsRegions(op->getRegions()) {

}

::mlir::ValueRange SquareF64OpAdaptor::getOperands() {
  return odsOperands;
}
std::pair<unsigned, unsigned> SquareF64OpAdaptor::getODSOperandIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::ValueRange SquareF64OpAdaptor::getODSOperands(unsigned index) {
  auto valueRange = getODSOperandIndexAndLength(index);
  return {std::next(odsOperands.begin(), valueRange.first),
           std::next(odsOperands.begin(), valueRange.first + valueRange.second)};
}
::mlir::DictionaryAttr SquareF64OpAdaptor::getAttributes() {
  return odsAttrs;
}
::mlir::LogicalResult SquareF64OpAdaptor::verify(::mlir::Location loc) {
  return ::mlir::success();
}


std::pair<unsigned, unsigned> SquareF64Op::getODSOperandIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::Operation::operand_range SquareF64Op::getODSOperands(unsigned index) {
  auto valueRange = getODSOperandIndexAndLength(index);
  return {std::next(getOperation()->operand_begin(), valueRange.first),
           std::next(getOperation()->operand_begin(), valueRange.first + valueRange.second)};
}
std::pair<unsigned, unsigned> SquareF64Op::getODSResultIndexAndLength(unsigned index) {
  return {index, 1};
}
::mlir::Operation::result_range SquareF64Op::getODSResults(unsigned index) {
  auto valueRange = getODSResultIndexAndLength(index);
  return {std::next(getOperation()->result_begin(), valueRange.first),
           std::next(getOperation()->result_begin(), valueRange.first + valueRange.second)};
}
void SquareF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::Type resultType0, ::mlir::Value odsArg_0) {
  odsState.addOperands(odsArg_0);
  odsState.addTypes(resultType0);
}
void SquareF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::Value odsArg_0) {
  odsState.addOperands(odsArg_0);

        ::llvm::SmallVector<::mlir::Type, 2> inferredReturnTypes;
        if (::mlir::succeeded(SquareF64Op::inferReturnTypes(odsBuilder.getContext(),
                      odsState.location, odsState.operands,
                      odsState.attributes.getDictionary(odsState.getContext()),
                      /*regions=*/{}, inferredReturnTypes)))
          odsState.addTypes(inferredReturnTypes);
        else
          ::llvm::report_fatal_error("Failed to infer result type(s).");
}
void SquareF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::TypeRange resultTypes, ::mlir::Value odsArg_0) {
  odsState.addOperands(odsArg_0);
  assert(resultTypes.size() == 1u && "mismatched number of results");
  odsState.addTypes(resultTypes);
}
void SquareF64Op::build(::mlir::OpBuilder &, ::mlir::OperationState &odsState, ::mlir::TypeRange resultTypes, ::mlir::ValueRange operands, ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  assert(operands.size() == 1u && "mismatched number of parameters");
  odsState.addOperands(operands);
  odsState.addAttributes(attributes);
  assert(resultTypes.size() == 1u && "mismatched number of return types");
  odsState.addTypes(resultTypes);
}
void SquareF64Op::build(::mlir::OpBuilder &odsBuilder, ::mlir::OperationState &odsState, ::mlir::ValueRange operands, ::llvm::ArrayRef<::mlir::NamedAttribute> attributes) {
  assert(operands.size() == 1u && "mismatched number of parameters");
  odsState.addOperands(operands);
  odsState.addAttributes(attributes);

    ::mlir::SmallVector<::mlir::Type, 2> inferredReturnTypes;
    if (::mlir::succeeded(SquareF64Op::inferReturnTypes(odsBuilder.getContext(),
                  odsState.location, operands,
                  odsState.attributes.getDictionary(odsState.getContext()),
                  /*regions=*/{}, inferredReturnTypes))) {  assert(inferredReturnTypes.size() == 1u && "mismatched number of return types");
      odsState.addTypes(inferredReturnTypes);
    } else
      ::llvm::report_fatal_error("Failed to infer result type(s).");
}
::mlir::LogicalResult SquareF64Op::verify() {
  {
    unsigned index = 0; (void)index;
    auto valueGroup0 = getODSOperands(0);

    for (auto v : valueGroup0) {
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels0(*this, v.getType(), "operand", index++)))
        return ::mlir::failure();
    }
  }
  {
    unsigned index = 0; (void)index;
    auto valueGroup0 = getODSResults(0);

    for (auto v : valueGroup0) {
      if (::mlir::failed(__mlir_ods_local_type_constraint_clink_kernels0(*this, v.getType(), "result", index++)))
        return ::mlir::failure();
    }
  }
  return ::mlir::success();
}
::mlir::LogicalResult SquareF64Op::inferReturnTypes(::mlir::MLIRContext *context, ::llvm::Optional<::mlir::Location> location, ::mlir::ValueRange operands, ::mlir::DictionaryAttr attributes, ::mlir::RegionRange regions, ::llvm::SmallVectorImpl<::mlir::Type>&inferredReturnTypes) {
  inferredReturnTypes.resize(1);
  ::mlir::Builder odsBuilder(context);
  inferredReturnTypes[0] = odsBuilder.getF64Type();
  return ::mlir::success();
}
::mlir::ParseResult SquareF64Op::parse(::mlir::OpAsmParser &parser, ::mlir::OperationState &result) {
  ::mlir::SmallVector<::mlir::OpAsmParser::OperandType, 4> allOperands;
  ::llvm::SMLoc allOperandLoc = parser.getCurrentLocation();
  if (parser.parseOperandList(allOperands))
    return ::mlir::failure();
  if (parser.parseOptionalAttrDict(result.attributes))
    return ::mlir::failure();
  ::mlir::Type odsBuildableType0 = parser.getBuilder().getF64Type();
  result.addTypes(odsBuildableType0);
  if (parser.resolveOperands(allOperands, odsBuildableType0, allOperandLoc, result.operands))
    return ::mlir::failure();
  return ::mlir::success();
}
void SquareF64Op::print(::mlir::OpAsmPrinter &_odsPrinter) {
  _odsPrinter << ' ';
  _odsPrinter << getOperation()->getOperands();
  _odsPrinter.printOptionalAttrDict((*this)->getAttrs(), /*elidedAttrs=*/{});
}
} // namespace clink
DEFINE_EXPLICIT_TYPE_ID(::clink::SquareF64Op)


#endif  // GET_OP_CLASSES
