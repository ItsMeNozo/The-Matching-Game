#pragma once
#include <chrono>
#include <fstream>
#define defaultColor 37 
#define highlightColor 28 //yellow
using namespace std::chrono;

bool canMatchOnLineX(list2D&, Point, Point);
bool canMatchOnLineY(list2D&, Point, Point);
bool canIMatch(list2D&, Point, Point);
bool checkZMatchingX(list2D&, Point&, Point&);
bool checkZMatchingY(list2D&, Point&, Point&);
bool canUMatchX(list2D&, Point&, Point&, int);
bool canZMatch(list2D&, Point&, Point&);
bool canUMatch(list2D&, Point&, Point&);
void printMenu();
void onStartChoices();

struct Player {
	char name[127]; // = {0}
	time_t timeRec;

	Player()
	{
		timeRec = 0; 
	}
};
//cntDigits
int cntDigits(long long n)
{
	if (n / 10 == 0)
		return 1;
	return 1 + cntDigits(n / 10);
}


void sortLeaderboard(Player records[], int n)
{
	//bubble sort
	for (int times = 1; times < n; ++times)
	{
		for (int i = 0; i < n - times; ++i)
		{
			if (records[i].timeRec > records[i + 1].timeRec)
			{
				//swap
				Player temp = records[i];
				records[i] = records[i + 1]; 
				records[i + 1] = temp; 
			}
		}
	}
	//open file for overwriting
	std::fstream fs("leaderboard.bin", std::ios::out | std::ios::binary); 
	if (!fs)
		std::cout << "Error opening file\n"; 
	else {
		//get file size
		std::streampos begin, end, fileSize;

		begin = fs.tellg();
		fs.seekg(0L, std::ios::end);
		end = fs.tellg();
		fileSize = end - begin;
		fs.clear();
		fs.seekg(0L, std::ios::beg);

		for (int i = 0; i < n; ++i)
		{
			fs.write(reinterpret_cast<char*>(&records[i]), sizeof(records[i])); 
		}
		
		fs.close(); 
	}
	
}
void updateleaderboard(Player& plr)
{
	//update array of players according to the file to later sort the array
	std::fstream fs("leaderboard.bin", std::ios::in | std::ios::out | std::ios::binary); 
	Player plrRecord[100]; 
	int cnt = 0;
	bool playerFound = false; 

	if (!fs.is_open())
	{
		fs.clear();
		fs.open("leaderboard.bin", std::ios::out | std::ios::binary);
		fs.close();
		fs.open("leaderboard.bin",std::ios::in | std::ios::out | std::ios::binary); //open again for reading
	}
	//find player
	if (!fs)
		std::cout << "Cannot open file\n"; 
	else {
		//get file size
		std::streampos begin, end, fileSize; 
		
		begin = fs.tellg();
		fs.seekg(0L, std::ios::end);
		end = fs.tellg();
		fileSize = end - begin;
		fs.clear();
		fs.seekg(0L, std::ios::beg);

		if (fileSize != 0)
		{
			while (fs.peek() != EOF && fs.peek() != 0)
			{
				long lastReadPos = fs.tellg(); 
				fs.read(reinterpret_cast<char*>(&plrRecord[cnt]), sizeof(plrRecord[cnt]));//error
				//Player found. Update player's record if the time record is smaller
				if (strncmp(plrRecord[cnt].name, plr.name, strlen(plr.name)) == 0)
				{
					playerFound = true; 
					if (plr.timeRec < plrRecord[cnt].timeRec )
					{
						plrRecord[cnt] = plr; //add new record to the array
					}
				}
				++cnt; //add player
			}
		}
		
		//Player is not found so append new player
		if (!playerFound)
		{
			fs.clear();
			fs.seekp(0L, std::ios::end);
			fs.write(reinterpret_cast<char*>(&plr), sizeof(plr));
			plrRecord[cnt] = plr;
			++cnt; //add 1 more player
		}
		
		fs.close(); 
	}

	sortLeaderboard(plrRecord, cnt); 
}

