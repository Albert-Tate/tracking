#include <stdio.h>
#include <stdlib.h>

#include <digilent/waveforms/dwf.h>

#include <unistd.h>
#include <sys/time.h>
#define Wait(ts) usleep((int)(1000000*ts))
