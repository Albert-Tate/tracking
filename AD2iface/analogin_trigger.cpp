//g++ analogin_trigger.cpp -ldwf
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "sample.h"

void exit_handler( int s) {
    printf("Exiting...\n");	
    FDwfDeviceCloseAll();
    exit(0);
}

int main(int carg, char **szarg){
    HDWF hdwf;
    STS sts;
    int cSamples = 8192;
    double *vinSamples = new double[cSamples];
    double *voutSamples = new double[cSamples];
    char szError[512] = {0};
    FILE* vin = fopen("vin.txt", "a");
    FILE* vout = fopen("vout.txt", "a");


    //Catch ctrl-c events
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);



    printf("Open automatically the first available device\n");
    if(!FDwfDeviceOpen(-1, &hdwf)) {
        FDwfGetLastErrorMsg(szError);
        printf("Device open failed\n\t%s", szError);
        return 0;
    }

    //500k Fs
    //8192 samples =~ 16ms
    FDwfAnalogInFrequencySet(hdwf, 500000.0);
    FDwfAnalogInBufferSizeSet(hdwf, cSamples);
    //vin
    FDwfAnalogInChannelEnableSet(hdwf, 0, true);
    FDwfAnalogInChannelRangeSet(hdwf, 0, 5.0);
    //vout
    FDwfAnalogInChannelEnableSet(hdwf, 1, true);
    FDwfAnalogInChannelRangeSet(hdwf, 1, 5.0);

    // set up trigger
    // disable auto trigger
    FDwfAnalogInTriggerAutoTimeoutSet(hdwf, 0);
 
    FDwfAnalogInTriggerSourceSet(hdwf, trigsrcExternal1); //T1
    FDwfAnalogInTriggerTypeSet(hdwf, trigtypeEdge);
    // first channel
    FDwfAnalogInTriggerChannelSet(hdwf, 0);
    FDwfAnalogInTriggerLevelSet(hdwf, 0.5);
    FDwfAnalogInTriggerConditionSet(hdwf, trigcondRisingPositive);

    // wait at least 2 seconds with Analog Discovery for the offset to stabilize, before the first reading after device open or offset/range change
    Wait(2);

    printf("Starting repeated acquisitions:\n");
    for(int iTrigger = 0; iTrigger < 100; iTrigger++){
        // begin acquisition
        FDwfAnalogInConfigure(hdwf, false, true);

        while(true){
            FDwfAnalogInStatus(hdwf, true, &sts);
            if(sts == DwfStateDone) {
                break;
            }
            Wait(0.001);
        }
        FDwfAnalogInStatusData(hdwf, 0, vinSamples, cSamples);
	FDwfAnalogInStatusData(hdwf, 0, voutSamples, cSamples);
	printf("Triggered...\n");
        for(int i = 0; i < cSamples; i++){
	    if(i < cSamples-1) {
	    	fprintf(vin, "%f, ", vinSamples[i]);
	    	fprintf(vout, "%f, ", voutSamples[i]);
	    } else {
		fprintf(vin, "%f\n", vinSamples[i]);
	    	fprintf(vout, "%f\n", voutSamples[i]);
	    }
        }
	fflush(vin);
	fflush(vout);
    }

    FDwfDeviceCloseAll();
}
