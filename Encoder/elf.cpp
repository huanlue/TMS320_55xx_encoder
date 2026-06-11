
#include "pch.h"
#include "elf.h"

unsigned char g_FileBuff[2048];

/**
* @brief 将2字节数的字符顺序颠倒
* @param[in] u16DataIn，需要调节字符顺序的2字节数.
* @return 颠倒顺序后的2字节数.
*/
u16 TransferOrder16(u16 u16DataIn)
{
	u16 u16Ret;
	u8* pu8TempIn;
	u8* pu8TempRet;

	pu8TempIn = (u8*)(&u16DataIn);
	pu8TempRet = (u8*)(&u16Ret);

	pu8TempRet[0] = pu8TempIn[1];
	pu8TempRet[1] = pu8TempIn[0];

	return u16Ret;
}

/**
* @brief 将4字节数的字符顺序颠倒
* @param[in] u16DataIn，需要调节字符顺序的2字节数.
* @return 颠倒顺序后的4字节数.
*/
u32 TransferOrder32(u32 u32DataIn)
{
	u32 u32Ret;
	u8* pu8TempIn;
	u8* pu8TempRet;

	pu8TempIn = (u8*)(&u32DataIn);
	pu8TempRet = (u8*)(&u32Ret);

	pu8TempRet[0] = pu8TempIn[3];
	pu8TempRet[1] = pu8TempIn[2];
	pu8TempRet[2] = pu8TempIn[1];
	pu8TempRet[3] = pu8TempIn[0];

	return u32Ret;
}
