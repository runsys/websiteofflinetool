#ifndef SOFTWARELOCKERMICRO_H
#define SOFTWARELOCKERMICRO_H

#ifdef QT_VERSION
#include <QApplication>
#endif
#include <Windows.h>
#include "winioctl.h"
#include "IPTypes.h"
#include "iphlpapi.h"
#include "iostream"     
#include "winioctl.h"
#include <sstream>
#include <vector>  
#include <bitset>  
#include <array>  
#include <string>  
#include <intrin.h>  
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include <openssl/md5.h>
#include "C:\DevLibrary\UtilityFunc\autidebug.h"
#include <windows.h>
#include <winioctl.h>
#include <deque>
#include "C:\DevLibrary\UtilityFunc\LbLog.h"


#include <winsock.h>

//**********************************
//comment to disable
#define AllowRegBufferKey
#ifdef _DEBUG
#define DisableDebugQuit
#endif

//延时检测注册码时间
int delaychecktime = 3600;

//add to main() begin
/*
WSADATA WSAData;
WSAStartup(MAKEWORD(2, 2), &WSAData);//加载winsock dll的相应版本
*/


static char *_softcode =   "ccbel`edouzcqxyqccbel`edouzcqxyqlebb.ccfjfjfjf";
static char *_wangwangid = "ccbjhxeled`ocqxyccbjhxeled`ocqxy\x0zaaflebb.ccfjfj";
static char *pubkey = "-----BEGIN PUBLIC KEY-----\nMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCoTypqxmoQSN4m7zfkoR93MFgY\nBQyxE8CakwT/Hdc/Pzc15cIkUsjzzh6A3Id0PbcjqMFTjzMOw+BVCjRgRNDg3R8O\nkydLnMThTTY8Rw/J0+/8Wc8GrUYW+em9Zsxu2aDBhfG4nNsLIIQdciaZC7MXBWqU\nc2wh2kk4LZkIjd/jYwIDAQAB\n-----END PUBLIC KEY-----\n";
#ifdef _DEBUG
static char *prikey = "-----BEGIN RSA PRIVATE KEY-----\nMIICWwIBAAKBgQCoTypqxmoQSN4m7zfkoR93MFgYBQyxE8CakwT/Hdc/Pzc15cIk\nUsjzzh6A3Id0PbcjqMFTjzMOw+BVCjRgRNDg3R8OkydLnMThTTY8Rw/J0+/8Wc8G\nrUYW+em9Zsxu2aDBhfG4nNsLIIQdciaZC7MXBWqUc2wh2kk4LZkIjd/jYwIDAQAB\nAoGAbxCmrgU20n4zUsADQzTr3awiRd93K5ccPySLZIpdkxf1J/Tw8yVKBFDdIi3k\nmNls8/QVdz2E9NZfP97XBzwCFX76BkSmX45JbJU7QBgLpn1ysIQlLD6gJ+BktKJs\nyuNVryZgnmBonPpCsfOMjXj7+2snEr9t2V+nCuLhsPOkXdkCQQDSdjfStA+KubL/\n3DOAwmdKSwjSOtBeD4P/HdGa9UbMHnARDnk+T68+2Uxj75ywhGu0/GMqLLE5MSwz\nwK3+V23NAkEAzLoOpT2Jq8jgonaKZFAInr8J5ntjsgBIl5gpMsQU/0f5DfwWT8T3\nDj+zyD/Y3Bmff4LYoIPn6OynXOeAsSjl7wJAJWcLDrAfXvkXna8FnClwb9yb6bi4\nFS9OtwUWniJe/aGfKvjWhUNmEpZNpCkOQLSX66c6/GDK8i4R+/SPN3dUNQJASk6B\nwFIMpRA9EFqsMmqmJlA6nLBOaC4YKwWsdmObJ2kNKZlE0+EutrpCW71ozcJzFMn+\nLIyT4QP8XwkbubGZewJAYK98b1Z+Ymg4akOkbDDCl4FU7qcgPGT7ilMpXeiVqd1R\n1lSJwhgZs7jzShURIYaUdQxOnSfajtUo11oX+JnS1Q==\n-----END RSA PRIVATE KEY-----\n";
#endif

using namespace std;

static int g_val;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")

#ifdef _DLL
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win32-md\\lib\\release\\libeay32.lib")
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win32-md\\lib\\release\\ssleay32.lib")
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win64-md\\lib\\release\\libeay32.lib")
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win64-md\\lib\\release\\ssleay32.lib")
#else
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win32-mt\\lib\\release\\libeay32.lib")
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win32-mt\\lib\\release\\ssleay32.lib")
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win64-mt\\lib\\release\\libeay32.lib")
#pragma comment(lib,"C:\\DevLibrary\\openssl-1.0.1k\\vcall-win64-mt\\lib\\release\\ssleay32.lib")
#endif

//外部程序注册码控
int com_3833 = 9232388123;
std::string kname;

//rsa function
#define OPENSSLKEY "test.key"
#define PUBLICKEY "test_pub.key" 9 #define BUFFSIZE 1024
char* rsa_encrypt(char *str, char *path_key);//加密
char* rsa_decrypt(char *str, char *path_key);//解密
//int main(void) {
//	char *source = "i like dancing !";
//	char *ptr_en, *ptr_de;
//	printf("source is    :%s\n", source);
//	ptr_en = rsa_encrypt(source, PUBLICKEY);
//	printf("after encrypt:%s\n", ptr_en);
//	ptr_de = rsa_decrypt(ptr_en, OPENSSLKEY); 19    printf("after decrypt:%s\n", ptr_de);
//	if (ptr_en != NULL) {
//		free(ptr_en);
//	}
//	if (ptr_de != NULL) {
//		free(ptr_de);
//	}
//	return 0;
//}
char *rsa_pubkey_encrypt(char *str, char *pubkeybuf,int *encodelen)
{
	char *p_en;
	RSA *p_rsa;
	int flen, rsa_len;
	BIO *bio = NULL;
	*encodelen = 0;
	if ((bio = BIO_new_mem_buf(pubkeybuf, -1)) == NULL)       //从字符串读取RSA公钥
	{
		return NULL;
	}
	if ((p_rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL)) == NULL) {
		ERR_print_errors_fp(stdout);
		return NULL;
	}
	flen = strlen(str);
	rsa_len = RSA_size(p_rsa);
	p_en = (char *)malloc(rsa_len + 1);
	memset(p_en, 0, rsa_len + 1);
	if (RSA_public_encrypt(rsa_len - RSA_PKCS1_PADDING_SIZE, (unsigned char *)str, (unsigned char*)p_en, p_rsa, RSA_PKCS1_PADDING)<0)
	{
		if (p_en)free(p_en);
		return NULL;
	}
	RSA_free(p_rsa);
	BIO_free_all(bio);
	*encodelen = rsa_len;
	return p_en;
}
char *rsa_pubkey_decrypt(char *str, char *pubkeybuf, int *encodelen)
{
	char *p_de;
	RSA *p_rsa;
	int rsa_len;
	BIO *bio = NULL;
	*encodelen = 0;
	if ((bio = BIO_new_mem_buf(pubkeybuf, -1)) == NULL)       //从字符串读取RSA公钥
	{
		return NULL;
	}
	if ((p_rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL)) == NULL)
	{
		ERR_print_errors_fp(stdout);
		return NULL;
	}
	rsa_len = RSA_size(p_rsa);
	p_de = (char *)malloc(rsa_len + 1);
	memset(p_de, 0, rsa_len + 1);
	if (RSA_public_decrypt(rsa_len, (unsigned char *)str, (unsigned char*)p_de, p_rsa, RSA_PKCS1_PADDING)<0)
	{
		return NULL;
	}
	RSA_free(p_rsa);
	BIO_free_all(bio);
	*encodelen = rsa_len;
	return p_de;
}


char *rsa_prikey_encrypt(char *str, char *prikeybuf, int *encodelen)
{
	char *p_en;
	RSA *p_rsa;
	int flen, rsa_len;
	BIO *bio = NULL;
	*encodelen = 0;
	if ((bio = BIO_new_mem_buf(prikeybuf, -1)) == NULL)       //从字符串读取RSA公钥
	{
		return NULL;
	}
	if ((p_rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL)) == NULL) {
		ERR_print_errors_fp(stdout);
		return NULL;
	}
	flen = strlen(str);
	rsa_len = RSA_size(p_rsa);
	p_en = (char *)malloc(rsa_len + 1);
	memset(p_en, 0, rsa_len + 1);
	if (RSA_private_encrypt(rsa_len, (unsigned char *)str, (unsigned char*)p_en, p_rsa, RSA_NO_PADDING)<0)
	{
		if (p_en)free(p_en);
		return NULL;
	}
	RSA_free(p_rsa);
	BIO_free_all(bio);
	*encodelen = rsa_len;
	return p_en;
}
char *rsa_prikey_decrypt(char *str, char *prikeybuf, int *encodelen)
{
	char *p_de;
	RSA *p_rsa;
	int rsa_len;
	BIO *bio = NULL;
	*encodelen = 0;
	if ((bio = BIO_new_mem_buf(prikeybuf, -1)) == NULL)       //从字符串读取RSA公钥
	{
		return NULL;
	}
	if ((p_rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL)) == NULL)
	{
		ERR_print_errors_fp(stdout);
		return NULL;
	}
	rsa_len = RSA_size(p_rsa);
	p_de = (char *)malloc(rsa_len + 1);
	memset(p_de, 0, rsa_len + 1);
	if (RSA_private_decrypt(rsa_len, (unsigned char *)str, (unsigned char*)p_de, p_rsa, RSA_NO_PADDING)<0)
	{
		return NULL;
	}
	RSA_free(p_rsa);
	BIO_free_all(bio);
	*encodelen = rsa_len;
	return p_de;
}



