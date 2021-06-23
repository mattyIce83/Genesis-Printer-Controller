//  Job_IO.cpp
//

#include "stdafx.h"
#include "Controller4.h"
#include "NumericEntry.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CController4App theApp;
extern CController4Dlg1 *theDlg;



/*-----------------------------------------------------------------------------*/
/*-                                                                           -*/
/*-----------------------------------------------------------------------------*/
BOOL UnlockAdvanced() {
CNumericEntry dlg;

	dlg.iNumberOfCharsExpected = strlen(theApp.GVars->szPassword);
	dlg.bIsPassword = TRUE;

	sprintf(dlg.szTitle, "Supervisor Password\r\nRequired (%d-digit number)", dlg.iNumberOfCharsExpected);

	if(dlg.DoModal() == 1) {

		//if(strcmpi(dlg.szCode, "062264")) {
		if(strcmpi(dlg.szCode, theApp.GVars->szPassword)) {

			theDlg->OKBox("Invalid", "Password", NULL);

			return FALSE;
		}
		return TRUE;

	}

	return FALSE;

}

/*-----------------------------------------------------------------------------*/
/*-  See if ok...                                                             -*/
/*-----------------------------------------------------------------------------*/
BOOL CheckAdvanced() {

	if(!theApp.GVars->bAdvancedFeaturesUnlocked) {
		theApp.GVars->bAdvancedFeaturesUnlocked = UnlockAdvanced();
	}


	return(theApp.GVars->bAdvancedFeaturesUnlocked);

}
