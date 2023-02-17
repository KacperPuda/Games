#include <iostream>
using namespace std;

#define pA 0
#define pB 1
#define MAXSHIPS 40

struct playerPlace
{
	int xmax=10;
	int ymax=21;
	int xmin=0;
	int ymin=0;
}playerPlace[2],boardPlace;

struct playerFleet
{
	int ShipsInFleet[4];
	int availableShips[4];
	int fleetParts=0;
	int shipsCords[4][80];
	int moves[4][MAXSHIPS];
	int howManyShots[4][MAXSHIPS];
	bool cannonWorking[4][MAXSHIPS];
	bool isThatShipPlaced[4][MAXSHIPS];
	char shipsDirection[4][MAXSHIPS];
	int cannonCords[4][80];
	bool isThatAi = false;
	bool AIplaced = false;
	int spyPlanes[2][100];
	int howManySpyPlanes=0;
}playerFleet[2],AItemp;

void boardClear(char** board, char** radarBoard, char** tempBoard)
{
	for (int i = 0;i < boardPlace.ymax;i++)
	{
		delete[] board[i];
		delete[] radarBoard[i];
		delete[] tempBoard[i];
	}
	delete[] board;
	delete[] radarBoard;
	delete[] tempBoard;
}

void BoardPrint(char** board,int print)
{
	if (print == 0)
	{
		for (int i = 0;i < boardPlace.ymax;i++) 
		{
			for (int j = 0;j < boardPlace.xmax;j++)
			{
				if (board[i][j] == 'x' || board[i][j] == ' '||board[i][j]=='?') cout << board[i][j];
				else	cout << '+';
			}
			cout << endl;
		}
	}
	if (print == 1)
	{
		int bigger=1;
		if (boardPlace.xmax > 10)bigger = 0;
		for(int j=bigger;j<2;j++)
		{
			cout << "  ";
			for (int i = 0;i < boardPlace.xmax;i++)
			{
				int a=0;
				if (j == 0)a = i / 10;
				if (j == 1)a = i % 10;
				cout << a;
			}
			cout << endl;
		}
		for (int i = 0;i < boardPlace.ymax;i++)
		{
			int a=0;
			a=i / 10;
			cout << a;
			a=i % 10;
			cout << a;
			for (int j = 0;j < boardPlace.xmax;j++)
			{
				cout << board[i][j];
			}
			cout << endl;
		}
	}
}

//ustawianie floty graczy
void setfleet(int a, int b, int c, int f,char player)
{
	playerFleet[player - 'A'].availableShips[0] = a;
	playerFleet[player - 'A'].ShipsInFleet[0] = a;
	playerFleet[player - 'A'].availableShips[1] = b;
	playerFleet[player - 'A'].ShipsInFleet[1] = b;
	playerFleet[player - 'A'].availableShips[2] = c;
	playerFleet[player - 'A'].ShipsInFleet[2] = c;
	playerFleet[player - 'A'].availableShips[3] = f;
	playerFleet[player - 'A'].ShipsInFleet[3] = f;
}
//ustawianie wielkosci planszy graczy
void init_position()
{
	char p;
	cin >> p;
	if (p == 'A' || p == 'B')
	{
		cin >> playerPlace[ p - 'A'].ymin;
		cin >> playerPlace[p - 'A'].xmin;
		cin >> playerPlace[p - 'A'].ymax;
		cin >> playerPlace[p - 'A'].xmax;
	}
}
//automatyczne dostosowanie planszy graczy do rozmiarow tablicy
void set_place(int yMax, int xMax)
{
	playerPlace[pA].ymin = 0;
	playerPlace[pA].xmin = 0;
	playerPlace[pA].ymax = yMax / 2 - 1;
	playerPlace[pA].xmax = xMax - 1;
	playerPlace[pB].ymin = yMax / 2 + (yMax % 2);
	playerPlace[pB].xmin = 0;
	playerPlace[pB].ymax = yMax - 1;
	playerPlace[pB].xmax = xMax - 1;
}

bool tooCloseV(char** board, int y, int x, int one, int xmax, int ymax)
{
	if ((y - one >= 0) && (y - one < ymax))
	{
		if (board[y - one][x] == '+'|| board[y - one][x] == 'x'|| board[y - one][x] == '!'|| board[y - one][x] == '@'||board[y - one][x] == '%')return true;
		if (x + 1 < xmax)
		{
			if (board[y - one][x + 1] == '+' || board[y - one][x+1] == 'x' || board[y - one][x+1] == '!' || board[y - one][x+1] == '@' || board[y - one][x+1] == '%')return true;
		}
		if (x - 1 >= 0)
		{
			if (board[y - one][x - 1] == '+' || board[y - one][x-1] == 'x' || board[y - one][x-1] == '!' || board[y - one][x-1] == '@' || board[y - one][x-1] == '%')return true;
		}
	}
	return false;
}

bool tooCloseH(char** board, int y, int x, int one, int xmax, int ymax)
{
	if ((x - one >= 0) && (x - one < xmax))
	{
		if (board[y][x - one] == '+' || board[y][x - one] == 'x' || board[y][x - one] == '!' || board[y][x - one] == '@' || board[y][x - one] == '%')return true;
		if (y + 1 < ymax)
		{
			if (board[y + 1][x - one] == '+' || board[y+1][x - one] == 'x' || board[y+1][x - one] == '!' || board[y+1][x - one] == '@' || board[y+1][x - one] == '%')return true;
		}
		if (y - 1 >= 0)
		{
			if (board[y - 1][x - one] == '+' || board[y-1][x - one] == 'x' || board[y-1][x - one] == '!' || board[y-1][x - one] == '@' || board[y-1][x - one] == '%')return true;
		}
	}
	return false;
}

bool canBePlacedV(char** board, int yShip, int xShip, int one, int shipLength)
{
	int xmax, ymax, j, z;
	xmax = boardPlace.xmax;
	ymax = boardPlace.ymax;
	if (tooCloseV(board, yShip, xShip, one, xmax, ymax))return false;
	z = yShip;
	for (j = 0;j < shipLength;j++)
	{
		if (board[z][xShip] == '#')return false;
		if (tooCloseV(board, z, xShip, 0, xmax, ymax))return false;
		board[z][xShip] = 's';
		z = z + one;
		if (z < 0||z>=ymax) return false;
	}
	if (tooCloseV(board, z, xShip, 0, xmax, ymax))	return false;
	return true;
}

bool canBePlacedH(char** board, int yShip, int xShip, int one, int shipLength)
{
	int xmax, ymax, j, z;
	xmax = boardPlace.xmax;
	ymax = boardPlace.ymax;
	if (tooCloseH(board, yShip, xShip, one, xmax, ymax)) return false;
	z = xShip;
	for (j = 0;j < shipLength;j++)
	{
		if (board[yShip][z] == '#')return false;
		if (tooCloseH(board, yShip, z, 0, xmax, ymax))return false;
		board[yShip][z] = 's';
		z = z + one;
		if (z < 0 || z >= xmax) return false;
	}
	if (tooCloseH(board, yShip, z, 0, xmax, ymax))return false;
	return true;
}

void setLJdirection(int* j, int* l, char direction)
{
	if (direction == 'N') *j = 1;
	if (direction == 'S') *j = -1;
	if (direction == 'W') *l = 1;
	if (direction == 'E') *l = -1;
}

