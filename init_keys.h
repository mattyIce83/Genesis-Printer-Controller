#define INI_FILE		"MANHATTAN.INI"

#define TSTR 0 // STRING
#define TINT 1 // INT
#define TFLT 2 // FLOAT
#define TBOL 3 // BOOL
#define TSTU 4 // STRUCT WINDOWPLACEMENT
#define TSTV 5 // STRUCT LV_COLUMN
#define TCPX 6 // XY

#define MAXINITKEYS 	186

#ifdef INIKEYS_IS_EXTERN

	extern int *INIKeys[MAXINITKEYS][4];

#else

	int *INIKeys[MAXINITKEYS][4] = {
		(int *)"USB",				(int *)"HostName",					(int *)&G_Vars.HostUSBName,				(int *)TSTR,
		(int *)"USB",				(int *)"FPGAName",					(int *)&G_Vars.FPGAUSBName,				(int *)TSTR,
		(int *)"PATHS",				(int *)"Incoming",					(int *)&G_Vars.Incoming,				(int *)TSTR,
		(int *)"PATHS",				(int *)"Repository",				(int *)&G_Vars.Repository,				(int *)TSTR,
		(int *)"PATHS",				(int *)"Thumbnails",				(int *)&G_Vars.Thumbnails,				(int *)TSTR,
		(int *)"PATHS",				(int *)"FPGAImage",					(int *)&G_Vars.FPGAImage,				(int *)TSTR,
		(int *)"PATHS",				(int *)"ExternalFiles",				(int *)&G_Vars.ExternalFiles,			(int *)TSTR,
		(int *)"QUEUE",				(int *)"MaxJobs",					(int *)&G_Vars.MaxJobs,					(int *)TINT,
		(int *)"QUEUE",				(int *)"NextJobNumber",				(int *)&G_Vars.NextJobNumber,			(int *)TINT,
		(int *)"DISPLAY",			(int *)"PreviewFlags",				(int *)&G_Vars.PreviewFlags,			(int *)TINT,
		(int *)"DEBUG",				(int *)"ConfirmBands",				(int *)&G_Vars.ConfirmBands,			(int *)TINT,
		(int *)"DEBUG",				(int *)"HideDebugWindows",			(int *)&G_Vars.bHideDebugWindows,		(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"USE_ONEBPP",				(int *)&G_Vars.bUseOneBitPP,			(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"CustBiDiCor",				(int *)&G_Vars.CustBiDiCor,				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"AutoHD",					(int *)&G_Vars.bAutoHD,					(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"RapidRT",					(int *)&G_Vars.bRapidRT,				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"HeadTemp",					(int *)&G_Vars.HeadTemp[0],				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"HeadTemp2",					(int *)&G_Vars.HeadTemp[1],				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"FPGA_COM",					(int *)&G_Vars.FPGA_COM,				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"BACKCHANNEL_COM",			(int *)&G_Vars.Backchannel_COM,			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"POWER_DOWN_ON_EXIT",		(int *)&G_Vars.bShutDownOnExit,			(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_150",				(int *)&G_Vars.Advance[0],				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_300",				(int *)&G_Vars.Advance[1],				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_600",				(int *)&G_Vars.Advance[2],				(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_1200",			(int *)&G_Vars.Advance[3],				(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_150_GENMOD",		(int *)&G_Vars.AdvanceGM[0],			(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_300_GENMOD",		(int *)&G_Vars.AdvanceGM[1],			(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_600_GENMOD",		(int *)&G_Vars.AdvanceGM[2],			(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"Y_ADVANCE_1200_GENMOD",		(int *)&G_Vars.AdvanceGM[3],			(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"INTER_HEAD_SPACING_150",	(int *)&G_Vars.InterHeadOffset[0],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_HEAD_SPACING_300",	(int *)&G_Vars.InterHeadOffset[1],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_HEAD_SPACING_600",	(int *)&G_Vars.InterHeadOffset[2],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_HEAD_SPACING_1200",	(int *)&G_Vars.InterHeadOffset[3],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_150_A",	(int *)&G_Vars.InterNozzleSpace150[0],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_150_B",	(int *)&G_Vars.InterNozzleSpace150[1],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_150_C",	(int *)&G_Vars.InterNozzleSpace150[2],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_150_D",	(int *)&G_Vars.InterNozzleSpace150[3],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_300_A",	(int *)&G_Vars.InterNozzleSpace300[0],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_300_B",	(int *)&G_Vars.InterNozzleSpace300[1],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_300_C",	(int *)&G_Vars.InterNozzleSpace300[2],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_300_D",	(int *)&G_Vars.InterNozzleSpace300[3],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_600_A",	(int *)&G_Vars.InterNozzleSpace600[0],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_600_B",	(int *)&G_Vars.InterNozzleSpace600[1],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_600_C",	(int *)&G_Vars.InterNozzleSpace600[2],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_600_D",	(int *)&G_Vars.InterNozzleSpace600[3],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_1200_A",	(int *)&G_Vars.InterNozzleSpace1200[0],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_1200_B",	(int *)&G_Vars.InterNozzleSpace1200[1],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_1200_C",	(int *)&G_Vars.InterNozzleSpace1200[2],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"INTER_NOZZLE_SPACE_1200_D",	(int *)&G_Vars.InterNozzleSpace1200[3],	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"IGNORE_PLATEN_HEIGHT",		(int *)&G_Vars.bIgnorePlatenHeight,	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"HEAD_CAP_TIMEOUT",			(int *)&G_Vars.iHeadCapTimeout,		(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"PRINTER_TYPE",				(int *)&G_Vars.iPrinterType,		(int *)TINT,

		(int *)"PRINTERSETTINGS",   (int *)"AUTO_SPIT_INTERVAL",		(int *)&G_Vars.iPeriodicSpitInterval,	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"AUTO_SPIT_DURATION",		(int *)&G_Vars.iPeriodicSpitDuration,	(int *)TINT,

		(int *)"PRINTERSETTINGS",   (int *)"DEFAULT_PLATEN_HEIGHT",		(int *)&G_Vars.iDefaultPlatenHeight,	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"PLATEN_FEED_HEIGHT",		(int *)&G_Vars.iPlatenFeedHeight,		(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"LASER_TO_HEIGHT_OFFSET",	(int *)&G_Vars.iLaserToPrintHeightOffset,	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"LASER_TO_FEED_OFFSET",		(int *)&G_Vars.iLaserToFeedHeightOffset,	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"LASER_LOC",					(int *)&G_Vars.iLaserLoc,				(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"PLATEN_PARK_OFFSET",		(int *)&G_Vars.iPlatenParkOffset,		(int *)TINT,

		(int *)"PRINTERSETTINGS",   (int *)"DISABLE_INK_BAG_AUTOFF",	(int *)&G_Vars.bInkBagAutoOffDisabled,	(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"CENTERING_OFFSET",			(int *)&G_Vars.iCenteringOffset,		(int *)TINT,
		(int *)"PRINTERSETTINGS",   (int *)"BAD_TEMP_FAILSAFE",			(int *)&G_Vars.bBadTempFailsafe,		(int *)TINT,

		(int *)"PRINTERSETTINGS",   (int *)"MAX_PLATEN_STEPPER_THROW",	(int *)&G_Vars.iMaxStepperThrow,		(int *)TINT,


		(int *)"SYSTEMSETTINGS",    (int *)"UPDATE_DRIVE",				(int *)&G_Vars.szUpdateDrive,			(int *)TSTR,
		(int *)"SYSTEMSETTINGS",    (int *)"JOBS_DRIVE",				(int *)&G_Vars.szJobsDrive,				(int *)TSTR,
		(int *)"SYSTEMSETTINGS",    (int *)"PENDING_UPDATE",			(int *)&G_Vars.bPendingUpdate,			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"DUAL_CMYK",					(int *)&G_Vars.bDualCMYK,				(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SUPRESS_AMB_TEMP",			(int *)&G_Vars.bSuppressAmbTemp,		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"LOG_UPDATE_INTERVAL",		(int *)&G_Vars.LogUploadInterval,		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"NO_ACTIONS_BEFORE_FPGA",	(int *)&G_Vars.bNoUIBeforeFPGAReady,	(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"DELETE_JOB_AFTER_PRINT",	(int *)&G_Vars.bDeleteAfterPrinting,	(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"IGNORE_OBSTRUCTIONS",		(int *)&G_Vars.bIgnoreObstruction,		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"HEAD_HEATER_TIMEOUT",		(int *)&G_Vars.iHeaterTimeout,		(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_FULL_0",			(int *)&G_Vars.inkADCFull[0],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_FULL_1",			(int *)&G_Vars.inkADCFull[1],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_FULL_2",			(int *)&G_Vars.inkADCFull[2],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_FULL_3",			(int *)&G_Vars.inkADCFull[3],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_FULL_4",			(int *)&G_Vars.inkADCFull[4],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_FULL_5",			(int *)&G_Vars.inkADCFull[5],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_EMPTY_0",			(int *)&G_Vars.inkADCEmpty[0],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_EMPTY_1",			(int *)&G_Vars.inkADCEmpty[1],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_EMPTY_2",			(int *)&G_Vars.inkADCEmpty[2],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_EMPTY_3",			(int *)&G_Vars.inkADCEmpty[3],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_EMPTY_4",			(int *)&G_Vars.inkADCEmpty[4],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"INK_ADC_EMPTY_5",			(int *)&G_Vars.inkADCEmpty[5],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"INK_ALARM_PERCENT",			(int *)&G_Vars.fInkAlarmPercent,		(int *)TFLT,

		(int *)"SYSTEMSETTINGS",    (int *)"FULL_CAP_VAC_PSI",			(int *)&G_Vars.FullCapVacPsi,			(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"FULL_CAP_VAC_ADC",			(int *)&G_Vars.FullCapVacADC,			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"CAP_VAC_ATMOSPHERE",		(int *)&G_Vars.CapVacAtmosphere,		(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"DISABLE_INK_ADC",			(int *)&G_Vars.bDisableInkADC,			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"IGNORE_INK_OUT",			(int *)&G_Vars.bInkOutDisableIgnore,	(int *)TINT,
	
		(int *)"SYSTEMSETTINGS",    (int *)"CAP_FILL_TIMER",			(int *)&G_Vars.iCapFillTimer,			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"PRINT_ON_JOB_ARRIVAL",		(int *)&G_Vars.bPrintOnJobArrival,		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"JOB_ARRIVAL_TIMEOUT",		(int *)&G_Vars.iJobWaitTimeout,		(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SELECTED_PLATEN",			(int *)&G_Vars.iSelectedPlaten,		(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_0_NAME",				(int *)&G_Vars.szPlatenName[0],		(int *)TSTR,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_1_NAME",				(int *)&G_Vars.szPlatenName[1],		(int *)TSTR,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_2_NAME",				(int *)&G_Vars.szPlatenName[2],		(int *)TSTR,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_3_NAME",				(int *)&G_Vars.szPlatenName[3],		(int *)TSTR,

		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_0_WIDTH",				(int *)&G_Vars.PlatenWidth[0],		(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_1_WIDTH",				(int *)&G_Vars.PlatenWidth[1],		(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_2_WIDTH",				(int *)&G_Vars.PlatenWidth[2],		(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_3_WIDTH",				(int *)&G_Vars.PlatenWidth[3],		(int *)TFLT,

		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_0_HEIGHT",				(int *)&G_Vars.PlatenHeight[0],		(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_1_HEIGHT",				(int *)&G_Vars.PlatenHeight[1],		(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_2_HEIGHT",				(int *)&G_Vars.PlatenHeight[2],		(int *)TFLT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_3_HEIGHT",				(int *)&G_Vars.PlatenHeight[3],		(int *)TFLT,

		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_0_OFFSET",				(int *)&G_Vars.PlatenOffset[0],		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_1_OFFSET",				(int *)&G_Vars.PlatenOffset[1],		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_2_OFFSET",				(int *)&G_Vars.PlatenOffset[2],		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_3_OFFSET",				(int *)&G_Vars.PlatenOffset[3],		(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_0_DELAY",				(int *)&G_Vars.PlatenDelay[0],		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_1_DELAY",				(int *)&G_Vars.PlatenDelay[1],		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_2_DELAY",				(int *)&G_Vars.PlatenDelay[2],		(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"PLATEN_3_DELAY",				(int *)&G_Vars.PlatenDelay[3],		(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"IGNORE_UNDERRUNS",				(int *)&G_Vars.bIgnoreUnderruns,	(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_0",					(int *)&G_Vars.X_Speed[0],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_0",					(int *)&G_Vars.X_Accel[0],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_1",					(int *)&G_Vars.X_Speed[1],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_1",					(int *)&G_Vars.X_Accel[1],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_2",					(int *)&G_Vars.X_Speed[2],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_2",					(int *)&G_Vars.X_Accel[2],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_3",					(int *)&G_Vars.X_Speed[3],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_3",					(int *)&G_Vars.X_Accel[3],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_4",					(int *)&G_Vars.X_Speed[4],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_4",					(int *)&G_Vars.X_Accel[4],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_5",					(int *)&G_Vars.X_Speed[5],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_5",					(int *)&G_Vars.X_Accel[5],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_6",					(int *)&G_Vars.X_Speed[6],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_6",					(int *)&G_Vars.X_Accel[6],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_7",					(int *)&G_Vars.X_Speed[7],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_7",					(int *)&G_Vars.X_Accel[7],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_8",					(int *)&G_Vars.X_Speed[8],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_8",					(int *)&G_Vars.X_Accel[8],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_SPEED_9",					(int *)&G_Vars.X_Speed[9],			(int *)TINT,
		(int *)"SYSTEMSETTINGS",    (int *)"SET_X_ACCEL_9",					(int *)&G_Vars.X_Accel[9],			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"HIDE_FORCE_BUTTON",				(int *)&G_Vars.bHideForceButton,	(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"SUPERVISOR_PW",				(int *)&G_Vars.szPassword,				(int *)TSTR,

		(int *)"SYSTEMSETTINGS",    (int *)"SPECIAL_FLAGS",				(int *)&G_Vars.iSpecialFlags,			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"WHITE_DRIP_TIME",			(int *)&G_Vars.iWhiteDripTime,			(int *)TINT,

		(int *)"SYSTEMSETTINGS",    (int *)"WHITE_RECIRC_TIME",			(int *)&G_Vars.iWhiteRecircTime,		(int *)TINT,

		(int *)"PATHS",				(int *)"FPGA_MAIN",					(int *)&G_Vars.szFPGAMain,				(int *)TSTR,
		(int *)"PATHS",				(int *)"FPGA_AUX",					(int *)&G_Vars.szFPGAAux,				(int *)TSTR,
		(int *)"PATHS",				(int *)"CONTROLLER",				(int *)&G_Vars.szController,			(int *)TSTR,

		(int *)"PRINTERSETTINGS",	(int *)"PLATEN_HEIGH_OFFSET_1",		(int *)&G_Vars.PlatenHeightOffset[0],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"PLATEN_HEIGH_OFFSET_2",		(int *)&G_Vars.PlatenHeightOffset[1],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"PLATEN_HEIGH_OFFSET_3",		(int *)&G_Vars.PlatenHeightOffset[2],	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"PLATEN_HEIGH_OFFSET_4",		(int *)&G_Vars.PlatenHeightOffset[3],	(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"CURRENT_HEIGH_OFFSET",		(int *)&G_Vars.CurrentHeightOffset,		(int *)TINT,

		(int *)"DEBUG",				(int *)"SuppressHeadA",				(int *)&G_Vars.SuppressHeadA,			(int *)TINT,
		(int *)"DEBUG",				(int *)"SuppressHeadB",				(int *)&G_Vars.SuppressHeadB,			(int *)TINT,
		(int *)"DEBUG",				(int *)"SuppressAwayBands",			(int *)&G_Vars.SuppressAwayBands,		(int *)TINT,
		(int *)"DEBUG",				(int *)"SuppressHomeBands",			(int *)&G_Vars.SuppressHomeBands,		(int *)TINT,
		(int *)"DEBUG",				(int *)"NoYShuffle",				(int *)&G_Vars.bNoYShuffle,				(int *)TINT,
		(int *)"DEBUG",				(int *)"DontMirrorTestBands",		(int *)&G_Vars.bDontMirrorTestBands,	(int *)TINT,
		(int *)"DEBUG",				(int *)"DebugInternet",				(int *)&G_Vars.bDebugNetwork,			(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"AUTO_SEND_COLOR_LAYER",		(int *)&G_Vars.bAutoSendColorLayer,		(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"PlatenRollCorrection",		(int *)&G_Vars.PlatenRollCorrection,	(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"PlatenPitchCorrection",		(int *)&G_Vars.PlatenPitchCorrection,	(int *)TINT,
	
		(int *)"PRINTERSETTINGS",	(int *)"Enable600X300RR",			(int *)&G_Vars.bEnable600x300RR,		(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"LiftServoCorrFL",			(int *)&G_Vars.LiftServoCorr[0],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"LiftServoCorrFR",			(int *)&G_Vars.LiftServoCorr[1],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"LiftServoCorrRL",			(int *)&G_Vars.LiftServoCorr[2],		(int *)TINT,
		(int *)"PRINTERSETTINGS",	(int *)"LiftServoCorrRR",			(int *)&G_Vars.LiftServoCorr[3],		(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"CurrentJobNumber",			(int *)&G_Vars.CurrentJobNumber,		(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"UseMultipleCorArrays",		(int *)&G_Vars.UseMultipleCorArrays,	(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"CurrentCount",				(int *)&G_Vars.CurrentCount,			(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"ColorJobsBeforeClean",		(int *)&G_Vars.iColorJobsBeforeClean,	(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"WhiteJobsBeforeClean",		(int *)&G_Vars.iWhiteJobsBeforeClean,	(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"JobsBeforeDry",				(int *)&G_Vars.JobsBeforeDry,			(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"LayerDelaySec",				(int *)&G_Vars.LayerDelaySec,			(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"ColorQuickReturn",			(int *)&G_Vars.bEnableColorQuickReturn,	(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"OneButtonOperation",		(int *)&G_Vars.bOneButtonOperation,		(int *)TINT,

		(int *)"PRINTERSETTINGS",	(int *)"EnableDuplicateHandling",	(int *)&G_Vars.bEnableDuplicateHandling,(int *)TINT,

		(int *)"NETWORK",			(int *)"DisplayedAdaptor",			(int *)&G_Vars.DisplayedAdaptor,		(int *)TINT,

		(int *)"NETWORK",			(int *)"IPAddress",					(int *)&G_Vars.IPAddy,					(int *)TINT,

		(int *)"NETWORK",			(int *)"DefaultGateway",			(int *)&G_Vars.Gateway,					(int *)TINT,

		(int *)"NETWORK",			(int *)"Subnet",					(int *)&G_Vars.Subnet,					(int *)TINT,

		(int *)"NETWORK",			(int *)"AutoIP",					(int *)&G_Vars.AutoIP,					(int *)TINT,

		(int *)"NETWORK",			(int *)"LogURL",					(int *)&G_Vars.szLogURL,				(int *)TSTR,

		(int *)"NETWORK",			(int *)"LogPort",					(int *)&G_Vars.LogPort,				 	(int *)TINT,

		(int *)"NETWORK",			(int *)"SerialNumber",				(int *)&G_Vars.szPrinterSerialNumber, 	(int *)TSTR,

		(int *)"NETWORK",			(int *)"EncryptionKey",				(int *)&G_Vars.EncryptionKey, 			(int *)TSTR,

		(int *)"NETWORK",			(int *)"SupressLogging",			(int *)&G_Vars.bNoLogging,				(int *)TINT,

		(int *)"NETWORK",			(int *)"FailSafeIP",				(int *)&G_Vars.szFailSafeIP,			(int *)TSTR
	
	};

#endif