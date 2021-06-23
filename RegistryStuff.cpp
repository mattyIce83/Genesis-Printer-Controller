//  RegistryStuff.cpp
//

#include "stdafx.h"
#include "Controller4.h"
#include "Controller4Dlg1.h"


extern CController4Dlg1 *theDlg;

/// First encryption level
static unsigned long ENC_KEY = 0x000F35F1;

/// Second encryption level
#define SECONDARY_ENC_KEY		0x69

#define MASTER_REG_KEY_1	"SOFTWARE"
#define MASTER_REG_KEY_2	"SOFTWARE\\MICROSOFTFOOCACHE"
#define MASTER_REG_KEY_3	"SOFTWARE\\MICROSOFTFOOCACHE\\VARS"

#define EODO_SUBKEY		"FXX1"
#define ESN_SUBKEY		"FXX2"
#define UODO_SUBKEY		"FXX3"
#define USN_SUBKEY		"FXX4"


extern DWORD CalcCRC(LPVOID, UINT );


/*------------------------------------------------------------------*/
/*- Get unique mac address                                         -*/
/*------------------------------------------------------------------*/
static BOOL GetMACaddress(char *szCode) {

	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information
                                         // for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
								AdapterInfo,                 // [out] buffer to receive data
								&dwBufLen);                  // [in] size of receive data buffer

	if(dwStatus == ERROR_SUCCESS) {
		
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
                                               // current adapter info
		do {
			//PrintMACaddress(pAdapterInfo->Address); // Print MAC address
			sprintf(szCode, "%d.%d.%d.%d.%d.%d",
				pAdapterInfo->Address[0],
				pAdapterInfo->Address[1],
				pAdapterInfo->Address[2],
				pAdapterInfo->Address[3],
				pAdapterInfo->Address[4],
				pAdapterInfo->Address[5]);
			//strcat(szCode, pAdapterInfo->AdapterName);
			//strcat(szCode, pAdapterInfo->Description);
			pAdapterInfo = pAdapterInfo->Next;    // Progress through
			return TRUE;                                      // linked list
		}
		while(pAdapterInfo);                    // Terminate if last adapter
	
		return TRUE;
	}
	return FALSE;
}

