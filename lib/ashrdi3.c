//===-- ashrdi3.c - Implement __ashrdi3 -----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements __ashrdi3 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"

// Returns: arithmetic a >> b

// Precondition:  0 <= b < bits_in_dword

di_int
__ashrdi3(di_int a, si_int b)
{
    const int bits_in_word = (int)(sizeof(si_int) * CHAR_BIT);
    dwords input;
    dwords result;
    input.all = a;
    if (b & bits_in_word)  // bits_in_word <= b < bits_in_dword
    {
        // result.high = input.high < 0 ? -1 : 0
        result.high = input.high >> (bits_in_word - 1);
        result.low = input.high >> (b - bits_in_word);
    }
    else  // 0 <= b < bits_in_word
    {
        if (b == 0)
            return a;
        result.high  = input.high >> b;
        result.low = (input.high << (bits_in_word - b)) | (input.low >> b);
    }
    return result.all;
}