//IsWow64
BOOL _skke23_64()
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{

		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	return bIsWow64;
}


//get unique machine code string
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.      
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.      
#define  IOCTL_GET_DRIVE_INFO   0x0007c088
#define  IOCTL_GET_VERSION          0x00074080
#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ( ( FILE_DEVICE_SCSI << 16 ) + 0x0501 )
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition
#define  IDENTIFY_BUFFER_SIZE  512
#define  SENDIDLENGTH  ( sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE )
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088
typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;


typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;



typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;


//结构定义 
typedef struct _UNICODE_STRING
{
	USHORT  Length;//长度 
	USHORT  MaximumLength;//最大长度 
	PWSTR  Buffer;//缓存指针 
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;//长度 18h 
	HANDLE RootDirectory;//  00000000 
	PUNICODE_STRING ObjectName;//指向对象名的指针 
	ULONG Attributes;//对象属性00000040h 
	PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR，0 
	PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE，0 
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

//函数指针变量类型
typedef DWORD(__stdcall *ZWOS)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);
typedef DWORD(__stdcall *ZWMV)(HANDLE, HANDLE, PVOID, ULONG, ULONG, PLARGE_INTEGER, PSIZE_T, DWORD, ULONG, ULONG);
typedef DWORD(__stdcall *ZWUMV)(HANDLE, PVOID);



static UINT FindAwardBios(BYTE** ppBiosAddr)
{
	BYTE* pBiosAddr = *ppBiosAddr + 0xEC71;

	BYTE szBiosData[128];
	CopyMemory(szBiosData, pBiosAddr, 127);
	szBiosData[127] = 0;

	int iLen = lstrlenA((char*)szBiosData);
	if (iLen > 0 && iLen < 128)
	{
		//AWard:         07/08/2002-i845G-ITE8712-JF69VD0CC-00 
		//Phoenix-Award: 03/12/2002-sis645-p4s333
		if (szBiosData[2] == '/' && szBiosData[5] == '/')
		{
			BYTE* p = szBiosData;
			while (*p)
			{
				if (*p < ' ' || *p >= 127)
				{
					break;
				}
				++p;
			}
			if (*p == 0)
			{
				*ppBiosAddr = pBiosAddr;
				return (UINT)iLen;
			}
		}
	}
	return 0;
}

DWORD WINAPI _kkdkks_3428438(void *param)
{

	//花指令
	{__asm
	{
		jz  GG6
			jnz GG6
			_emit 0xe8
		GG6:
	}}//花指令end



	//花指令
		{__asm
		{
			jz  GG7
				jnz GG7
				_emit 0xe8
			GG7:
		}}//花指令end



		//花指令
		{__asm
		{
			jz  GG8
				jnz GG8
				_emit 0xe8
			GG8:
		}}//花指令end



		//花指令
		{__asm
		{
			jz  GG9
				jnz GG9
				_emit 0xe8
			GG9:
		}}//花指令end


	return 0;
}


DWORD WINAPI _kllsioir_9942(void *param)
{
	if ((bool)param == true)
	{
		Sleep(192938383);
	}
	return 0;
}


static UINT FindAmiBios(BYTE** ppBiosAddr)
{
	BYTE* pBiosAddr = *ppBiosAddr + 0xF478;

	BYTE szBiosData[128];
	CopyMemory(szBiosData, pBiosAddr, 127);
	szBiosData[127] = 0;

	int iLen = lstrlenA((char*)szBiosData);
	if (iLen > 0 && iLen < 128)
	{
		// Example: "AMI: 51-2300-000000-00101111-030199-"
		if (szBiosData[2] == '-' && szBiosData[7] == '-')
		{
			BYTE* p = szBiosData;
			while (*p)
			{
				if (*p < ' ' || *p >= 127)
				{
					break;
				}
				++p;
			}
			if (*p == 0)
			{
				*ppBiosAddr = pBiosAddr;
				return (UINT)iLen;
			}
		}
	}
	return 0;
}

static UINT FindPhoenixBios(BYTE** ppBiosAddr)
{
	UINT uOffset[3] = { 0x6577, 0x7196, 0x7550 };
	for (UINT i = 0; i < 3; ++i)
	{
		BYTE* pBiosAddr = *ppBiosAddr + uOffset[i];

		BYTE szBiosData[128];
		CopyMemory(szBiosData, pBiosAddr, 127);
		szBiosData[127] = 0;

		int iLen = lstrlenA((char*)szBiosData);
		if (iLen > 0 && iLen < 128)
		{
			// Example: Phoenix "NITELT0.86B.0044.P11.9910111055"
			if (szBiosData[7] == '.' && szBiosData[11] == '.')
			{
				BYTE* p = szBiosData;
				while (*p)
				{
					if (*p < ' ' || *p >= 127)
					{
						break;
					}
					++p;
				}
				if (*p == 0)
				{
					*ppBiosAddr = pBiosAddr;
					return (UINT)iLen;
				}
			}
		}
	}
	return 0;
}

static bool GetDiskSerialNumber(int phy_driver_num, std::string *out_serialnum, std::string *out_module_name = nullptr)
{
	bool b_got_serialnum = false;
	HMODULE DllInst = LoadLibrary(L"GetDiskSerial.dll");
	if (DllInst != nullptr)
	{
		char* (__stdcall *GetSerialNumber)(unsigned char);
		char* (__stdcall *GetModelNumber)(unsigned char);

		GetSerialNumber = (char* (__stdcall*)(unsigned char))GetProcAddress(DllInst, "GetSerialNumber");
		GetModelNumber = (char* (__stdcall*)(unsigned char))GetProcAddress(DllInst, "GetModelNumber");

		const char *c_serial = nullptr, *c_modulename = nullptr;
		c_serial = GetSerialNumber(phy_driver_num);
		if (c_serial != nullptr && out_serialnum != nullptr)
		{
			*out_serialnum = c_serial;
			b_got_serialnum = true;
		}
		c_modulename = GetModelNumber(phy_driver_num);
		if (c_modulename != nullptr && out_module_name != nullptr)
		{
			*out_module_name = c_modulename;
		}
	}
	if (DllInst != nullptr)
	{
		FreeLibrary(DllInst);
	}
	return b_got_serialnum;
}