/*------------------------------------------------------------------*/
/*-                                                                -*/
/*------------------------------------------------------------------*/
BOOL CreateMasterRegKey() {
char szKey[MAX_PATH];
HKEY key, hsubkey = NULL;
unsigned ret, type = 0;
DWORD Disposition;
int error;


	sprintf(szKey, "%s", MASTER_REG_KEY_1);
	if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {
		MessageBox(GetFocus(), "Unable to open level 1 r_key", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
		return 0;
	}
	
	RegCloseKey(key);

	sprintf(szKey, "%s", MASTER_REG_KEY_2);

	if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {

		sprintf(szKey, "%s", MASTER_REG_KEY_1);
		if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {
			MessageBox(GetFocus(), "Unable to open level 1 r_key (2)", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
			return 0;
		}


		sprintf(szKey, "%s", MASTER_REG_KEY_2);

		ret = RegCreateKeyEx(
						key,		// handle to an open key
						"MICROSOFTFOOCACHE",       // address of subkey name
						0,          // reserved
						"",         // address of class string
						REG_OPTION_NON_VOLATILE,          // special options flag
						KEY_ALL_ACCESS,        // desired security access
						NULL,
						&hsubkey,          // address of buffer for opened handle
						&Disposition);   // address of disposition value buffer
		
		
		if(ret != ERROR_SUCCESS) {
			error = GetLastError();
			MessageBox(GetFocus(), "Unable to create second key", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
			RegCloseKey(key);
			return 0;
		}

		RegCloseKey(hsubkey);
		hsubkey = NULL;

	}
	
	RegCloseKey(key);
	
	sprintf(szKey, "%s", MASTER_REG_KEY_3);

	if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {

		sprintf(szKey, "%s", MASTER_REG_KEY_2);
		if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {
			MessageBox(GetFocus(), "Unable to open level 2 r_key (2)", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
			return 0;
		}

		if(RegCreateKey(HKEY_LOCAL_MACHINE, MASTER_REG_KEY_3, &hsubkey) != ERROR_SUCCESS) {
		
			MessageBox(GetFocus(), "Unable to create third key", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
			RegCloseKey(key);
			return 0;
			
		}

	}

	if(hsubkey != NULL)
		RegCloseKey(hsubkey);
	RegCloseKey(key);

	return TRUE;
}

/*------------------------------------------------------------------*/
/*-                                                                -*/
/*------------------------------------------------------------------*/
BOOL SetRegKeys(char *szSN, unsigned char *ESN, int Count, unsigned char *ECount, unsigned char *aux1, unsigned char *aux2) {
char szKey[MAX_PATH];
HKEY key, hsubkey;
unsigned ret, type = 0;
DWORD Disposition;
int error, i;

	sprintf(szKey, "%s", MASTER_REG_KEY_3);
	if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {
		theDlg->OKBox("Unable to open master key", "", "");
		return 0;
	}

	ret = RegSetValueEx(	key,
							ESN_SUBKEY,
							0,
							REG_BINARY,
							ESN,
							strlen(szSN));

	if(ret != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to create ESN value", "BQ Serializer",  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to create ESN value", "", "");
		RegCloseKey(key);
		return 0;
	}

	char obs[32];

	strcpy(obs, szSN);
	
	for(i = 0; i < strlen(szSN); i++) {
		obs[i] ^= 0xAA;
	}


	ret = RegSetValueEx(	key,
							USN_SUBKEY,
							0,
							REG_BINARY,
							(unsigned char *)obs,
							strlen(szSN));

	if(ret != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to create USN value", "BQ Serializer",  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to create USN value", "", "");
		RegCloseKey(key);
		return 0;
	}


	ret = RegSetValueEx(	key,
							EODO_SUBKEY,
							0,
							REG_DWORD,
							ECount,
							4);

	if(ret != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to create EODO value", "BQ Serializer",  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to create EODO value", "", "");
		RegCloseKey(key);
		return 0;
	}

	unsigned long eodo = Count;
	unsigned char *odoptr = (unsigned char *)&eodo;
	for(i = 0; i < 4; i++) {
		odoptr[i] ^= 0xAA;
	}


	ret = RegSetValueEx(	key,
							UODO_SUBKEY,
							0,
							REG_DWORD,
							(unsigned char *)&eodo,
							4);

	if(ret != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to create UODO value", "BQ Serializer",  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to create UODO value", "", "");
		RegCloseKey(key);
		return 0;
	}

	if(aux1 != 0) {
		ret = RegSetValueEx(	key,
								"AUX1",
								0,
								REG_SZ,
								(unsigned char *)"0",
								3);

		if(ret != ERROR_SUCCESS) {
			//MessageBox(GetFocus(), "Unable to create AUX1 value", "BQ Serializer",  MB_OK|MB_SYSTEMMODAL);
		
			theDlg->OKBox("Unable to create AUX1 value", "", "");

			RegCloseKey(key);
			return 0;
		}
	}

	if(aux2 != 0) {

		ret = RegSetValueEx(	key,
								"AUX2",
								0,
								REG_SZ,
								(unsigned char *)"0",
								3);

		if(ret != ERROR_SUCCESS) {
			//MessageBox(GetFocus(), "Unable to create AUX2 value", "BQ Serializer",  MB_OK|MB_SYSTEMMODAL);
			theDlg->OKBox("Unable to create AUX2 value", "", "");
			RegCloseKey(key);
			return 0;
		}

	}

	RegCloseKey(key);

	return TRUE;


}


/*------------------------------------------------------------------*/
/*- Get printer's unique ID                                        -*/
/*------------------------------------------------------------------*/
unsigned long GetFingerPrint() {
char macaddress[32];
unsigned long fp = 0;
unsigned char *tptr;
unsigned char *mk = (unsigned char *)&ENC_KEY;
int i;

	if(GetMACaddress(macaddress)) {

		fp = CalcCRC(macaddress, strlen(macaddress));

		tptr = (unsigned char *)&fp;

		for(i = 0; i < 4; i++) {
			tptr[i] = tptr[i] ^ mk[i];
			tptr[i] = tptr[i] ^ SECONDARY_ENC_KEY;
		}

	}


	return fp;

}


/*------------------------------------------------------------------*/
/*- Get printer serial number (encrypted)                          -*/
/*------------------------------------------------------------------*/
static BOOL GetEncryptedSerialNumber(unsigned char *dest) {
HKEY hkey;
char szContents[128], *subkey;
unsigned long ret, len, type;

	subkey = ESN_SUBKEY;

	len = 256;
			
 	if ((ret = RegOpenKey (HKEY_LOCAL_MACHINE, MASTER_REG_KEY_3, &hkey)) != ERROR_SUCCESS) {
		return FALSE;
	}

 	len = 256;
	if ((ret = RegQueryValueEx (hkey, subkey, NULL, &type, (LPBYTE)szContents, &len)) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return FALSE;
	}

	memcpy(dest, szContents, len);

	RegCloseKey(hkey);

	return TRUE;

}

/*------------------------------------------------------------------*/
/*- Get printer serial number (encrypted)                          -*/
/*------------------------------------------------------------------*/
static BOOL GetEncryptedJobCount(unsigned char *dest) {
HKEY hkey;
char szContents[128], *subkey;
unsigned long ret, len, type;

	subkey = EODO_SUBKEY;

	len = 256;
			
 	if ((ret = RegOpenKey (HKEY_LOCAL_MACHINE, MASTER_REG_KEY_3, &hkey)) != ERROR_SUCCESS) {
		return FALSE;
	}

 	len = 256;
	if ((ret = RegQueryValueEx (hkey, subkey, NULL, &type, (LPBYTE)szContents, &len)) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return FALSE;
	}

	memcpy(dest, szContents, len);

	RegCloseKey(hkey);

	return TRUE;

}

/*------------------------------------------------------------------*/
/*- Get printer serial number (encrypted)                          -*/
/*------------------------------------------------------------------*/
static BOOL GetJobCount(unsigned long *dest) {
HKEY hkey;
char szContents[128], *subkey;
unsigned long ret, len, type;

	subkey = UODO_SUBKEY;
	
	len = 256;
			
 	if ((ret = RegOpenKey (HKEY_LOCAL_MACHINE, MASTER_REG_KEY_3, &hkey)) != ERROR_SUCCESS) {
		return FALSE;
	}

 	len = 256;
	if ((ret = RegQueryValueEx (hkey, subkey, NULL, &type, (LPBYTE)szContents, &len)) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return FALSE;
	}

	int i;

	for(i = 0; i < 4; i++) {
		szContents[i] ^= 0xAA;
	}
	
	*dest = (unsigned long)*szContents;

	RegCloseKey(hkey);

	return TRUE;
}

/*------------------------------------------------------------------*/
/*- Get printer serial number (encrypted)                          -*/
/*------------------------------------------------------------------*/
static BOOL GetPlainTextSerialNumber(char *dest) {
HKEY hkey;
char szContents[128], *subkey;
unsigned long ret, len, type;

	subkey = USN_SUBKEY;

	len = 256;
			
 	if ((ret = RegOpenKey (HKEY_LOCAL_MACHINE, MASTER_REG_KEY_3, &hkey)) != ERROR_SUCCESS) {
		return FALSE;
	}

 	len = 256;
	if ((ret = RegQueryValueEx (hkey, subkey, NULL, &type, (LPBYTE)szContents, &len)) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return FALSE;
	}

	int i;

	for(i = 0; i < len; i++) {
		szContents[i] ^= 0xAA;
	}

	szContents[len] = 0;

	strcpy(dest, szContents);



	RegCloseKey(hkey);

	return TRUE;
}


/*------------------------------------------------------------------*/
/*-                                                                -*/
/*------------------------------------------------------------------*/
void EncryptSN(unsigned char *enc, char *szSN, unsigned long infp) {
unsigned char *eptr = enc;
int len = strlen(szSN), i, ii = 0;
unsigned long fp = infp;
unsigned char *uptr = (unsigned char *)szSN;
unsigned char *fpptr = (unsigned char *)&fp;

	for(i = 0; i < len; i++) {

		eptr[i] = uptr[i] ^ fpptr[ii++];

		eptr[i] ^= 0xFF;

		if(ii >3)
			ii = 0;

	}


}

/*------------------------------------------------------------------*/
/*-                                                                -*/
/*------------------------------------------------------------------*/
void DecryptSN(unsigned char *enc, char *szSN, unsigned long infp) {
unsigned char *eptr = enc;
int len = strlen(szSN), i, ii = 0;
unsigned long fp = infp;
unsigned char *uptr = (unsigned char *)szSN;
unsigned char *fpptr = (unsigned char *)&fp;

	for(i = 0; i < len; i++) {

		eptr[i] ^= 0xFF;

		eptr[i] = uptr[i] ^ fpptr[ii++];

		if(ii >3)
			ii = 0;

	}


}

/*------------------------------------------------------------------*/
/*-                                                                -*/
/*------------------------------------------------------------------*/
void EncryptCount(unsigned char *ecnt, unsigned long *cnt, unsigned long infp) {
unsigned char *eptr = ecnt;
int len = 4, i, ii = 0;
unsigned char *uptr = (unsigned char *)cnt;
unsigned long fp = infp;
unsigned char *fpptr = (unsigned char *)&fp;

	for(i = 0; i < len; i++) {

		eptr[i] = uptr[i] ^ fpptr[ii++];

		eptr[i] ^= 0xFF;

		if(ii >3)
			ii = 0;

	}


}


/*------------------------------------------------------------------*/
/*- Get all the secret stuff                                       -*/
/*------------------------------------------------------------------*/
BOOL CController4App::GetEncryptedRegistryEntries(char *szSerialNumber, int *JobCount, unsigned char *aux1, unsigned char *aux2) {
char szSN[32];
unsigned char *mk = (unsigned char *)&ENC_KEY, *tptr;
unsigned long fp = GetFingerPrint();
unsigned char esn[128];
char sn[128];
unsigned char tmp_decrypt[128];
int i, jobs;


	if(szSerialNumber != NULL) {
		if(!GetEncryptedSerialNumber(esn)) {
			strcpy(szSerialNumber, "???????1");
		}
		else {
			if(!GetPlainTextSerialNumber(sn)) {
				strcpy(szSerialNumber, "??????2");
			}
			else {
				EncryptSN(tmp_decrypt, sn, GetFingerPrint());
				strcpy(szSerialNumber, sn);
				for(i = 0; i < strlen(szSerialNumber); i++) {
					if(tmp_decrypt[i] != esn[i]) {
						strcpy(szSerialNumber, "????HK??");
						break;
					}
				}
			}
		}
	}


	if(JobCount != NULL) {
		if(!GetEncryptedJobCount(esn)) {
			*JobCount = 0xFFFFFFFF;
		}
		else {
			if(!GetJobCount((unsigned long *)&jobs)) {
				*JobCount = 0xEEEEEEEE;
			}
			else {
				EncryptCount(tmp_decrypt, (unsigned long *)&jobs, GetFingerPrint());
				*JobCount = (unsigned long)jobs;
				for(i = 0; i < strlen(szSerialNumber); i++) {
					if(tmp_decrypt[i] != esn[i]) {
						*JobCount = 0xDDDDDDDD;
						break;
					}
				}
			}
		}


	}
	if(aux1 != NULL) {

	}
	if(aux2 != NULL) {

	}

	return TRUE;

}

/*------------------------------------------------------------------*/
/*-                                                                -*/
/*------------------------------------------------------------------*/
BOOL WriteEncodedPrintCount(int newCount) {
char szKey[MAX_PATH];
unsigned char tmp_crypt[128];
HKEY key, hsubkey;
unsigned long ret, type = 0;
DWORD Disposition, cnt = (DWORD)newCount;
unsigned char *cptr = (unsigned char *)&cnt;
int error, i;

	/// no valid serial, no go
	if(strstr(theApp.GVars->szSN, "?") != NULL)
		return FALSE;

	sprintf(szKey, "%s", MASTER_REG_KEY_3);
	if(RegOpenKey (HKEY_LOCAL_MACHINE, szKey, &key) != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to open master key", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to open master key", "", "");
		return 0;
	}

	EncryptCount((unsigned char *)&tmp_crypt, &cnt, GetFingerPrint());

	ret = RegSetValueEx(	key,
							EODO_SUBKEY,
							0,
							REG_DWORD,
							(unsigned char *)tmp_crypt,
							4);

	if(ret != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to create EODO value", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to create EODO value", "", "");
		RegCloseKey(key);
		return 0;
	}

	for(i = 0; i < 4; i++) {
		cptr[i] ^= 0xAA;
	}

	ret = RegSetValueEx(	key,
							UODO_SUBKEY,
							0,
							REG_DWORD,
							(unsigned char *)&cnt,
							4);

	if(ret != ERROR_SUCCESS) {
		//MessageBox(GetFocus(), "Unable to create UODO value", APP_NAME,  MB_OK|MB_SYSTEMMODAL);
		theDlg->OKBox("Unable to create UODO value", "", "");
		RegCloseKey(key);
		return 0;
	}


	RegCloseKey(key);

	return TRUE;


}

