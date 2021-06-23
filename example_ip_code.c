/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples.
*       Copyright 1996 - 1998 Microsoft Corporation.
*       All rights reserved.
*       This source code is only intended as a supplement to
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the
*       Microsoft samples programs.
\******************************************************************************/


/*

Module Name: IPChangeExamplesrc.cpp


Abstract:

    This module illustrates how to programmatically change an IP address for
    a specific network adapter on your machine.  This program also demonstrates how
    to retrieve existing network adapter IP configuration information.


    IP configurations can be changed for a specific network adapter by using the
    AddIpAddress() and DeleteIpAddress() Ip Helper APIs.  These APIs require you to
    understand adapter index numbers and IP context numbers.  In Windows, every network
    adapter has a unique index ID and every IP address has a unique context ID.
    Adapter index IDs and IP context numbers can be retrieved using the GetAdaptersInfo()
    IP Helper API.  This program features a list option that displays current network
    adapter configuration information by showing all adapters index numbers and IP
    address context numbers associated with their corresponding network adaptors.


    IPChangeExample [ -l ] [ -a -n<index id> -i<ip address> -m<subnet mask> ]

                 [ -d -c<context id>]



    -l List adapter index IDs and IP Address context ID information

    -a Add IP Address option

    -d Delete IP Address option

    -i IP Address to specify with -a option

    -m Subnet Mask to specify with -a option

    -c IP context ID for an existing IP address

    -n Index ID of an existing network adapter

Original author: Jim Ohlund 21-Apr-98

Revision History:

*/

 
// Link to ws2_32.lib

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
// Link to Iphlpapi.lib
#include <iphlpapi.h>
 

void Usage(void) {

    printf("Usage:  IPChangeExample [ -l ] [ -a -n<index id> -i<ip address> -m<subnet mask> ] "

        "[ -d -c<context id>]\n\n"

        "\t -l List adapter index IDs and IP Address context ID information\n"

        "\t -a Add IP Address option\n"

        "\t -d Delete IP Address option\n"

        "\t -i IP Address to specify with -a option\n"

        "\t -m Subnet Mask to specify with -a option\n"

        "\t -c IP context ID for an existing IP address\n"

        "\t -n Index ID of an existing network adapter\n");

}

 

void main(int argc, char *argv[]) {

    WSADATA wsaData;
    int i;
    UINT j;
    ULONG NTEContext = 0;
    ULONG NTEInstance;
    IPAddr NewIP;
    IPAddr NewMask;
    DWORD Index;
    DWORD Context;
    CHAR NewIPStr[64];
    CHAR NewMaskStr[64];
 

    PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
    PIP_ADDR_STRING pAddrStr;

    DWORD AdapterInfoSize;
    DWORD Err;
    BOOL OptList = FALSE;
    BOOL OptAdd = FALSE;
    BOOL OptDel = FALSE;

    NewIPStr[0] = '\0';
    NewMaskStr[0] = '\0';
    Context = Index = -1;

    if(argc < 2) {
        Usage();
        exit(1);
    }

    for (i = 1; i < argc; i++) {

        if ((argv[i][0] == '-') || (argv[i][0] == '/')) {

            switch(tolower(argv[i][1])) {

                case 'l':
                    OptList = TRUE;
                    break;

                case 'a':
                    OptAdd = TRUE;
                    break;

                case 'c':
                    if (strlen(argv[i]) > 2)
                        Context = atoi(&argv[i][2]);
                    break;

                case 'd':
                    OptDel = TRUE;
                    break;

                case 'i':
                    if (strlen(argv[i]) > 2)
                        strcpy_s(NewIPStr, sizeof(NewIPStr), &argv[i][2]);
                    break;

                case 'm':
                    if (strlen(argv[i]) > 2)
				        strcpy_s(NewMaskStr, sizeof(NewMaskStr), &argv[i][2]);
                    break;

                case 'n':

                    if (strlen(argv[i]) > 2)
                        Index = atoi(&argv[i][2]);

                    break;

                default:

                    printf("default\n");
                    Usage();
                    return;

                }

        }

        else {

            printf("else\n");
            Usage();
            return;

        }

    }

 

    // Check options

    if ((OptAdd && (Index == -1 || NewIPStr[0] == '\0' || NewMaskStr[0] == '\0'))   || (OptDel && Context == -1))  {
        Usage();
        return;
    }


    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup() is OK!\n");
        return;
    }

    else
        printf("WSAStartup() is OK!\n");

 

    // Get sizing information about all adapters

    AdapterInfoSize = 0;

    if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0) {
        
		if (Err != ERROR_BUFFER_OVERFLOW) {
            printf("GetAdaptersInfo sizing failed with error %d\n", Err);
            return;
        }
    }

    // Allocate memory from sizing information

    if ((pAdapterInfo = (PIP_ADAPTER_INFO) GlobalAlloc(GPTR, AdapterInfoSize)) == NULL) {
        printf("Memory allocation error\n");
        return;

    }

    // Get actual adapter information

    if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0) {
        printf("GetAdaptersInfo failed with error %d\n", Err);
        return;
    }

    if (OptList)  {

        printf("MAC Address - Adapter\n"
            "Index     Context   Ip Address          Subnet Mask\n"
            "--------------------------------------------------------------\n");

		pAdapt = pAdapterInfo;

		while (pAdapt) {

			for (j=0; j<pAdapt->AddressLength; j++) {
                if (j == (pAdapt->AddressLength - 1))
                    printf("%.2X - ",(int)pAdapt->Address[j]);
                else
                    printf("%.2X-",(int)pAdapt->Address[j]);
            }

            printf("%s\n", pAdapt->Description);

            pAddrStr = &(pAdapt->IpAddressList);

            while(pAddrStr) {
                printf("%-10.d%-10.d%-20.20s%s\n", pAdapt->Index, pAddrStr->Context, pAddrStr->IpAddress.String, pAddrStr->IpMask.String);
                pAddrStr = pAddrStr->Next;
            }
            pAdapt = pAdapt->Next;
        }

    }

    if (OptAdd) {

        NewIP = inet_addr(NewIPStr);
        NewMask = inet_addr(NewMaskStr);

		if ((Err = AddIPAddress(NewIP, NewMask, Index, &NTEContext, &NTEInstance)) != 0) {
            printf("AddIPAddress failed with error %d, %d\n", NTEContext, Err);
            return;
        }
    }

    if (OptDel) {
        if ((Err = DeleteIPAddress(Context)) != 0)
            printf("DeleteIPAddress failed %d\n", Err);
    }

    if(WSACleanup() == 0)
         printf("WSACleanup() is pretty fine!\n");
    else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());

}
