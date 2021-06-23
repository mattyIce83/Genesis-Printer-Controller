
#define ESC						0x1B


#define TOTAL_CHANNELS			4

#define CMD_UNITS				0
#define CMD_PRINT_DIRECTION		1
#define CMD_PAGE_MARGINS		2
#define CMD_PAGE_LENGTH			3
#define CMD_PITSPERPIXEL		4
#define CMD_SPOTSIZE			5
#define CMD_RESOLUTION			6
#define CMD_HORIZONTAL_POS		7
#define CMD_RASTER_DATA			8
#define CMD_VERTICAL_POS		9
#define CMD_START_GRAPHICS		10
#define CMD_JOB_NAME			11
#define CMD_JOB_COMMENT			12
#define CMD_EOJ					13
#define CMD_VER					14
#define CMD_JOBNUMBER			15
#define CMD_LAYERTYPE			16
#define CMD_BEGINCOLORPREVIEW	17
#define CMD_BEGINWHITEPREVIEW	18

#define TOTAL_COMMANDS			19

#define MAX_INCHES				17

#define LAYER_TP_WHITE						0x01
#define LAYER_TP_SINGLE_COLOR				0x02
#define LAYER_TP_SINGLE_COLOR_SINGLE_WAVE   0x07 

typedef void (WINAPI *PDEV_HANDLER) (char *, int);

#ifndef GS_STUFF_DEFINED
typedef struct unitscmd {

	unsigned char nL;
	unsigned char nH;
	unsigned char p;
	unsigned char v;
	unsigned char h;
	unsigned char mL;
	unsigned char mH;
	unsigned long m;
	unsigned long n;

} UNITSCMD;


typedef struct rastercmdi {
	unsigned char esc;
	unsigned char i;
	unsigned char r;
	unsigned char c;
	unsigned char b;
	unsigned char nL;
	unsigned char nH;
	unsigned char mL;
	unsigned char mH;

	unsigned long m;
	unsigned long n;

} RASTERCMDI;

typedef struct pagemargins {

	unsigned char t1;
	unsigned char t2;
	unsigned char t3;
	unsigned char t4;

	unsigned char l1;
	unsigned char l2;
	unsigned char l3;
	unsigned char l4;

	unsigned long t;
	unsigned long l;

} pagemargins;

typedef struct 	pixeldimensionscmd {

	unsigned char nL;
	unsigned char nH;

	unsigned char w1;
	unsigned char w2;
	unsigned char w3;
	unsigned char w4;

	unsigned char l1;
	unsigned char l2;
	unsigned char l3;
	unsigned char l4;

	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;

	unsigned long n;
	unsigned long w;
	unsigned long l;
	unsigned long b;


} pixeldimensionscmd;

typedef struct colormodecmd {

	unsigned char nL;
	unsigned char nH;
	unsigned long m;
	unsigned long n;

} colormodecmd;

typedef struct spotsizecmd {

	unsigned char m;
	unsigned char d;

	int index;

} spotsizecmd;

typedef struct resolutioncmd {

	unsigned char xL;
	unsigned char xH;

	unsigned char yL;
	unsigned char yH;

	unsigned long resX;
	unsigned long resY;

} resolutioncmd;

typedef struct jobnumbercmd {
	unsigned char n1;
	unsigned char n2;
	unsigned char n3;
	unsigned char n4;

	unsigned long n;

} jobnumbercmd;

#include "graphics_state.h"


typedef enum {
  searching,
  lookupCMD,
  inCMD,
  inRasterDataSearching,
  inRasterDataInRun,
  inRasterDataInLiteral,
  inPreviewCapture,
} State;

typedef struct {
	int printerType;
	int ValidCheck;
	int index;
	int ESCBindex;
	char ESCBuffer[128];
	int cmdLength;
	char ESCcmdBuffer[128];
	char last;
	State state;
	int inData;
	int lastType;
	int rasterCount;
	int rasterCompCount;
	BOOL firstBuff;
	PDEV_HANDLER cmdFunc;
} ESCP2Filter;

typedef struct _BELOSE_CMD {
	int ID;
	int length;
	int cmd_length;
	char command[20];
	PDEV_HANDLER handler;
} BELOSE_CMD;

typedef struct _graphics_state {

	int channels;
	int pixelWidth;
	int pixelHeight;
	int resX;
	int resY;
	int byteWidth;

	int xDiv;
	int yDiv;


	unsigned char *pix;
	unsigned char *tempraster;

} graphics_state;

#define GS_STUFF_DEFINED

#endif
