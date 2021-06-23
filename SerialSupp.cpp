//  microweave.cpp
//  microweave related code

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "parser.h"
#include "HEAD_structs.h"
#include "SerialPort.h"
#include "SerialSupp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CController4Dlg1 *theDlg;

COM_SETTINGS com_settings[2] = {FPGA_BACKCHANNEL_PORT, FPGA_BACKCHANNEL_BAUD, 0,
								DEBUGGER_BACKCHANNEL_PORT, DEBUGGER_BACKCHANNEL_BAUD, 0};

CSerialPort UART[2];

/*-------------------------------------------------------------------------*/
/*-                                                                       -*/
/*-------------------------------------------------------------------------*/
void OpenUtilityCOMs() {
char portstring[10], msg[128];
int i;
BOOL causeredraw = FALSE;

	// OPEN ALL PORTS

	for(i = 0; i < 2; i++) {

		com_settings[i].active = FALSE;

		if(com_settings[i].port == 0)
			continue;

		sprintf(portstring, "COM%d:", com_settings[i].port);

		if(!UART[i].OpenPort(portstring)) {
			wsprintf(msg, "Could not open COM%d", com_settings[i].port);
			//theDlg->OKBox("** ERROR **", msg, NULL);
			causeredraw = TRUE;
			com_settings[i].active = FALSE;
			continue;
		}
		else {

			if(!(UART[i].ConfigureSerialPort(com_settings[i].port, (DWORD)com_settings[i].baud,(BYTE)8,(DWORD)0,(BYTE)NOPARITY , (BYTE)ONESTOPBIT ))) {
				UART[i].ClosePort();
				wsprintf(msg, "COM %d NOT configured\r\n", com_settings[i].port);
				theDlg->OKBox("** ERROR **", msg, NULL);
				causeredraw = TRUE;
				continue;
			} else if(!(UART[i].SetCommunicationTimeouts(0,100,0,0,0))) {
				UART[i].ClosePort();
				wsprintf(msg, "COM %d failed to set timeouts\r\n", com_settings[i].port);
				theDlg->OKBox("** ERROR **", msg, NULL);
				causeredraw = TRUE;
				continue;
			}
			else {
				com_settings[i].active = TRUE;
			}
		}

	}

}

/*-------------------------------------------------------------------------*/
/*-                                                                       -*/
/*-------------------------------------------------------------------------*/
BOOL GetHeadTemps() {
unsigned char cmd_buff[4], rsp_buff[6];
int timeout = 0, i = 0;

	

	if(!com_settings[FPGA_BACKCHANNEL].active)
		return FALSE;

	cmd_buff[0] = 0xAA;
	cmd_buff[1] = GET_HEAD_TEMP;
	cmd_buff[2] = 1;
	cmd_buff[3] = 0;

	UART[FPGA_BACKCHANNEL].WriteString(cmd_buff, 3);

	while(timeout < 250)  {
		if(UART[FPGA_BACKCHANNEL].ReadByte(rsp_buff[i])) {
		    i++;
			if(i == 3) break;
		}
		Sleep(1);
		timeout++;
	}
	if(timeout < 250 && rsp_buff[0] == 0xAA) {
		theApp.GVars->CurHeadTemp[0] = (int)rsp_buff[2];
		theApp.GVars->CurHeadTemp[0] = (int)rsp_buff[3];
		return TRUE;
	}
	return FALSE;
}