void validation(int validNumber, char type[10], char player, int yShip, int xShip, char directionShip, int i, char classShip[4], char where, char shipState[6], int shipLength, char** board, char** tempBoard,char** radarBoard)
{
	if ((strcmp(type, "starting") == 0) && (validNumber == 2)) return;

	if (validNumber == 0)
	{
		cout << "INVALID OPERATION " << '"' << "SHIP " << player << " " << yShip << " " << xShip << " " << directionShip << " " << i << " " << classShip << " ";
		for (int j = 0;j < shipLength;j++)cout << shipState[j];
		cout << '"';
	}
	if (validNumber == 1) cout << "INVALID OPERATION " << '"' << "PLACE_SHIP " << yShip << " " << xShip << " " << directionShip << " " << i << " " << classShip << '"';
	if (validNumber == 2) cout << "INVALID OPERATION " << '"' << "MOVE " << i << " " << classShip[0] << classShip[1] << classShip[2] << " " << where << '"';
	if (validNumber == 3) cout << "INVALID OPERATION " << '"' << "SHOOT " << yShip << " " << xShip << '"';
	if (validNumber == 4) cout << "INVALID OPERATION " << '"' << "REEF " << yShip << " " << xShip << '"';
	if (validNumber == 5) cout << "INVALID OPERATION " << '"' << "SHOOT " << i << " " << classShip << " " << yShip << " " << xShip << '"';
	if (validNumber == 6) cout << "INVALID OPERATION " << '"' << "SPY " << i << " " << yShip << " " << xShip << '"';

	if (strcmp(type, "starting") == 0) cout << ": NOT IN STARTING POSITION" << "\n";
	if (strcmp(type, "reef") == 0) cout << ": PLACING SHIP ON REEF" << "\n";
	if (strcmp(type, "tooclose") == 0) cout << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << "\n";
	if (strcmp(type, "already") == 0) cout << ": SHIP ALREADY PRESENT" << "\n";
	if (strcmp(type, "all") == 0) cout << ": ALL SHIPS OF THE CLASS ALREADY SET" << "\n";
	if (strcmp(type, "went") == 0) cout << ": SHIP WENT FROM BOARD" << "\n";
	if (strcmp(type, "shootall") == 0) cout <<  ": NOT ALL SHIPS PLACED" << "\n";
	if (strcmp(type, "shootnot") == 0) cout << ": FIELD DOES NOT EXIST" << "\n"; 
	if (strcmp(type, "reefnot") == 0) cout << ": REEF IS NOT PLACED ON BOARD" << "\n";
	if (strcmp(type, "movedA") == 0) cout << ": SHIP MOVED ALREADY" << "\n";
	if (strcmp(type, "cantmove") == 0) cout << ": SHIP CANNOT MOVE" << "\n";
	if (strcmp(type, "notAshoot") == 0) cout << ": TOO MANY SHOOTS" << "\n";
	if (strcmp(type, "cannonnot") == 0) cout << ": SHIP CANNOT SHOOT" << "\n";
	if (strcmp(type, "toofar") == 0) cout << ": SHOOTING TOO FAR" << "\n";
	if (strcmp(type, "spyCbroke") == 0) cout << ": CANNOT SEND PLANE" << "\n";
	if (strcmp(type, "allspy") == 0) cout << ": CANNOT SEND PLANE" << "\n";
	boardClear(board,radarBoard,tempBoard);
	exit(0);
}
//ustawianie radaru w zaleznosci od odleglosci
void setRadar(char** board, char** radarBoard,int zMax,int lMax,int zMin,int lMin,int AI,int yShip,int xShip,int shipLength,bool isSpy)
{
	for (int z = zMax - 1;z <= zMin + 1;z++)
	{
		if ((z >= 0) && (z < boardPlace.ymax))
		{
			for (int l = lMax - 1;l <= lMin + 1;l++)
			{
				if ((l >= 0) && (l < boardPlace.xmax))
				{
					double distance;
					distance = sqrt(pow(yShip - z, 2) + pow(xShip - l, 2));
					if (isSpy)distance = 0;
					if (distance <= shipLength)
					{
						radarBoard[z][l] = board[z][l];
						if ((AI == 1) && (board[z][l] == '@' || board[z][l] == '+' || board[z][l] == '!' || board[z][l] == '%')) radarBoard[z][l] = 'P';
					}
				}
			}
		}
	}
}
//wypisywanie radaru
void radarPrint(char** board, char player, bool spyPlane, char** radarBoard,int print,int AI, char** tempBoard)
{
	int i, j, xShip = 0, yShip = 0, directionInY = 0, directionInX = 0, plusY = 0, plusX = 0, shipLength = 0;
	char direction;

	if (spyPlane)
	{
		int iShip, xSpy = 0, ySpy = 0;
		cin >> iShip;
		cin >> ySpy;
		cin >> xSpy;
		if (board[playerFleet[player - 'A'].cannonCords[0][iShip * 2]][playerFleet[player - 'A'].cannonCords[0][iShip * 2 + 1]]=='x')
		{
			char validType[10] = { 's','p','y','C','b','r','o','k','e' };
			validation(6, validType, player, ySpy, xSpy, NULL, iShip, NULL, NULL, NULL, NULL, board,tempBoard,radarBoard);
		}
		playerFleet[player - 'A'].howManyShots[0][iShip]--;
		if (playerFleet[player - 'A'].howManyShots[0][iShip] < 0 || iShip >= playerFleet[player - 'A'].ShipsInFleet[0])
		{
			char validType[10] = { 'a','l','l','s','p','y' };
			validation(6, validType, player, ySpy, xSpy, NULL, iShip, NULL, NULL, NULL, NULL, board, tempBoard, radarBoard);
		}
		playerFleet[player - 'A'].spyPlanes[0][playerFleet[player - 'A'].howManySpyPlanes] = ySpy;
		playerFleet[player - 'A'].spyPlanes[1][playerFleet[player - 'A'].howManySpyPlanes] = xSpy;
		playerFleet[player - 'A'].howManySpyPlanes++;
	}
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < playerFleet[player - 'A'].ShipsInFleet[i];j++)
		{
			if (playerFleet[player - 'A'].isThatShipPlaced[i][j])
			{
				xShip = playerFleet[player - 'A'].shipsCords[i][j * 2 + 1];
				yShip = playerFleet[player - 'A'].shipsCords[i][j * 2];
				shipLength = 5 - i;
				if (board[yShip][xShip] == 'x') shipLength = 1;
				setRadar(board,radarBoard, yShip - shipLength, xShip - shipLength, yShip + shipLength, xShip + shipLength,AI,yShip,xShip,shipLength,false);
			}
		}
	}
	for (int m = 0;m < playerFleet[player - 'A'].howManySpyPlanes;m++) setRadar(board, radarBoard, playerFleet[player - 'A'].spyPlanes[0][m], playerFleet[player - 'A'].spyPlanes[1][m], playerFleet[player - 'A'].spyPlanes[0][m], playerFleet[player - 'A'].spyPlanes[1][m], AI, 0, 0, 100, true);
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < playerFleet[player - 'A'].ShipsInFleet[i];j++)
		{
			if (playerFleet[player - 'A'].isThatShipPlaced[i][j])
			{
				plusX = 0;
				plusY = 0;
				xShip = playerFleet[player - 'A'].shipsCords[i][j * 2 + 1];
				yShip = playerFleet[player - 'A'].shipsCords[i][j * 2];
				direction = playerFleet[player - 'A'].shipsDirection[i][j];
				directionInX = 0;
				directionInY = 0;
				setLJdirection(&directionInY, &directionInX, direction);
				for (int n = 0;n < 5 - i;n++)
				{
					radarBoard[yShip + plusY][xShip + plusX] = board[yShip + plusY][xShip + plusX];
					if (AI == 1)radarBoard[yShip + plusY][xShip + plusX] = 'A';
					plusX += directionInX;
					plusY += directionInY;
				}
				directionInX = 0;
				directionInY = 0;
			}
		}
	}
	if (!spyPlane)BoardPrint(radarBoard, print);	
}


