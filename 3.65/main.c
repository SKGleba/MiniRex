#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/shellutil.h>
#include <psp2/power.h>
#include <psp2/kernel/processmgr.h>
#include <vita2d.h>
#include <taihen.h>

#include "TIDUtils.h"
#include "stuff.h"

int main() {
	SceCtrlData pad, old_pad;
	unsigned int keys_down;
	old_pad.buttons = 0;
	int col1 = 92;
	int col2 = 226;
	int col3 = 92;
	vita2d_pgf *font;
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	font = vita2d_load_default_pgf();
	memset(&pad, 0, sizeof(pad));
	
	const char Disclaimer[512] = "Welcome to MiniRex - Project \"DeepLoad\" Flasher!\nThis installer overwrites MBR with a modified version.\nI provide this tool \"as is\" without warranty of any kind.\n\nAll credits go to Team Molecule for enso exploit\n\nMod by SKGleba (twitter.com/skgleba)";
	const char PSTVNotContinue[256] = "This installer can NOT continue.\n\nThis version is made for fw 3.65 ONLY\n\nPress X to exit...";
	const char MainMenuOpts[5][256] = {"Flash DEX Spoof", "Flash TOOL Spoof", "Flash ITU Spoof", "Revert changes", "Exit this app"};
	const char SubMenuOpts[5][256] = {"Deeper Spoof", "Flash RAW ITU spoof", "Flash ux0:skg/mrex/custom/fat.bin", "UI colour", "Exit this app"};
	const char Sub2MenuOpts[4][256] = {"DEX", "ITU", "TOOL", "Exit this app"};
	const char ColMenuOpts[5][256] = {"Red", "Green", "Dark Blue", "Light Blue", "Back"};
	const char YesNo[2][4] = {"No", "Yes"};
	
	int config_exists = exists("ur0:tai/boot_config.txt");
	int UIScreen = 0;
	int selected = 0;
	int doStuff = 0;
	int slimbin = 0;
	
	while(1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);
		keys_down = pad.buttons & ~old_pad.buttons;
		
		vita2d_start_drawing();
		vita2d_clear_screen();
		if (exists("ur0:temp/mrexred") == 1){col1 = 226; col2 = 92; col3 = 92;}
		if (exists("ur0:temp/mrexsbl") == 1){col1 = 0; col2 = 0; col3 = 226;}
		if (exists("ur0:temp/mrexdbl") == 1){col1 = 92; col2 = 92; col3 = 226;}
		
		switch(UIScreen) {
			case 0: // Disclaimer Message
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "MINIREX V5 LV3");
				if (config_exists == 0) vita2d_pgf_draw_text(font, 35, 100, RGBA8(255,255,255,255), 1.5f, Disclaimer);
				else if (config_exists == 1) vita2d_pgf_draw_text(font, 35, 100, RGBA8(255,255,255,255), 1.5f, PSTVNotContinue);
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 1: // Main Menu
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "MINIREX V5 LV3");
				if (selected < 0) selected = 0;
				if (selected > 4) selected = 4;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, MainMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, MainMenuOpts[i]);
				}
				break;
			case 3: // Install Confirmation
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Are you sure you want to flash that mod?");
				if (selected < 0) selected = 0;
				if (selected > 1) selected = 1;
				for (int i = 0; i < 2; i++) {
					if (i == selected) {
						vita2d_draw_rectangle((200*i) + 302, 328, 145, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, (200*i) + 310, 373, RGBA8(0,0,0,255), 1.5f, YesNo[i]);
					} else vita2d_pgf_draw_text(font, (200*i) + 310, 373, RGBA8(255,255,255,255), 1.5f, YesNo[i]);
				}
				break;
			case 4: // Install Ongoing
				if (doStuff == 0) vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Preparing...");
				if (doStuff == 1) {
				if (slimbin == 1) filecopy("ux0:app/SKGM1NR3X/stuff/mbrd", "ur0:temp/mrexmbr");
				if (slimbin == 2) filecopy("ux0:app/SKGM1NR3X/stuff/mbrt", "ur0:temp/mrexmbr");
				if (slimbin == 3) filecopy("ux0:app/SKGM1NR3X/stuff/mbri", "ur0:temp/mrexmbr");
				if (slimbin == 4) filecopy("ux0:app/SKGM1NR3X/stuff/fat.bin", "ur0:temp/mrexmbr");
				if (slimbin == 29) filecopy("ux0:app/SKGM1NR3X/stuff/deepd", "ur0:temp/mrexmbr");
				if (slimbin == 89) filecopy("ux0:app/SKGM1NR3X/stuff/deepi", "ur0:temp/mrexmbr");
				if (slimbin == 79) filecopy("ux0:app/SKGM1NR3X/stuff/mbroi", "ur0:temp/mrexmbr");
				if (slimbin == 99) filecopy("ux0:app/SKGM1NR3X/stuff/deept", "ur0:temp/mrexmbr");
				if (slimbin == 69) filecopy("ux0:skg/mrex/custom/fat.bin", "ur0:temp/mrexmbr");
				filecopy("ur0:tai/config.txt", "ur0:temp/mrexflag");
					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Checking...");
				} else if (doStuff == 2) {
					if (exists("ur0:temp/mrexmbr") == 0) sceKernelExitProcess(0);
					if (exists("ur0:temp/mrexflag") == 0) sceKernelExitProcess(0);
					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Flashing...\nDo not turn off your Vita or exit the app!");
				} else if (doStuff == 3) {
						flash(1, 1, 0, "ur0:temp/mrexmbr", 1, "ur0:temp/mrexflag", NULL);

					vita2d_pgf_draw_textf(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Cleaning up...");
				} else if (doStuff == 4) {
					if (exists("ur0:temp/mrexflag") == 1) UIScreen = 6;
					sceIoRemove("ur0:temp/mrexmbr");
				} else if (doStuff == 5) UIScreen = 5;
				doStuff++;
				break;
			case 5: // Install Complete
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Flash success. Press X to restart the system.");
				vita2d_draw_rectangle(402, 425, 145, 70, RGBA8(col1,col2,col3,255));
				vita2d_pgf_draw_text(font, 410, 470, RGBA8(0,0,0,255), 1.5f, "Continue");
				break;
			case 6: // Install Complete
				vita2d_pgf_draw_text(font, 35, 115, RGBA8(255,255,255,255), 1.5f, "Flash FAILED.");
				break;
			case 7: 
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "Advanced options");
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
		vita2d_clear_screen();
				if (selected < 0) selected = 0;
				if (selected > 4) selected = 4;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, SubMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, SubMenuOpts[i]);
				}
				break;
			case 27: 
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "Flash deeper spoof");
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
		vita2d_clear_screen();
				if (selected < 0) selected = 0;
				if (selected > 3) selected = 3;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, Sub2MenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, Sub2MenuOpts[i]);
				}
				break;
			case 8: 
				vita2d_pgf_draw_text(font, 215, 45, RGBA8(255,255,255,255), 1.5f, "UI Colour");
		if (exists("ur0:temp/mrexred") == 1) sceIoRemove("ur0:temp/mrexred");
		if (exists("ur0:temp/mrexgrn") == 1) sceIoRemove("ur0:temp/mrexsbl");
		if (exists("ur0:temp/mrexdbl") == 1) sceIoRemove("ur0:temp/mrexdbl");
				if (selected < 0) selected = 0;
				if (selected > 4) selected = 4;
				for (int i = 0; i < 5; i++) {
					if (i == selected) {
						vita2d_draw_rectangle(70, (80*i) + 85, 820, 70, RGBA8(col1,col2,col3,255));
						vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(0,0,0,255), 1.5f, ColMenuOpts[i]);
					} else vita2d_pgf_draw_text(font, 85, (80*i) + 130, RGBA8(255,255,255,255), 1.5f, ColMenuOpts[i]);
				}
