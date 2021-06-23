
///////// EVENT CODES


#define EVCD_JOB_START					0x00000001
#define EVCD_JOB_END					0x00000002
#define EVCD_CLEAN_START				0x00000003
#define EVCD_CLEAN_END					0x00000004
#define EVCD_CLEAN_NOZZLE_CHK			0x00000005
#define EVCD_DB_UPLOAD					0x00000006
#define EVCD_DB_SYNCH					0x00000007
#define EVCD_DB_MACHINE_START			0x00000008
#define EVCD_MACHINE_SHUTDOWN			0x00000009
#define EVCD_SOFTWARE_UPDATE			0x00000010
#define EVCD_ERROR						0x00000011
#define EVCD_CAP_STATUS					0x00000012
#define EVCD_SOFTWARE_UPDATE			0x00000013
#define EVCD_JOB_IMPORT					0x00000014
#define EVCD_JOB_DELETE					0x00000015

/*** record types   *****/

typedef struct _log_record {

	unsigned int event;
	unsigned char p1[128];
	unsigned char p2[128];
	unsigned char p3[128];
	unsigned char p4[128];
	unsigned int p5;
	unsigned int p6;
	unsigned int p7;
	unsigned int p8;
	unsigned int p9;
	unsigned int p10;
	unsigned int p11;
	unsigned int p12;
	unsigned int p136;
	unsigned int p14;
	unsigned int p15;
	unsigned int p16;
} log_record;


