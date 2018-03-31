// We do it using 2-layers
#include <stdio.h>
#include <string.h>
#include <psp2/io/fcntl.h>
#include <stdlib.h>
#include <vitasdk.h>
#include <taihen.h>
#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/shellutil.h>
#include <psp2/power.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
int flash(const char *sfpath){
	tai_module_args_t argg;
	argg.size = sizeof(argg);
	argg.pid = KERNEL_PID;
	argg.args = 0;
	argg.argp = NULL;
	argg.flags = 0;
	taiLoadStartKernelModuleForUser(sfpath, &argg);
    return 0;}
int main() {
	SceCtrlData pad, old_pad;
	unsigned int keys_down;
	old_pad.buttons = 0;

	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	const char MainMenuOpts[5][64] = {"Flash DEX spoof", "Flash TOOL spoof", "Flash TEST spoof", "UnFlash spoof", "Exit"};
	int UIScreen = 1;
	int selected = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		keys_down = pad.buttons & ~old_pad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "MINIREX v3.0 by SKGleba");
		
		switch(UIScreen) {
			case 1: // Main Menu
				if (selected < 0) selected = 0;
				if (selected > 4) selected = 4;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, MainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, MainMenuOpts[i]);
				}
				break;
		}
		
		if (keys_down & SCE_CTRL_CROSS) {
			switch(UIScreen) {
				case 0:
                     sceKernelExitProcess(0);
					break;
				case 1:
					if (selected == 3) flash("ux0:app/SKGM1NR3X/unflasher");
					if (selected == 0) flash("ux0:app/SKGM1NR3X/dflasher");
					if (selected == 1) flash("ux0:app/SKGM1NR3X/tflasher");
					if (selected == 2) flash("ux0:app/SKGM1NR3X/itflasher");
                    if (selected == 4) sceKernelExitProcess(0);
					break;
			}
		}
		
		if (keys_down & SCE_CTRL_UP) {
			switch(UIScreen) {
				case 1:
					selected -= 1;
					break;
			}
		} if (keys_down & SCE_CTRL_DOWN) {
			switch(UIScreen) {
				case 1:
					selected += 1;
					break;
}
			} 
		
		old_pad = pad;
		
		vita2d_end_drawing();
		vita2d_swap_buffers();
	}
	vita2d_fini();
	vita2d_free_pgf(font);
	
	sceKernelExitProcess(0);
	return 0;
}
