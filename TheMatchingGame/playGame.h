#pragma once

bool isEmptyBoard(list2D &B)
{
	Node2D* Rp;
	Node2D* Dp = B.heads[1];

	while (Dp) {
		Rp = Dp;
		while (Rp) {
			if (Rp->data != 0)
				return false; 
			Rp = Rp->right; 
		}
		Dp = Dp->down;
	}
	return true; 
}

struct Point {
	int x; 
	int y; 
	Point(int row, int col) :x(row), y(col) {}; 
};
void moveBoardCursor(list2D &B, int& color)
{
	//keep moving till endgame (no more tiles)
	bool didSelectOne = false;

	//reset values
	B.cursor = B.colSize + 2 + 1; //default position for cursor
	int originalColor = color; 

	while (!isEmptyBoard(B)) {
		char c;
		Point startingP(0, 0), endingP(0, 0);
		

		printBoard(B, color);

		c = _getch();
		//reset color
		color = originalColor;
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
			int curRow = B.cursor / B.colSize; //y coordinates of the cursor
			int curCol = B.cursor % B.colSize; //x 


			if (!didSelectOne)
			{
				startingP.x = curRow; 
				startingP.y = curCol; 
				color = 10;
				didSelectOne = 1;
			}
			else {
				endingP.x = curRow; 
				endingP.y = curCol; 
				color = 10;
				//reset if the player choose a different starting point
				didSelectOne = 0;
			}
		}
		}
	}
}

bool canMatchOnLineX(list2D& B, Point sp, Point ep)
{
	//check 2 points lying on the same row
	int minY = minV(sp.y, ep.y);
	int maxY = maxV(sp.y, sp.y);

	if (sp.x == 0 || sp.x == B.rowSize - 1)
		return true;
	if (minY + 1 == maxY)
		return true;
	//3rd situation
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
	int minX = minV(sp.x, ep.x);
	int maxX = maxV(sp.x, sp.x);

	if (sp.y == 0 || sp.y == B.colSize - 1)
		return true;
	if (minX + 1 == maxX)
		return true;
	//3rd situation
	Node2D* node = B.getNode(minX, sp.y); //start node
	while (node->down != B.getNode(maxX, sp.y)) //loops from min to max point
	{
		node = node->down;
		if (node->data != 0)
		{
			return false;
		}
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
	int maxX = maxV(sp.x, ep.x); 
	int maxY = maxV(sp.y, ep.y); 
	int minX = minV(sp.x, ep.x); 
	int minY = minV(sp.y, ep.y);

	for (int i = minY + 1; i <= maxY - 1; ++i)
	{
		if (canMatchOnLineY(B, { minX, i }, { maxX, i }) && canMatchOnLineX(B, { minX, minY }, { minX, i }) && canMatchOnLineX(B, { maxX, i }, {maxX, maxY}))
		{
			return true; 
		}
	}
	return false; 
}

bool checkZMatchingY(list2D& B, Point& sp, Point& ep)
{
	// Z matching has 3 lines we need to check, first is the X route and 2 corresponding Y routes
	// then find an empty X route between starting point col and ending point col
	//the heads of each X route starts at (minX + 1, minY) and ends at (maxX - 1, minY)
	//we go from each head to the right (maxX - minX - 1) times and also check the 2 corresponding X lines to see if they are empty or not
	int maxX = maxV(sp.x, ep.x);
	int maxY = maxV(sp.y, ep.y);
	int minX = minV(sp.x, ep.x);
	int minY = minV(sp.y, ep.y);

	for (int i = minX + 1; i <= maxX - 1; ++i)
	{
		if (canMatchOnLineX(B, { i, minY }, { i, maxY }) && canMatchOnLineY(B, { i, minY }, { maxX, minY }) && canMatchOnLineY(B, { minX, maxY}, { i, maxY }))
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

}