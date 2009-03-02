/******************************************************/
/*          CSHADigest class By Adrien Pinet          */
/*       Use this class like you want, it's free !    */
/******************************************************/

#include "CSHADigest.h"

#include <fstream>
using namespace std;

/* MACROS DEFINITION */
#define SHA1CircularShift(bits,word) \
                (((word) << (bits)) | ((word) >> (32-(bits))))

/*******************************************/
/*  Constructor and Destructor declaration */
/*******************************************/

CSHADigest::CSHADigest()
{
	Start();
}

CSHADigest::~CSHADigest()
{

}


/*******************************************/
/*     Use for compute SHA from a file     */
/*******************************************/

int CSHADigest::ComputeInputFile(const char filePathName[])
{
	ifstream FileIn(filePathName, ios::in | ios::binary);
	
	if(!FileIn.good())
	return 0;

	uByte buffer[1000];

	uInt32 fileSize = GetFileSize(filePathName);
	uInt32 n = fileSize / 1000;
	uInt32 rest = fileSize % 1000;
	
	Start();

	for(uInt32 i = 0 ; i < n ; i++)
	{
		FileIn.read(reinterpret_cast<char*>(buffer), 1000);
		if(!FileIn.good() || !ComputeInputBuffer(buffer, 1000))
		return 0;
	}
	
	FileIn.read(reinterpret_cast<char*>(buffer), rest);
	if(!FileIn.good() || !ComputeInputBuffer(buffer, rest))
	return 0;

	Stop();

	return 1;
}


/*******************************************/
/*    Use for compute SHA from a buffer    */
/*******************************************/

void CSHADigest::Start()
{
	SHAContext.lengthLow = 0;
	SHAContext.lengthHigh = 0;
	SHAContext.messageBlockIndex = 0;

	SHAContext.intermediateHash[0] = 0x67452301;
	SHAContext.intermediateHash[1] = 0xEFCDAB89;
	SHAContext.intermediateHash[2] = 0x98BADCFE;
	SHAContext.intermediateHash[3] = 0x10325476;
	SHAContext.intermediateHash[4] = 0xC3D2E1F0;

	uInt32 i;

	for(i = 0 ; i < 20 ; i++)
	byteResult[i] = 0;

	for(i = 0 ; i < 41 ; i++)
	hexResult[i] = 0;
}

int CSHADigest::ComputeInputBuffer(const uByte* buffer, uInt32 bufferSize)
{
    if (!bufferSize)
    return 1;

    while(bufferSize--)
    {
		SHAContext.messageBlock[SHAContext.messageBlockIndex++] = (*buffer & 0xFF);

		SHAContext.lengthLow += 8;
		
		if(SHAContext.lengthLow == 0)
		{
			SHAContext.lengthHigh++;

			if (SHAContext.lengthHigh == 0)
			return 0; // Overflow
		}

		if(SHAContext.messageBlockIndex == 64)
		ProcessMessageBlock();
 
		buffer++;
    }

    return 1;
}


void CSHADigest::Stop()
{
    PadMessage();

	uInt32 i;

    for(i = 0 ; i < 64 ; i++)
    SHAContext.messageBlock[i] = 0;

    SHAContext.lengthLow = 0;
    SHAContext.lengthHigh = 0;

    for(i = 0 ; i < 20 ; i++)
    byteResult[i] = (uByte) (SHAContext.intermediateHash[i >> 2] >> 8 * (3 - (i & 0x03)));

    ConvertByteResultToHexResult(byteResult, hexResult);
}


/*******************************************/
/*           Return the last result        */
/*******************************************/

uByte* CSHADigest::GetByteResult()
{
	return byteResult;
}
void CSHADigest::GetByteResult(uByte* byteResult)
{
	for(uInt32 i = 0 ; i < 20 ; i++)
	byteResult[i] = this->byteResult[i];
}

char* CSHADigest::GetHexResult()
{
	return hexResult;
}

void CSHADigest::GetHexResult(char* hexResult)
{
	for(uInt32 i = 0 ; i < 20 ; i++)
	hexResult[i] = this->hexResult[i];
}


