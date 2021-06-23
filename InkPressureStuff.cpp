//

#include "stdafx.h"
#include "Controller4.h"
#include "OKBox.h"
#include "Controller4Dlg1.h"
#include "Controller4Dlg2.h"
#include "Controller4Dlg3.h"
#include "Controller4Dlg4.h"
#include "Controller4Dlg5.h"
#include "DevelopmentTeam.h"
#include "Printing.h"
#include "afxdialogex.h"
#include "parser.h"
#include "PleaseWaitInit.h"

#include "ftd2xx.h"
#include "ftdisupp.h"

#include "HEAD_structs.h"

extern CController4Dlg1 *theDlg;
extern CController4App theApp;

/*
void CController4App::CalculateInkFullVars() {
int i, range, level, toticks;
CStatic *ctrl;

	for(i = 0; i < 5; i++) {

		switch(i) {
			case 0:
				ctrl = &theDlg->m_BagMarker1;
				break;
			case 1:
				ctrl = &theDlg->m_BagMarker2;
				break;
			case 2:
				ctrl = &theDlg->m_BagMarker3;
				break;
			case 3:
				ctrl = &theDlg->m_BagMarker4;
				break;
			case 4:
				ctrl = &theDlg->m_BagMarker5;
				break;
		}

		if(theApp.GVars->inkADC[i] == 0) {
			theApp.GVars->inkFullPercent[i] = 0.0;
			theDlg->InkMarkersDisplay[i].CopyRect(theDlg->InkMarkersOriginal[i]);
		}
		else {
 			range = theApp.GVars->inkADCFull[i] - theApp.GVars->inkADCEmpty[i];
			level = theApp.GVars->inkADC[i] - theApp.GVars->inkADCEmpty[i];
			theApp.GVars->inkFullPercent[i] = (float)level / (float)range;

		}

		theDlg->InkMarkersDisplay[i].CopyRect(theDlg->InkMarkersOriginal[i]);

		toticks = (theDlg->InkMarkersOriginal[i].bottom - theDlg->InkMarkersOriginal[i].top) * theApp.GVars->inkFullPercent[i];

		if(toticks > (theDlg->InkMarkersDisplay[i].bottom - theDlg->InkMarkersDisplay[i].top))
			toticks = theDlg->InkMarkersDisplay[i].bottom - theDlg->InkMarkersDisplay[i].top;

		theDlg->InkMarkersDisplay[i].top = theDlg->InkMarkersDisplay[i].bottom - toticks;
	
		if(theApp.GVars->inkADC[i] != 0) {
			theApp.GVars->InkAlarm[i] = (theApp.GVars->inkFullPercent[i] < (theApp.GVars->fInkAlarmPercent / 100.00));
		}

	}


}
*/

#define TWO_KPA		0.290075

void CController4App::CalculateInkFullVars() {
int i, units[6], range[6], boxheight[6];
float percentfull[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
CStatic *ctrl[6] = {&theDlg->m_BagMarker1, &theDlg->m_BagMarker1, &theDlg->m_BagMarker2, &theDlg->m_BagMarker3, &theDlg->m_BagMarker4, &theDlg->m_BagMarker5};


	for(i = 0; i < 6; i++) {

		theApp.GVars->inkFullPercent[i] = 0;

		if(theApp.GVars->inkADC[i] != 0) {

			if(theApp.GVars->inkADC[i] > theApp.GVars->inkADCFull[i])
				theApp.GVars->inkADC[i] = theApp.GVars->inkADCFull[i];

			if(theApp.GVars->inkADC[i] < theApp.GVars->inkADCEmpty[i])
				theApp.GVars->inkADC[i] = theApp.GVars->inkADCEmpty[i];

			theDlg->InkMarkersDisplay[i].CopyRect(theDlg->InkMarkersOriginal[i]);

			units[i] = theApp.GVars->inkADC[i] - theApp.GVars->inkADCEmpty[i];
			
			if(units[i] < 0)
				units[i] = 0;

			else {

				range[i] = theApp.GVars->inkADCFull[i] - theApp.GVars->inkADCEmpty[i];

				if(units[i] > range[i])
					units[i] = range[i];

				theApp.GVars->inkFullPercent[i] = percentfull[i] =  ((float)units[i] / (float)range[i]);

				theApp.GVars->InkAlarm[i]  = (percentfull[i] < (theApp.GVars->fInkAlarmPercent / 100.00));

				switch(i) {
					case 0:
						theApp.GVars->Valves.fMBagPressure = percentfull[i] * TWO_KPA;
						break;
					case 1:
						theApp.GVars->Valves.fYBagPressure = percentfull[i] * TWO_KPA;
						break;
					case 2:
						theApp.GVars->Valves.fMBagPressure = percentfull[i] * TWO_KPA;
						break;
					case 3:
						theApp.GVars->Valves.fCBagPressure = percentfull[i] * TWO_KPA;
						break;
					case 4:
						theApp.GVars->Valves.fKBagPressure = percentfull[i] * TWO_KPA;
						break;
					case 5:
						theApp.GVars->Valves.fWBagPressure = percentfull[i] * TWO_KPA;
						break;
				}

			}

		}
		else {
			theApp.GVars->InkAlarm[i] = TRUE;
		}

	}


}



float CController4App::CalculateCapPressure(int ADC) {
//int range = VAC_ATMOSPHERE_ADC - FULL_VAC_ADC;
int range = theApp.GVars->CapVacAtmosphere - theApp.GVars->FullCapVacADC;
int val, level;
float psi = 5.0;

	if(ADC == 0) {
		//psi = FULL_VAC_PSI;
		psi = theApp.GVars->FullCapVacPsi;
	}
	else {
		//range = VAC_ATMOSPHERE_ADC - FULL_VAC_ADC;
		
		//level = ADC - VAC_ATMOSPHERE_ADC;

		level = ADC - theApp.GVars->CapVacAtmosphere;
		psi = (float)level / (float)range;
		if(psi < 0.0) {
			psi = abs(psi);
		}
		else {
			psi = 0.0 - psi;
		}
		//psi *= FULL_VAC_PSI;
		psi *= theApp.GVars->FullCapVacPsi;

	}

	return psi;

}