//
// Model Number: 40 ASCII Chars
// SerialNumber: 20 ASCII Chars
//
void ToLittleEndian(PUSHORT pWords, int nFirstIndex, int nLastIndex, LPTSTR pBuf);
BOOL GetPhyDriveSerial(LPTSTR pModelNo, LPTSTR pSerialNo)
{
	//-1是因为 SENDCMDOUTPARAMS 的结尾是 BYTE bBuffer[1];
	BYTE IdentifyResult[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
	DWORD dwBytesReturned;
	GETVERSIONINPARAMS get_version;
	SENDCMDINPARAMS send_cmd = { 0 };

	HANDLE hFile = CreateFile(L"\\\\.\\PHYSICALDRIVE0", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	//get version
	DeviceIoControl(hFile, SMART_GET_VERSION, NULL, 0,
		&get_version, sizeof(get_version), &dwBytesReturned, NULL);

	//identify device
	send_cmd.irDriveRegs.bCommandReg = (get_version.bIDEDeviceMap & 0x10) ? ATAPI_ID_CMD : ID_CMD;
	DeviceIoControl(hFile, SMART_RCV_DRIVE_DATA, &send_cmd, sizeof(SENDCMDINPARAMS) - 1,
		IdentifyResult, sizeof(IdentifyResult), &dwBytesReturned, NULL);
	CloseHandle(hFile);

	//adjust the byte order
	PUSHORT pWords = (USHORT*)(((SENDCMDOUTPARAMS*)IdentifyResult)->bBuffer);
	ToLittleEndian(pWords, 27, 46, pModelNo);
	ToLittleEndian(pWords, 10, 19, pSerialNo);
	return TRUE;
}

//把WORD数组调整字节序为little-endian，并滤除字符串结尾的空格。
void ToLittleEndian(PUSHORT pWords, int nFirstIndex, int nLastIndex, LPTSTR pBuf)
{
	int index;
	LPTSTR pDest = pBuf;
	for (index = nFirstIndex; index <= nLastIndex; ++index)
	{
		pDest[0] = pWords[index] >> 8;
		pDest[1] = pWords[index] & 0xFF;
		pDest += 2;
	}
	*pDest = 0;

	//trim space at the endof string; 0x20: _T(' ')
	--pDest;
	while (*pDest == 0x20)
	{
		*pDest = 0;
		--pDest;
	}
}

//滤除字符串起始位置的空格
void TrimStart(LPTSTR pBuf)
{
	if (*pBuf != 0x20)
		return;

	LPTSTR pDest = pBuf;
	LPTSTR pSrc = pBuf + 1;
	while (*pSrc == 0x20)
		++pSrc;

	while (*pSrc)
	{
		*pDest = *pSrc;
		++pDest;
		++pSrc;
	}
	*pDest = 0;
}
//test main
/*
int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szModelNo[48], szSerialNo[24];
	if (GetPhyDriveSerial(szModelNo, szSerialNo))
	{
		_tprintf(_T(" : 0 1 2\n"));
		_tprintf(_T(" : 012345678901234567890123456789\n"));
		_tprintf(_T("Model No: %s\n"), szModelNo);
		_tprintf(_T("Serial No: %s\n"), szSerialNo);
		TrimStart(szSerialNo);
		_tprintf(_T("Serial No: %s\n"), szSerialNo);
	}
	else
	{
		_tprintf(_T("Failed.\n"));
	}
	getchar();
	return 0;
}
*/




std::string _klgdfk_stringTrim(const std::string &str2)
{
	std::string str = str2;
	const char *spacestr = " \r\n\t";
	while (1)
	{
		if (str.empty() || strchr(spacestr, str[0]) == NULL)
		{
			break;
		}
		else {
			str = str.substr(1);
			if (str.empty())break;
		}
	}

	while (1)
	{
		if (str.empty() || strchr(spacestr, str[str.size() - 1]) == NULL)
		{
			break;
		}
		else {
			str = str.substr(0, str.size() - 1);
			if (str.empty())break;
		}
	}
	return str;
}

DWORD WINAPI _kkdf_59922(void *param)
{


#ifdef QT_VERSION
	//LBLOGD(WARNING << "thrad 16");
	qApp->quit();
#endif
	//花指令
	{__asm
	{
		jz  GG31
			jnz GG31
			_emit 0xe8
		GG31:
	}}//花指令end


	//LBLOGD(WARNING << "thrad 17");

	DWORD curpocid = GetCurrentProcessId();
	HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
	TerminateProcess(ProcH, 0);
	CloseHandle(ProcH);


	//LBLOGD(WARNING << "thrad 18");

	//花指令
	{__asm
	{
		jz  GG30
			jnz GG30
			_emit 0xe8
		GG30:
	}}//花指令end

	exit(0);
	return 0;
}


DWORD WINAPI _fdkfdsk_83281(void *param)
{
	return 0;
}


DWORD WINAPI _hasds_58222(void *param)
{


	//LBLOGD(WARNING << "thrad 19");
#ifndef DisableDebugQuit
	if (PebIsDebuggedApproach())
	{

		//LBLOGD(WARNING << "thrad 20");
		//花指令
		{__asm
		{
			jz  GG12
				jnz GG12
				_emit 0xe8
			GG12:
		}
		}//花指令end

#ifdef QT_VERSION
		//LBLOGD(WARNING << "thrad 21");
		qApp->quit();
#endif
		//花指令
		{__asm
		{
			jz  GG11
				jnz GG11
				_emit 0xe8
			GG11:
		}}//花指令end


		//LBLOGD(WARNING << "thrad 22");
		DWORD curpocid = GetCurrentProcessId();
		HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
		TerminateProcess(ProcH, 0);
		CloseHandle(ProcH);

		//花指令
		{__asm
		{
			jz  GG10
				jnz GG10
				_emit 0xe8
			GG10:
		}}//花指令end


		//LBLOGD(WARNING << "thrad 23");
		exit(0);

		//LBLOGD(WARNING << "thrad 24");
	}

	//花指令
	//花指令
		{__asm
		{
			jz  GG5
				jnz GG5
				_emit 0xe8
			GG5:
		}}//花指令end

		if (PebNtGlobalFlagsApproach())
		{

			//LBLOGD(WARNING << "thrad 25");
			//花指令
			{__asm
			{
				jz  GG22
					jnz GG22
					_emit 0xe8
				GG22:
			}
			}//花指令end

#ifdef QT_VERSION
			//LBLOGD(WARNING << "thrad 26");
			qApp->quit();
#endif
			//花指令
			{__asm
			{
				jz  GG21
					jnz GG21
					_emit 0xe8
				GG21:
			}}//花指令end


			//LBLOGD(WARNING << "thrad 27");
			DWORD curpocid = GetCurrentProcessId();
			HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
			TerminateProcess(ProcH, 0);
			CloseHandle(ProcH);

			//花指令
			{__asm
			{
				jz  GG20
					jnz GG20
					_emit 0xe8
				GG20:
			}}//花指令end


			//LBLOGD(WARNING << "thrad 28");
			exit(0);
		}

#endif
		return 0;
}


static std::string get_hardware_idstr()
{

	std::string szSystemInfo; // 在程序执行完毕后，此处存储取得的系统特征码
	// 网卡 MAC 地址，注意: MAC 地址是可以在注册表中修改的
	std::string onlyonemac;
	{
		UINT uErrorCode = 0;
		IP_ADAPTER_INFO iai;
		ULONG uSize = 0;
		DWORD dwResult = GetAdaptersInfo(&iai, &uSize);
		if (dwResult == ERROR_BUFFER_OVERFLOW)
		{
			IP_ADAPTER_INFO* piai = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, uSize);
			if (piai != NULL)
			{
				dwResult = GetAdaptersInfo(piai, &uSize);
				if (ERROR_SUCCESS == dwResult)
				{
					IP_ADAPTER_INFO* piai2 = piai;
					while (piai2 != NULL)
					{
						//if (strstr(piai2->Description, "real") != NULL || strstr(piai2->Description, "intel") != NULL || strstr(piai2->Description, "网络连接") != NULL)
						if (strstr(piai2->Description, "虚拟") == NULL && strstr(piai2->Description, "蓝牙") == NULL && strstr(piai2->Description, "tooth") == NULL && strstr(piai2->Description, "VM") == NULL && strstr(piai2->Description, "TAP") == NULL && strstr(piai2->Description, "Vir") == NULL && strstr(piai2->Description, "vir") == NULL)
						{
							for (int mi = 0; mi < piai2->AddressLength; mi++)
							{
								char mp[8] = { 0 };
								sprintf(mp, "%x", (unsigned int)(unsigned char)piai2->Address[mi]);
								szSystemInfo += mp;
							}
							szSystemInfo += ",";
							break;
						}
						else {
							for (int mi = 0; mi < piai2->AddressLength; mi++)
							{
								char mp[8] = { 0 };
								sprintf(mp, "%x", (unsigned int)(unsigned char)piai2->Address[mi]);
								onlyonemac += mp;
							}
						}
						piai2 = piai2->Next;
					}
				}
				else
				{
					uErrorCode = 0xF0000000U + dwResult;
				}
				//VERIFY(HeapFree(GetProcessHeap(), 0, piai));
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			uErrorCode = 0xE0000000U + dwResult;
		}
		if (uErrorCode != 0U)
		{
			return FALSE;
		}
	}
	if (szSystemInfo.size() && szSystemInfo[szSystemInfo.size() - 1] == ',')szSystemInfo = szSystemInfo.substr(0, szSystemInfo.size() - 1);
	if(onlyonemac.size()==0)szSystemInfo= onlyonemac;

	szSystemInfo += "`";

	//系统盘序列号
	TCHAR szModelNo[48] = { 0 }, szSerialNo[24] = { 0 };
	GetPhyDriveSerial(szModelNo, szSerialNo);
	char szSerialNoC[24] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, szSerialNo, wcslen(szSerialNo), szSerialNoC, 24, 0, 0);

	szSystemInfo = szSystemInfo + _klgdfk_stringTrim(szSerialNoC) + "`";

	// CPU ID
	{
		BOOL bException = FALSE;
		BYTE szCpu[16] = { 0 };
		UINT uCpuID = 0U;

		//__try
		{
			_asm
			{
				mov eax, 0
				cpuid
				mov dword ptr szCpu[0], ebx
				mov dword ptr szCpu[4], edx
				mov dword ptr szCpu[8], ecx
				mov eax, 1
				cpuid
				mov uCpuID, edx
			}
		}
		//__except (EXCEPTION_EXECUTE_HANDLER)
		//{
		//	bException = TRUE;
		//}

		if (uCpuID!=0)
		{
			char cpuidstr[32];
			sprintf(cpuidstr, "%u", uCpuID);
			OutputDebugStringA(cpuidstr);
			szSystemInfo += cpuidstr;
		}
	}
	szSystemInfo += "`";

	//// BIOS 编号，支持 AMI, AWARD, PHOENIX
	//{
	//	SIZE_T ssize;

	//	LARGE_INTEGER so;
	//	so.LowPart = 0x000f0000;
	//	so.HighPart = 0x00000000;
	//	ssize = 0xffff;
	//	wchar_t strPH[30] = L"//device//physicalmemory";

	//	DWORD ba = 0;

	//	UNICODE_STRING struniph;
	//	struniph.Buffer = strPH;
	//	struniph.Length = 0x2c;
	//	struniph.MaximumLength = 0x2e;

	//	OBJECT_ATTRIBUTES obj_ar;
	//	obj_ar.Attributes = 64;
	//	obj_ar.Length = 24;
	//	obj_ar.ObjectName = &struniph;
	//	obj_ar.RootDirectory = 0;
	//	obj_ar.SecurityDescriptor = 0;
	//	obj_ar.SecurityQualityOfService = 0;

	//	HMODULE hinstLib = LoadLibraryA("ntdll.dll");
	//	ZWOS ZWopenS = (ZWOS)GetProcAddress(hinstLib, "ZwOpenSection");
	//	ZWMV ZWmapV = (ZWMV)GetProcAddress(hinstLib, "ZwMapViewOfSection");
	//	ZWUMV ZWunmapV = (ZWUMV)GetProcAddress(hinstLib, "ZwUnmapViewOfSection");

	//	//调用函数，对物理内存进行映射 
	//	HANDLE hSection;
	//	if (0 == ZWopenS(&hSection, 4, &obj_ar) &&
	//		0 == ZWmapV(
	//		(HANDLE)hSection,   //打开Section时得到的句柄 
	//			(HANDLE)0xFFFFFFFF, //将要映射进程的句柄， 
	//			&ba,                  //映射的基址 
	//			0,
	//			0xFFFF,               //分配的大小 
	//			&so,                  //物理内存的地址 
	//			&ssize,               //指向读取内存块大小的指针 
	//			1,                    //子进程的可继承性设定 
	//			0,                    //分配类型 
	//			2                     //保护类型 
	//		))
	//		//执行后会在当前进程的空间开辟一段64k的空间，并把f000:0000到f000:ffff处的内容映射到这里 
	//		//映射的基址由ba返回,如果映射不再有用,应该用ZwUnmapViewOfSection断开映射 
	//	{
	//		BYTE* pBiosSerial = (BYTE*)ba;
	//		UINT uBiosSerialLen = FindAwardBios(&pBiosSerial);
	//		if (uBiosSerialLen == 0U)
	//		{
	//			uBiosSerialLen = FindAmiBios(&pBiosSerial);
	//			if (uBiosSerialLen == 0U)
	//			{
	//				uBiosSerialLen = FindPhoenixBios(&pBiosSerial);
	//			}
	//		}
	//		if (uBiosSerialLen != 0U)
	//		{
	//			szSystemInfo+=std::string((char*)pBiosSerial, uBiosSerialLen);
	//		}
	//		ZWunmapV((HANDLE)0xFFFFFFFF, (void*)ba);
	//	}
	//}
	//// 完毕， 系统特征码已取得。
	//szSystemInfo += "|";


	return szSystemInfo;
}




static std::string _ksdkf_2343()
{
	// CPU ID
	{
		BOOL bException = FALSE;
		BYTE szCpu[16] = { 0 };
		UINT uCpuID = 0U;

		//__try
		{
			_asm
			{
				mov eax, 0
					cpuid
					mov dword ptr szCpu[0], ebx
					mov dword ptr szCpu[4], edx
					mov dword ptr szCpu[8], ecx
					mov eax, 1
					cpuid
					mov uCpuID, edx
			}
		}
		//__except (EXCEPTION_EXECUTE_HANDLER)
		//{
		//	bException = TRUE;
		//}

		if (uCpuID != 0)
		{
			char cpuidstr[32] = { 0 };
			sprintf(cpuidstr, "%u", uCpuID);
			return cpuidstr;
		}
	}
	return "";
}


//get domain ip
static std::string getdomainip(std::string domain)
{
	int WSA_return = 0;
	WSADATA WSAData;

	//WSA_return = WSAStartup(0x0101, &WSAData);

	/* 结构指针 */
	char ch;
	HOSTENT *host_entry;

	char host_name[256];
	strcpy(host_name, domain.c_str());

	if (WSA_return == 0)
	{
		/* 即要解析的域名或主机名 */

		host_entry = gethostbyname(host_name);
		if (host_entry != 0)
		{
			string str1, str2, str3, str4, str5;

			int i1 = host_entry->h_addr_list[0][0] & 0x00ff,
				i2 = host_entry->h_addr_list[0][1] & 0x00ff,
				i3 = host_entry->h_addr_list[0][2] & 0x00ff,
				i4 = host_entry->h_addr_list[0][3] & 0x00ff;


			std::stringstream strconvert1, strconvert2, strconvert3, strconvert4;
			strconvert1 << i1;//把int型转换成string类型的数据
			strconvert1 >> str1;
			strconvert2 << i2;
			strconvert2 >> str2;
			strconvert3 << i3;
			strconvert3 >> str3;
			strconvert4 << i4;
			strconvert4 >> str4;
			str5 = str1 + '.' + str2 + '.' + str3 + '.' + str4;
			return str5;
		}
	}

	//WSACleanup();

	return "";
}

//获得exe 版本,公司等信息
//获取 文件版本号的  
//    pName:_T("CompanyName"),_T("FileDescription"),_T("FileVersion"),  
//    _T("InternalName"),_T("LegalCopyright"),_T("OriginalFilename"),  
//    _T("ProductName"),_T("ProductVersion"),  

BOOL GetExeFileDetailInfo(CHAR * pFileName, char * pRecvStr, char * pName /* = NULL */)
{
	DWORD    dwDummyHandle; // will always be set to zero  
	DWORD    dwLen;
	BYTE    * pVersionInfo;
	BOOL    bRetVal;

	VS_FIXEDFILEINFO    FileVersion;
	*pRecvStr = 0;

	struct TRANSLATION {
		WORD langID;            // language ID  
		WORD charset;            // character set (code page)  
	} Translation;

	HMODULE        hVerDll;
	hVerDll = LoadLibrary(L"VERSION.dll");
	if (hVerDll == NULL)
		return FALSE;

	typedef DWORD(WINAPI *Fun_GetFileVersionInfoSizeA)(CHAR *, DWORD *);
	typedef BOOL(WINAPI *Fun_GetFileVersionInfoA)(CHAR *, DWORD, DWORD, LPVOID);
	typedef BOOL(WINAPI *Fun_VerQueryValueA)(LPVOID, CHAR *, LPVOID, PUINT);


	Fun_GetFileVersionInfoSizeA        pGetFileVersionInfoSizeA;
	Fun_GetFileVersionInfoA            pGetFileVersionInfoA;
	Fun_VerQueryValueA                pVerQueryValueA;

	pGetFileVersionInfoSizeA = (Fun_GetFileVersionInfoSizeA)::GetProcAddress(hVerDll, "GetFileVersionInfoSizeA");
	pGetFileVersionInfoA = (Fun_GetFileVersionInfoA)::GetProcAddress(hVerDll, "GetFileVersionInfoA");
	pVerQueryValueA = (Fun_VerQueryValueA)::GetProcAddress(hVerDll, "VerQueryValueA");

	Translation.langID = 0x0409;    //  
	Translation.charset = 1252;        // default = ANSI code page  

	dwLen = pGetFileVersionInfoSizeA(pFileName, &dwDummyHandle);
	if (dwLen == 0)
		return FALSE;

	pVersionInfo = new BYTE[dwLen]; // allocate version info  
	bRetVal = pGetFileVersionInfoA(pFileName, 0, dwLen, pVersionInfo);
	if (bRetVal == FALSE)
	{
		delete[] pVersionInfo;
		return FALSE;
	}

	VOID    * pVI;
	UINT    uLen;

	bRetVal = pVerQueryValueA(pVersionInfo, "\\", &pVI, &uLen);
	if (bRetVal == FALSE)
	{
		delete[] pVersionInfo;
		return FALSE;
	}

	memcpy(&FileVersion, pVI, sizeof(VS_FIXEDFILEINFO));

	bRetVal = pVerQueryValueA(pVersionInfo, "\\VarFileInfo\\Translation",
		&pVI, &uLen);
	if (bRetVal && uLen >= 4)
	{
		memcpy(&Translation, pVI, sizeof(TRANSLATION));
	}

	//  BREAKIF(FileVersion.dwSignature != VS_FFI_SIGNATURE);  
	if (FileVersion.dwSignature != VS_FFI_SIGNATURE)
	{
		return FALSE;
	}

	CHAR        szQuery[1024];
	char        * pVal;
	UINT        iLenVal;

	if (pName == NULL)
	{
		sprintf(szQuery, "%d.%d.%d.%d",
			HIWORD(FileVersion.dwFileVersionMS), LOWORD(FileVersion.dwFileVersionMS),
			HIWORD(FileVersion.dwFileVersionLS), LOWORD(FileVersion.dwFileVersionLS));
		strcpy(pRecvStr,szQuery);
	}
	else
	{
		char* bStrNtyLine1 = pName;
		sprintf(szQuery, "\\StringFileInfo\\%04X%04X\\%s", Translation.langID, Translation.charset, (CHAR *)bStrNtyLine1);

		bRetVal = pVerQueryValueA(pVersionInfo, szQuery, (LPVOID*)&pVal, &iLenVal);
		if (bRetVal)
			strcpy(pRecvStr, pVal);
		else
			*pRecvStr = 0;
	}

	delete[] pVersionInfo;
	return TRUE;
}



//send data to seerver

std::string _fkkfdf_k30(std::string ip, std::string data, bool bindebug)
{
	WSADATA wsaData;//
	SOCKET sSocket, cSocket;	//=	INVALID_SOCKET; socket对象
	SOCKADDR_IN sockAddrIn;// 服务器地址

	//WSAStartup(MAKEWORD(2, 2), &wsaData);//加载winsock dll的相应版本

	sockAddrIn.sin_family = AF_INET;//协议
	sockAddrIn.sin_port = htons(80);//端口
	sockAddrIn.sin_addr.s_addr = inet_addr(ip.c_str());//IP//INADDR_ANY代表本机IP

	cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connect(cSocket, (SOCKADDR *)&sockAddrIn, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		return "";
	}

	char sendBuff[1024] = { 0 };//发送数据
	char rcvBuff[10240] = { 0 };//接受数据
	memset(rcvBuff, 0, 10240);
	char *pRcv = rcvBuff;//指针
	int num = 0;
	//构造http请求头
	std::string geturl = "GET /r.php?mc=" + data + "&querytry=1 HTTP/1.1\r\nHost: www.lebb.cc\r\n\r\n";
	strcpy(sendBuff, geturl.c_str());//经测试r没有也可以 按照标准还是加上吧

										//发送
	send(cSocket, (char *)sendBuff, sizeof(sendBuff), 0);

	//接收http响应
	while (1)
	{
		num = recv(cSocket, pRcv, 10240-(pRcv- rcvBuff), 0);
		pRcv += num;

		if (num == 0 || num == -1 || strstr(rcvBuff, "lebbkey")!=0 && strrchr(rcvBuff,'\n')>strstr(rcvBuff,"lebbkey"))
		{
			break;
		}
		//break;
	}
	closesocket(cSocket);
	//WSACleanup();
	return std::string(rcvBuff/*, pRcv- rcvBuff*/);
}



int _fjjjdf_splitString(std::string str, std::string split, std::deque < std::string> &strArray)
{
	strArray.clear();

	std::string strTemp = str; //此赋值不能少 
	int nIndex = 0; // 
	while (1)
	{
		nIndex = strTemp.find(split);
		if (nIndex != std::string::npos)
		{
			std::string addstr = strTemp.substr(0, nIndex);
			if (!addstr.empty())strArray.push_back(addstr);
			strTemp = strTemp.substr(nIndex + split.size());
		}
		else break;
	}
	if (!strTemp.empty())strArray.push_back(strTemp);
	return strArray.size();
}


//
//
//char * Base64Encode(const char* input, int length, bool with_new_line);
//char * Base64Decode(char* input, int length, bool with_new_line);
//
//int main(int argc, char* argv[])
//{
//	cout << "With new line? y/n ";
//	string option;
//	cin >> option;
//	bool with_new_line = (("y" == option || "Y" == option) ? true : false);
//
//	string enc_input = "Henry Alfred Kissinger is a German-born American writer, political scientist, diplomat, and businessman. A recipient of the Nobel Peace Prize, he served as National Security Advisor and later concurrently as Secretary of State in the administrations of Presidents Richard Nixon and Gerald Ford.";
//
//	cout << endl << "To be encoded:" << endl << "~" << enc_input << "~" << endl << endl;
//
//	char * enc_output = Base64Encode(enc_input.c_str(), enc_input.length(), with_new_line);
//	cout << "Base64 Encoded:" << endl << "~" << enc_output << "~" << endl << endl;
//
//	string dec_input = enc_output;
//	char * dec_output = Base64Decode((char *)dec_input.c_str(), dec_input.length(), with_new_line);
//	cout << "Base64 Decoded:" << endl << "~" << dec_output << "~" << endl << endl;
//
//	free(enc_output);
//	free(dec_output);
//}

char * Base64Encode(const char * input, int length, bool with_new_line)
{
	BIO * bmem = NULL;
	BIO * b64 = NULL;
	BUF_MEM * bptr = NULL;

	b64 = BIO_new(BIO_f_base64());
	if (!with_new_line) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char * buff = (char *)malloc(bptr->length + 1);
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;

	BIO_free_all(b64);

	return buff;
}

char * Base64Decode(char * input, int length, bool with_new_line)
{
	BIO * b64 = NULL;
	BIO * bmem = NULL;
	char * buffer = (char *)malloc(length);
	memset(buffer, 0, length);

	b64 = BIO_new(BIO_f_base64());
	if (!with_new_line) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);
	BIO_read(bmem, buffer, length);

	BIO_free_all(bmem);

	return buffer;
}






