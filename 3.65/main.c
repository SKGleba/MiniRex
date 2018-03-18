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
#include <psp2/appmgr.h>

int main() {
	SceCtrlData pad, old_pad;
	unsigned int keys_down;
	old_pad.buttons = 0;

	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	const char MainMenuOpts[3][64] = {"Flash spoof", "UnFlash spoof", "Exit"};
	const char PSubMenuOpts[4][64] = {"Back", "Back", "Start the flasher", "Back"};
	const char USubMenuOpts[4][64] = {"Back", "Back", "Start the flasher", "Back"};
	int UIScreen = 1;
	int selected = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		keys_down = pad.buttons & ~old_pad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		
		vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "MINIREX v2.0 by SKGleba");
		
		switch(UIScreen) {
			case 1: // Main Menu
				if (selected < 0) selected = 0;
				if (selected > 2) selected = 2;
				for (int i = 0; i < 3; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, MainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, MainMenuOpts[i]);
				}
				break;
			case 11: // Main Menu
				if (selected < 0) selected = 0;
				if (selected > 3) selected = 3;
				for (int i = 0; i < 4; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, PSubMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, PSubMenuOpts[i]);
				}
				break;
			case 12: // Main Menu
				if (selected < 0) selected = 0;
				if (selected > 3) selected = 3;
				for (int i = 0; i < 4; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(226,92,92,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, USubMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, USubMenuOpts[i]);
				}
				break;
		}
		
		if (keys_down & SCE_CTRL_CROSS) {
			switch(UIScreen) {
				case 0:
                     sceKernelExitProcess(0);
					break;
				case 1:
					if (selected == 0) UIScreen = 11;
					if (selected == 1) UIScreen = 12;
                    if (selected == 2) sceKernelExitProcess(0);
					break;
				case 11:
					if (selected == 0) UIScreen = 1;
					if (selected == 1) UIScreen = 1;
					if (selected == 2) sceAppMgrLoadExec("app0:flashman", NULL, NULL);
					if (selected == 3) UIScreen = 1;
					break;
				case 12:
					if (selected == 0) UIScreen = 1;
					if (selected == 1) UIScreen = 1;
					if (selected == 2) sceAppMgrLoadExec("app0:unflashman", NULL, NULL);
					if (selected == 3) UIScreen = 1;
					break;
			}
		}
		
		if (keys_down & SCE_CTRL_UP) {
			switch(UIScreen) {
				case 1:
					selected -= 1;
					break;
				case 11:
					selected -= 1;
					break;
				case 12:
					selected -= 1;
					break;
			}
		} if (keys_down & SCE_CTRL_DOWN) {
			switch(UIScreen) {
				case 1:
					selected += 1;
					break;
				case 11:
					selected += 1;
					break;
				case 12:
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
