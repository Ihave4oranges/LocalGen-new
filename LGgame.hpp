/* This is LGgame.hpp file of LocalGen.                                  */
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

#ifndef __LGGAME_HPP__
#define __LGGAME_HPP__

// standard libraries
#include <functional>
#include <algorithm>
#include <deque>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>
using std::string;
using namespace std::literals;
// windows libraries
#include <windows.h>
// #include <conio.h>
// #include<graphics.h>
//  project headers
#include "LGcons.hpp"
#include "LGmaps.hpp"
// Bot header
#include "LGbot.hpp"
// Graphics header
//  #include "LGGraphics.hpp"

const int dx[5] = {0, -1, 0, 1, 0};
const int dy[5] = {0, 0, -1, 0, 1};

struct passS
{
	int id, turn;
};
std::vector<passS> passId[505][505];
playerCoord lastTurn[20];

struct gameStatus
{
	bool isWeb;
	int cheatCode;
	int playerCnt;
	int isAlive[64];
	int stepDelay; /* ms */
	bool played;

	// constructor
	gameStatus() = default;
	gameStatus(bool iW, int chtC, int pC, int sD)
	{
		isWeb = iW;
		cheatCode = chtC;
		playerCnt = pC;
		stepDelay = sD;
		for (register int i = 1; i <= pC; ++i)
			isAlive[i] = 1;
		played = 0;
	}
	// destructor
	~gameStatus() = default;

	int curTurn;
	struct gMes
	{
		int turn, plId;
		string mes;
	} mess[205];
	int gameMesC;

	void addGameMessage(int turn, int plId, string mes)
	{
		++gameMesC;
		mess[gameMesC].turn = turn;
		mess[gameMesC].plId = plId;
		mess[gameMesC].mes = mes;
	}
	void printGameMessage()
	{
		gotoxy(2 + mapH + 1, 63);
		underline();
		printf("| %-41s |", "G a m e   M e s s a g e");
		resetattr();
		putchar('|');
		gotoxy(2 + mapH + 2, 63);
		underline();
		printf("| %6s | %7s | %-22s |", "TURN", "PLAYER", "MESSAGE");
		resetattr();
		putchar('|');
		for (int i = 1; i <= gameMesC; ++i)
		{
			gotoxy(2 + mapH + 2 + i, 63);
			underline();
			printf("| %6d | ", mess[i].turn);
			setfcolor(defTeams[mess[i].plId].color);
			printf("%7s", defTeams[mess[i].plId].name.c_str());
			setfcolor(0xffffff);
			printf(" | ");
			printf("%s", mess[i].mes.c_str());
			setfcolor(0xffffff);
			fflush(stdout);
			int x = 0, y = 0;
			getxy(x, y);
			while (y < 106)
				putchar(' '), ++y;
			gotoxy(2 + mapH + 2 + i, 107);
			printf("|");
			resetattr();
			printf("|");
		}
		fflush(stdout);
	}

	void updateMap()
	{
		++curTurn;
		for (int i = 1; i <= mapH; ++i)
		{
			for (int j = 1; j <= mapW; ++j)
			{
				if (gameMap[i][j].team == 0)
					continue;
				switch (gameMap[i][j].type)
				{
				case 0:
				{ /* plain */
					if (curTurn % 25 == 0)
						++gameMap[i][j].army;
					break;
				}
				case 1:
				{ /* swamp */
					if (gameMap[i][j].army > 0)
						if (!(--gameMap[i][j].army))
							gameMap[i][j].team = 0;
					break;
				}
				case 2:	   /* mountain */
					break; /* ??? */
				case 3:
				{ /* general */
					++gameMap[i][j].army;
					break;
				}
				case 4:
				{ /* city */
					++gameMap[i][j].army;
					break;
				}
				}
			}
		}
	}