std::string _fkkfdf_UrlEncode(const std::string& szToEncode)
{
	std::string src = szToEncode;
	char hex[] = "0123456789ABCDEF";
	std::string dst;


	for (size_t i = 0; i < src.size(); ++i)
	{
		unsigned char cc = src[i];
		if (cc >= 'A' && cc <= 'Z'
			|| cc >= 'a' && cc <= 'z'
			|| cc >= '0' && cc <= '9'
			|| cc == '.'
			|| cc == '_'
			|| cc == '-'
			|| cc == '*')
		{
			if (cc == ' ')
			{
				dst += "+";
			}
			else
				dst += cc;
		}
		else
		{
			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += '%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}
	}
	return dst;
}



std::string _kkf_UrlDecode(const std::string& szToDecode)
{
	std::string result;
	int hex = 0;
	for (size_t i = 0; i < szToDecode.length(); ++i)
	{
		switch (szToDecode[i])
		{
		case '+':
			result += ' ';
			break;
		case '%':
			if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
			{
				std::string hexStr = szToDecode.substr(i + 1, 2);
				if( (hexStr[0]>='0' && hexStr[0]<='9' || hexStr[0] >= 'a' && hexStr[0] <= 'f' || hexStr[0] >= 'A' && hexStr[0] <= 'F')
					&& (hexStr[1] >= '0' && hexStr[1] <= '9' || hexStr[1] >= 'a' && hexStr[1] <= 'f' || hexStr[1] >= 'A' && hexStr[1] <= 'F')
					)
				{
					hex = strtol(hexStr.c_str(), 0, 16);
					result += char(hex);
					i += 2;
				}else result += (char)'%';
			}
			else {
				result += '%';
			}
			break;
		default:
			result += szToDecode[i];
			break;
		}
	}
	return result;
}



bool Reg_SetValue(std::string path, std::string value)
{
	int err = 0;
	BOOL bRet = FALSE;
	HKEY hKey = NULL;
	//创建项，如果存在就打开  
	if (ERROR_SUCCESS != (err = RegCreateKeyExA(HKEY_LOCAL_MACHINE, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)))
	{
		return false;
	}
	else {
		RegSetKeyValueA(hKey, "", "lebbkey", REG_SZ, value.c_str(), value.size());
		RegCloseKey(hKey);
		return true;
	}
}


//
//std::string Reg_SetValue(std::string path, std::string val)
//{
//	int err = 0;
//	BOOL bRet = FALSE;
//	HKEY hKey = NULL;
//	//创建项，如果存在就打开  
//	if (ERROR_SUCCESS != (err = RegCreateKeyExA(HKEY_LOCAL_MACHINE, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)))
//	{
//		return "";
//	}
//	else {
//		char pvalue[1024] = { 0 };
//		DWORD pvaluel = 1024;
//		DWORD outtype = REG_SZ;
//		RegSetKeyValueA(hKey, NULL, "lebbkey", REG_SZ, val.c_str(), val.size());
//		RegCloseKey(hKey);
//		return pvalue;
//	}
//}
//
//

std::string _ksdfj_gtl234(std::string path)
{
	int err = 0;
	BOOL bRet = FALSE;
	HKEY hKey = NULL;
	//创建项，如果存在就打开  
	if (ERROR_SUCCESS != (err = RegCreateKeyExA(HKEY_LOCAL_MACHINE, path.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)))
	{
		return "";
	}
	else {
		char pvalue[1024] = { 0 };
		DWORD pvaluel = 1024;
		DWORD outtype = REG_SZ;
		RegQueryValueExA(hKey, "lebbkey", 0, &outtype, (LPBYTE)pvalue, &pvaluel);
		RegCloseKey(hKey);
		return pvalue;
	}
}


class RegcodeCPU
{
	// forward declarations  
	class InstructionSet_Internal;

public:
	// getters  
	static std::string Vendor(void) { return CPU_Rep.vendor_; }
	static std::string Brand(void) { return CPU_Rep.brand_; }

	static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
	static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
	static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
	static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
	static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
	static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
	static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
	static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
	static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
	static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
	static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
	static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
	static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
	static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
	static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
	static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

	static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
	static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
	static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
	static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
	static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
	static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
	static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
	static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
	static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

	static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
	static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
	static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
	static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
	static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
	static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
	static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
	static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
	static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
	static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
	static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
	static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
	static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
	static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
	static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

	static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

	static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
	static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
	static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
	static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
	static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
	static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

	static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
	static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
	static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
	static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
	static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
	static const InstructionSet_Internal CPU_Rep;

	class InstructionSet_Internal
	{
	public:
		InstructionSet_Internal()
			: nIds_{ 0 },
			nExIds_{ 0 },
			isIntel_{ false },
			isAMD_{ false },
			f_1_ECX_{ 0 },
			f_1_EDX_{ 0 },
			f_7_EBX_{ 0 },
			f_7_ECX_{ 0 },
			f_81_ECX_{ 0 },
			f_81_EDX_{ 0 },
			data_{},
			extdata_{}
		{
			//int cpuInfo[4] = {-1};  
			std::array<int, 4> cpui;

			// Calling __cpuid with 0x0 as the function_id argument  
			// gets the number of the highest valid function ID.  
			__cpuid(cpui.data(), 0);
			nIds_ = cpui[0];

			for (int i = 0; i <= nIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				data_.push_back(cpui);
			}

			// Capture vendor string  
			char vendor[0x20];
			memset(vendor, 0, sizeof(vendor));
			*reinterpret_cast<int*>(vendor) = data_[0][1];
			*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
			*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
			vendor_ = vendor;
			if (vendor_ == "GenuineIntel")
			{
				isIntel_ = true;
			}
			else if (vendor_ == "AuthenticAMD")
			{
				isAMD_ = true;
			}

			// load bitset with flags for function 0x00000001  
			if (nIds_ >= 1)
			{
				f_1_ECX_ = data_[1][2];
				f_1_EDX_ = data_[1][3];
			}

			// load bitset with flags for function 0x00000007  
			if (nIds_ >= 7)
			{
				f_7_EBX_ = data_[7][1];
				f_7_ECX_ = data_[7][2];
			}

			// Calling __cpuid with 0x80000000 as the function_id argument  
			// gets the number of the highest valid extended ID.  
			__cpuid(cpui.data(), 0x80000000);
			nExIds_ = cpui[0];

			char brand[0x40];
			memset(brand, 0, sizeof(brand));

			for (int i = 0x80000000; i <= nExIds_; ++i)
			{
				__cpuidex(cpui.data(), i, 0);
				extdata_.push_back(cpui);
			}

			// load bitset with flags for function 0x80000001  
			if (nExIds_ >= 0x80000001)
			{
				f_81_ECX_ = extdata_[1][2];
				f_81_EDX_ = extdata_[1][3];
			}

			// Interpret CPU brand string if reported  
			if (nExIds_ >= 0x80000004)
			{
				memcpy(brand, extdata_[2].data(), sizeof(cpui));
				memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
				memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
				brand_ = brand;
			}
		};

		int nIds_;
		int nExIds_;
		std::string vendor_;
		std::string brand_;
		bool isIntel_;
		bool isAMD_;
		std::bitset<32> f_1_ECX_;
		std::bitset<32> f_1_EDX_;
		std::bitset<32> f_7_EBX_;
		std::bitset<32> f_7_ECX_;
		std::bitset<32> f_81_ECX_;
		std::bitset<32> f_81_EDX_;
		std::vector<std::array<int, 4>> data_;
		std::vector<std::array<int, 4>> extdata_;
	};
};

// Initialize static member data  
const RegcodeCPU::InstructionSet_Internal RegcodeCPU::CPU_Rep;

// Print out supported instruction set extensions


HANDLE h_kkgna5;
DWORD WINAPI _lczppr_298844(void *param)
{
	for (int i = 0; i < 7; i++)
		Sleep(100);

	SetEvent((HANDLE)param);
	return 0;
}


std::string _akkd_2343(time_t intime)
{
	struct tm intm;
	localtime_s(&intm, &intime);
	char out_time_str[100] = { 0 };
	int out_time_str_bufsize = 100;
	strftime(out_time_str, out_time_str_bufsize, "%Y-%m-%d %X", &intm);
	return out_time_str;
}



BOOL GetDiskSerialNum(PCHAR pIDBufer)

{
	HANDLE hDevice = NULL;
	hDevice = ::CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (!hDevice)

	{

		hDevice = ::CreateFileA("\\\\.\\Scsi0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	}

	if (!hDevice)

	{

		return FALSE;

	}

	DWORD dwBytesReturned = 0;
	GETVERSIONINPARAMS gVersionParsams;
	ZeroMemory(&gVersionParsams, sizeof(GETVERSIONINPARAMS));


	if (!DeviceIoControl(hDevice, SMART_GET_VERSION, NULL, NULL, &gVersionParsams, sizeof(GETVERSIONINPARAMS), &dwBytesReturned, NULL)
		|| dwBytesReturned == 0 || gVersionParsams.bIDEDeviceMap <= 0)
	{
		::CloseHandle(hDevice);
		return FALSE;
	}

	SENDCMDINPARAMS scip;

	ZeroMemory(&scip, sizeof(SENDCMDINPARAMS));
	scip.cBufferSize = IDENTIFY_BUFFER_SIZE;
	scip.irDriveRegs.bSectorCountReg = 1;
	scip.irDriveRegs.bSectorNumberReg = 1;
	scip.irDriveRegs.bDriveHeadReg = 0xA0;
	scip.irDriveRegs.bCommandReg = 0xEC;

	BYTE btBuffer[1024] = { 0 };

	if (!DeviceIoControl(hDevice, SMART_RCV_DRIVE_DATA, &scip, sizeof(SENDCMDINPARAMS),
		btBuffer, 1024, &dwBytesReturned, NULL))
	{
		::CloseHandle(hDevice);
		return FALSE;
	}

	int nPos = 0x24;            //序列号的开始位置,具体请参考SENDCMDOUTPARAMS与IDSECTOR结构
	while (btBuffer[nPos]<128)
	{
		*pIDBufer = btBuffer[nPos++];
		pIDBufer++;
	}
	*pIDBufer = 00;
	return TRUE;
}


void _cdskfkp_3883(char *kk)
{
	std::array<int, 4> cpui;    //#include <array>
	// Calling __cpuid with 0x0 as the function_id argument  
	// gets the number of the highest valid function ID.  什么是function ID？
	__cpuid(cpui.data(), 0x0);    //cpui[0] = "funcition_id的最大值"
	int nIds_ = cpui[0];
	std::vector<std::array<int, 4>> data_;  //保存遍历到的所有cpui的值    
	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);
	}
	//reinterpret_cast<int*>(vendor) //*reinterpret_cast<int*>(vendor)
	//索引0 0+4 0+8的值构成了CPU芯片的名称
	char vendor[0x20] = { 0 };
	*reinterpret_cast<int*>(vendor) = data_[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data_[0][2];  // vendor="GenuineIntel"    
	std::string vendor_ = vendor;
	bool isIntel_ = false;
	bool isAMD = false;
	if ("GenuineIntel" == vendor_)
	{
		isIntel_ = true;    //厂商为INTEL
	}
	else if ("AuthenticAMD" == vendor_)
	{
		isAMD = true;       //厂商为AMD
	}
	char vendor_serialnumber[0x14] = { 0 };
	sprintf_s(vendor_serialnumber, sizeof(vendor_serialnumber), "%08X%08X", data_[1][3], data_[1][0]);
	//用“wmic cpu get processorid”获取的结果进行比对，结果应该是一致的。    //vendor_serialnumber = "BFEBFBFF000406E3";    
	strcpy(kk,vendor_serialnumber);
	return ;
}



DWORD WINAPI _hjfd84267909(void *param)
{
	bool bindebug = false;
	bool bftime = true;
	Sleep(1000);////LBLOGD(WARNING << "thrad 1");
	int timei = 0;
	while (1)
	{
		//花指令
		__asm
		{
			jz  GG
			jnz GG
			_emit 0xe8
			GG:
		}//花指令end


#ifndef DisableDebugQuit

		CreateThread(NULL, 100 * 1024, _hasds_58222, NULL, 0, NULL);
		
		if (PebNtGlobalFlagsApproach())
		{

			//LBLOGD(WARNING << "thrad 25");
			//花指令
			{__asm
			{
				jz  GG41
					jnz GG41
					_emit 0xe8
				GG41:
			}
			}//花指令end
			bindebug = true;
		}

		 //花指令
		{__asm
		{
			jz  GG2
			jnz GG2
			_emit 0xe8
			GG2:
		}}//花指令end


		if (PebNtGlobalFlagsApproach())
		{

			//LBLOGD(WARNING << "thrad 25");
			//花指令
			{__asm
			{
				jz  GG40
					jnz GG40
					_emit 0xe8
				GG40:
			}
			}//花指令end
			bindebug = true;
		}

#endif


		Sleep(20);////LBLOGD(WARNING << "thrad 2");
		timei++;
		if (timei < 200)continue;


		if (bftime)
		{
			//LBLOGD(WARNING << "thrad 3");

			bftime = false;

			std::string curtimestr = _akkd_2343(time(NULL));

			char szAppPath[1024];
			GetModuleFileNameA(NULL, szAppPath, 1024);

			std::string pcinfostr;
			char OriginalFilename[256] = { 0 };
			GetExeFileDetailInfo(szAppPath, OriginalFilename, "OriginalFilename");
			strrchr(OriginalFilename, '.')[0] = 0;
			pcinfostr = curtimestr+"`a`" + std::string(OriginalFilename) + "`4`";
			
			char disksn2[128] = { 0 };
			_cdskfkp_3883(disksn2);//cpu serial number
			pcinfostr += std::string(disksn2) + "`";
			std::string kksk2=_ksdkf_2343();
			pcinfostr += kksk2 + "`";

			//char disksn[128] = { 0 };
			//GetDiskSerialNum(disksn);
			//pcinfostr = std::string(disksn) + "`";


			std::string vendor = RegcodeCPU::Vendor();
			pcinfostr += vendor + "`";
			
			std::string brand = RegcodeCPU::Brand();
			pcinfostr += brand + "`";


			std::string out_serialnum, out_module_name;
			bool rl = GetDiskSerialNumber(0, &out_serialnum, &out_module_name);
			pcinfostr += out_serialnum + "`" + out_module_name + "`";

			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			char sysinfostr[128];
			sprintf(sysinfostr, "v1,%u,%u,%d", sysinfo.dwNumberOfProcessors, sysinfo.wProcessorArchitecture, (int)_skke23_64());
			pcinfostr += std::string(sysinfostr) + "`";


			char *username2=getenv("USERNAME");
			char UserName[128] = { 0 };
			DWORD UserNameLen;
			GetUserNameA(UserName, &UserNameLen);
			pcinfostr += std::string(username2) + "`";


			char pcname[15] = { 0 };
			DWORD pcnamelen = 15;
			GetComputerNameA(pcname, &pcnamelen);
			pcinfostr += std::string(pcname) + "`";

			while (pcinfostr.find("  ")!=string::npos)pcinfostr = pcinfostr.replace(pcinfostr.find("  "), 2, " ");

			//LBLOGD(WARNING << "thrad 5");

			bool breg = false;

			wchar_t *pcinfostrw = new wchar_t[1024];
			DWORD len=MultiByteToWideChar(CP_ACP, 0, pcinfostr.c_str(), pcinfostr.size(), pcinfostrw, 10240);
			pcinfostrw[len] = 0;
			char *u8pcinfostr = new char[1024];
			DWORD len2=WideCharToMultiByte(CP_UTF8, 0, pcinfostrw, len, u8pcinfostr, 10240, 0, 0);
			u8pcinfostr[len2] = 0;
			int encypteddatal = 0;

			//LBLOGD(WARNING << "thrad 6");

	
			//LBLOGD(WARNING << "thrad 7");



			HANDLE ddd = CreateThread(NULL, 100 * 1024, _kkdkks_3428438, (LPVOID)breg, 0, NULL);;

			if(breg==false)
			{
				//LBLOGD(WARNING << "thrad 8");
				char *u8pcinfostr_1 = new char[1024];
				memset(u8pcinfostr_1, 0, 1024);
				char *u8pcinfostr_2 = new char[1024];
				memset(u8pcinfostr_2, 0, 1024);
				std::string seg2str = u8pcinfostr;
				std::string seg1 = seg2str.substr(0, 117);
				std::string seg2 = "";
				if (seg2str.size()>117)
					seg2=seg2str.substr(117);
				memcpy(u8pcinfostr_1, seg1.c_str(), seg1.size());
				memcpy(u8pcinfostr_2, seg2.c_str(), seg2.size());
				int encypteddatal_1 = 0, encypteddatal_2 = 0;
				char *encypteddata_1 = rsa_pubkey_encrypt(u8pcinfostr_1, pubkey, &encypteddatal_1);
				char *encypteddata_2 = rsa_pubkey_encrypt(u8pcinfostr_2, pubkey, &encypteddatal_2);
				auto b64str3_1 = Base64Encode(encypteddata_1, encypteddatal_1, false);
				auto b64str3_2 = Base64Encode(encypteddata_2, encypteddatal_2, false);
				std::string b64str2_1 = b64str3_1;
				std::string b64str2_2 = b64str3_2;
				free(b64str3_1);
				auto b64urlstr_1 = _fkkfdf_UrlEncode(b64str2_1);
				auto b64urlstr_2 = _fkkfdf_UrlEncode(b64str2_2);


				//LBLOGD(WARNING << "thrad 9");

				//int outdatal = 0;
				//auto b64urlstr2 = _kkf_UrlDecode(b64urlstr);
				//auto b64urlstr3 = Base64Decode((char*)b64urlstr2.c_str(), b64urlstr2.size(), false);
				//std::string kfkkf(b64urlstr3, strlen(b64urlstr3));
				//char *kkks=rsa_prikey_decrypt(b64urlstr3, prikey, &outdatal);
				//发送数据到服务器, 服务器返回加密后的发送时间, 与注册码

				auto ipstr=getdomainip("www.lebb.cc");
				std::string getdata = _fkkfdf_k30(ipstr, b64urlstr_1 + b64urlstr_2, bindebug);
#ifdef _DEBUG
				FILE *getdf = fopen("getdf.txt", "wb");
				fwrite(getdata.c_str(), 1, getdata.size(), getdf);
				fclose(getdf);
#endif

				//get http return data
				if (getdata.rfind("=")!=std::string::npos)getdata = getdata.substr(0, getdata.rfind("=")+1);
				if (getdata.rfind("\n")!=std::string::npos)getdata = getdata.substr(getdata.rfind("\n") + 1);
				if (getdata.size())
				{
					auto strend = Base64Decode((char*)getdata.c_str(), getdata.size(), false);
					//解密, 解密后格式应该是:time,regcode
					int decryptdatal = 0;
					char *decryptdata = rsa_pubkey_decrypt((char*)strend, pubkey, &decryptdatal);
					free(strend);
					//确认无误, 保存注册码, 退出
					if (decryptdata && decryptdatal)
					{
						//LBLOGD(WARNING << "thrad 11");

						std::deque<std::string> rldeq;
						_fjjjdf_splitString(decryptdata, "`", rldeq);
						if (rldeq.size()>=1)
						{
							//LBLOGD(WARNING << "thrad 12");
							if (rldeq[0] == curtimestr)
							{
								//网络认证
								if (rldeq[1].find("allow") != std::string::npos && rldeq[1].find("success") != std::string::npos)
								{
									//LBLOGD(WARNING << "thrad 13");

									//save to regstry
									//auto offeridkey = std::string() + "SoftWare\\lebb.cc\\" + (_softcode + 32);
									//Reg_SetValue(offeridkey, getdata);

									breg = true;

									ddd = CreateThread(NULL, 100 * 1024, _kllsioir_9942, (LPVOID)breg, 0, NULL);

									

									g_val = 8235;
								}
							}
						}
					}
					if (decryptdata)free(decryptdata);
				}
				if (encypteddata_1)free(encypteddata_1);
				if (encypteddata_2)free(encypteddata_2);
			}

			//LBLOGD(WARNING << "thrad 14");

			if (sysinfostr)delete[]sysinfostr;
			if(u8pcinfostr)delete []u8pcinfostr;
			if (pcinfostrw)delete[]pcinfostrw;
			//if (decryptdata)free(decryptdata);

			//LBLOGD(WARNING << "thrad 15");

			auto t1 = GetTickCount();

			WaitForSingleObject(ddd, INFINITE);

			h_kkgna5 = CreateEvent(NULL, FALSE, FALSE, NULL);
			for (int i = 0; i < delaychecktime; i += 1)
			{
				Sleep(100);
				HANDLE ddd = CreateThread(NULL, 100 * 1024, _lczppr_298844, h_kkgna5, 0, NULL);
				Sleep(100);
				WaitForSingleObject(ddd, 1000);
				Sleep(100);
				WaitForSingleObject(h_kkgna5, INFINITE);
			}



			if (!breg)
			{
				CreateThread(NULL, 100 * 1024, _kkdf_59922, NULL, 0, NULL);

#ifdef QT_VERSION
				//LBLOGD(WARNING << "thrad 16");
				qApp->quit();
#endif
				//花指令
				{__asm
				{
					jz  GG31
					jnz GG31
					_emit 0xe8
					GG31:
				}}//花指令end


				//LBLOGD(WARNING << "thrad 17");

				DWORD curpocid = GetCurrentProcessId();
				HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
				TerminateProcess(ProcH, 0);
				CloseHandle(ProcH);


				//LBLOGD(WARNING << "thrad 18");

				//花指令
				{__asm
				{
					jz  GG30
					jnz GG30
					_emit 0xe8
					GG30:
				}}//花指令end

				CreateThread(NULL, 100 * 1024, _kkdf_59922, NULL, 0, NULL);

				exit(0);
			}
		}

		//花指令
		{__asm
		{
			jz  GG3
			jnz GG3
			_emit 0xe8
			GG3:
		}}//花指令end

		  //花指令
		{__asm
		{
			jz  GG4
			jnz GG4
			_emit 0xe8
			GG4:
		}}//花指令end


		//LBLOGD(WARNING << "thrad 19");
#ifndef DisableDebugQuit


		CreateThread(NULL, 100 * 1024, _hasds_58222, NULL, 0, NULL);
		
		if (PebIsDebuggedApproach())
		{

			//LBLOGD(WARNING << "thrad 20");
			//花指令
			{__asm
			{
				jz  GG12
				jnz GG12
				_emit 0xe8
				GG12:
			}
			}//花指令end

#ifdef QT_VERSION
			//LBLOGD(WARNING << "thrad 21");
			qApp->quit();
#endif
			//花指令
			{__asm
			{
				jz  GG11
				jnz GG11
				_emit 0xe8
				GG11:
			}}//花指令end


			//LBLOGD(WARNING << "thrad 22");
			DWORD curpocid = GetCurrentProcessId();
			HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
			TerminateProcess(ProcH, 0);
			CloseHandle(ProcH);

			//花指令
			{__asm
			{
				jz  GG10
				jnz GG10
				_emit 0xe8
				GG10:
			}}//花指令end


			//LBLOGD(WARNING << "thrad 23");
			exit(0);

			//LBLOGD(WARNING << "thrad 24");
		}
		
		//花指令
		//花指令
		{__asm
		{
			jz  GG5
			jnz GG5
			_emit 0xe8
			GG5:
		}}//花指令end

		if (PebNtGlobalFlagsApproach())
		{

			//LBLOGD(WARNING << "thrad 25");
			//花指令
			{__asm
			{
				jz  GG22
				jnz GG22
				_emit 0xe8
				GG22:
			}
			}//花指令end

#ifdef QT_VERSION
			//LBLOGD(WARNING << "thrad 26");
			qApp->quit();
#endif
			//花指令
			{__asm
			{
				jz  GG21
				jnz GG21
				_emit 0xe8
				GG21:
			}}//花指令end


			//LBLOGD(WARNING << "thrad 27");
			DWORD curpocid = GetCurrentProcessId();
			HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
			TerminateProcess(ProcH, 0);
			CloseHandle(ProcH);

			//花指令
			{__asm
			{
				jz  GG20
				jnz GG20
				_emit 0xe8
				GG20:
			}}//花指令end


			//LBLOGD(WARNING << "thrad 28");
			exit(0);
		}

#endif
		
		//花指令
		{__asm
		{
			jz  GG6
			jnz GG6
			_emit 0xe8
			GG6:
		}}//花指令end

		Sleep(1000);

		//花指令
		{__asm
		{
			jz  GG7
			jnz GG7
			_emit 0xe8
			GG7:
		}}//花指令end

		 //花指令
		{__asm
		{
			jz  GG8
			jnz GG8
			_emit 0xe8
			GG8:
		}}//花指令end


		Sleep(24000000);
	}


#ifdef QT_VERSION
	//LBLOGD(WARNING << "thrad 16");
	qApp->quit();
#endif
	//花指令
	{__asm
	{
		jz  GG51
			jnz GG51
			_emit 0xe8
		GG51:
	}}//花指令end


	//LBLOGD(WARNING << "thrad 17");

	DWORD curpocid = GetCurrentProcessId();
	HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
	TerminateProcess(ProcH, 0);
	CloseHandle(ProcH);


	//LBLOGD(WARNING << "thrad 18");

	//花指令
	{__asm
	{
		jz  GG50
			jnz GG50
			_emit 0xe8
		GG50:
	}}//花指令end

	exit(0);


	return 0;
}




DWORD WINAPI _kslsl3667643(void *data)
{
	memset((char*)data, 0, strlen((char*)data));
	HeapFree(GetProcessHeap(), 0, data);
	return 0;
}

DWORD WINAPI _lsdk469993(void *data)
{
	wchar_t *dir = new wchar_t[1024];
	DWORD l = GetModuleFileName(NULL, dir, 1024);
	dir[l] = 0;
	wcsrchr(dir, '\\')[1] = 0;
	char *dirc = new char[1024];
	l=WideCharToMultiByte(CP_ACP, 0, dir, wcslen(dir), dirc, 1024, 0, 0);
	dirc[l] = 0;
	strcat(dirc, "机器码[注册地址lebb.cc／regtool.php].txt");
	FILE *ff = fopen(dirc, "wb");
	fwrite((char*)data, 1, strlen((char*)data), ff);
	fclose(ff);
	HANDLE ddd = CreateThread(NULL, 100 * 1024, _kslsl3667643, data, 0, NULL);
	WaitForSingleObject(ddd, INFINITE);
	delete[]dir;
	delete[]dirc;
	return 0;
}

HANDLE h_kkgna3;
DWORD WINAPI _kloso_0387854(void *param)
{
	for (int i = 0; i < 7;i++)
		Sleep(100);

	SetEvent((HANDLE)param);
	return 0;
}
DWORD WINAPI _nskl_72234(void *param)
{


	//rsa加密computeuniquestr 后传给服务器, computeuniquestr 要有发送时间,软件exe名字, 软件名,  电脑名,硬件信息, 用户信息, 系统信息, 软件码信息(每个客户有个唯一的16位码, 替换软件内容方式生成)
	std::string pcinfostr;
	//系统信息 40byte
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	char *sysinfostr = new char[1024];
	sprintf(sysinfostr, "v1,%u,%u,%d", sysinfo.dwNumberOfProcessors, sysinfo.wProcessorArchitecture, (int)_skke23_64());
	pcinfostr = pcinfostr + sysinfostr + "|";
	//时间 10byte
	time_t curtime = time(NULL);
	char curtimestr[32] = { 0 };
	sprintf(curtimestr, "%I64d", curtime);
	pcinfostr = pcinfostr + curtimestr + "|";
	//offerid and wangwanguser
	//
	pcinfostr = pcinfostr + (_softcode + 32) + "|";
	pcinfostr = pcinfostr + (_wangwangid + 32) + "|";
	//file name 48byte
	char szAppPath[MAX_PATH];
	GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
	char OriginalFilename[256] = { 0 };
	GetExeFileDetailInfo(szAppPath, OriginalFilename, "OriginalFilename");
	strrchr(OriginalFilename, '.')[0] = 0;
	if (strlen(OriginalFilename) > 16)
		OriginalFilename[16] = 0;
	pcinfostr = pcinfostr + OriginalFilename + "|";

	//LBLOGD(WARNING << "thrad 4");

	//hardware name
	pcinfostr = pcinfostr + get_hardware_idstr();//will have "|"
	//pc name + username
	char pcname[15] = { 0 };
	DWORD pcnamelen = 15;
	GetComputerNameA(pcname, &pcnamelen);
	char uname[15] = { 0 };
	DWORD unamel = 15;
	GetUserNameA(uname, &unamel);
	std::string pcuname = std::string() + pcname + "." + uname;
	pcinfostr = pcinfostr + pcuname;
	if (pcinfostr.size() > 117)pcinfostr = pcinfostr.substr(0, 117);

	//LBLOGD(WARNING << "thrad 5");

	wchar_t *pcinfostrw = new wchar_t[1024];
	DWORD len = MultiByteToWideChar(CP_ACP, 0, pcinfostr.c_str(), pcinfostr.size(), pcinfostrw, 10240);
	pcinfostrw[len] = 0;
	char *u8pcinfostr = new char[1024];
	DWORD len2 = WideCharToMultiByte(CP_UTF8, 0, pcinfostrw, len, u8pcinfostr, 10240, 0, 0);
	u8pcinfostr[len2] = 0;
	int encypteddatal = 0;


	char *encypteddata = rsa_pubkey_encrypt(u8pcinfostr, pubkey, &encypteddatal);
	auto b64str3 = Base64Encode(encypteddata, encypteddatal, false);
	std::string b64str2 = b64str3;
	free(b64str3);
	b64str2 = b64str2.substr(0, 14) + "S" + b64str2.substr(14);
	char *ccc = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, b64str2.size() + 1);
	memcpy(ccc, b64str2.c_str(), b64str2.size());

	HANDLE ddd = CreateThread(NULL, 100 * 1024, _lsdk469993, ccc, 0, NULL);
	WaitForSingleObject(ddd, INFINITE);
	return 0;

}

HANDLE h_kkgna4;
DWORD WINAPI _nlkz0_82912(void *param)
{
	for (int i = 0; i < 7; i++)
		Sleep(100);

	SetEvent((HANDLE)param);
	return 0;
}

DWORD WINAPI _mxkf_483838(void *param)
{
	auto t1=GetTickCount();

	h_kkgna4 = CreateEvent(NULL, FALSE, FALSE, NULL);
	for (int i = 0; i < 2; i += 1)
	{
		Sleep(100);
		HANDLE ddd = CreateThread(NULL, 100 * 1024, _nlkz0_82912, h_kkgna4, 0, NULL);
		Sleep(100);
		WaitForSingleObject(ddd, 1000);
		Sleep(100);
		WaitForSingleObject(h_kkgna4, INFINITE);
	}


	HANDLE ddd = CreateThread(NULL, 100 * 1024, _hjfd84267909, NULL, 0, NULL);
	WaitForSingleObject(ddd, INFINITE);

	h_kkgna3 = CreateEvent(NULL, FALSE, FALSE, NULL);
	for (int i = 0; i < delaychecktime; i += 1)
	{
		Sleep(100);
		HANDLE ddd = CreateThread(NULL, 100 * 1024, _kloso_0387854, h_kkgna3, 0, NULL);
		Sleep(100);
		WaitForSingleObject(ddd, 1000);
		Sleep(100);
		WaitForSingleObject(h_kkgna3, INFINITE);
	}

#ifdef QT_VERSION
	//LBLOGD(WARNING << "thrad 16");
	qApp->quit();
#endif
	//花指令
	{__asm
	{
		jz  GG51
			jnz GG51
			_emit 0xe8
		GG51:
	}}//花指令end


	//LBLOGD(WARNING << "thrad 17");

	DWORD curpocid = GetCurrentProcessId();
	HANDLE ProcH = OpenProcess(PROCESS_TERMINATE, FALSE, curpocid);
	TerminateProcess(ProcH, 0);
	CloseHandle(ProcH);


	//LBLOGD(WARNING << "thrad 18");

	//花指令
	{__asm
	{
		jz  GG50
			jnz GG50
			_emit 0xe8
		GG50:
	}}//花指令end

	exit(0);



}

struct _rdgfggghfggfg
{
	_rdgfggghfggfg()
	{
//#if !defined(_DEBUG) || defined(SoftwareLockerMicroDebug)
#if 1
		WSADATA wsadata;
		WSAStartup(MAKEWORD(1, 1), &wsadata);
		
		HANDLE ddd = CreateThread(NULL, 100 * 1024, _nskl_72234, NULL, 0, NULL);

	//LBLOGD(WARNING << "thrad end");
		CreateThread(NULL, 100 * 1024, _mxkf_483838, NULL, 0, NULL);
	//LBLOGD(WARNING << "thrad end");

#endif
	}
};

static _rdgfggghfggfg  _gfgfsg;

#endif

