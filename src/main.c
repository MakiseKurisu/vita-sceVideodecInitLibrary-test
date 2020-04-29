#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/videodec.h> 
#include <stdio.h>

#include "debugScreen.h"

#define printf psvDebugScreenPrintf

int main(int argc, char *argv[]) {
	psvDebugScreenInit();

	FILE* log = fopen("ux0:data/sceVideodecInitLibrary.log", "w");
	if (log == NULL)
	{
		printf("Failed to open log file.\n");
		while (1) sceKernelDelayThread(60*1000000);
		return 1;
	}

	printf("Resolution test begin in 3 seconds.\n");
	sceKernelDelayThread(3*1000000);

	SceVideodecQueryInitInfoHwAvcdec init;
	init.size = sizeof(SceVideodecQueryInitInfoHwAvcdec);
	init.numOfRefFrames = 5;
    init.numOfStreams = 1;

	for (int x = 1; x <= 2560; x++)
	{
		printf("x = %i\n", x);
		init.horizontal = x;
		for (int y = 1; y <= 2560; y++)
		{
			init.vertical = y;
			if (!sceVideodecInitLibrary(SCE_VIDEODEC_TYPE_HW_AVCDEC, &init))
			{
      			fprintf(log, "%i, %i\n", x, y);
    			if (sceVideodecTermLibrary(SCE_VIDEODEC_TYPE_HW_AVCDEC) < 0)
				{
					fflush(log);
					fclose(log);

					printf("Cannot release decoder.\n");
					while (1) sceKernelDelayThread(60*1000000);
					return 2;
				}
			}
		}
	}

	fflush(log);
	fclose(log);

	printf("Test completed.\n");
	while (1) sceKernelDelayThread(60*1000000);
	return 0;
}
