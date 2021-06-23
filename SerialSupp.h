/*------------------------------------*/
// C-Horse Software, Inc.


typedef struct _COM_SETTINGS {
	int port;
	int baud;
	BOOL active;
} COM_SETTINGS;


////// Serial stuff
#define FPGA_BACKCHANNEL		0
#define DEBUGGER_BACKCHANNEL	1

	///// real setup
#define FPGA_BACKCHANNEL_PORT		1
#define DEBUGGER_BACKCHANNEL_PORT	2

#define FPGA_BACKCHANNEL_BAUD		9600
#define DEBUGGER_BACKCHANNEL_BAUD	9600


#define GET_SYSTEM_STATE        0x01
#define RESET_PRINTING_SYSTEM   0x02
#define GET_HEAD_TEMP           0x03