bool isEmptyBoard(list2D& B)
{
	Node2D* Rp;

	for (int i = 1; i <= B.rowSize; ++i)
	{
		Rp = B.heads[i];
		while (Rp)
		{
			if (Rp->data != 0)
				return false;
			Rp = Rp->right;
		}
	}
	return true;
}

bool stillValidPairs(list2D& B, Point& startPoint, Point& endPoint)
{
	//iterate through all cells and find valid matching for each 
	for (int i = 1; i <= B.rowSize; i++)
	{
		Node2D* startPtr = B.heads[i]->right;
		startPoint.y = 1; 
		startPoint.x = i; 
		//find the matching cell. If a cell is found, return true
		while (startPtr != B.tails[i] && startPtr)
		{
			for (int j = 1; j <= B.rowSize; j++)
			{
				Node2D* endPtr = B.heads[j]->right;
				endPoint.x = j; 
				endPoint.y = 1; 
				while (endPtr != B.tails[j] && endPtr)
				{
					int val1 = startPtr->data; 
					int val2 = endPtr->data; 
					//if 2 points have different values and they are not NULL then false
					if (val1 != val2 && val1 != 0 && val2 != 0)
					{
						endPtr = endPtr->right;
						endPoint.y++;
						continue; 
					}

					if (val1 == val2 && val1 == 0)
					{
						endPtr = endPtr->right;
						endPoint.y++;
						continue;
					}
					//if one of the val is NULL then continue
					if ((val1 == 0 || val2 == 0))
					{
						endPtr = endPtr->right;
						endPoint.y++;
						continue;
					}

					//if the same cell is selected, skip
					if (startPoint.x == endPoint.x && startPoint.y == endPoint.y)
					{
						endPtr = endPtr->right;
						endPoint.y++;
						continue;
					}
					if (canIMatch(B, startPoint, endPoint) || canUMatch(B, startPoint, endPoint) || canZMatch(B, startPoint, endPoint))
					{
						return true; 
					}
					endPtr= endPtr->right; 
					endPoint.y++; 
				}
			}
			startPtr = startPtr->right;
			startPoint.y++; 
		}
	}
	return false; 
}

