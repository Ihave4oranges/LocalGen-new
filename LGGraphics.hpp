/* This is LGgraphics.hpp file of LocalGen.                                */
/* Copyright (c) 2023 LocalGen-dev; All rights reserved.                 */
/* Developers: http://github.com/LocalGen-dev                            */
/* Project: http://github.com/LocalGen-dev/LocalGen-new                  */
/*                                                                       */
/* This project is licensed under the MIT license. That means you can    */
/* download, use and share a copy of the product of this project. You    */
/* may modify the source code and make contribution to it too. But, you  */
/* must print the copyright information at the front of your product.    */
/*                                                                       */
/* The full MIT license this project uses can be found here:             */
/* http://github.com/LocalGen-dev/LocalGen-new/blob/main/LICENSE.md      */
#include <bits/stdc++.h>
#include "LGcons.hpp"
#include "LGmaps.hpp"
#include "LGgame.hpp"
#include <graphics.h>
using namespace std;

void exitExe();

// struct for buttons
struct GBUTTON {
	PIMAGE button; // image info
	int hei, wid; // height & width
	color_t bgcol; // background color
	color_t txtcol; // text color
	vector<string> text; // text
	string font; // font face name
	int fonthei, fontwid; // font height & width
	int halign, walign; // align method
	int hloc, wloc; // location on screen
	std::function<void()> clickEvent; // event when clicked
	int status; // button status: free(0) / cursor-on(1) / clicked(2)

	explicit GBUTTON() {
		button = newimage();
		hei = wid = 1;
		halign = TOP_TEXT, walign = LEFT_TEXT;
	}
	~GBUTTON() {
		delimage(button);
	}
	GBUTTON(int h, int w) {
		hei = h, wid = w;
		button = newimage(h, w);
	}
	GBUTTON(GBUTTON&& but) {
		delimage(button);
		button = but.button;
		hei = but.hei, wid = but.wid;
		bgcol = but.bgcol, txtcol = but.txtcol;
		text = but.text;
	}
	GBUTTON(const GBUTTON& but) {
		delimage(button);
		button = but.button;
		hei = but.hei, wid = but.wid;
		bgcol = but.bgcol, txtcol = but.txtcol;
		text = but.text;
	}
	inline void draw() {
		delimage(button);
		button = newimage(wid, hei);
		setbkmode(TRANSPARENT, button);
		setfillcolor(bgcol, button);
		ege_fillrect(0, 0, wid, hei, button);
		setcolor(txtcol, button);
		setfont(fonthei, fontwid, font.c_str(), button);
		settextjustify(walign, halign, button);
		register int ox, oy;
		if(walign == LEFT_TEXT) ox = 0;
		else if(walign == CENTER_TEXT) ox = wid / 2;
		else ox = wid;
		if(halign == TOP_TEXT) oy = 0;
		else if(halign == CENTER_TEXT) oy = (hei - fonthei * (text.size() - 1)) / 2;
		else oy = hei - fonthei * (text.size() - 1);
		for(auto s:text) {
			outtextxy(ox, oy, s.c_str(), button);
			oy += fonthei;
		}
		setcolor(0xff000000 | ~bgcol, button);
		if(status == 1) {
			ege_rectangle(1, 1, wid - 1, hei - 1, button);
		} else if(status == 2) {
			ege_rectangle(1, 1, wid - 1, hei - 1, button);
		}
	}
	inline void display() {
		draw();
		putimage(NULL, wloc, hloc, wid, hei, button, 0, 0);
	}
	inline void seth(int h) { hei = h; }
	inline void setw(int w) { wid = w; }
	inline void sethw(int h, int w) { hei = h; wid = w; }
	inline void setbgcol(color_t col) { bgcol = col; }
	inline void settxtcol(color_t col) { txtcol = col; }
	inline void addtext(string txt) { text.push_back(txt); }
	inline void poptext() { if(!text.empty()) text.pop_back(); }
	inline void cleartext() { text.clear(); }
	inline void setfontname(string ft) { font = ft; }
	inline void setfonth(int fh) { fonthei = fh; }
	inline void setfontw(int fw) { fontwid = fw; }
	inline void setfonthw(int fh, int fw) { fonthei = fh; fontwid = fw; }
	inline void setlocation(int h, int w) { hloc = h, wloc = w; }
	inline void setalign(int ha = -1, int wa = -1) {
		if(~ha) halign = ha;
		if(~wa) walign = wa;
	}
	inline void setevent(std::function<void()> event) { clickEvent = event; }
	inline int detect() {
		/* todo */
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(getHWnd(), &mousePos);
		if(mousePos.x < wloc || mousePos.x > min(wloc + wid - 1, getwidth()) || mousePos.y < hloc || mousePos.y > min(hloc + hei - 1, getheight()))
			return status = 0;
		while(mousemsg()) {
			mouse_msg msg = getmouse();
			if(!(msg.x < wloc || msg.x > min(wloc + wid - 1, getwidth()) || msg.y < hloc || msg.y > min(hloc + hei - 1, getheight()))
			        && msg.is_left() && msg.is_down()) return status = 2;
		}
		return status = 1;
	}
};

