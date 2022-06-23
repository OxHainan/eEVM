// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "eEVM/disassembler.h"

using namespace std;

#define EVM_OPCODE(x) \
    { \
        x, \
        { \
            x, #x \
        } \
    }
#define EVM_OPCODE_IMM(x, imm) \
    { \
        x, \
        { \
            x, #x, imm \
        } \
    }

namespace eevm
{
    const std::unordered_map<uint8_t, Op> Disassembler::ops = {
      EVM_OPCODE(STOP),
      EVM_OPCODE(ADD),
      EVM_OPCODE(MUL),
      EVM_OPCODE(SUB),
      EVM_OPCODE(DIV),
      EVM_OPCODE(SDIV),
      EVM_OPCODE(MOD),
      EVM_OPCODE(SMOD),
      EVM_OPCODE(ADDMOD),
      EVM_OPCODE(MULMOD),
      EVM_OPCODE(EXP),
      EVM_OPCODE(SIGNEXTEND),
      EVM_OPCODE(LT),
      EVM_OPCODE(GT),
      EVM_OPCODE(SLT),
      EVM_OPCODE(SGT),
      EVM_OPCODE(EQ),
      EVM_OPCODE(ISZERO),
      EVM_OPCODE(AND),
      EVM_OPCODE(OR),
      EVM_OPCODE(XOR),
      EVM_OPCODE(NOT),
      EVM_OPCODE(BYTE),
      EVM_OPCODE(SHA3),
      EVM_OPCODE(ADDRESS),
      EVM_OPCODE(BALANCE),
      EVM_OPCODE(ORIGIN),
      EVM_OPCODE(CALLER),
      EVM_OPCODE(CALLVALUE),
      EVM_OPCODE(CALLDATALOAD),
      EVM_OPCODE(CALLDATASIZE),
      EVM_OPCODE(CALLDATACOPY),
      EVM_OPCODE(CODESIZE),
      EVM_OPCODE(CODECOPY),
      EVM_OPCODE(GASPRICE),
      EVM_OPCODE(EXTCODESIZE),
      EVM_OPCODE(EXTCODECOPY),
      // EVM_OPCODE(RETURNDATASIZE),
      // EVM_OPCODE(RETURNDATACOPY),
      EVM_OPCODE(BLOCKHASH),
      EVM_OPCODE(COINBASE),
      EVM_OPCODE(TIMESTAMP),
      EVM_OPCODE(NUMBER),
      EVM_OPCODE(PREVRANDAO),
      EVM_OPCODE(GASLIMIT),
      EVM_OPCODE(POP),
      EVM_OPCODE(MLOAD),
      EVM_OPCODE(MSTORE),
      EVM_OPCODE(MSTORE8),
      EVM_OPCODE(SLOAD),
      EVM_OPCODE(SSTORE),
      EVM_OPCODE(JUMP),
      EVM_OPCODE(JUMPI),
      EVM_OPCODE(PC),
      EVM_OPCODE(MSIZE),
      EVM_OPCODE(GAS),
      EVM_OPCODE(JUMPDEST),
      EVM_OPCODE_IMM(PUSH1, 1),
      EVM_OPCODE_IMM(PUSH2, 2),
      EVM_OPCODE_IMM(PUSH3, 3),
      EVM_OPCODE_IMM(PUSH4, 4),
      EVM_OPCODE_IMM(PUSH5, 5),
      EVM_OPCODE_IMM(PUSH6, 6),
      EVM_OPCODE_IMM(PUSH7, 7),
      EVM_OPCODE_IMM(PUSH8, 8),
      EVM_OPCODE_IMM(PUSH9, 9),
      EVM_OPCODE_IMM(PUSH10, 10),
      EVM_OPCODE_IMM(PUSH11, 11),
      EVM_OPCODE_IMM(PUSH12, 12),
      EVM_OPCODE_IMM(PUSH13, 13),
      EVM_OPCODE_IMM(PUSH14, 14),
      EVM_OPCODE_IMM(PUSH15, 15),
      EVM_OPCODE_IMM(PUSH16, 16),
      EVM_OPCODE_IMM(PUSH17, 17),
      EVM_OPCODE_IMM(PUSH18, 18),
      EVM_OPCODE_IMM(PUSH19, 19),
      EVM_OPCODE_IMM(PUSH20, 20),
      EVM_OPCODE_IMM(PUSH21, 21),
      EVM_OPCODE_IMM(PUSH22, 22),
      EVM_OPCODE_IMM(PUSH23, 23),
      EVM_OPCODE_IMM(PUSH24, 24),
      EVM_OPCODE_IMM(PUSH25, 25),
      EVM_OPCODE_IMM(PUSH26, 26),
      EVM_OPCODE_IMM(PUSH27, 27),
      EVM_OPCODE_IMM(PUSH28, 28),
      EVM_OPCODE_IMM(PUSH29, 29),
      EVM_OPCODE_IMM(PUSH30, 30),
      EVM_OPCODE_IMM(PUSH31, 31),
      EVM_OPCODE_IMM(PUSH32, 32),
      EVM_OPCODE(DUP1),
      EVM_OPCODE(DUP2),
      EVM_OPCODE(DUP3),
      EVM_OPCODE(DUP4),
      EVM_OPCODE(DUP5),
      EVM_OPCODE(DUP6),
      EVM_OPCODE(DUP7),
      EVM_OPCODE(DUP8),
      EVM_OPCODE(DUP9),
      EVM_OPCODE(DUP10),
      EVM_OPCODE(DUP11),
      EVM_OPCODE(DUP12),
      EVM_OPCODE(DUP13),
      EVM_OPCODE(DUP14),
      EVM_OPCODE(DUP15),
      EVM_OPCODE(DUP16),
      EVM_OPCODE(SWAP1),
      EVM_OPCODE(SWAP2),
      EVM_OPCODE(SWAP3),
      EVM_OPCODE(SWAP4),
      EVM_OPCODE(SWAP5),
      EVM_OPCODE(SWAP6),
      EVM_OPCODE(SWAP7),
      EVM_OPCODE(SWAP8),
      EVM_OPCODE(SWAP9),
      EVM_OPCODE(SWAP10),
      EVM_OPCODE(SWAP11),
      EVM_OPCODE(SWAP12),
      EVM_OPCODE(SWAP13),
      EVM_OPCODE(SWAP14),
      EVM_OPCODE(SWAP15),
      EVM_OPCODE(SWAP16),
      EVM_OPCODE(LOG0),
      EVM_OPCODE(LOG1),
      EVM_OPCODE(LOG2),
      EVM_OPCODE(LOG3),
      EVM_OPCODE(LOG4),
      EVM_OPCODE(CREATE),
      EVM_OPCODE(CALL),
      EVM_OPCODE(CALLCODE),
      EVM_OPCODE(RETURN),
      EVM_OPCODE(DELEGATECALL),
      // EVM_OPCODE(STATICCALL),
      EVM_OPCODE(REVERT),
      // EVM_OPCODE(INVALID),
      EVM_OPCODE(SELFDESTRUCT)};
} // namespace eevm
