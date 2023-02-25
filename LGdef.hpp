/* This is LGdef.hpp file of LocalGen.                                   */
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

#ifndef __LGDEF_HPP__
#define __LGDEF_HPP__

//====def====//

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <mutex>
#include <thread>
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <graphics.h>
#include "LocalGen_private.h"
#include "bin/LGGrectbut.hpp"

using std::deque;
using std::string;
using std::to_string;

//====const====//

const int dx[5] = {0, -1, 0, 1, 0};
const int dy[5] = {0, 0, -1, 0, 1};
const char NUM_s[20] = {0, 'H', 'K', 'W', 'L', 'M', 'Q', 'I', 'G', 'B', 'N', 'T'};
const int LEN_ZIP = 100005, CHAR_AD = 48, LEN_MOVE = 30005, replaySorter = 2000;

//====struct====//

struct MapInfoS {
	int id;
	string chiname;
	string engname;
	string auth;
	int hei;
	int wid;
	int generalcnt;
	int swampcnt;
	int citycnt;
	int mountaincnt;
	int plaincnt;
	string filename;
	MapInfoS() = default;
	~MapInfoS() = default;
};

struct movementS {
	int id, op;
	long long turn;
	void clear() {
		id = turn = op = 0;
	}
};

struct playerCoord {
	int x, y;
};

struct passS {
	int id, turn;
};

struct playerS {
	string name;		/* team name */
	unsigned int color; /* team color */
};

struct Block {
	int team; /* the team who holds this block */
	int type; /* the block's type: 0->plain, 1->swamp, 2->mountain, 3->general, 4->city */
	long long army;  /* count of army on this block */
	bool lit; /* whether the block is lighted(lit) */
};

struct gameMessageStore {
	int playerA, playerB;
	int turnNumber;
};

struct moveS {
	int id;
	playerCoord from;
	playerCoord to;
};

//====value====//

PIMAGE pimg[7];
MapInfoS maps[5005];
Block gameMap[505][505]; /* maximum 500*500 */
playerS playerInfo[64] = {
	{"White", 0xffffffff},
	{"Red", 0xffff0000},
	{"Aqua", 0xff4363d8},
	{"Green", 0xff008000},
	{"Teal", 0xff008080},
	{"Orange", 0xfff58231},
	{"Pink", 0xfff032e6},
	{"Purple", 0xff800080},
	{"Maroon", 0xff800000},
	{"Yellow", 0xffb09f30},
	{"Brown", 0xff9a6324},
	{"Blue", 0xff0000ff},
	{"Indigo", 0xff483d8b},
};

int mapH, mapW;
int widthPerBlock, heightPerBlock;
int mapNum;

char strZipStatus[LEN_ZIP];
char strZip[LEN_ZIP];
char strdeZip[LEN_ZIP];
char strGameZip[4 * LEN_ZIP];
char strdeGameZip[4 * LEN_ZIP];
char strStatusZip[4*LEN_ZIP];
char strdeStatusZip[4*LEN_ZIP];

int curLen = 0;
Block curMap[505][505];
playerCoord mapCoord[17][30], curCoord[30];
Block mapSet[17][505][505];
long long totTurn, curTurn, totMove;
std::pair<long long, long long> curMoveS;
std::queue<long long> signMap;
std::queue<long long> signCmd;
movementS dezipedMovementS[4 * LEN_ZIP];
movementS tmp;
std::queue<movementS> movementPack;

std::vector<passS> passId[505][505];
playerCoord lastTurn[20];

//====function====//

bool operator== (playerCoord a,playerCoord b) {
	return a.x==b.x&&a.y==b.y;
}

inline void exitExe() { exit(0); }

bool isVisible(int,int,int);
void printNum(bool visible, long long army, int team, int curx, int cury);

void createRandomMap(int crtH, int crtW);
void createStandardMap(int crtH, int crtW);
void createFullCityMap(int crtH, int crtW, long long armyMN, long long armyMX, int plCnt);
void createFullSwampMap(int crtH, int crtW, int plCnt);
void createFullPlainMap(int crtH, int crtW, int plCnt);

void getAllFiles(string path, std::vector<string>& files, string fileType);
void initMaps();
void readMap(int mid);
void printMap(int printCode, playerCoord coo);

inline long long PMod(long long& x);
void trans(int st, int en);
void retrans(int cur);
std::pair<long long, long long> bin_search(long long curTurn);
void Zip();
void deZip();
void zipStatus(int playerCnt);
void deZipStatus(int st,int en,int cur);
void zipGame(long long totTurn);
void deZipGame(int playerCnt);

void toAvoidCEBugInGraphicsImportMap(string fileName);

void printGameMessage(gameMessageStore now, int playerCnt);
void kill(int p1, int p2, int isAlive[], int curTurn);
int analyzeMove(deque<moveS>& inlineMove, int id, int mv, playerCoord& coo, playerCoord genCoo[], int curTurn);
void flushMove(deque<moveS>& inlineMove, int isAlive[], int curTurn);
void initGenerals(int playerCnt, playerCoord coos[]);
void updateMap(int& curTurn);
void ranklist(int playerCnt, playerCoord coos[], int isAlive[], int robotId[]);

int GAME(bool isWeb, int cheatCode, int plCnt, int stDel);

/***** graphics *****/

bool FullScreen(HWND hwnd, int fullscreenWidth = GetSystemMetrics(SM_CXSCREEN), int fullscreenHeight = GetSystemMetrics(SM_CYSCREEN), int colourBits = 32, int refreshRate = 60);

namespace imageOperation {
	void zoomImage(PIMAGE& pimg, int zoomWidth, int zoomHeight);
	void setWindowTransparent(bool enable, int alpha = 0xFF);
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
	void inputMapData(int a, int b, int c, int d);
	int select = 0;
	void initWindowSize();
	bool cheatCodeSelected[13];
	void init();
}

inline int getHeightPerBlock();
inline int getWidthPerBlock();

namespace LGlocal {
};

namespace LGclient {
};

namespace LGserver {
};

void MainPage() {
	std::mt19937 mtrd(std::chrono::system_clock::now().time_since_epoch().count());
	LGGraphics::WelcomePage();
	LGGraphics::selectOrImportMap();
	return;
}

#endif // __LGDEF_HPP__
