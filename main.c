#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xproto.h>

#include <NVCtrl.h>
#include <NVCtrlLib.h>
#include <NvCtrlAttributes.h>

#include "query-assign.h"

void
die(char* format, ...)
{
	va_list args;
	va_start(args, format);

	if(format != NULL) {
		char* message;
		if(vasprintf(&message, format, args) > 0) {
			fprintf(stderr, "nvidia-probe: %s\n", message);
			
			free(message);
		}
	}
	fflush(NULL);
	
	exit(1);
}


static char* display_name = NULL;


static void
probe()
{
	ReturnStatus ret;

        // Get handles for all possible devices (OVERKILL!)
        CtrlHandles* handles = nv_alloc_ctrl_handles(display_name);
        if(!handles || !handles->dpy) {
                die("Could not probe hardware");
        }
        if(handles->targets[GPU_TARGET].n < 1) {
                printf("nvidia-probe: No NVidia GPU found");
        }

        for(int i = 0; i < handles->targets[GPU_TARGET].n; i++) {
                // Do the probing
                int display_num;
                ret = NvCtrlGetAttribute(handles->targets[GPU_TARGET].t[i].h, NV_CTRL_PROBE_DISPLAYS, (int *) &display_num);
                if(ret != NvCtrlSuccess) {
                        printf("GPU %d: Failed to probe GPU for new devices\n", i);
                } else {
                        printf("nvidia-probe: GPU %d: Probing successful\n", i);
                }
        }
	
	nv_free_ctrl_handles(handles);
}


int
main(int argc, char** argv)
{
	// Read display name only once
	display_name = getenv("DISPLAY");

	int loop = 0;
	if(argc > 1) {
		if(strcmp(argv[1], "--loop") > -1) {
			loop = 1;
		}
	}

	
	
	if(loop) {
		/*while(1) {
			probe();
			
			sleep(1);	
		}*/

		//XXX: Unfortunetally we cannot just call probe() over and
		// over again because the NVidia backend is leaking memory
		
		// Send probe command
		probe();
		
		// Wait a short time
		sleep(1);
		
		// Reexecute ourselves
		execlp(argv[0], "--loop");
	} else {
		// Send probe command
		probe();
	}

/*
	ReturnStatus ret;
	int num;
	
	char* display_name = getenv("DISPLAY");
	
	// Open X display connection
	Display* display = XOpenDisplay(display_name);
	if(display == NULL) {
		die("Could not open display: %s", display_name);
	}
	
	// Create NVidia control handle from X display connection
	void* nvidia = NvCtrlAttributeInit(display, NV_CTRL_TARGET_TYPE_X_SCREEN, DefaultScreen(display), NV_CTRL_ATTRIBUTES_NV_CONTROL_SUBSYSTEM);
	if(nvidia == NULL) {
		die("Could not create NV-CONTROL handle");
	}

	// Query number of framelocks
	int framelock_num;
	ret = NvCtrlQueryTargetCount(nvidia, NV_CTRL_TARGET_TYPE_FRAMELOCK, &framelock_num);
	if(ret != NvCtrlSuccess) {
		die("Could not query number of framelocks (%d)", ret);
	}
		
	for(int framelock_idx = 0; framelock_idx < (framelock_num+1); framelock_idx++) {
		// Create framelock handle from index
		void* framelock = NvCtrlAttributeInit(NvCtrlGetDisplayPtr(nvidia), NV_CTRL_TARGET_TYPE_FRAMELOCK, framelock_idx, NV_CTRL_ATTRIBUTES_NV_CONTROL_SUBSYSTEM);
		if(framelock == NULL) {
			die("Framelock %d: Could not get framelock handle from index", framelock_idx);
		}
		
		// Query number of GPUs for framelock
		int* gpu_ids;
		int  gpu_num;
		ret = NvCtrlGetBinaryAttribute(NvCtrlGetDisplayPtr(nvidia), 0, NV_CTRL_BINARY_DATA_GPUS_USING_FRAMELOCK, (unsigned char**) &gpu_ids, &gpu_num);
		if(ret != NvCtrlSuccess) {
			die("Framelock %d: Could not query GPU data", framelock_idx);
		}
		
		gpu_num = gpu_ids[0]; 
		for(int gpu_idx = 0; gpu_idx < gpu_num; gpu_idx++) {
			// Find GPU ID for GPU number
			int gpu_id = gpu_ids[gpu_idx + 1];
			
			// Create GPU handle from ID
			void* gpu = NvCtrlAttributeInit(NvCtrlGetDisplayPtr(nvidia), NV_CTRL_TARGET_TYPE_GPU, gpu_id, NV_CTRL_ATTRIBUTES_NV_CONTROL_SUBSYSTEM);
			if(gpu == NULL) {
				die("Framelock %d, GPU %8X: Could not get GPU handle from ID", framelock_idx, gpu_id);
			}
			
			// Do the probing
			int display_num;
			ret = NvCtrlGetAttribute(gpu, NV_CTRL_PROBE_DISPLAYS, (int *) &display_num);
			if(ret != NvCtrlSuccess) {
				die("Framelock %d, GPU %8X: Failed to probe GPU for new devices", framelock_idx, gpu_id);
			} else {
				printf("nvidia-probe: Framelock %d, GPU %8X: Probing successful", framelock_idx, gpu_id);
			}
		}
	} */
}