/*******************************************/
/*     Static functions (perhaps usefull)  */
/*******************************************/

uInt32 CSHADigest::GetFileSize(const char filePathName[])
{
	ifstream FileIn(filePathName, ios::in | ios::binary);
	
	if(!FileIn.good())
	return 0;

	FileIn.seekg(0, ios::end);

	return FileIn.tellg();
}

void CSHADigest::ConvertByteResultToHexResult(const uByte* byteResult, char* hexResult)
{
	hexResult[0] = 0;

	for(uInt32 i = 0 ; i < 20 ; i++)
    sprintf(hexResult, "%s%02X", hexResult, byteResult[i]);
}



/******************************************************/
/*         Internal functions for computation         */
/******************************************************/

void CSHADigest::ProcessMessageBlock()
{
	const uInt32 K[] = {0x5A827999, 0x6ED9EBA1,
						0x8F1BBCDC,	0xCA62C1D6};

	uInt32 t;
    uInt32 temp;
    uInt32 W[80];
    uInt32 A, B, C, D, E;

    for(t = 0; t < 16; t++)
    {
        W[t] =  SHAContext.messageBlock[t * 4] << 24;
        W[t] |= SHAContext.messageBlock[t * 4 + 1] << 16;
        W[t] |= SHAContext.messageBlock[t * 4 + 2] << 8;
        W[t] |= SHAContext.messageBlock[t * 4 + 3];
    }

    for(t = 16 ; t < 80 ; t++)
    W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);

    A = SHAContext.intermediateHash[0];
    B = SHAContext.intermediateHash[1];
    C = SHAContext.intermediateHash[2];
    D = SHAContext.intermediateHash[3];
    E = SHAContext.intermediateHash[4];

    for(t = 0 ; t < 20 ; t ++)
    {
        temp =  SHA1CircularShift(5,A) +
                ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);

        B = A;
        A = temp;
    }

    for(t = 20 ; t < 40 ; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 40 ; t < 60 ; t++)
    {
        temp = SHA1CircularShift(5,A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 60 ; t < 80 ; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    SHAContext.intermediateHash[0] += A;
    SHAContext.intermediateHash[1] += B;
    SHAContext.intermediateHash[2] += C;
    SHAContext.intermediateHash[3] += D;
    SHAContext.intermediateHash[4] += E;

    SHAContext.messageBlockIndex = 0;
}

void CSHADigest::PadMessage()
{
    if (SHAContext.messageBlockIndex > 55)
    {
        SHAContext.messageBlock[SHAContext.messageBlockIndex++] = 0x80;

        while(SHAContext.messageBlockIndex < 64)
        SHAContext.messageBlock[SHAContext.messageBlockIndex++] = 0;

        ProcessMessageBlock();

        while(SHAContext.messageBlockIndex < 56)
        SHAContext.messageBlock[SHAContext.messageBlockIndex++] = 0;
    }
    else
    {
        SHAContext.messageBlock[SHAContext.messageBlockIndex++] = 0x80;

        while(SHAContext.messageBlockIndex < 56)
        SHAContext.messageBlock[SHAContext.messageBlockIndex++] = 0;
    }

	SHAContext.messageBlock[56] = (uByte) (SHAContext.lengthHigh >> 24);
    SHAContext.messageBlock[57] = (uByte) (SHAContext.lengthHigh >> 16);
    SHAContext.messageBlock[58] = (uByte) (SHAContext.lengthHigh >> 8);
    SHAContext.messageBlock[59] = (uByte) (SHAContext.lengthHigh);
    SHAContext.messageBlock[60] = (uByte) (SHAContext.lengthLow >> 24);
    SHAContext.messageBlock[61] = (uByte) (SHAContext.lengthLow >> 16);
    SHAContext.messageBlock[62] = (uByte) (SHAContext.lengthLow >> 8);
    SHAContext.messageBlock[63] = (uByte) (SHAContext.lengthLow);

    ProcessMessageBlock();
}
