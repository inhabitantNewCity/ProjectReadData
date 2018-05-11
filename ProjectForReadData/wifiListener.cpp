#ifndef UNICODE
#define UNICODE
#endif


#include "stdafx.h"
#include "wifiListener.h"

#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")


WIFIPoint* getWIFIPoints(int* size){

HANDLE hClient = NULL;
DWORD dwMaxClient = 2;       
DWORD dwCurVersion = 0;
DWORD dwResult = 0;
DWORD dwRetVal = 0;
int iRet = 0;

WCHAR GuidString[39] = { 0 };

unsigned int i, j, k;

/* variables used for WlanEnumInterfaces  */

PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
PWLAN_INTERFACE_INFO pIfInfo = NULL;

PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

int iRSSI = 0;

WIFIPoint* result;

dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
if (dwResult != ERROR_SUCCESS) {
	wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
	return NULL;
	// You can use FormatMessage here to find out why the function failed
}

dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
if (dwResult != ERROR_SUCCESS) {
	wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
	return NULL;
	// You can use FormatMessage here to find out why the function failed
}
else {
	wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
	wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
	
	for (i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
		pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
		wprintf(L"  Interface Index[%u]:\t %lu\n", i, i);
		iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString,
			sizeof(GuidString) / sizeof(*GuidString));

		if (iRet == 0)
			wprintf(L"StringFromGUID2 failed\n");
		else {
			wprintf(L"  InterfaceGUID[%d]: %ws\n", i, GuidString);
		}
		dwResult = WlanGetAvailableNetworkList(hClient,
			&pIfInfo->InterfaceGuid,
			0,
			NULL,
			&pBssList);

		if (dwResult != ERROR_SUCCESS) {
			wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n",
				dwResult);
			dwRetVal = 1;
			// You can use FormatMessage to find out why the function failed
		}
		else {
			
			result = new WIFIPoint[(int)pIfList->dwNumberOfItems];
			*size = (int)pIfList->dwNumberOfItems;

			for (j = 0; j < pBssList->dwNumberOfItems; j++) {
				pBssEntry =
					(WLAN_AVAILABLE_NETWORK *)& pBssList->Network[j];
				
				if (pBssEntry->dot11Ssid.uSSIDLength == 0)
					wprintf(L"\n");
				else {
					result[i].ssid = new int[pBssEntry->dot11Ssid.uSSIDLength];
					result[i].size = 2;
					for (k = 0; k <  pBssEntry->dot11Ssid.uSSIDLength ; k++) {
						result[i].ssid[k] = (int)pBssEntry->dot11Ssid.ucSSID[k];
					}
				}


				if (pBssEntry->wlanSignalQuality == 0)
					iRSSI = -100;
				else if (pBssEntry->wlanSignalQuality == 100)
					iRSSI = -50;
				else
					iRSSI = -100 + (pBssEntry->wlanSignalQuality / 2);
				result[i].level = pBssEntry->wlanSignalQuality;
				wprintf(L"  Signal Quality[%u]:\t %u (RSSI: %i dBm)\n", j,
					pBssEntry->wlanSignalQuality, iRSSI);
				

			}
		}
	}

}
if (pBssList != NULL) {
	WlanFreeMemory(pBssList);
	pBssList = NULL;
}

if (pIfList != NULL) {
	WlanFreeMemory(pIfList);
	pIfList = NULL;
}
return result;
}