bool FullScreen(HWND hwnd, int fullscreenWidth = GetSystemMetrics(SM_CXSCREEN), int fullscreenHeight = GetSystemMetrics(SM_CYSCREEN), int colourBits = 32, int refreshRate = 60) {
	DEVMODE fullscreenSettings;
	bool isChangeSuccessful;
	RECT windowBoundary;

	EnumDisplaySettings(NULL, 0, &fullscreenSettings);
	fullscreenSettings.dmPelsWidth = fullscreenWidth;
	fullscreenSettings.dmPelsHeight = fullscreenHeight;
	fullscreenSettings.dmBitsPerPel = colourBits;
	fullscreenSettings.dmDisplayFrequency = refreshRate;
	fullscreenSettings.dmFields = DM_PELSWIDTH |
	                              DM_PELSHEIGHT |
	                              DM_BITSPERPEL |
	                              DM_DISPLAYFREQUENCY;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
	isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	ShowWindow(hwnd, SW_MAXIMIZE);

	return isChangeSuccessful;
}

namespace imageOperation {
	void zoomImage(PIMAGE &pimg, int zoomWidth, int zoomHeight) {
		if ((pimg == NULL) || (zoomWidth == getwidth(pimg) && zoomHeight == getheight(pimg)))
			return;

		PIMAGE zoomImage = newimage(zoomWidth, zoomHeight);
		putimage(zoomImage, 0, 0, zoomWidth, zoomHeight, pimg, 0, 0, getwidth(pimg), getheight(pimg));
		delimage(pimg);

		pimg = zoomImage;
	}
	void setWindowTransparent(bool enable, int alpha = 0xFF) {
		HWND egeHwnd = getHWnd();
		LONG nRet = ::GetWindowLong(egeHwnd, GWL_EXSTYLE);
		nRet |= WS_EX_LAYERED;
		::SetWindowLong(egeHwnd, GWL_EXSTYLE, nRet);
		if (!enable)
			alpha = 0xFF;
		SetLayeredWindowAttributes(egeHwnd, 0, alpha, LWA_ALPHA);
	}
}

bool isdllOK();
void toAvoidCEBugInGraphicsImportMap(string fileName);
int returnMapNum();
int GAME(bool isWeb, int cheatCode, int plCnt, int stDel);

void ege_circle(int x, int y, int r) {
	ege_ellipse(x - r, y - r, r << 1, r << 1);
}