break;
		}
		
		if (keys_down & SCE_CTRL_CROSS) {
			switch(UIScreen) {
				case 0:
					if (config_exists == 0) UIScreen = 1;
					else sceKernelExitProcess(0);
					break;
				case 1:
					if (selected == 0) {slimbin = 1; UIScreen = 3;}
					if (selected == 1) {slimbin = 2; UIScreen = 3;}
					if (selected == 2) {slimbin = 3; UIScreen = 3;}
					if (selected == 3) {slimbin = 4; UIScreen = 3;}
					if (selected == 4) sceKernelExitProcess(0);
					break;
				case 2:
					UIScreen = 1;
					break;
				case 3:
					if (selected == 0) UIScreen = 1;
					if (selected == 1) UIScreen = 4;
					break;
				case 5:
					scePowerRequestColdReset();
					break;
				case 7:
					if (selected == 0) {UIScreen = 27;}
					if (selected == 1) {slimbin = 79; UIScreen = 3;}
					if (selected == 2) {slimbin = 69; UIScreen = 3;}
					if (selected == 3) {UIScreen = 8;}
					if (selected == 4) UIScreen = 1;
					break;
				case 27:
					if (selected == 0) {slimbin = 29; UIScreen = 3;}
					if (selected == 1) {slimbin = 89; UIScreen = 3;}
					if (selected == 2) {slimbin = 99; UIScreen = 3;}
					if (selected == 3) UIScreen = 7;
					break;
				case 8:
					if (selected == 0) {filecopy("ur0:tai/config.txt", "ur0:temp/mrexred"); col1 = 226; col2 = 92; col3 = 92; UIScreen = 7;}
					if (selected == 3) {filecopy("ur0:tai/config.txt", "ur0:temp/mrexsbl"); col1 = 0; col2 = 0; col3 = 226; UIScreen = 7;}
					if (selected == 2) {filecopy("ur0:tai/config.txt", "ur0:temp/mrexdbl"); col1 = 92; col2 = 92; col3 = 226; UIScreen = 7;}
					if (selected == 1) {col1 = 92; col2 = 226; col3 = 92; UIScreen = 7;}
					if (selected == 4) UIScreen = 7;
					break;
			 }
		}
		
		if (keys_down & SCE_CTRL_UP) {
			switch(UIScreen) {
				case 1:
					selected -= 1;
					break;
				case 7:
					selected -= 1;
					break;
				case 8:
					selected -= 1;
					break;
			}
		} if (keys_down & SCE_CTRL_DOWN) {
			switch(UIScreen) {
				case 1:
					selected += 1;
					break;
				case 7:
					selected += 1;
					break;
				case 8:
					selected += 1;
					break;
			}
		} if (keys_down & SCE_CTRL_RIGHT) {
			switch(UIScreen) {
				case 3:
					selected += 1;
					break;
				case 6:
					selected += 1;
					break;
			}
		} if (keys_down & SCE_CTRL_LEFT) {
			switch(UIScreen) {
				case 3:
					selected -= 1;
					break;
				case 6:
					selected -= 1;
					break;
			}
		} if (keys_down & SCE_CTRL_SELECT) {
			switch(UIScreen) {
				case 1:
					slimbin = 69;
					UIScreen = 3;
					break;
			}
		} if (keys_down & SCE_CTRL_TRIANGLE) {
			switch(UIScreen) {
				case 1:
					UIScreen = 7;
					break;
				case 7:
					UIScreen = 1;
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
