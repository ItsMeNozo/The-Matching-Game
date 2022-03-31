#pragma once
bool canMatchOnLineX(list2D& B, Point sp, Point ep);
bool canMatchOnLineY(list2D& B, Point sp, Point ep);
bool canIMatch(list2D& B, Point sp, Point ep);
bool checkZMatchingX(list2D& B, Point& sp, Point& ep);
bool checkZMatchingY(list2D& B, Point& sp, Point& ep);
bool canUMatchX(list2D& B, Point& sp, Point& ep, int side); 
bool canZMatch(list2D& B, Point& sp, Point& ep); 
bool canUMatch(list2D& B, Point& sp, Point& ep); 

bool isEmptyBoard(list2D &B)
{
	Node2D* Rp;

	for (int i = 0; i < B.rowSize; ++i)
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


void moveBoardCursor(list2D &B, int color)
{
	//keep moving till endgame (no more tiles)
	bool didSelectOne = false;
	bool rewrite = false; 
	//reset values
	B.cursor = B.colSize + 2 + 1; //default position for cursor= 
	Point startingP(0, 0), endingP(0, 0);
	int val1 = 0, val2 = 0; //to store 2 points' values

	while (!isEmptyBoard(B)) {
		char c;
		printBoard(B, color, rewrite, startingP);

		c = _getch();
		switch (c)
		{
		case 'w':
		{
			if (B.cursor >= (B.colSize + 2)* 2 + 1) {
				B.cursor -= (B.colSize + 2);
			}
			break;
		}
		case 's':
		{
			if (B.cursor <= (B.rowSize + 2) * (B.colSize + 2) - (B.colSize + 2)*2 - 2) { //check if B.cursor is at the second row, if it is at the last row, B.cursor wont move up
				B.cursor += (B.colSize + 2);
			}
			break;
		}
		case 'a':
		{
			//B.cursor%(B.colSize + 2) != 1 are the ones at the the first column 
			if (B.cursor > B.colSize + 2 + 1 && B.cursor%(B.colSize + 2) != 1) { 
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
			int curRow = B.cursor / (B.colSize + 2); //y coordinates of the cursor
			int curCol = B.cursor % (B.colSize + 2); //x 

			if (!didSelectOne)
			{
				startingP.x = curRow;
				startingP.y = curCol;
				color = 10;
				didSelectOne = 1;
				val1 = B.getNode(startingP.x, startingP.y)->data; 
				rewrite = true; //to not delete this cell in the next print
			}
			else {
				endingP.x = curRow;
				endingP.y = curCol;
				color = 10;
				val2 = B.getNode(endingP.x, endingP.y)->data; 
				//reset if the player choose a different starting point
				didSelectOne = 0;
				//reset 2 selected cells
				rewrite = false; 
			}
			
			
			//if 2 points have different values and they are not NULL then false
			if (!didSelectOne && val1 != val2 && val1 != 0 && val2 != 0) 
				continue; 
			//if one of the val is NULL then continue
			if (!didSelectOne && (val1 == 0 || val2 == 0))
				continue; 

			//do I, L, U, Z checkings
			if (!didSelectOne && (canIMatch(B, startingP, endingP) || canUMatch(B, startingP, endingP) || canZMatch(B, startingP, endingP)))
			{
				B.getNode(startingP.x, startingP.y)->data = 0; 
				B.getNode(endingP.x, endingP.y)->data = 0; 
			}
		}
		}
	}

	//when the game is finished, print the result
	printBoard(B, color, rewrite, startingP);
}

bool canMatchOnLineX(list2D& B, Point sp, Point ep)
{
	//if 2 points have different values and both are not NULL then false
	int val1 = B.getNode(sp.x, sp.y)->data; 
	int val2 = B.getNode(ep.x, ep.y)->data; 
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

	if (sp.x == 1 || sp.x == B.rowSize)
		return true;
	if (minY + 1 == maxY)
		return true;
	//check if there are spaces between 2 points
	Node2D* node = B.getNode(sp.x, minY); //start node
	while (node->right != B.getNode(sp.x, maxY)) //this loops from min to max point
	{
		node = node->right;
		if (node->data != 0)
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
	//if 2 points are the same return true
	if (sp.x == ep.x && sp.y == ep.y)
		return true;
	//if 2 points are on the same row then false
	if (sp.x == ep.x)
		return false; 
	

	int minX = minV(sp.x, ep.x);
	int maxX = maxV(sp.x, ep.x);

	if (sp.y == 1 || sp.y == B.colSize)
		return true;
	if (minX + 1 == maxX)
		return true;
	//3rd situation
	Node2D* node = B.getNode(minX, sp.y); //start node

	for (int i = minX + 1; i < maxX; ++i)
	{
		if (B.getNode(i, sp.y) != 0)
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