namespace LGGraphics {
	string fileName;
	int stDel = 1;
	int plCnt = 0;
	int mapSelected = 0;
	int cheatCode = 0;
	struct mapData {
		int heightPerBlock;
		int widthPerBlock;
		int height, width;
		double mapSizeX, mapSizeY;
	} mapDataStore;
	void selectOrImportMap();
	void doMapImport();
	void WelcomePage();
	void doMapSelect();
	void importGameSettings();
	void inputMapData(int a, int b, int c, int d) {
		mapDataStore.heightPerBlock = a;
		mapDataStore.widthPerBlock = b;
		mapDataStore.height = c;
		mapDataStore.width = d;
		return;
	}
	int select = 0;
	void initWindowSize() {
		initgraph(800, 600);
		ege_enable_aa(true, NULL);
		setcaption("LocalGen Windows Size Selection");
		setbkcolor(WHITE);
		setbkcolor_f(WHITE);
		bool changeMade = true;
		cleardevice();
		setfont(100, 0, "Freestyle Script");
		setcolor(BLUE);
		xyprintf(10, 10, "LocalGen");
		setfont(50, 0, "Freestyle Script");
		xyprintf(10, 130, "Please Select Window Size:");
		GBUTTON scrsz[10];
		for (int i = 200; i <= 500; i += 100) {
			register int p = i / 100 - 2;
			scrsz[p].sethw(50, 500);
			scrsz[p].setbgcol(0xffffffff);
			scrsz[p].settxtcol(BLUE);
			scrsz[p].setfontname("Freestyle Script");
			scrsz[p].setfonth(40);
			scrsz[p].setlocation(180 + i / 4 * 2, 10);
			scrsz[p].addtext(to_string(i * 4) + " * " + to_string(i * 9 / 4));
			scrsz[p].clickEvent = [&]()->void { select = i / 100; };
			scrsz[p].setalign(CENTER_TEXT, CENTER_TEXT);
			scrsz[p].display();
		} {
			int i = 600;
			register int p = i / 100 - 2;
			scrsz[p].sethw(50, 500);
			scrsz[p].setbgcol(0xffffffff);
			scrsz[p].settxtcol(BLUE);
			scrsz[p].setfontname("Freestyle Script");
			scrsz[p].setfonth(40);
			scrsz[p].setlocation(180 + i / 4 * 2, 10);
			scrsz[p].addtext("Auto Fit (Full Screen)");
			scrsz[p].clickEvent = [&]()->void { select = i / 100; };
			scrsz[p].setalign(CENTER_TEXT, CENTER_TEXT);
			scrsz[p].display();
		}
		for (; is_run(); delay_fps(60)) {
			for(int i = 200; i <= 600; i += 100) {
				register int p = i / 100 - 2;
				register int k = scrsz[p].status;
				scrsz[p].detect();
				scrsz[p].display();
				if(scrsz[p].status == 2) scrsz[p].clickEvent(), changeMade = false;
			}
			if(!changeMade) break;
		}
	finishSelect:
		if (select == 6) {
//			movewindow(0, 0, false);
			resizewindow(-1, -1);
			FullScreen(getHWnd());
			int w = getmaxx(), h = getmaxy();
			mapDataStore.mapSizeX = (double)(1.0 * (double)w / 1600.0);
			mapDataStore.mapSizeY = (double)(1.0 * (double)h / 900.0);
		} else
			mapDataStore.mapSizeX = mapDataStore.mapSizeY = (double)select / 4.0;
	}

	void WelcomePage() {
		initWindowSize();
		if (select != 6) {
			int nScreenWidth, nScreenHeight;
			nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
			initgraph(1600 * mapDataStore.mapSizeX, 900 * mapDataStore.mapSizeY, RENDER_AUTO);
//			movewindow((nScreenWidth - 1600 * mapDataStore.mapSize) / 2, (nScreenHeight - 900 * mapDataStore.mapSize) / 2, true);
			setcaption("LocalGen");
		}
		setbkcolor(WHITE);
		setbkcolor_f(WHITE);
		cleardevice();
		// xyprintf(100, 100, "%f", mapDataStore.mapSize);
		setfont(500 * mapDataStore.mapSizeY, 0, "Freestyle Script");
		setcolor(BLUE);
		xyprintf(380 * mapDataStore.mapSizeY, 0, "LocalGen");
		setfillcolor(GREEN);
		ege_fillrect(300 * mapDataStore.mapSizeX, 600 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY);
		setbkmode(TRANSPARENT);
		setcolor(WHITE);
		setfont(100 * mapDataStore.mapSizeY, 0, "Freestyle Script");
		xyprintf(350 * mapDataStore.mapSizeX, 650 * mapDataStore.mapSizeY, "Single Player");
		setfillcolor(RED);
		ege_fillrect(900 * mapDataStore.mapSizeX, 600 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY);
		xyprintf(950 * mapDataStore.mapSizeX, 650 * mapDataStore.mapSizeY, "MultiPlayer");
		delay_ms(0);
		mouse_msg msg;
		for (; is_run(); delay_fps(120)) {
			msg = getmouse();
			// xyprintf(750, 650, "Mouse!");
			// cout << msg.x << ' ' << msg.y << endl;
			if (msg.is_left() && msg.is_down() && msg.x >= 300 * mapDataStore.mapSizeX && msg.x <= 700 * mapDataStore.mapSizeX && msg.y >= 600 * mapDataStore.mapSizeY && msg.y <= 800 * mapDataStore.mapSizeY)
				break;
			if (msg.is_left() && msg.is_down() && msg.x >= 900 * mapDataStore.mapSizeX && msg.x <= 1300 * mapDataStore.mapSizeX && msg.y >= 600 * mapDataStore.mapSizeY && msg.y <= 800 * mapDataStore.mapSizeY) {
				xyprintf(400 * mapDataStore.mapSizeX, 900 * mapDataStore.mapSizeY, "Sorry! Multiplayer Mode is still developing.");
				Sleep(4000);
				exitExe();
			}
		}
	}

