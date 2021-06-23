#define PVMODE_INKCOUNT		0
#define PVMODE_PREVIEW		1

#define PRINTER_TYPE_UNKNOWN		-1
#define PRINTER_TYPE_PRO_SERIES		0
#define PRINTER_TYPE_PHOTO_SERIES	1

#define ESC						0x1B

#define CMD_GRAPHIC_MODE		0
#define CMD_UNITS				1
#define CMD_PRINT_DIRECTION		2
#define CMD_BOT_MARGIN			3
#define CMD_PAGE_MARGINS		4
#define CMD_PAGE_LENGTH			5
#define CMD_COLORMODE			6
#define CMD_MICROWEAVE			7
#define CMD_SPOTSIZE			8
#define CMD_RESOLUTION			9
#define CMD_HORIZONTAL_POS		10
#define CMD_RASTER_DATA			11
#define CMD_VERTICAL_POS		12

#define TOTAL_COMMANDS			13

typedef void (WINAPI *PDEV_HANDLER) (char *, int, void *theDlg);

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

	unsigned char nL;
	unsigned char nH;

	unsigned char t1;
	unsigned char t2;
	unsigned char t3;
	unsigned char t4;

	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;

	unsigned long n;
	unsigned long t;
	unsigned long b;

} pagemargins;

typedef struct 	paperdimensionscmd {

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

	unsigned long n;
	unsigned long w;
	unsigned long l;
} paperdimensionscmd;

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

	unsigned char nL;
	unsigned char nH;

	unsigned char rL;
	unsigned char rH;

	unsigned char v;
	unsigned char h;

	unsigned long resX;
	unsigned long resY;

} resolutioncmd;
typedef struct {

	int Channels;
	int ColorMode;
	int ResolutionX;
	int ResolutionY;
	int PageUnitsX;
	int PageUnitsY;
	int PrintDirection;
	int byteWidth;
	unsigned char Microweve;
	unsigned char HorizontalPosM1;
	unsigned char HorizontalPosM2;
	unsigned char HorizontalPosM3;
	unsigned char HorizontalPosM4;
	unsigned char VerticalPosL;
	unsigned char VerticalPosH;
	unsigned long HorizontalPos;
	unsigned long VerticalPos;
	unsigned char *bits[8];
	unsigned char *compressedRaster;
	int cmprasindex;
	int CurrentChannel;
	BOOL inited;
	BOOL RasterReady;
	float InchUnitsX;
	float InchUnitsY;
	int XOffset;
	int Bottom;

} displayGraphicState;

typedef enum {
  searching,
  lookupCMD,
  inCMD,
  inRasterDataSearching,
  inRasterDataInRun,
  inRasterDataInLiteral,
} State;

typedef struct {
	int printerType;
	int ValidCheck;
	int index;
	int ESCBindex;
	char ESCBuffer[30];
	int cmdLength;
	char ESCcmdBuffer[30];
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