void placeingV(char** board, int one, int shipLength, int xShip, int yShip, char directionShip, int i, char classShip[4], int ymax, int ymin, int xmax, char shipState[5], int w, char player, char where,char** radarBoard, char** tempBoard)
{
	if ((yShip + (shipLength * one) - one > ymax || yShip + (shipLength * one) - one < ymin)&&(w!=0)&&(!playerFleet[player-'A'].isThatAi))
	{
		char validType[10] = { 's','t','a','r','t','i','n','g' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength,board, tempBoard, radarBoard);
	}
	if (tooCloseV(board, yShip, xShip, one, xmax, ymax))
	{
		char validType[10] = { 't','o','o','c','l','o','s','e' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
	}
	int j, z;
	z = yShip;
	for (j = 0;j < shipLength;j++)
	{
		if (board[z][xShip] == '#')
		{
			char validType[10] = { 'r','e','e','f' };
			validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
		}
		if (tooCloseV(board, z, xShip, 0, xmax, ymax))
		{
			char validType[10] = { 't','o','o','c','l','o','s','e' };
			validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
		}
		if (shipState[j] == '1')
		{
			board[z][xShip] = '+';
			if(j==0)board[z][xShip] = '@';
			if (shipLength == 2)
			{
				if(j==shipLength-1) board[z][xShip] = '%';
			}
			else
			{
				if (j == 1) board[z][xShip] = '!';
				if (j == shipLength - 1) board[z][xShip] = '%';
			}
			if (j == 1)playerFleet[player - 'A'].cannonWorking[5-shipLength][i] = true;
		}
		else board[z][xShip] = 'x';
		z = z + one;
	}
	if (tooCloseV(board, z, xShip, 0, xmax, ymax))
	{
		char validType[10] = { 't','o','o','c','l','o','s','e' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
	}
	playerFleet[player - 'A'].cannonCords[5 - shipLength][i * 2] = yShip + one;
	playerFleet[player - 'A'].cannonCords[5 - shipLength][i * 2 + 1] = xShip;
}

void placeingH(char** board, int one, int shipLength, int xShip, int yShip, char directionShip, int i, char classShip[4], int xmax, int xmin, int ymax, char shipState[5], int w, char player, char where, char** radarBoard, char** tempBoard)
{
	if ((xShip + (shipLength * one) - one < xmin || xShip + (shipLength * one) - one > xmax) && (w != 0) && (!playerFleet[player - 'A'].isThatAi))
	{
		char validType[10] = { 's','t','a','r','t','i','n','g' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
	}
	if (tooCloseH(board, yShip, xShip, one, xmax, ymax))
	{
		char validType[10] = { 't','o','o','c','l','o','s','e' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
	}
	int j, z;
	z = xShip;
	for (j = 0;j < shipLength;j++)
	{
		if (board[yShip][z] == '#')
		{
			char validType[10] = { 'r','e','e','f' };
			validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
		}
		if (tooCloseH(board, yShip, z, 0, xmax, ymax))
		{
			char validType[10] = { 't','o','o','c','l','o','s','e' };
			validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board, tempBoard, radarBoard);
		}
		if (shipState[j] == '1')
		{
			board[yShip][z] = '+';
			if (j == 0)board[yShip][z] = '@';
			if (shipLength == 2)
			{
				if (j == shipLength - 1) board[yShip][z] = '%';
			}
			else
			{
				if (j == 1) board[yShip][z] = '!';
				if (j == shipLength - 1) board[yShip][z] = '%';
			}
			if(j==1)playerFleet[player - 'A'].cannonWorking[5 - shipLength][i] = true;
		}	
		else board[yShip][z] = 'x';
		
		z = z + one;
	}
	if (tooCloseH(board, yShip, z, 0, xmax, ymax))
	{
		char validType[10] = { 't','o','o','c','l','o','s','e' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, where, shipState, shipLength, board,tempBoard,radarBoard);
	}
	playerFleet[player - 'A'].cannonCords[5 - shipLength][i * 2] = yShip;
	playerFleet[player - 'A'].cannonCords[5 - shipLength][i * 2 + 1] = xShip+ one;
}
//sprawdzanie czy mozna wylazyc kolejne statki danego typu
void checkingShips(int xShip, int yShip, char directionShip, int i, char classShip[4], int iShip, int asision, int w, char player, int shipLength, char shipState[6],char** board, char** radarBoard, char** tempBoard)
{
	if (playerFleet[player-'A'].isThatShipPlaced[iShip][i])
	{
		char validType[10] = { 'a','l','r','e','a','d','y' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, ' ', shipState, shipLength, board, tempBoard, radarBoard);
	}
	else
	{
		playerFleet[player - 'A'].isThatShipPlaced[iShip][i] = true;
		playerFleet[player - 'A'].availableShips[asision]--;
		if (playerFleet[player - 'A'].availableShips[asision] < 0 || i> playerFleet[player - 'A'].ShipsInFleet[asision])
		{
			char validType[10] = { 'a','l','l' };
			validation(w, validType, player, yShip, xShip, directionShip, i, classShip, ' ', shipState, shipLength, board, tempBoard, radarBoard);
		}
	}
	playerFleet[player - 'A'].shipsCords[iShip][i * 2] = yShip;
	playerFleet[player - 'A'].shipsCords[iShip][(i * 2) + 1] = xShip;
}



void place_ship(char** board, char player, int w,int yShip,int xShip, char directionShip,int i, char classShipAI[4],char shipState[6],bool AddParts, bool playerPlay, char** radarBoard, char** tempBoard)
{
	int shipLength = 2, negativeParts = 0,iShip;
	char  classShip[4] = {};
	if (playerPlay)	cin >> yShip>>xShip>>directionShip>>i>>classShip;
	if ((playerFleet[player - 'A'].isThatAi) && (!playerPlay))
	{
		classShip[0] = classShipAI[0];
		classShip[1] = classShipAI[1];
		classShip[2] = classShipAI[2];
	}
	if (w == 0)
	{
		for (int z=0;z < 5;z++) shipState[z] = NULL;
		cin >> shipState;
		for (int j = 0;j < 5;j++) if (shipState[j] == '0') negativeParts++;
	}
	if ((xShip > playerPlace[player - 'A'].xmax || yShip > playerPlace[player - 'A'].ymax || yShip < playerPlace[player - 'A'].ymin || xShip < playerPlace[player - 'A'].xmin) && (w != 0)&&(!playerFleet[player - 'A'].isThatAi))
	{
		char validType[10] = { 's','t','a','r','t','i','n','g' };
		validation(w, validType, player, yShip, xShip, directionShip, i, classShip, NULL, shipState, shipLength, board,tempBoard,radarBoard);
	}
	if (classShip[2] == 'R')shipLength = 5;
	if (classShip[2] == 'T')shipLength = 4;
	if (classShip[2] == 'U')shipLength = 3;
	if (classShip[2] == 'S')shipLength = 2;
	iShip = 5 - shipLength;
	if (AddParts)playerFleet[player - 'A'].fleetParts = playerFleet[player - 'A'].fleetParts + shipLength - negativeParts;
	checkingShips(xShip, yShip, directionShip, i, classShip, iShip, iShip, w, player, shipLength, shipState, board,radarBoard,tempBoard);
	playerFleet[player - 'A'].shipsDirection[5 - shipLength][i] = directionShip;
	playerFleet[player - 'A'].isThatShipPlaced[5 - shipLength][i] = true;
	if(directionShip =='S')placeingV(board, -1, shipLength, xShip, yShip, directionShip, i, classShip, playerPlace[player - 'A'].ymax, playerPlace[player - 'A'].ymin, playerPlace[player - 'A'].xmax, shipState, w, player, ' ',radarBoard,tempBoard);
	if (directionShip == 'N')placeingV(board, 1, shipLength, xShip, yShip, directionShip, i, classShip, playerPlace[player - 'A'].ymax, playerPlace[player - 'A'].ymin, playerPlace[player - 'A'].xmax, shipState, w, player, ' ', radarBoard, tempBoard);
	if (directionShip == 'E')	placeingH(board, -1, shipLength, xShip, yShip, directionShip, i, classShip, playerPlace[player - 'A'].xmax, playerPlace[player - 'A'].xmin, playerPlace[player - 'A'].ymax, shipState, w, player, ' ', radarBoard, tempBoard);
	if (directionShip == 'W')	placeingH(board, 1, shipLength, xShip, yShip, directionShip, i, classShip, playerPlace[player - 'A'].xmax, playerPlace[player - 'A'].xmin, playerPlace[player - 'A'].ymax, shipState, w, player, ' ', radarBoard, tempBoard);
}
//walidacja strzalania
void extendedShootCheck(int shipType, int i,char classShip[4],char player,char**board,int *cannonY,int *cannonX,int *shipLength,int yship,int xship,char** radarBoard,char ** tempBoard)
{
	*cannonY = playerFleet[player - 'A'].cannonCords[shipType][i * 2];
	*cannonX = playerFleet[player - 'A'].cannonCords[shipType][i * 2 +1];
	*shipLength = 5 - shipType;
	if (board[*cannonY][*cannonX] == 'x')
	{
		playerFleet[player - 'A'].cannonWorking[shipType][i] = false;
	}
	if (playerFleet[player - 'A'].cannonWorking[shipType][i])
	{
		playerFleet[player - 'A'].howManyShots[shipType][i]--;
		if (playerFleet[player - 'A'].howManyShots[shipType][i] < 0)
		{
			char validType[10] = { 'n','o','t','A','s','h','o','o','t' };
			validation(5, validType, player, yship, xship, NULL, i, classShip, NULL, NULL, NULL, board, tempBoard, radarBoard);
		}
	}
	else
	{
		char validType[10] = { 'c','a','n','n','o','n','n','o','t' };
		validation(5, validType, player, yship, xship, NULL, i, classShip, NULL, NULL, NULL, board, tempBoard, radarBoard);
	}
}
//SPRAWDZANIE CZY MOZNA STRZELAC
bool canPlayerShoot()
{
	for (int i = 0;i < 2;i++)
	for (int j = 0;j < 4;j++)if (playerFleet[i].availableShips[j] != 0)return false;
	return true;
}
//STRZELANIE
bool shoot(char** board,bool extendedOn,char player,char ** radarBoard,char** tempBoard)
{
	int x, y,cannonX=0,cannonY=0,shipLength=0,iship=0;
	char classShip[4];
	if (extendedOn)
	{
		cin >> iship;
		cin >> classShip;
	}	 
	cin >> y;
	cin >> x;
	int w = 3;
	if (!canPlayerShoot())
	{
		char validType[10] = { 's','h','o','o','t','a','l','l' };
		validation(w, validType, NULL, y, x, NULL, NULL, NULL, NULL, NULL, NULL, board, tempBoard, radarBoard);
	}
	if (extendedOn)
	{
	if (strcmp(classShip, "CAR") == 0) extendedShootCheck(0, iship, classShip, player, board,&cannonY,&cannonX,&shipLength,y,x, radarBoard, tempBoard);
	if (strcmp(classShip, "BAT") == 0) extendedShootCheck(1, iship, classShip, player, board, &cannonY, &cannonX, &shipLength,y, x, radarBoard, tempBoard);
	if (strcmp(classShip, "CRU") == 0) extendedShootCheck(2, iship, classShip, player, board, &cannonY, &cannonX, &shipLength, y, x, radarBoard, tempBoard);
	if (strcmp(classShip, "DES") == 0) extendedShootCheck(3, iship, classShip, player, board, &cannonY, &cannonX, &shipLength, y, x, radarBoard, tempBoard);
	//obliczanie dystansu
	double distance;
	distance = sqrt(pow(cannonY - y, 2) + pow(cannonX - x, 2));
	if (strcmp(classShip, "CAR") == 0) distance = 0;
	if (distance > shipLength)
	{
		char validType[10] = { 't','o','o','f','a','r' };
		validation(5, validType, NULL, y, x, NULL, iship, classShip, NULL, NULL, NULL, board, tempBoard, radarBoard);
	}
	}	
	if (x > boardPlace.xmax || y > boardPlace.ymax)
	{
		char validType[10] = { 's','h','o','o','t','n','o','t' };
		validation(w, validType, NULL, y, x, NULL, NULL, NULL, NULL, NULL, NULL, board, tempBoard, radarBoard);
	}
	if (board[y][x] == '+'|| board[y][x] == '@'|| board[y][x] == '%'|| board[y][x] == '!')
	{
		board[y][x] = 'x';
		return true;
	}
	return false;
}

void placeReef(char** board, int y, int x,char** radarBoard,char** tempBoard)
{
	int i, j;
	cin >> i;
	cin >> j;
	if (i >= y || j >= x)
	{
		char validType[10] = { 'r','e','e','f','n','o','t' };
		validation(4, validType, NULL, i, j, NULL, NULL, NULL, NULL, NULL, NULL, board, tempBoard, radarBoard);
	}
	board[i][j] = '#';
}

//sprawdzanie stanu statku
void checkShipState(char** board, int xShip, int yShip, int  shipLength, char state[6], bool *czy,char direction)
{
	int j=0, l=0;
	setLJdirection(&j, &l, direction);
	
	int i, plusV = 0, plusH = 0;
	for (i = 0;i < shipLength; i++)
	{
		if (board[yShip + plusV][xShip + plusH] == '+'||board[yShip + plusV][xShip + plusH] == '!'|| board[yShip + plusV][xShip + plusH] == '@'|| board[yShip + plusV][xShip + plusH] == '%') state[i] = '1';
		if (board[yShip + plusV][xShip + plusH] == 'x') state[i] = '0';
		if (i == shipLength - 1)
		{
			if (board[yShip + plusV][xShip + plusH] == 'x') *czy = false;		
		}
		if (l == 0) plusV = (i + 1) * j;
		if (j == 0) plusH = (i + 1) * l;
	}
	
}
//czyszczenie planszy przed zmiena pozycji statku
void clearShip(char** board, int xShip, int yShip, int shipLength,char direction)
{
	int j = 0, l = 0;
	setLJdirection(&j, &l, direction);

	int i, plusV = 0, plusH = 0;
	for (i = 0;i < shipLength; i++)
	{
		board[yShip + plusV][xShip + plusH] = ' ';
		if (l == 0) plusV = (i + 1) * j;
		if (j == 0) plusH = (i + 1) * l;
	}
}

void changingDirection(char *direction,int *l,int  *j,char where,int *xShip,int *yShip,int shipLength)
{
	if (where == 'F')
	{
		*xShip -= *l;
		*yShip -= *j;
	}
	if (where == 'R')
	{
		if (*direction == 'N' || *direction == 'S')
		{
			*yShip -= *j;
			*xShip += (shipLength - 1) * (*j);
			if (*direction == 'N')*direction = 'E';
			else *direction = 'W';
		}
		else
		{
			*yShip -= (shipLength - 1) * (*l);
			*xShip -= *l;
			if (*direction == 'W')*direction = 'N';
			else *direction = 'S';
		}
	}
	if (where == 'L')
	{
		if (*direction == 'N' || *direction == 'S')
		{
			*yShip -= *j;
			*xShip -= (shipLength - 1) * (*j);
			if (*direction == 'N')*direction = 'W';
			else *direction = 'E';
		}
		else
		{
			*yShip += (shipLength - 1) * (*l);
			*xShip -= *l;
			if (*direction == 'W')*direction = 'S';
			else *direction = 'N';
		}
	}
}

void Move(char** board, char player, bool extendedOn,int i,char classShipAI[4],char where,char state[6],bool AIplay,char** radarBoard,char** tempBoard)
{
	int classCord = 0, yShip, xShip, shipLength = 2, j = 0, l = 0, xmin = 0, ymin = 0;
	char direction = ' ', classShip[4] = {};
	bool czy = true,isAI= playerFleet[player - 'A'].isThatAi;
	if (!AIplay) cin >> i >> classShip>> where;
	if (AIplay)
	{
	classShip[0] = classShipAI[0];
	classShip[1] = classShipAI[1];
	classShip[2] = classShipAI[2];
	}
	//ustalenie dlugosci i rozaju statku
	if (strcmp(classShip, "CAR") == 0)shipLength = 5;
	if (strcmp(classShip, "BAT") == 0)shipLength = 4;
	if (strcmp(classShip, "CRU") == 0)shipLength = 3;
	if (strcmp(classShip, "DES") == 0)shipLength = 2;
	classCord = 5 - shipLength;
	playerFleet[player - 'A'].moves[classCord][i] --;
	if (playerFleet[player - 'A'].moves[classCord][i] < 0)
	{
		if (isAI) return;
		char validType[10] = { 'm','o','v','e','d','A' };
		validation(2, validType, player, NULL, NULL, NULL, i, classShip, where, NULL, shipLength, board,tempBoard,radarBoard);
	}
	//przypisanie koordynatow statku
	yShip = playerFleet[player-'A'].shipsCords[classCord][i * 2];
	xShip = playerFleet[player - 'A'].shipsCords[classCord][(i * 2) + 1];
	//sprawdzanie kierunku statku
	direction = playerFleet[player - 'A'].shipsDirection[classCord][i];
	//przypisywanie kierunku
	if(direction=='N')j = 1;
	if (direction == 'S')j = -1;
	if (direction == 'W')l = 1;
	if (direction == 'E')l = -1;
	//sprawdzanie stanu statku i czyszczenie
	checkShipState(board, xShip, yShip, shipLength, state, &czy,direction);
	if ((extendedOn) && (czy == false))
	{
		if (isAI) return;
		char validType[10] = { 'c','a','n','t','m','o','v','e'};
		validation(2, validType, player, yShip, xShip, ' ', i, classShip, where, NULL, shipLength, board, tempBoard, radarBoard);
	}
	clearShip(board, xShip, yShip, shipLength,direction);
	//obliczanie nowych koordynatow
	changingDirection(&direction, &l, &j, where, &xShip, &yShip, shipLength);
	if (xShip >= boardPlace.xmax || yShip >= boardPlace.ymax || xShip < 0 || yShip < 0)
	{
		if (isAI) return;
		char validType[10] = { 'w','e','n','t' };
		validation(2, validType, player, yShip, xShip, ' ', i, classShip, where, NULL, shipLength, board, tempBoard, radarBoard);
	}
	//kladzenie nowych statkow
	if((AIplay) && (!canBePlacedV(board, yShip, xShip, -1, shipLength))&&(direction=='S')) return;
	if ((AIplay) && (!canBePlacedV(board, yShip, xShip, 1, shipLength)) && (direction == 'N')) return;
	if ((AIplay) && (!canBePlacedH(board, yShip, xShip, -1, shipLength)) && (direction == 'E')) return;
	if ((AIplay) && (!canBePlacedH(board, yShip, xShip, 1,shipLength)) && (direction == 'W')) return;
	if(direction=='S')	placeingV(board, -1, shipLength, xShip, yShip, direction, i, classShip, boardPlace.ymax, ymin, boardPlace.xmax, state, 2, player, where, radarBoard, tempBoard);
	if(direction=='N')	placeingV(board, 1, shipLength, xShip, yShip, direction, i, classShip, boardPlace.ymax, ymin, boardPlace.xmax, state, 2, player, where, radarBoard, tempBoard);
	if(direction=='E')	placeingH(board, -1, shipLength, xShip, yShip, direction, i, classShip, boardPlace.xmax, xmin, boardPlace.ymax, state, 2, player, where, radarBoard, tempBoard);
	if(direction=='W')	placeingH(board, 1, shipLength, xShip, yShip, direction, i, classShip, boardPlace.xmax, xmin, boardPlace.ymax, state, 2, player, where, radarBoard, tempBoard);
	//ustawianie nowych koordynatow
	playerFleet[player - 'A'].shipsCords[classCord][i * 2] = yShip;
	playerFleet[player - 'A'].shipsCords[classCord][(i * 2) + 1] = xShip;
	playerFleet[player - 'A'].shipsDirection[classCord][i] = direction;
}
//zapisywanie stanu gry
void save(char player,char **board)
{
	cout << "[state]" << endl;
	cout << "BOARD_SIZE " << boardPlace.ymax<<" "<<boardPlace.xmax << endl;
	cout << "NEXT_PLAYER "<<player << endl;
	player = 'A';
	for (int i2 = 0;i2 < 2;i2++)
	{
		cout << "INIT_POSITION " << player << " " << playerPlace[player - 'A'].ymin << " " << playerPlace[player - 'A'].xmin << " " << playerPlace[player - 'A'].ymax << " " << playerPlace[player - 'A'].xmax << endl;
		cout << "SET_FLEET " << player;
		for (int j = 0;j < 4;j++) cout << " " << playerFleet[player - 'A'].ShipsInFleet[j];
		cout << endl;
		for (int j = 0;j < 4;j++)
		{
			for (int i = 0;i < playerFleet[player - 'A'].ShipsInFleet[j];i++)
			{
				if (playerFleet[player - 'A'].isThatShipPlaced[j][i])
				{
					cout << "SHIP " << player << " " << playerFleet[player - 'A'].shipsCords[j][i * 2] << " " << playerFleet[player - 'A'].shipsCords[j][i * 2 + 1] << " " << playerFleet[player - 'A'].shipsDirection[j][i] << " " << i << " ";
					if (j == 0) cout << "CAR ";
					if (j == 1) cout << "BAT ";
					if (j == 2) cout << "CRU ";
					if (j == 3) cout << "DES ";
					int shipLength,l=0,z=0;
					char state[6] = { '0','0','0','0','0' },direction;
					direction = playerFleet[player - 'A'].shipsDirection[j][i];
					setLJdirection(&z, &l, direction);
					shipLength = 5 - j;
					checkShipState(board, playerFleet[player - 'A'].shipsCords[j][i * 2 + 1], playerFleet[player - 'A'].shipsCords[j][i * 2], shipLength, state,NULL,direction);
					for (int o = 0;o < shipLength;o++) cout << state[o];
					cout << endl;
				}
			}
		}
		player = 'B';
	}
	cout << "[state]" << endl;
}

void randomYX(int *x,int *y)
{
	*y = rand() % boardPlace.ymax;
	*x = rand() % boardPlace.xmax;
}

void clearBoardFromS(char** board)
{
	for (int b = 0;b < boardPlace.ymax;b++)
		for (int v = 0;v < boardPlace.xmax;v++) if (board[b][v] == 's')board[b][v] = ' ';
}
//RUSZANIE STATKAMI BOTA
void AImove(char** board,char AI,bool extendedOn,char** radarBoard,char** tempBoard)
{
	
	char where = ' ', type[4], state[6] = {}, direction;
	int randomWhere=0,x,y,shipLength=0;
	bool isMoved = false;
	for (int i = 0;i < 4;i++)
	{
		if (playerFleet[AI - 'A'].ShipsInFleet[i] == 0) continue;
		if (i == 0)strcpy_s(type, "CAR");
		if (i == 1)strcpy_s(type, "BAT");
		if (i == 2)strcpy_s(type, "CRU");
		if (i == 3)strcpy_s(type, "DES");
		shipLength = 5 - i;
		for (int g = 0;g < shipLength; g++) state[g] = ' ';
		for (int j = 0;j < playerFleet[AI - 'A'].ShipsInFleet[i];j++)
		{
			direction = playerFleet[AI - 'A'].shipsDirection[i][j];
			isMoved = false;
			x = playerFleet[AI - 'A'].shipsCords[i][(j * 2) + 1];
			y = playerFleet[AI - 'A'].shipsCords[i][j * 2];
			randomWhere = rand() % 3;
			if (randomWhere == 0) where = 'F';
			if (randomWhere == 1) where = 'R';
			if (randomWhere == 2) where = 'L';
			Move(board,AI,extendedOn,j,type,where,state,true, radarBoard, tempBoard);
			if (x != playerFleet[AI - 'A'].shipsCords[i][j * 2 + 1])isMoved = true;
			if (y != playerFleet[AI - 'A'].shipsCords[i][j * 2])isMoved = true;
			if (!isMoved)
			{
				playerFleet[AI - 'A'].isThatShipPlaced[i][j] = false;
				playerFleet[AI - 'A'].availableShips[i]++;
				clearShip(board, x, y, shipLength, direction);
				place_ship(board, AI, 1, y, x, playerFleet[AI - 'A'].shipsDirection[i][j], j, type,state,false,false, radarBoard, tempBoard);
			}
			else
			{
				cout << "MOVE " << j << " " << type << " " << where << endl;
				playerFleet[AI - 'A'].moves[i][j] --;
				clearBoardFromS(board);
			}
			clearBoardFromS(board);
		}
	}
}

void winCheck(char player, char** radarBoard, char** tempBoard, char** board)
{
	char otherplayer;
	if (player == 'A')otherplayer = 'B';
	else otherplayer = 'A';
	if (playerFleet[otherplayer - 'A'].fleetParts == 0)
	{
		cout << player << " won" << "\n";
		boardClear(board, radarBoard, tempBoard);
		exit(0);
	}
}

bool AIseeingOponent(char** radarBoard,char AI,int i,int j,int shiplength,int *yShoot,int *xShoot)
{
	for (int ry = playerFleet[AI - 'A'].cannonCords[i][j * 2] - shiplength - 1;ry < playerFleet[AI - 'A'].cannonCords[i][j * 2] + shiplength + 1;ry++)
	{
		if ((ry >= 0) && (ry < boardPlace.ymax))
		{
			for (int rx = playerFleet[AI - 'A'].cannonCords[i][(j * 2) + 1] - shiplength - 1;rx < playerFleet[AI - 'A'].cannonCords[i][(j * 2) + 1] + shiplength + 1;rx++)
			{
				if ((rx >= 0) && (rx < boardPlace.xmax))
				{
					if (radarBoard[ry][rx] == 'P')
					{
						*xShoot = rx;
						*yShoot = ry;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool cannonCheck(char AI,char** board,int i,int j)
{
	if (board[playerFleet[AI - 'A'].cannonCords[i][j*2]][playerFleet[AI - 'A'].cannonCords[i][(j*2)+1]] == 'x') return false;
	else return true;
}
//STRZALY BOTA
void extendedAIshoot(char AI, char** board, char** radarBoard,char** tempBoard)
{
	int xShoot=0, yShoot=0;
	bool AIsee = false;
	char type[4],otherplayer;
	if (AI== 'A')otherplayer = 'B';
	else otherplayer = 'A';
	radarPrint(board, AI, false, radarBoard,2, 1,tempBoard);
	for (int i = 0;i < 4;i++)
	{
		if (i == 0)strcpy_s(type, "CAR");
		if (i == 1)strcpy_s(type, "BAT");
		if (i == 2)strcpy_s(type, "CRU");
		if (i == 3)strcpy_s(type, "DES");
		int shiplength = 5 - i;
		for (int j = 0;j < playerFleet[AI - 'A'].ShipsInFleet[i];j++)
		{
			if (!playerFleet[AI - 'A'].cannonWorking[i][j])continue;
			for (int m = 0;m < 5 - i;m++)
			{
				radarPrint(board, AI, false, radarBoard, 2, 1,tempBoard);
				AIsee=AIseeingOponent(radarBoard, AI, i, j, shiplength, &yShoot, &xShoot);					
				if (AIsee)
				{
					double distance;
					distance = sqrt(pow(playerFleet[AI - 'A'].cannonCords[i][j * 2] - yShoot, 2) + pow(playerFleet[AI - 'A'].cannonCords[i][(j * 2) + 1] - xShoot, 2));
					if (i == 0) distance = 0;
					if ((distance <= shiplength) && (cannonCheck(AI,board,i,j)))
					{
						board[yShoot][xShoot] = 'x';
						cout << "SHOOT " << j << " " << type << " " << yShoot << " " << xShoot << endl;
						playerFleet[otherplayer - 'A'].fleetParts--;
						winCheck(AI,radarBoard,tempBoard,board);
					}
				}
				else if(cannonCheck(AI, board, i,j))
				{
					if (i != 0)break;
					bool canShoot = false;
					while (!canShoot)
					{
						randomYX(&xShoot, &yShoot);
						if (radarBoard[yShoot][xShoot] == '?')canShoot = true;
					}
					if (board[yShoot][xShoot] == '+' || board[yShoot][xShoot] == '@' || board[yShoot][xShoot] == '%' || board[yShoot][xShoot] == '!')
					{
						board[yShoot][xShoot] = 'x';
						playerFleet[otherplayer - 'A'].fleetParts--;
					}
					cout << "SHOOT " << j << " " << type << " " << yShoot << " " << xShoot << endl;
					winCheck(AI,radarBoard, tempBoard, board);
				}
			}
		}
	}
}

void AIshoot(char AI, char** board, char** radarBoard, char** tempBoard)
{
	int xShoot = 0, yShoot = 0;
	bool AIsee = false;
	char otherplayer;
	if (AI == 'A')otherplayer = 'B';
	else otherplayer = 'A';
	for (int i = 0;i < boardPlace.ymax;i++)
	{
		for (int j = 0;j < boardPlace.xmax;j++)
		{
			if (radarBoard[i][j] == 'P')
			{
				xShoot = j;
				yShoot = i;
				AIsee = true;
				break;
			}
		}
		if (AIsee)break;
	}
	if (!AIsee)
	{
		bool canShoot = false;
		while (!canShoot)
		{
			randomYX(&xShoot, &yShoot);
			if (radarBoard[yShoot][xShoot] != 'A')canShoot = true;
			if (radarBoard[yShoot][xShoot] == '#')canShoot = false;
		}
	}
	if (board[yShoot][xShoot] == '+' || board[yShoot][xShoot] == '@' || board[yShoot][xShoot] == '%' || board[yShoot][xShoot] == '!')
	{
		board[yShoot][xShoot] = 'x';
		playerFleet[otherplayer - 'A'].fleetParts--;
	}
	cout << "SHOOT " << yShoot << " " << xShoot << endl;
	winCheck(AI,radarBoard,tempBoard,board);
}

void settingAIdirect(int direct,char *direction,int *one,int yShip,int xShip,int shipLength,char AI)
{
	int temp;
	do
	{
		temp=direct;
		if (direct == 0)*direction = 'N';
		if (direct == 1)*direction = 'S';
		if (direct == 2)*direction = 'W';
		if (direct == 3)*direction = 'E';
		if (*direction == 'N' || *direction == 'W')*one = 1;
		else *one = -1;
		if (yShip + shipLength - 1 > playerPlace[AI - 'A'].ymax) direct = 1;
		if (yShip - shipLength + 1 < playerPlace[AI - 'A'].ymin) direct = 0;
		if (xShip + shipLength - 1 > playerPlace[AI - 'A'].xmax) direct = 3;
		if (xShip - shipLength + 1 < playerPlace[AI - 'A'].xmin) direct = 2;
	} while (temp != direct);
}


//RUCHY BOTA
void AIplay(char AI,char **board,char **radarBoard,bool extendedOn,bool autoPlay,char** tempBoard)
{
	cout << "[player" << AI << "]" << endl;
	playerFleet[AI - 'A'].AIplaced = true;
	for (int i = 0;i < 4;i++) if (playerFleet[AI - 'A'].availableShips != 0) playerFleet[AI - 'A'].AIplaced = false;
	for (int i = 0;i < boardPlace.ymax;i++)
		for (int j = 0;j < boardPlace.xmax;j++) tempBoard[i][j] = board[i][j];
	if (!autoPlay) AItemp=playerFleet[AI-'A'];
	if (!playerFleet[AI - 'A'].AIplaced)
	{
		char direction = ' ', type[4], state[6] = { '1','1','1','1','1' };
		int xShip = 0, yShip = 0, direct, one = 0;
		bool canBePlaced = false;
		for (int i = 0;i < 4;i++)
		{
			if (i == 0)strcpy_s(type, "CAR");
			if (i == 1)strcpy_s(type, "BAT");
			if (i == 2)strcpy_s(type, "CRU");
			if (i == 3)strcpy_s(type, "DES");
			int shipLength = 5 - i;
			for (int j = 0;j < playerFleet[AI - 'A'].ShipsInFleet[i];j++)
			{
				if (playerFleet[AI - 'A'].isThatShipPlaced[i][j]) continue;
				while (!canBePlaced)
				{
					canBePlaced = true;
					xShip = rand() % (playerPlace[AI - 'A'].xmax - playerPlace[AI - 'A'].xmin) + playerPlace[AI - 'A'].xmin;
					yShip = rand() % (playerPlace[AI - 'A'].ymax - playerPlace[AI - 'A'].ymin) + playerPlace[AI - 'A'].ymin;
					direct = rand() % 4;
					settingAIdirect(direct,&direction,&one,yShip,xShip,shipLength,AI);
					if (direction == 'N' || direction == 'S') canBePlaced = canBePlacedV(board, yShip, xShip, one, shipLength);
					else canBePlaced = canBePlacedH(board, yShip, xShip, one, shipLength);
				}
				clearBoardFromS(board);
				cout << "PLACE_SHIP " << yShip << " " << xShip << " " << direction << " " << j << " " << type << endl;
				canBePlaced = false;
				place_ship(board, AI, 1, yShip, xShip, direction, j, type,state,true, false, radarBoard, tempBoard);
			}
		}
		playerFleet[AI - 'A'].AIplaced = true;
	}
	clearBoardFromS(board);
	if (canPlayerShoot())
	{
	if(extendedOn)extendedAIshoot(AI, board, radarBoard,tempBoard);
	else AIshoot(AI, board, radarBoard,tempBoard);
	}
	AImove(board,AI, extendedOn, radarBoard, tempBoard);
	cout << "[player" << AI << "]" << endl;
	if (!autoPlay)
	{
		for (int i = 0;i < boardPlace.ymax;i++)
			for (int j = 0;j < boardPlace.xmax;j++) board[i][j] = tempBoard[i][j];
		playerFleet[AI - 'A'] = AItemp;
	}
}

char **createTab()
{
	char** board;
	board = new char* [boardPlace.ymax];
	for (int i = 0;i < boardPlace.ymax;i++) board[i] = new char[boardPlace.xmax];
	return board;
}

void setTab(char** board,bool radar)
{
	for (int i = 0;i < boardPlace.ymax;i++)
	{
		for (int j = 0;j < boardPlace.xmax;j++)
		{
			board[i][j] = ' ';
			if(radar)board[i][j] = '?';
		}
	}
}

void setMoves()
{
	for (int j = 0;j < 2;j++)
	{
		for (int i = 0;i < MAXSHIPS;i++)playerFleet[j].moves[0][i] = 2;

		for (int i = 1;i < 4;i++)
		{
			for (int z = 0;z < MAXSHIPS;z++)playerFleet[j].moves[i][z] = 3;
		}
		for (int i = 0;i < 4;i++)
		{
			for (int z = 0;z < MAXSHIPS;z++) playerFleet[j].howManyShots[i][z] = 5 - i;
		}
	}

}

void setAllTabs(char**board,char**radarBoard,char**tempBoard)
{
	setTab(board, false);
	setTab(radarBoard, true);
	setTab(tempBoard, false);
	set_place(boardPlace.ymax, boardPlace.xmax);
}

void stateComands(char *player,bool *extendedOn,char ***board,char***radarBoard,char***tempBoard,bool *autoAIplay,bool *allPlayersAI,char *AIplayer,char state[6])
{
	char comand[20], playerfleet;
	int amfleet[4];
	long int seed;
	int print;
	do
	{
		cin >> comand;
		if (strcmp(comand, "EXTENDED_SHIPS") == 0)	*extendedOn = true;
		if (strcmp(comand, "SRAND") == 0)
		{
			cin >> seed;
			srand(seed);
		}
		if (strcmp(comand, "AUTO") == 0)*autoAIplay = true;
		if (strcmp(comand, "SET_AI_PLAYER") == 0)
		{
			cin >> *AIplayer;
			playerFleet[*AIplayer - 'A'].isThatAi = true;
			if ((playerFleet[pA].isThatAi) && (playerFleet[pB].isThatAi))* allPlayersAI = true;
		}
		if (strcmp(comand, "SAVE") == 0)save(*player, *board);
		if (strcmp(comand, "PRINT") == 0)
		{
			cin >> print;
			BoardPrint(*board, print);
			cout << "PARTS REMAINING:: A : " << playerFleet[pA].fleetParts << " B : " << playerFleet[pB].fleetParts << endl;
		}
		if (strcmp(comand, "SET_FLEET") == 0)
		{
			cin >> playerfleet;
			{
				cin >> amfleet[0];
				cin >> amfleet[1];
				cin >> amfleet[2];
				cin >> amfleet[3];
			}
			setfleet(amfleet[0], amfleet[1], amfleet[2], amfleet[3], playerfleet);
		}
		if (strcmp(comand, "BOARD_SIZE") == 0)
		{
			boardClear(*board,*radarBoard,*tempBoard);
			cin >> boardPlace.ymax >> boardPlace.xmax;
			*board = createTab();
			*radarBoard = createTab();
			*tempBoard = createTab();
			setAllTabs(*board, *radarBoard, *tempBoard);
		}
		if (strcmp(comand, "INIT_POSITION") == 0)init_position();
		if (strcmp(comand, "NEXT_PLAYER") == 0)	cin >> player;
		if (strcmp(comand, "REEF") == 0)placeReef(*board, boardPlace.ymax, boardPlace.xmax, *radarBoard, *tempBoard);
		if (strcmp(comand, "SHIP") == 0)
		{
			cin >> playerfleet;
			place_ship(*board, playerfleet, 0, NULL, NULL, NULL, NULL, NULL, state, true, true, *radarBoard, *tempBoard);
		}
	} while (strcmp(comand, "[state]") != 0);
}

void playerComands(char comand[20],char player,bool autoAIplay,char **board,char **radarBoard,char **tempBoard,bool extendedOn, char state[6],char nextplayer)
{
	int print;
	if (player != comand[7])
	{
			cout << "INVALID OPERATION " << '"' << "[player"<<nextplayer<<"] " << '"' << ": THE OTHER PLAYER EXPECTED" << "\n";
			boardClear(board, radarBoard, tempBoard);
			exit(0);
	}

	if ((playerFleet[player - 'A'].isThatAi) && (autoAIplay)) comand[0] = 'X';
	else cin >> comand;
	while ((strcmp(comand, "[playerA]") != 0) && (strcmp(comand, "[playerB]") != 0))
	{
			if ((playerFleet[player - 'A'].isThatAi) && (autoAIplay))
			{
				AIplay(player, board, radarBoard, extendedOn, autoAIplay, tempBoard);
				break;
			}
			if (strcmp(comand, "PLACE_SHIP") == 0) place_ship(board, player, 1, 0, 0, '0', 0, {}, state, true, true, radarBoard, tempBoard);
			if (strcmp(comand, "SHOOT") == 0)
			{
				if (shoot(board, extendedOn, player, radarBoard, tempBoard))
				{
					playerFleet[nextplayer-'A'].fleetParts--;
					winCheck(player, radarBoard, tempBoard, board);
				}
			}
			if (strcmp(comand, "MOVE") == 0)Move(board, player, extendedOn, 0, {}, 'Z', state, false, radarBoard, tempBoard);
			if (strcmp(comand, "PRINT") == 0)
			{
				cin >> print;
				radarPrint(board, player, false, radarBoard, print, 0,tempBoard);
			}
			if (strcmp(comand, "SPY") == 0)radarPrint(board, player, true, radarBoard, 0, 0,tempBoard);
			cin >> comand;
	}
}
//MAIN
int main()
{
	int i, j, amfleet[4] = { 1,2,3,4 };
	long int seed = 4753465;
	char comand[20], player = 'A', nextplayer='B', AIplayer = 'N', state[6] = {'1','1','1','1','1'}, ** board, ** tempBoard, ** radarBoard;
	bool extendedOn = false,allPlayersAI=false,autoAIplay=false;
	srand(seed);
	board = createTab();
	radarBoard = createTab();
	tempBoard = createTab();
	setAllTabs(board, radarBoard, tempBoard);

	for (i = 0;i < 4;i++)
		for (j = 0;j < MAXSHIPS;j++)
		{
			playerFleet[pA].isThatShipPlaced[i][j] = false;
			playerFleet[pB].isThatShipPlaced[i][j] = false;
			playerFleet[pA].cannonWorking[i][j] = false;
			playerFleet[pB].cannonWorking[i][j] = false;
			playerFleet[pB].isThatShipPlaced[i][j] = false;
		}
	setMoves();
	setfleet(amfleet[0], amfleet[1], amfleet[2], amfleet[3], 'A');
	setfleet(amfleet[0], amfleet[1], amfleet[2], amfleet[3], 'B');
	//ustawianie planszy dla kazdego gracza
	set_place(boardPlace.ymax, boardPlace.xmax);
	//wpisywanie komend

	while (cin >> comand)
	{
		if (strcmp(comand, "[state]") == 0) stateComands(&player, &extendedOn, &board, &radarBoard, &tempBoard, &autoAIplay, &allPlayersAI, &AIplayer, state);
		if (player == 'B') nextplayer = 'A';
		else nextplayer = 'B';
		do
		{
			if ((playerFleet[player - 'A'].isThatAi) && (autoAIplay))
			{
				if (player == 'A') strcpy_s(comand, "[playerA]");
				if (player == 'B') strcpy_s(comand, "[playerB]");
			}
			setMoves();
			if ((strcmp(comand, "[playerA]") == 0) || (strcmp(comand, "[playerB]") == 0))
			{
				playerComands(comand, player, autoAIplay, board, radarBoard, tempBoard, extendedOn, state, nextplayer);
				setTab(radarBoard, true);
				seed = (rand()%seed) + 1;
				swap(player, nextplayer);
			}
			if (!allPlayersAI) break;
		} while((allPlayersAI) && (autoAIplay));
		if ((playerFleet[player - 'A'].isThatAi) && (!autoAIplay))AIplay(player, board, radarBoard, extendedOn, autoAIplay, tempBoard);
	};
	return 0;
}