	void selectOrImportMap() {
		setfillcolor(BROWN);
		ege_fillrect(300 * mapDataStore.mapSizeX, 600 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY);
		setbkmode(TRANSPARENT);
		setcolor(WHITE);
		setfont(100 * mapDataStore.mapSizeY, 0, "Freestyle Script");
		xyprintf(350 * mapDataStore.mapSizeX, 650 * mapDataStore.mapSizeY, "Choose a Map");
		setfillcolor(BROWN);
		ege_fillrect(900 * mapDataStore.mapSizeX, 600 * mapDataStore.mapSizeY, 400 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY);
		xyprintf(950 * mapDataStore.mapSizeX, 650 * mapDataStore.mapSizeY, "Import Map");
		mouse_msg msg;
		bool select;
		while (1) {
			msg = getmouse();
			// xyprintf(750, 650, "Mouse!");
			// cout << msg.x << ' ' << msg.y << endl;
			if (msg.is_left() && msg.is_down() && msg.x >= 300 * mapDataStore.mapSizeX && msg.x <= 700 * mapDataStore.mapSizeX && msg.y >= 600 * mapDataStore.mapSizeY && msg.y <= 800 * mapDataStore.mapSizeY) {
				select = true;
				break;
			}
			if (msg.is_left() && msg.is_down() && msg.x >= 900 * mapDataStore.mapSizeX && msg.x <= 1300 * mapDataStore.mapSizeX && msg.y >= 600 * mapDataStore.mapSizeY && msg.y <= 800 * mapDataStore.mapSizeY) {
				select = false;
				break;
			}
		}
		cleardevice();
		if (!select)
			doMapImport();
		else
			doMapSelect();
		importGameSettings();
	}