void showHint(list2D &B, Point startPoint, Point endPoint)
{
	printACell(B, startPoint.x, startPoint.y, highlightColor, 1);
	printACell(B, endPoint.x, endPoint.y, highlightColor, 1);
}
void clearBoard(list2D& B)
{
	for (int y = startY; y <= startY + cellH * (B.rowSize - 1); y += cellH)
	{
		int tempy = y; 
		for (int j = 0; j < cellH + 1; ++j)
		{
			gotoxy(startX, tempy++); 
			for (int i = 0; i < B.colSize; ++i)
				printSpaces(cellW + 1);
		}
	}
}
//playGame
void playGame(list2D& B, Player& player, int color)
{
	PlaySound(TEXT("Speechless.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	//keep track of time
	auto start = high_resolution_clock::now();
	//keep moving till endgame (no more tiles)
	bool didSelectOne = false;
	bool rewrite = false;
	bool buttonToggle = false;
	Point startPoint{ 0, 0 }, endPoint{ 0, 0 };
	//reset values
	B.cursor = B.colSize + 2 + 1; //default position for cursor= 
	Point startingP(0, 0), endingP(0, 0);
	int val1 = 0, val2 = 0; //to store 2 points' values

	printBackground();
	while (!isEmptyBoard(B) && stillValidPairs(B, startPoint, endPoint)) {
		char c;
		printBoard(B, color, rewrite, startingP, buttonToggle);

		//reset color
		color = defaultColor;
		c = _getch();
		switch (c)
		{
		case 'w':
		{
			//go to hint button
			if (B.cursor == (B.colSize + 2) * (B.rowSize + 1) + 1)
			{
				B.cursor -= (B.colSize + 2);
				buttonToggle = 0;
				printHintButton(B, buttonToggle);
				break;
			}
			if (B.cursor >= (B.colSize + 2) * 2 + 1) {
				B.cursor -= (B.colSize + 2);
			}

			break;
		}
		case 's':
		{
			//go to hint button
			if (B.cursor == (B.colSize + 2) * (B.rowSize) + 1)
			{
				B.cursor += (B.colSize + 2);
				buttonToggle = 1;
				printHintButton(B, buttonToggle);
				break;
			}
			if (B.cursor <= (B.rowSize + 2) * (B.colSize + 2) - (B.colSize + 2) * 2 - 2) { //check if B.cursor is at the second row, if it is at the last row, B.cursor wont move up
				B.cursor += (B.colSize + 2);
			}

			break;
		}
		case 'a':
		{
			//B.cursor%(B.colSize + 2) != 1 are the ones at the the first column 
			if (B.cursor > B.colSize + 2 + 1 && B.cursor % (B.colSize + 2) != 1) {
				--B.cursor;
			}
			break;
		}
		case 'd':
		{
			//B.cursor % (B.colSize + 2) != B.colSize are those at B.colSize column
			if (B.cursor < (B.rowSize + 2) * (B.colSize + 2) - (B.colSize + 2) - 2 && B.cursor % (B.colSize + 2) != B.colSize) {
				++B.cursor;
			}
			break;
		}
		case 13: //enter 
		{
			mciSendString(TEXT("play Open1.wav"), NULL, 0, 0); //play sound 
			int curRow = B.cursor / (B.colSize + 2); //y coordinates of the cursor
			int curCol = B.cursor % (B.colSize + 2); //x 

			if (B.cursor == (B.colSize + 2) * (B.rowSize + 1) + 1)
			{
				showHint(B, startPoint, endPoint);
				Sleep(500);
				continue;
			}

			if (!didSelectOne)
			{
				startingP.x = curRow;
				startingP.y = curCol;
				color = highlightColor;
				didSelectOne = 1;
				val1 = B.getNode(startingP.x, startingP.y)->data;
				rewrite = true; //to not delete this cell in the next print
			}
			else {
				endingP.x = curRow;
				endingP.y = curCol;
				color = highlightColor;
				val2 = B.getNode(endingP.x, endingP.y)->data;
				//reset if the player choose a different starting point
				didSelectOne = 0;
				//reset 2 selected cells
				rewrite = false;
			}


			//if 2 points have different values and they are not NULL then false
			if (!didSelectOne && val1 != val2 && val1 != 0 && val2 != 0)
				continue;

			if (val1 == val2 && val1 == 0)
				continue;
			//if one of the val is NULL then continue
			if (!didSelectOne && (val1 == 0 || val2 == 0))
				continue;

			//if the same cell is selected, skip
			if (startingP.x == endingP.x && startingP.y == endingP.y)
				continue;

			//do I, L, U, Z checkings
			if (!didSelectOne && (canIMatch(B, startingP, endingP) || canUMatch(B, startingP, endingP) || canZMatch(B, startingP, endingP)))
			{
				if (startingP.x == endingP.x) {
					if (startingP.y < endingP.y)
						--endingP.y; //in case they are on the same row, after deleting the first node, the position of endingP.y will be off by 1

				}
				B.deleteNode(startingP.x, startingP.y);
				B.deleteNode(endingP.x, endingP.y);
				clearBoard(B);
				//reprint background 
				printBackground();
			}
		}
		}
	}

	//when the game is finished, print the result
	printBoard(B, color, rewrite, startingP, buttonToggle);
	//game finished
	system("cls");
	gotoxy(6, 3);
	std::cout << "Game has no more moves!";
	auto stop = high_resolution_clock::now();
	//get time played
	auto duration = duration_cast<seconds>(stop - start);
	player.timeRec = duration.count();
	updateleaderboard(player);
}


bool canMatchOnLineX(list2D& B, Point sp, Point ep)
{
	//if 2 points have different values and both are not NULL then false
	int val1 = B.getNode(sp.x, sp.y)->data; 
	int val2 = B.getNode(ep.x, ep.y)->data; //error
	if (val1 != val2 && val1 != 0 && val2 != 0)
		return false; 

	//if 2 points overlapse return true
	if (sp.x == ep.x && sp.y == ep.y)
		return true; 
	//if 2 points are on the same column then quit
	if (sp.y == ep.y) 
		return false; 

	//check 2 points lying on the same row
	int minY = minV(sp.y, ep.y);
	int maxY = maxV(sp.y, ep.y);
	//2 points next to each other
	if (minY + 1 == maxY)
		return true;
	//check if there are spaces between 2 points
	Node2D* node = B.getNode(sp.x, minY); //start node
	while (node->right != B.getNode(sp.x, maxY) && node->right) //this loops from min to max point
	{
		node = node->right;
		if (node->data != 0) //error
		{
			return false;
		}
	}
	return true;
}

bool canMatchOnLineY(list2D& B, Point sp, Point ep)
{
	//if 2 points have different values and both are not NULL then false
	int val1 = B.getNode(sp.x, sp.y)->data;
	int val2 = B.getNode(ep.x, ep.y)->data;
	if (val1 != val2 && val1 != 0 && val2 != 0)
		return false;
	//if 2 points overlapse return true
	if (sp.x == ep.x && sp.y == ep.y)
		return true;
	//if 2 points are on the same row then false
	if (sp.x == ep.x)
		return false; 
	

	int minX = minV(sp.x, ep.x);
	int maxX = maxV(sp.x, ep.x);
	//2 points below each other
	if (minX + 1 == maxX)
		return true;
	//3rd situation
	Node2D* node = B.getNode(minX, sp.y); //start node

	for (int i = minX + 1; i < maxX; ++i)
	{
		if (B.getNode(i, sp.y)->data != 0)
			return false; 
	}
	return true;
}
bool canIMatch(list2D &B, Point sp, Point ep)
{
	 //3 situations: 
	 //1. when 2 points lies on one of the borders of the board
	 //2. when 2 points are next to each other
	 //3. when 2 points are far away but no empty spaces between
	
	bool canMatch = 0; 
	//check 2 points lying on the same row
	if (sp.x == ep.x)
		canMatch = canMatchOnLineX(B, sp, ep); 
	//check 2 points lying on the same column
	else if (sp.y == ep.y)
		canMatch = canMatchOnLineY(B, sp, ep); 

	return canMatch; 
}

bool checkZMatchingX(list2D& B, Point& sp, Point& ep)
{
	//first we locate minx, miny for starting point and max x, max y for ending point
	// Z matching has 3 lines we need to check, first is the y route and 2 corresponding X routes
	// then find an empty y route between starting point col and ending point col
	//the heads of each y route starts at (minX, minY + 1) and ends at (minX, maxY - 1)
	//we go from each head down (maxY - minY) times and also check the 2 corresponding X routes to see if they are empty or not
	Point maxCol = sp, minCol = ep;

	if (minCol.y > maxCol.y)
		swapPoints(maxCol, minCol);

	for (int i = minCol.y + 1; i <= maxCol.y - 1; ++i)
	{
		if (canMatchOnLineY(B, { minCol.x, i }, { maxCol.x, i }) && canMatchOnLineX(B, minCol, { minCol.x, i }) && canMatchOnLineX(B, { maxCol.x, i }, maxCol))
			return true; 
	}
	return false; 
}

bool checkZMatchingY(list2D& B, Point& sp, Point& ep)
{
	// Z matching has 3 lines we need to check, first is the X route and 2 corresponding Y routes
	// then find an empty X route between starting point col and ending point col
	//the heads of each X route starts at (minX + 1, minY) and ends at (maxX - 1, minY)
	//we go from each head to the right (maxX - minX - 1) times and also check the 2 corresponding X lines to see if they are empty or not
	Point maxRow = sp, minRow = ep; 
	
	if (minRow.x > maxRow.x)
		swapPoints(minRow, maxRow); 

	for (int i = minRow.x + 1; i <= maxRow.x - 1; ++i)
	{
		if (canMatchOnLineX(B, { i, minRow.y }, { i, maxRow.y }) && canMatchOnLineY(B, minRow, { i, minRow.y }) && canMatchOnLineY(B, { i, maxRow.y }, maxRow))
		{
			return true;
		}
	}
	return false;
}
bool canZMatch(list2D& B, Point& sp, Point& ep)
{
	//2 ways Z can be formed: 
	//from the left to right: checkZMatchingX
	//from up to down: checkZMatchingY
	if (checkZMatchingX(B, sp, ep) || checkZMatchingY(B, sp, ep))
		return true; 
	return false; 
}

bool canUMatchX(list2D& B, Point& sp, Point& ep, int side)
{
	//Since L is included in U form, we do both L and U matching check
	//there are 2 ways U can be formed on X line: to the right or left. When going left y is decreased by 1 every y line check, it is the opposite for going right  
	//=> side = 1 right, side = -1 left 
	Point startPointCheck(0, 0);
	Point maxY(0, 0), minY(0, 0) ; 
	if (sp.y >= ep.y)
	{
		startPointCheck.y = ep.y; 
		startPointCheck.x = sp.x; 
		maxY = sp; 
		minY = ep; 
	}
	else {
		startPointCheck.y = sp.y; 
		startPointCheck.x = ep.x; 
		maxY = ep; 
		minY = sp; 
	}

	if (side == 1)
	{
		startPointCheck.x = minY.x; 
		startPointCheck.y = maxY.y; 
		swapPoints(maxY, minY); 
	}
	
	if (canMatchOnLineX(B, maxY, startPointCheck))
	{
		int tempy = startPointCheck.y; 
		while (tempy >= 0 && tempy <= B.colSize + 1)
		{
			if (canMatchOnLineY(B, { startPointCheck.x, tempy }, {minY.x, tempy}) && canMatchOnLineX(B, { startPointCheck.x, tempy}, startPointCheck) && canMatchOnLineX(B, { minY.x, tempy }, minY))
				return true; 
			tempy += side; 
		}
	}
	return false; 
}

bool canUMatchY(list2D& B, Point& sp, Point& ep, int side)
{
	//Since L is included in U form, we do both L and U matching check
	//there are 2 ways U can be formed on Y line: go up or down, when going up x is decreased by one every x line check, it is the opposite for going down 
	//=> side = -1 up, side = 1 down 
	Point startPointCheck(0, 0);
	Point maxX(0, 0), minX(0, 0);
	if (sp.x >= ep.x)
	{
		startPointCheck.y = sp.y;
		startPointCheck.x = ep.x;
		maxX = sp; 
		minX = ep; 
	}
	else {
		startPointCheck.y = ep.y;
		startPointCheck.x = sp.x;
		maxX = ep; 
		minX = sp; 
	}

	if (side == 1)
	{
		startPointCheck.x = maxX.x;
		startPointCheck.y = minX.y;
		swapPoints(maxX, minX);
	}

	if (canMatchOnLineY(B, maxX, startPointCheck))
	{
		int tempx = startPointCheck.x; 
		while (tempx >= 0 && tempx <= B.rowSize + 1)
		{
			if (canMatchOnLineX(B, { tempx ,startPointCheck.y }, { tempx, minX.y }) && canMatchOnLineY(B, { tempx ,startPointCheck.y }, startPointCheck) && canMatchOnLineY(B, { tempx, minX.y }, minX))
				return true;
			tempx += side; 
		}
	}
	return false;
}

bool canUMatch(list2D& B, Point& sp, Point& ep)
{
	if (canUMatchX(B, sp, ep, 1) || canUMatchX(B, sp, ep, -1) || canUMatchY(B, sp, ep, 1) || canUMatchY(B, sp, ep, -1))
		return true; 
	return false; 
}

void printLeaderboard()
{
	std::fstream fs("leaderboard.bin", std::ios::in | std::ios::binary);
	Player record;
	int cnt = 1;
	
	colorText(7); 
	system("CLS"); 
	if (!fs.is_open())
	{
		fs.clear();
		fs.open("leaderboard.bin", std::ios::out | std::ios::binary);
		fs.close();
		fs.open("leaderboard.bin", std::ios::in | std::ios::binary); //open again for reading
	}
	std::cout << "\n";  printSpaces(10); colorText(highlightColor); std::cout << "LEADERBOARD\n"; colorText(7); 
	if (!fs)
		std::cout << "Error opening file\n";
	else {
		//get file size
		std::streampos begin, end, fileSize;

		begin = fs.tellg();
		fs.seekg(0L, std::ios::end);
		end = fs.tellg();
		fileSize = end - begin;
		fs.clear();
		fs.seekg(0L, std::ios::beg);

		if (fileSize != 0)
		{
			while (fs.peek() != EOF)
			{
				fs.read(reinterpret_cast<char*>(&record), sizeof(record));
				std::cout << cnt; printSpaces((int)4 - cntDigits(cnt));
				std::cout << record.name; printSpaces(25 - strlen(record.name));
				std::cout << record.timeRec << "\n";
				++cnt;
			}
		}
		fs.close();
	}

	std::cout << "Press A to go back  ";
	char c = 0;
	while (c != 'a' && c != 'A')
	{
		c = _getch(); 
	}
	system("cls"); 
	printMenu();
}

Player getPlayerName()
{
	Player player;
	std::cout << "Tell me your name: ";
	std::cin >> player.name; 
	return player;
}

void printMenu()
{
	//print choices
	char c;
	bool boardSelectionisMade = false;
	int menuCursor = 0;
	int* colors = new int[3]{ 7, 7, 7 }; //3 items, default is white 
	//print title
	colorText(4);
	gotoxy(10, 4);  std::cout << "THE MATCHING GAME";
	colorText(7);

	gotoxy(10, 5);
	colorText(colors[0]);
	std::cout << "> Play game\n";

	gotoxy(10, 6);
	colorText(colors[1]);
	std::cout << "> Leaderboard\n";


	while (!boardSelectionisMade)
	{

		switch (menuCursor)
		{
		case 0: colors[0] = 9;
			break;
		case 1: colors[1] = 9;
			break;
		case 2: colors[2] = 9; 
		}

		gotoxy(10, 5);
		colorText(colors[0]);
		std::cout << "> Play game";

		gotoxy(10, 6);
		colorText(colors[1]);
		std::cout << "> Leaderboard";

		gotoxy(10, 7);
		colorText(colors[2]);
		std::cout << "> Exit";

		c = _getch();
		switch (c)
		{
		case 'w':
		{
			if (menuCursor >= 1)
				--menuCursor;
			break;
		}
		case 's': {
			if (menuCursor <= 1)
				++menuCursor;
			break;
		}
		case 13:
		{
			switch (menuCursor)
			{
			case 0:
			{
				system("CLS"); 
				onStartChoices();
				break;
			}
			case 1:
			{
				printLeaderboard(); 
				break;
			}
			case 2: 
				break; 
			}
			boardSelectionisMade = true;
			menuCursor = 0;
		}

		}
		//reset colors 
		for (int i = 0; i < 3; ++i)
			colors[i] = 7;
	}
	delete[] colors;
}
void onStartChoices()
{
	int row, col;
	colorText(7);
	Player player;
	char c;

	do {
		player = getPlayerName();
		std::cout << "Input row and column (both numbers are not odd numbers and must be <= 10): ";
		std::cin >> row >> col;
		while ((row % 2 == 1 && col % 2 == 1) || (row > 10 || col > 10))
		{
			std::cout << "Please re-enter row and column: ";
			std::cin >> row >> col;
		}
		list2D charBoard(row, col);
		generateCharMatrix(charBoard, row, col);
		//game instruction
		gotoxy(80, 5);
		std::cout << "W A S D to move around.";
		gotoxy(80, 6);
		std::cout << "Press Enter to select cell";
		playGame(charBoard, player, defaultColor);

		std::cout << " Game replay? Press Y or N\n";
		gotoxy(startX + 2, startX);
		c = _getch();
	} while (c == 'Y' || c == 'y');
	system("CLS"); 
	printMenu();
}