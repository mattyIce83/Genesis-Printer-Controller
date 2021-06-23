// CRCSupp.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ULONG Table[256];

static BOOL bCRCTableInited;

/*-----------------------------------------------------------*/
/*- Utility CRC Func                                        -*/
/*-----------------------------------------------------------*/
static ULONG Reflect(ULONG ref, char ch)
{
    ULONG value = 0;

    // Swap bit 0 for bit 7
    // bit 1 for bit 6, etc.
    for(int i = 1; i < (ch + 1); i++)
    {
        if (ref & 1)
            value |= 1 << (ch - i);
        ref >>= 1;
    }
    return value;
}

/*-----------------------------------------------------------*/
/*- Initialize CRC table                                    -*/
/*-----------------------------------------------------------*/
void InitCRC_32() {

    // This is the official polynomial used by CRC-32 
    // in PKZip, WinZip and Ethernet. 
    
	ULONG ulPolynomial = 0x04C11DB7;

    // 256 values representing ASCII character codes.
	for (int i = 0; i <= 0xFF; i++) {

		Table[i] = Reflect(i, 8) << 24;
		
		for (int j = 0; j < 8; j++)
			Table[i] = (Table[i] << 1) ^ (Table[i] & (1 << 31) ? ulPolynomial : 0);
		
		Table[i] = Reflect(Table[i], 32);
    }

	bCRCTableInited = TRUE;

}

/*-----------------------------------------------------------*/
/*- Calculate CRC                                           -*/
/*-----------------------------------------------------------*/
static void Calculate(const LPBYTE buffer, UINT size, ULONG &CRC) {
	
	// calculate the CRC
    LPBYTE pbyte = buffer;

    while (size--)
        CRC = (CRC >> 8) ^ Table[(CRC & 0xFF) ^ *pbyte++];


}

/*-----------------------------------------------------------*/
/*- Calculate CRC main function                             -*/
/*-----------------------------------------------------------*/
DWORD CalcCRC(LPVOID buffer, UINT size) { 
	
	// check the validity of the data
    if (!buffer || !size)
        return 0;

	if(!bCRCTableInited)
		InitCRC_32();

	DWORD CRC = 0xFFFFFFFF;
	Calculate ((LPBYTE)buffer, size, CRC);
        return CRC ^ 0xFFFFFFFF;



}