	playerCoord genCoo[64];
	// general init
	void initGenerals(playerCoord coos[])
	{
		std::deque<playerCoord> gens;
		for (int i = 1; i <= mapH; ++i)
			for (int j = 1; j <= mapW; ++j)
				if (gameMap[i][j].type == 3)
					gens.push_back(playerCoord{i, j});
		while (gens.size() < playerCnt)
		{
			std::mt19937 p(std::chrono::system_clock::now().time_since_epoch().count());
			int x, y;
			do
				x = p() % mapH + 1, y = p() % mapW + 1;
			while (gameMap[x][y].type != 0);
			gens.push_back(playerCoord{x, y});
			gameMap[x][y].type = 3;
			gameMap[x][y].army = 0;
		}
		sort(gens.begin(), gens.end(), [](playerCoord a, playerCoord b)
			 { return a.x == b.x ? a.y < b.y : a.x < b.x; });
		std::shuffle(gens.begin(), gens.end(), std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()));
		for (int i = 1; i <= playerCnt; ++i)
		{
			coos[i] = genCoo[i] = gens[i - 1];
			gameMap[genCoo[i].x][genCoo[i].y].team = i;
			gameMap[genCoo[i].x][genCoo[i].y].army = 0;
		}
		for (int i = 1; i <= mapH; ++i)
			for (int j = 1; j <= mapW; ++j)
				if (gameMap[i][j].type == 3 && gameMap[i][j].team == 0)
					gameMap[i][j].type = 0;
	}

	void kill(int p1, int p2)
	{
		if (p2 == 1)
			MessageBox(nullptr, string("YOU ARE KILLED BY PLAYER " + defTeams[p1].name + " AT TURN " + to_string(curTurn) + ".").c_str(), "", MB_OK);
		isAlive[p2] = 0;
		for (int i = 1; i <= mapH; ++i)
		{
			for (int j = 1; j <= mapW; ++j)
			{
				if (gameMap[i][j].team == p2 && gameMap[i][j].type != 3)
				{
					gameMap[i][j].team = p1;
					gameMap[i][j].army = (gameMap[i][j].army + 1) >> 1;
				}
			}
		}
		//		++gameMesC;
		int p2col = defTeams[p2].color;
		addGameMessage(curTurn, p1, string("KILLED PLAYER \033[38;2;" + to_string(p2col / 65536) + ";" + to_string(p2col / 256 % 256) + ";" + to_string(p2col % 256) + "m" + defTeams[p2].name));
		//		gotoxy(mapH + 2 + gameMesC, 65);
		//		setfcolor(0xffffff);
		//		fputs("PLAYER ", stdout);
		//		setfcolor(defTeams[p1].color);
		//		printf("%-7s", defTeams[p1].name.c_str());
		//		setfcolor(0xffffff);
		//		fputs(" KILLED PLAYER ", stdout);
		//		setfcolor(defTeams[p2].color);
		//		printf("%-7s", defTeams[p2].name.c_str());
		//		setfcolor(0xffffff);
		//		printf(" AT TURN %d.", curTurn);
		//		fflush(stdout);
	}

	// struct for movement
	struct moveS
	{
		int id;
		playerCoord from;
		playerCoord to;
	};
	// vector for inline movements
	std::deque<moveS> inlineMove;

	// movement analyzer
	int analyzeMove(int id, int mv, playerCoord &coo)
	{
		switch (mv)
		{
		case -1:
			break;
		case 0:
			coo = genCoo[id];
			break;
		case 1 ... 4:
		{
			playerCoord newCoo{coo.x + dx[mv], coo.y + dy[mv]};
			if (newCoo.x < 1 || newCoo.x > mapH || newCoo.y < 1 || newCoo.y > mapW || gameMap[newCoo.x][newCoo.y].type == 2)
				return 1;
			moveS insMv{
				id,
				coo,
				newCoo,
			};
			inlineMove.push_back(insMv);
			coo = newCoo;
			break;
		}
		case 5 ... 8:
		{
			playerCoord newCoo{coo.x + dx[mv - 4], coo.y + dy[mv - 4]};
			if (newCoo.x < 1 || newCoo.x > mapH || newCoo.y < 1 || newCoo.y > mapW)
				return 1;
			coo = newCoo;
			break;
		}
		default:
			return -1;
		}
		return 0;
	}
	// flush existing movements
	void flushMove()
	{
		while (!inlineMove.empty())
		{
			moveS cur = inlineMove.front();
			inlineMove.pop_front();
			if (!isAlive[cur.id])
				continue;
			if (gameMap[cur.from.x][cur.from.y].team != cur.id)
				continue;
			if (gameMap[cur.to.x][cur.to.y].team == cur.id)
			{
				gameMap[cur.to.x][cur.to.y].army += gameMap[cur.from.x][cur.from.y].army - 1;
				gameMap[cur.from.x][cur.from.y].army = 1;
			}
			else
			{
				gameMap[cur.to.x][cur.to.y].army -= gameMap[cur.from.x][cur.from.y].army - 1;
				gameMap[cur.from.x][cur.from.y].army = 1;
				if (gameMap[cur.to.x][cur.to.y].army < 0)
				{
					gameMap[cur.to.x][cur.to.y].army = -gameMap[cur.to.x][cur.to.y].army;
					int p = gameMap[cur.to.x][cur.to.y].team;
					gameMap[cur.to.x][cur.to.y].team = cur.id;
					if (gameMap[cur.to.x][cur.to.y].type == 3)
					{ /* general */
						kill(cur.id, p);
						gameMap[cur.to.x][cur.to.y].type = 4;
						for (auto &mv : inlineMove)
							if (mv.id == p)
								mv.id = cur.id;
					}
				}
			}
		}
	}

	// ranklist printings
	void ranklist(playerCoord coos[])
	{
		struct node
		{
			int id;
			long long army;
			int plain, city, tot;
			long long armyInHand;
		} rklst[64];
		for (int i = 1; i <= playerCnt; ++i)
		{
			rklst[i].id = i;
			rklst[i].army = rklst[i].armyInHand = 0;
			rklst[i].plain = rklst[i].city = rklst[i].tot = 0;
		}
		for (int i = 1; i <= mapH; ++i)
		{
			for (int j = 1; j <= mapW; ++j)
			{
				if (gameMap[i][j].team == 0)
					continue;
				if (gameMap[i][j].type == 2)
					continue;
				++rklst[gameMap[i][j].team].tot;
				if (gameMap[i][j].type == 0)
					++rklst[gameMap[i][j].team].plain;
				else if (gameMap[i][j].type == 4)
					++rklst[gameMap[i][j].team].city;
				else if (gameMap[i][j].type == 3)
					++rklst[gameMap[i][j].team].city;
				rklst[gameMap[i][j].team].army += gameMap[i][j].army;
			}
		}
		for (int i = 1; i <= playerCnt; ++i)
		{
			if (gameMap[coos[i].x][coos[i].y].team != i)
				continue;
			rklst[i].armyInHand = gameMap[coos[i].x][coos[i].y].army;
		}
		std::sort(rklst + 1, rklst + playerCnt + 1, [](node a, node b)
				  { return a.army > b.army; });
		setfcolor(0xffffff);
		underline();
		printf("|     R      A      N      K      L      I      S      T     |");
		resetattr();
		//		setfcolor(0x000000);
		putchar('|');
		putchar('\n');
		setfcolor(0xffffff);
		underline();
		printf("| %7s | %8s | %5s | %5s | %5s | %13s |", "PLAYER", "ARMY", "PLAIN", "CITY", "TOT", "ARMY IN HAND");
		resetattr();
		//		setfcolor(0x000000);
		putchar('|');
		putchar('\n');
		for (int i = 1; i <= playerCnt; ++i)
		{
			if (isAlive[rklst[i].id])
				setfcolor(defTeams[rklst[i].id].color);
			else
				setfcolor(defTeams[10].color);
			underline();
			printf("| %7s | ", defTeams[rklst[i].id].name.c_str());
			if (rklst[i].army < 100000000)
				printf("%8lld | ", rklst[i].army);
			else
			{
				register int p = std::to_string(rklst[i].army * 1.0L / 1e9L).find('.');
				printf("%*.*LfG | ", 7, 7 - 1 - p, rklst[i].army * 1.0L / 1e9L);
			}
			printf("%5d | %5d | %5d | %13lld |", rklst[i].plain, rklst[i].city, rklst[i].tot, rklst[i].armyInHand);
			resetattr();
			//			setfcolor(0x000000);
			putchar('|');
			putchar('\n');
		}
		resetattr();
		setfcolor(0xffffff);
		fflush(stdout);
	}

	// main
	int operator()()
	{
		if (played)
			return -1;
		LGGraphics::inputMapData(20, 20, mapH, mapW);
		LGGraphics::init();
		played = 1;
		gameMesC = 0;
		if (!isWeb)
		{
			int robotId[64];
			playerCoord coordinate[64];
			std::mt19937 mtrd(std::chrono::system_clock::now().time_since_epoch().count());
			for (int i = 2; i <= playerCnt; ++i)
				robotId[i] = mtrd() % 300 + 1;
			//			for(int i=2; i<=playerCnt/2+1; ++i) robotId[i] = 1;
			//			for(int i=playerCnt/2+2; i<=playerCnt; ++i) robotId[i] = 51; // for robot debug
			initGenerals(coordinate);
			updateMap();
			printMap(cheatCode, coordinate[1]);
			std::deque<int> movement;
			curTurn = 0;
			bool gameEnd = 0;
			std::chrono::nanoseconds lPT = std::chrono::steady_clock::now().time_since_epoch();
			for (; is_run(); delay_fps(60))
			{
				if (kbhit())
				{
					int ch = getch();
					switch (ch = tolower(ch))
					{
					case int(' '):
						while (getch() != ' ')
							;
						break;
					case int('c'):
						clearance();
						break;
					case int('w'):
						movement.emplace_back(1);
						break;
					case int('a'):
						movement.emplace_back(2);
						break;
					case int('s'):
						movement.emplace_back(3);
						break;
					case int('d'):
						movement.emplace_back(4);
						break;
					case 224:
					{ /**/
						ch = getch();
						switch (ch)
						{
						case 72: /*[UP]*/
							movement.emplace_back(5);
							break;
						case 75: /*[LEFT]*/
							movement.emplace_back(6);
							break;
						case 80: /*[RIGHT]*/
							movement.emplace_back(7);
							break;
						case 77: /*[DOWN]*/
							movement.emplace_back(8);
							break;
						}
						break;
					}
					case int('g'):
						movement.emplace_back(0);
						break;
					case int('e'):
						if (!movement.empty())
							movement.pop_back();
						break;
					case int('q'):
						movement.clear();
						break;
					case 27:
						MessageBox(nullptr, string("YOU QUIT THE GAME.").c_str(), "EXIT", MB_OK);
						return 0;
					case int('\b'):
					{
						if (!isAlive[1])
							break;
						int confirmSur = MessageBox(nullptr, string("ARE YOU SURE TO SURRENDER?").c_str(), "CONFIRM SURRENDER", MB_YESNO);
						if (confirmSur == 7)
							break;
						isAlive[1] = 0;
						for (int i = 1; i <= mapH; ++i)
						{
							for (int j = 1; j <= mapW; ++j)
							{
								if (gameMap[i][j].team == 1)
								{
									gameMap[i][j].team = 0;
									if (gameMap[i][j].type == 3)
										gameMap[i][j].type = 4;
								}
							}
						}
						++gameMesC;
						gotoxy(mapH + 2 + gameMesC, 65);
						setfcolor(0xffffff);
						fputs("PLAYER ", stdout);
						setfcolor(defTeams[1].color);
						printf("%-7s", defTeams[1].name.c_str());
						setfcolor(0xffffff);
						printf(" SURRENDERED AT TURN %d.", curTurn);
						fflush(stdout);
						break;
					}
					}
				}
				if (std::chrono::steady_clock::now().time_since_epoch() - lPT < std::chrono::milliseconds(stepDelay))
					continue;
				updateMap();
				while (!movement.empty() && analyzeMove(1, movement.front(), coordinate[1]))
					movement.pop_front();
				if (!movement.empty())
					movement.pop_front();
				for (int i = 2; i <= playerCnt; ++i)
				{
					if (!isAlive[i])
						continue;
					switch (robotId[i])
					{
					case 1 ... 100:
						analyzeMove(i, smartRandomBot::smartRandomBot(i, coordinate[i]), coordinate[i]);
						break;
					case 101 ... 200:
						analyzeMove(i, xrzBot::xrzBot(i, coordinate[i]), coordinate[i]);
						break;
					case 201 ... 300:
						analyzeMove(i, xiaruizeBot::xiaruizeBot(i, coordinate[i]), coordinate[i]);
						break;
					default:
						analyzeMove(i, 0, coordinate[i]);
					}
				}
				flushMove();
				if (cheatCode != 1048575)
				{
					int alldead = 0;
					for (int i = 1; i <= playerCnt && !alldead; ++i)
					{
						if (cheatCode & (1 << i))
							if (isAlive[i])
								alldead = 1;
					}
					if (!alldead)
					{
						cheatCode = 1048575;
						MessageBox(nullptr, "ALL THE PLAYERS YOU SELECTED TO BE SEEN IS DEAD.\nTHE OVERALL CHEAT MODE WILL BE SWITCHED ON.", "TIP", MB_OK);
					}
				}
				if (!gameEnd)
				{
					int ed = 0;
					for (int i = 1; i <= playerCnt; ++i)
						ed |= (isAlive[i] << i);
					if (__builtin_popcount(ed) == 1)
					{
						MessageBox(nullptr,
								   ("PLAYER " + defTeams[std::__lg(ed)].name + " WON!" + "\n" +
									"THE GAME WILL CONTINUE." + "\n" +
									"YOU CAN PRESS [ESC] TO EXIT.")
									   .c_str(),
								   "GAME END", MB_OK);
						gameEnd = 1;
						cheatCode = 1048575;
						++gameMesC;
						gotoxy(mapH + 2 + gameMesC, 65);
						setfcolor(0xffffff);
						fputs("PLAYER ", stdout);
						setfcolor(defTeams[std::__lg(ed)].color);
						printf("%-7s", defTeams[std::__lg(ed)].name.c_str());
						setfcolor(0xffffff);
						printf(" WON AT TURN %d!!!", curTurn);
						fflush(stdout);
					}
				}
				gotoxy(1, 1);
				printMap(cheatCode, coordinate[1]);
				ranklist(coordinate);
				printGameMessage();
				fflush(stdout);
				lPT = std::chrono::steady_clock::now().time_since_epoch();
			}
		}
		return 0;
	}
};

int GAME(bool isWeb, int cheatCode, int plCnt, int stDel)
{
	setvbuf(stdout, nullptr, _IOFBF, 5000000);
	hideCursor();
	clearance();
	gotoxy(1, 1);
	int ret = gameStatus(isWeb, cheatCode, plCnt, stDel)();
	setvbuf(stdout, nullptr, _IONBF, 0);
	return ret;
}

#endif // __LGGAME_HPP__
