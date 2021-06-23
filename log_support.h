#pragma once

#ifndef EVENT_CODES_DEFINED
#define EVENT_CODES_DEFINED


#define LOG_VERSION		2

/*** EVENT CODES ***/

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
//#define EVCD_SOFTWARE_UPDATE			0x00000013
#define EVCD_JOB_IMPORT					0x00000014
#define EVCD_JOB_DELETE					0x00000015
#define EVCD_JOB_SCAN_IN				0x00000016

/// Job  event codes

#define JOB_EVENT_START					0x00000001				
#define JOB_EVENT_END					0x00000002
#define JOB_EVENT_IMPORT				0x00000003
#define JOB_EVENT_SCAN_IN				0x00000004
#define JOB_EVENT_DELETE				0x00000005


#define TASK_GET_HOST_IP			0x0001
#define TASK_CONNECT				0x0002
#define TASK_U_4				0x0004
#define TASK_U_8				0x0008

#define TASK_UPDATE_JOBS			0x0010
#define TASK_UPDATE_2				0x0020
#define TASK_UPDATE_4				0x0040
#define TASK_UPDATE_8				0x0080

/*** RECORD STRUCTURE   *****/

typedef struct _log_record
{
	unsigned char log_version; /// current: 2
	unsigned int evnt;
	float imageAreaX;
	float imageAreaY;
	unsigned int sizeX;
	unsigned int sizeY;
	unsigned int resolutionX;
	unsigned int resolutionY;
	unsigned int layerType;
	unsigned int printMode;
	unsigned int inkUsageC;
	unsigned int inkUsageM;
	unsigned int inkUsageY;
	unsigned int inkUsageK;
	unsigned int inkUsageW;
	unsigned int jobNumber;
	unsigned int completionCode;
	unsigned long long startTime;
	unsigned long long endTime;
	unsigned char jobName[128];
} log_record;


//typedef struct _taskFlag // dont leave it to the compiler?
//{
//	unsigned short updateJobs : 1;
//} taskFlag;

#endif // #define EVENT_CODES_DEFINED

