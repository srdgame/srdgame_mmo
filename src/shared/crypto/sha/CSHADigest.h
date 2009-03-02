/******************************************************/
/*          CSHADigest class By Adrien Pinet          */
/*       Use this class like you want, it's free !    */
/******************************************************/

#ifndef __CSHADIGEST_H__
#define __CSHADIGEST_H__

typedef unsigned long uInt32;
typedef unsigned short uInt16;
typedef unsigned char uByte;

class CSHADigest
{

public:

	/*******************************************/
	/*  Constructor and Destructor definitions */
	/*******************************************/
	CSHADigest();
	virtual ~CSHADigest();


	/*******************************************/
	/*     Use for compute SHA from a file     */
	/*******************************************/
	int  ComputeInputFile(const char filePathName[]);


	/*******************************************/
	/*    Use for compute SHA from a buffer    */
	/*******************************************/
	void Start();
	int  ComputeInputBuffer(const uByte* buffer, uInt32 bufferSize);
	void Stop();


	/*******************************************/
	/*           Return the last result        */
	/*******************************************/
	//Note : Valid only after call Stop()
	uByte* GetByteResult();
	void GetByteResult(uByte* byteResult);
	
	char* GetHexResult();
	void GetHexResult(char* hexResult);


	/*******************************************/
	/*     Static functions (perhaps usefull)  */
	/*******************************************/
	static uInt32 GetFileSize(const char filePathName[]);
	static void ConvertByteResultToHexResult(const uByte* byteResult, char* hexResult);

protected:

	/******************************************************/
	/* Context struct for each call of ComputeInputBuffer */
	/******************************************************/
	struct SSHAContext
	{
	    uInt32 intermediateHash[5];
	    uInt32 lengthLow;
	    uInt32 lengthHigh;

	    uInt16 messageBlockIndex;
	    uByte messageBlock[64];
	};

	SSHAContext SHAContext;


	/******************************************************/
	/*         Internal functions for computation         */
	/******************************************************/
	void PadMessage();
	void ProcessMessageBlock();
	

	/******************************************************/
	/*   this buffer include the last SHA Digest result   */
	/******************************************************/
	uByte byteResult[20];
	char hexResult[41];
};

#endif
