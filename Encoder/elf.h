#pragma once

#include "elf_type.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern unsigned char g_FileBuff[2048];

u16 TransferOrder16(u16 u16DataIn);
u32 TransferOrder32(u32 u32DataIn);