	void doMapSelect() {
		int mapNum = returnMapNum();
		cleardevice();
		if (!isdllOK()) {
			xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "Oops, it seems that your 'defMap.dll' is missing!\n You can download it on GitHub. ");
			Sleep(4000);
			exitExe();
		}
		int left, right, up, down;
		int x, y;
		GBUTTON mapbut[50];
		for (int i = 1; i <= mapNum; i++) {
			x = (i + 5) / 6;
			y = ((i % 6 == 0) ? 6 : i % 6);
			left = ((y - 1) * 260) * mapDataStore.mapSizeX;
			right = (y * 260) * mapDataStore.mapSizeX;
			up = ((x - 1) * 180) * mapDataStore.mapSizeY;
			down = (x * 180) * mapDataStore.mapSizeY;
			mapbut[i].sethw(down - up, right - left);
			mapbut[i].setlocation(up, left);
			mapbut[i].setfontname("Segoe UI");
			mapbut[i].setfonthw(20 * mapDataStore.mapSizeY, 0);
			mapbut[i].settxtcol(BLUE);
			mapbut[i].setbgcol(WHITE);
			mapbut[i].addtext("id: " + to_string(maps[i].id) + " " + maps[i].chiname);
			mapbut[i].addtext(maps[i].engname);
			mapbut[i].addtext("General Count: " + to_string(maps[i].generalcnt));
			mapbut[i].addtext("Swamp Count: " + to_string(maps[i].swampcnt));
			mapbut[i].addtext("Mountain Count: " + to_string(maps[i].mountaincnt));
			mapbut[i].addtext("City Count: " + to_string(maps[i].citycnt));
			mapbut[i].addtext("Size: " + to_string(maps[i].hei) + " * " + to_string(maps[i].wid));
			mapbut[i].setalign(CENTER_TEXT, CENTER_TEXT);
			mapbut[i].clickEvent = [&mapSelected, i]()->void { mapSelected = i; };
			// imageOperation::zoomImage(pimg[5], 200, 200);
			// putimage_transparent(NULL, pimg[5], left, up, getpixel(0, 0, pimg[1]));
//			setcolor(BLUE);
//			setfont(18 * mapDataStore.mapSize, 0, "Segoe UI");
//			rectprintf(left, (up + 1 * mapDataStore.mapSize), 135 * mapDataStore.mapSize, 40 * mapDataStore.mapSize, "id:%02d %s", maps[i].id, maps[i].chiname.c_str());
//			setfont(16 * mapDataStore.mapSize, 0, "Segoe UI");
//			rectprintf(left, (up + 40 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "%s", maps[i].engname.c_str());
//			rectprintf(left, (up + 60 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "GeneralCount: %d", maps[i].generalcnt);
//			rectprintf(left, (up + 80 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "SwampCount: %d", maps[i].swampcnt);
//			rectprintf(left, (up + 100 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "MountainCount: %d", maps[i].mountaincnt);
//			rectprintf(left, (up + 120 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "CityCount: %d", maps[i].citycnt);
//			rectprintf(left, (up + 140 * mapDataStore.mapSize), 162 * mapDataStore.mapSize, 18 * mapDataStore.mapSize, "Size: %d * %d", maps[i].hei, maps[i].wid);
		}
		mouse_msg msg;
		for(; is_run(); delay_fps(60)) {
			for(int i = 1; i <= mapNum; ++i) {
				mapbut[i].detect();
				mapbut[i].display();
				if(mapbut[i].status == 2) mapbut[i].clickEvent();
			}
			if(mapSelected) break;
		}
		cleardevice();
		setcolor(GREEN);
		setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
		xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
		xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
		setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
		xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
		xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author: %s", maps[mapSelected].auth.c_str());
		xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
		xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
		xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
		if (mapSelected < 6) {
			setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
			setcolor(MAGENTA);
			int height = 0;
			key_msg msg;
			xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
			while (1) {
				msg = getkey();
				if (msg.msg == key_msg_char) {
					if (msg.key == 13 && height <= 500 && height >= 1)
						break;
					if (msg.key == 8)
						height /= 10;
					else if (msg.key >= '0' && msg.key <= '9') {
						height = height * 10 + msg.key - '0';
					}
					cleardevice();
					setcolor(GREEN);
					setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
					xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
					xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
					setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
					xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
					xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
					xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
					setcolor(MAGENTA);
					xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
					xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
				}
			}
			cleardevice();
			setcolor(GREEN);
			setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
			xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
			xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
			setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
			xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
			xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
			xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
			setcolor(MAGENTA);
			xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
			xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
			int width = 0;
			xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
			while (1) {
				msg = getkey();
				if (msg.msg == key_msg_char) {
					if (msg.key == 13 && width <= 500 && width >= 1)
						break;
					if (msg.key == 8)
						width /= 10;
					else if (msg.key >= '0' && msg.key <= '9') {
						width = width * 10 + msg.key - '0';
					}
					cleardevice();
					setcolor(GREEN);
					setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
					xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
					xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
					setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
					xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
					xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
					xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
					xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
					setcolor(MAGENTA);
					xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
					xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
					xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
					xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
				}
			}
			long long armyMin = 0, armyMax = 0;
			if (mapSelected == 3) {
				setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
				setcolor(MAGENTA);
				key_msg msg;
				xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
				bool isPositive = true;
				while (1) {
					msg = getkey();
					if (msg.msg == key_msg_char) {
						if (msg.key == 13)
							break;
						if (msg.key == 8)
							armyMin /= 10;
						else if (msg.key >= '0' && msg.key <= '9') {
							armyMin = armyMin * 10 + msg.key - '0';
						} else if (msg.key == '-')
							isPositive = !isPositive;
						cleardevice();
						setcolor(GREEN);
						setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
						xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
						xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
						setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
						xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
						xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
						xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
						setcolor(MAGENTA);
						xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
						xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
						xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
						xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
						xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
						long long realarmy = armyMin;
						if (!isPositive)
							realarmy = -realarmy;
						xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", realarmy);
					}
				}
				if (!isPositive)
					armyMin = -armyMin;
				cleardevice();
				setcolor(GREEN);
				setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
				xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
				xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
				setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
				xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
				xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
				xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
				setcolor(MAGENTA);
				xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
				xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
				xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
				xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
				xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
				xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMin);
				xyprintf(810 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Maximum Number of Army on each Block");
				isPositive = true;
				while (1) {
					msg = getkey();
					if (msg.msg == key_msg_char) {
						if (msg.key == 13 && armyMax * (isPositive ? 1 : (-1)) >= armyMin)
							break;
						if (msg.key == 8)
							armyMax /= 10;
						else if (msg.key >= '0' && msg.key <= '9') {
							armyMax = armyMax * 10 + msg.key - '0';
						} else if (msg.key == '-')
							isPositive = !isPositive;
						cleardevice();
						setcolor(GREEN);
						setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
						xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
						xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
						setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
						xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
						xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
						xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
						xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
						setcolor(MAGENTA);
						xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
						xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
						xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
						xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
						xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
						xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMin);
						xyprintf(810 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Maximum Number of Army on each Block");
						long long realarmy = armyMax;
						if (!isPositive)
							realarmy = -realarmy;
						xyprintf(810 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", realarmy);
					}
				}
				if (!isPositive)
					armyMax = -armyMax;
				cleardevice();
				setcolor(GREEN);
				setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
				xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
				xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
				setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
				xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
				xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
				xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
				xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
				setcolor(MAGENTA);
				xyprintf(10 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Height of the Map (<=500)");
				xyprintf(10 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", height);
				xyprintf(810 * mapDataStore.mapSizeX, 200 * mapDataStore.mapSizeY, "Please Input the Width of the Map (<=500)");
				xyprintf(810 * mapDataStore.mapSizeX, 230 * mapDataStore.mapSizeY, "%d", width);
				xyprintf(10 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Minimum Number of Army on each Block");
				xyprintf(10 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMin);
				xyprintf(810 * mapDataStore.mapSizeX, 270 * mapDataStore.mapSizeY, "Please Input the Maximum Number of Army on each Block");
				xyprintf(810 * mapDataStore.mapSizeX, 300 * mapDataStore.mapSizeY, "%lld", armyMax);
			}
			importGameSettings();
			switch (mapSelected) {
				case 1:
					createRandomMap(height, width);
					break;
				case 2:
					createStandardMap(height, width);
					break;
				case 3:
					createFullCityMap(height, width, armyMin, armyMax, plCnt);
					break;
				case 4:
					createFullSwampMap(height, width, plCnt);
					break;
				case 5:
					createFullPlainMap(height, width, plCnt);
					break;
			}
		} else {
			cleardevice();
			setcolor(GREEN);
			setfont(40 * mapDataStore.mapSizeY, 0, "Segoe UI");
			xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "id: %02d", maps[mapSelected].id);
			xyprintf(10 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].chiname.c_str());
			setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
			xyprintf(300 * mapDataStore.mapSizeX, 40 * mapDataStore.mapSizeY, "%s", maps[mapSelected].engname.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 70 * mapDataStore.mapSizeY, "Author of the Map: %s", maps[mapSelected].auth.c_str());
			xyprintf(10 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Size of the Map: %d * %d", maps[mapSelected].hei, maps[mapSelected].wid);
			xyprintf(10 * mapDataStore.mapSizeX, 130 * mapDataStore.mapSizeY, "GeneralCount : %d          PlainCount: %d", maps[mapSelected].generalcnt, maps[mapSelected].plaincnt);
			xyprintf(10 * mapDataStore.mapSizeX, 160 * mapDataStore.mapSizeY, "MountainCount: %d          CityCount : %d", maps[mapSelected].mountaincnt, maps[mapSelected].citycnt);
			importGameSettings();
			copyMap(mapSelected);
		}
		GAME(0, cheatCode, plCnt, stDel);
		exit(0);
	}

	void doMapImport() {
		cleardevice();
		setcolor(BLUE);
		setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
		xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "Please Input Filename with suffix (end with enter)");
		key_msg msg;
		while (1) {
			msg = getkey();
			if (msg.msg == key_msg_char) {
				if (msg.key == 13)
					break;
				if (msg.key == 8)
					fileName.pop_back();
				else
					fileName += (char)(msg.key);
				cleardevice();
				setcolor(BLUE);
				setfont(30 * mapDataStore.mapSizeY, 0, "Segoe UI");
				xyprintf(10 * mapDataStore.mapSizeX, 10 * mapDataStore.mapSizeY, "Please Input Filename with suffix (end with enter)");
				xyprintf(10 * mapDataStore.mapSizeX, 110 * mapDataStore.mapSizeY, "%s", fileName.c_str());
			}
			if (msg.msg == key_enter)
				break;
		}
		toAvoidCEBugInGraphicsImportMap(fileName);
	}

	bool cheatCodeSelected[13];

	void importGameSettings() {
		int circlePos = 450 * mapDataStore.mapSizeX;
		PIMAGE refreshCopy = newimage();
		getimage(refreshCopy, 0, 0, 1600 * mapDataStore.mapSizeX, 340 * mapDataStore.mapSizeY);
		setfont(20 * mapDataStore.mapSizeY, 0, "Segoe UI");
		setcolor(BLUE);
		bool changeMade = true;
		int mouseDownCount = 0;
		bool endConfig = false;
		cheatCode = 2;
		cheatCodeSelected[1] = true;
		for (; is_run(); delay_fps(120)) {
			if (changeMade) {
				cleardevice();
				putimage(0, 0, refreshCopy);
				setcolor(BLUE);
				rectprintf(60 * mapDataStore.mapSizeX, 350 * mapDataStore.mapSizeY, 430 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Please Select The Number of Players");
				for (int i = 1; i <= 12; i++) {
					int lineNumber = (i + 2) / 3;
					int columnNumber = i - (lineNumber - 1) * 3;
					if (i == plCnt) {
						setcolor(RED);
						ege_circle((100 * columnNumber + 50) * mapDataStore.mapSizeX, (500 + 100 * (lineNumber - 1)) * mapDataStore.mapSizeY, 50 * mapDataStore.mapSizeX);
						setcolor(BLUE);
					}
					xyprintf((100 * columnNumber + 50) * mapDataStore.mapSizeX, (500 + 100 * (lineNumber - 1)) * mapDataStore.mapSizeY, "%d", i);
					rectangle(((columnNumber - 1) * 100 + 100) * mapDataStore.mapSizeX, ((lineNumber - 1) * 100 + 450) * mapDataStore.mapSizeY, (columnNumber * 100 + 100) * mapDataStore.mapSizeX, (lineNumber * 100 + 450) * mapDataStore.mapSizeY);
				}
				rectprintf(560 * mapDataStore.mapSizeX, 350 * mapDataStore.mapSizeY, 380, 100, "Drag to Select the Speed of the Game");
				setfillcolor(LIGHTGRAY);
				ege_fillrect(725 * mapDataStore.mapSizeX, 450 * mapDataStore.mapSizeY, 50 * mapDataStore.mapSizeX, 400 * mapDataStore.mapSizeY);
				setfillcolor(BLUE);
				ege_fillellipse(720 * mapDataStore.mapSizeX, circlePos - 30 * mapDataStore.mapSizeY, 60 * mapDataStore.mapSizeX, 60 * mapDataStore.mapSizeY);
				rectprintf(1060 * mapDataStore.mapSizeX, 350 * mapDataStore.mapSizeY, 380 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "Select the Players you want to watch Directly");
				for (int i = 1; i <= 12; i++) {
					int lineNumber = (i + 2) / 3;
					int columnNumber = i - (lineNumber - 1) * 3;
					setcolor(BLUE);
					rectangle(((columnNumber - 1) * 100 + 1100) * mapDataStore.mapSizeX, ((lineNumber - 1) * 100 + 450) * mapDataStore.mapSizeY, (columnNumber * 100 + 1100) * mapDataStore.mapSizeX, (lineNumber * 100 + 450) * mapDataStore.mapSizeY);
				}
				for (int i = 1; i <= plCnt; i++) {
					int lineNumber = (i + 2) / 3;
					int columnNumber = i - (lineNumber - 1) * 3;
					setcolor(defTeams[i].color);
					rectprintf(((columnNumber - 1) * 100 + 1100) * mapDataStore.mapSizeX, ((lineNumber - 1) * 100 + 450) * mapDataStore.mapSizeY, 100 * mapDataStore.mapSizeX, 100 * mapDataStore.mapSizeY, "%s", defTeams[i].name.c_str());
					if (cheatCodeSelected[i]) {
						setcolor(RED);
						ege_circle((1050 + 100 * columnNumber) * mapDataStore.mapSizeX, (500 + 100 * (lineNumber - 1)) * mapDataStore.mapSizeY, 50 * mapDataStore.mapSizeX);
						setcolor(BLUE);
					}
				}
				rectangle(900 * mapDataStore.mapSizeX, 30 * mapDataStore.mapSizeY, 1100 * mapDataStore.mapSizeX, 105 * mapDataStore.mapSizeY);
				setfont(70 * mapDataStore.mapSizeY, 0, "Freestyle Script");
				rectprintf(900 * mapDataStore.mapSizeX, 30 * mapDataStore.mapSizeY, 200 * mapDataStore.mapSizeX, 75 * mapDataStore.mapSizeY, "Start Game!");
				setfont(20 * mapDataStore.mapSizeY, 0, "Segoe UI");
				if (stDel != 0)
					xyprintf(560 * mapDataStore.mapSizeX, 875 * mapDataStore.mapSizeY, "Speed Now: %d", stDel);
				else
					xyprintf(560 * mapDataStore.mapSizeX, 875 * mapDataStore.mapSizeY, "Speed Now: MAXSPEED");
				changeMade = false;
			}
			while (mousemsg()) {
				mouse_msg msg = getmouse();
				// cout << msg.x << ' ' << msg.y << ' ' << msg.is_left() << ' ' << msg.is_down() << ' ' << msg.is_up() << ' ' << msg.is_move() << ' ' << mouseDownCount << ' ' << circlePos << endl;
				if (100 * mapDataStore.mapSizeX <= msg.x && msg.x <= 400 * mapDataStore.mapSizeX && 450 * mapDataStore.mapSizeY <= msg.y && 850 * mapDataStore.mapSizeY >= msg.y) {
					if (msg.is_left() && msg.is_down()) {
						int col = (msg.x + 49 - 50 * mapDataStore.mapSizeX) / (100 * mapDataStore.mapSizeX);
						int lin = (msg.y - 450 * mapDataStore.mapSizeY + 99) / (100 * mapDataStore.mapSizeY);
						plCnt = (lin - 1) * 3 + col;
						changeMade = true;
					}
				}
				if (msg.is_left() && msg.is_down())
					mouseDownCount++;
				if (msg.is_left() && msg.is_up() && mouseDownCount > 0)
					mouseDownCount--;
				if (msg.x >= 720 * mapDataStore.mapSizeX && msg.x <= 780 * mapDataStore.mapSizeX && msg.y >= 450 * mapDataStore.mapSizeY && msg.y <= 850 * mapDataStore.mapSizeY) {
					if (mouseDownCount > 0) {
						circlePos = msg.y;
						if (circlePos >= 840 * mapDataStore.mapSizeY)
							stDel = 0;
						else
							stDel = ((circlePos - 450 * mapDataStore.mapSizeX) / (20 * mapDataStore.mapSizeX)) + 1;
						changeMade = true;
					}
				}
				if (1100 * mapDataStore.mapSizeX <= msg.x && 1400 * mapDataStore.mapSizeX >= msg.x && 450 * mapDataStore.mapSizeY <= msg.y && msg.y <= 850 * mapDataStore.mapSizeY) {
					if (msg.is_left() && msg.is_down()) {
						int col = (msg.x + (99 - 1100) * mapDataStore.mapSizeX) / (100 * mapDataStore.mapSizeX);
						int lin = (msg.y + (99 - 450) * mapDataStore.mapSizeY) / (100 * mapDataStore.mapSizeY);
						int num = (lin - 1) * 3 + col;
						if (num <= plCnt && num > 0) {
							cheatCode ^= (1 << (num));
							cheatCodeSelected[num] ^= 1;
							changeMade = true;
						}
					}
				}
				if (msg.x >= 900 * mapDataStore.mapSizeX && msg.x <= 1100 * mapDataStore.mapSizeX && msg.y >= 30 * mapDataStore.mapSizeY && msg.y <= 105 * mapDataStore.mapSizeY && msg.is_down() && msg.is_left()) {
					endConfig = true;
					cleardevice();
					if (stDel == 0)
						stDel = 60;
					return;
				}
			}
		}
	}

	void init() {
		heightPerBlock = (900.0 * mapDataStore.mapSizeX / (double)mapH);
		widthPerBlock = (900.0 * mapDataStore.mapSizeY / (double)mapW);
		heightPerBlock = widthPerBlock = min(heightPerBlock, widthPerBlock);
		mapDataStore.widthPerBlock = widthPerBlock;
		mapDataStore.heightPerBlock = heightPerBlock;
		setbkmode(TRANSPARENT);
		pimg[1] = newimage();
		getimage(pimg[1], "img/city.png");
		imageOperation::zoomImage(pimg[1], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[2] = newimage();
		getimage(pimg[2], "img/crown.png");
		imageOperation::zoomImage(pimg[2], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[3] = newimage();
		getimage(pimg[3], "img/mountain.png");
		imageOperation::zoomImage(pimg[3], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[4] = newimage();
		getimage(pimg[4], "img/swamp.png");
		imageOperation::zoomImage(pimg[4], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[5] = newimage();
		getimage(pimg[5], "img/obstacle.png");
		imageOperation::zoomImage(pimg[5], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		pimg[6] = newimage();
		getimage(pimg[6], "img/currentOn.png");
		imageOperation::zoomImage(pimg[6], mapDataStore.heightPerBlock, mapDataStore.widthPerBlock);
		for (int i = 1; i <= 6; i++)
			ege_enable_aa(true, pimg[i]);
		//		initgraph(1600 * mapDataStore.mapSize, 900 * mapDataStore.mapSize, INIT_RENDERMANUAL);
		setbkcolor(WHITE);
		setbkcolor_f(WHITE);
		cleardevice();
	}
}

int getHeightPerBlock() {
	return LGGraphics::mapDataStore.heightPerBlock;
}

int getWidthPerBlock() {
	return LGGraphics::mapDataStore.widthPerBlock;
}
