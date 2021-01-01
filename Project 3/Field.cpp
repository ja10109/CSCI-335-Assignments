//field.cpp
#include "Field.h"

//COPY CONSTRUCTOR copies input vector and modifies vector values into weighted values from (0,0)
//path vector also copies input vector
//O(N) time and SPACE
Field::Field(const std::vector<std::vector<int>> &rhs)
{
	//SET ROWS AND COLS
	rows = rhs.size();
	cols = rhs[0].size();

	//Convert input rhs vector into weight vector
	//[1 2 3 4 5 6] -> [1 3 6  10 15 21]
	//[1 2 3 4 5 6] -> [2 6 12 20 30 42]
	//[1 2 3 4 5 6] -> [3 9 18 30 45 63] 
	//[1 2 3 4 5 6] -> [4 12 24 40 60 84]
	//[1 2 3 4 5 6] -> [5 15 30 50 75 106]

	//set weight size with number of rows
	weight.resize(rows);
	//MODIFY WEIGHT VECTOR IN O(N) time and memory with N being the number of elements in 2d array
	for (int i{ 0 }; i < rows; i++)
	{
		//set each ROW of vectors with the number of cols
		//ALTHOUGH RESIZE IS LINEAR TO THE NUMBER OF COLS, ROWS * COLS = N, MAKING FUNCTION STILL LINEAR
		weight[i].resize(cols);
		for (int j{ 0 }; j < cols; j++)
		{
			//IN FIRST ROW, weight value is original value in array + previous weight value
			if (i == 0)
				weight[i][j] = rhs[i][j] + weight[i][j - 1];
			//IN FIRST VALUE of each row, weight value is original value + value in previous row 
			else if (j == 0)
				weight[i][j] = rhs[i][j] + weight[i - 1][j];
			//WEIGHT VALUE = ORIGINAL VALUE + WEIGHT IN PREVIOUS ROW SAME COL INDEX + PREVIOUS WEIGHT - WEIGHT IN PREVIOUS ROW AND PREVIOUS COL INDEX
			else
				weight[i][j] = rhs[i][j] + weight[i - 1][j] + weight[i][j - 1] - weight[i - 1][j - 1];
		}
	}
	
	//COPY PATH VECTOR. WILL BE MODIFIED IN PATH FUNCTION
	path = rhs;
}

//MOVE CONSTRUCTOR, MOVES input vector into a temporary vector which helps create weight vector values.
//temp vector is then moved onto path vector
//O(N) time AND space
Field::Field(std::vector<std::vector<int>> &&rhs)
{
	//MOVE RHS ONTO V and set ROWS AND COLS
	std::vector<std::vector<int>> temp = std::move(rhs);
	rows = temp.size();
	cols = temp[0].size();

	//set weight vector size to the number of rows
	weight.resize(rows);
	//MODIFY WEIGHT VECTOR IN O(N) time and memory with N being the number of elements in 2d array
	for (int i{ 0 }; i < rows; i++)
	{
		//set each ROW size in weight vector to the number of cols
		//ALTHOUGH RESIZE IS LINEAR TO THE NUMBER OF COLS, ROWS * COLS = N, MAKING FUNCTION STILL LINEAR
		weight[i].resize(cols);
		for (int j{ 0 }; j < cols; j++)
		{
			//IN FIRST ROW, weight value is original value in array + previous weight value
			if (i == 0)
				weight[i][j] = temp[i][j] + weight[i][j - 1];
			//IN FIRST VALUE of each row, weight value is original value + value in previous row 
			else if (j == 0)
				weight[i][j] = temp[i][j] + weight[i - 1][j];
			//WEIGHT VALUE = ORIGINAL VALUE + WEIGHT IN PREVIOUS ROW SAME COL INDEX + PREVIOUS WEIGHT - WEIGHT IN PREVIOUS ROW AND PREVIOUS COL INDEX
			else
				weight[i][j] = temp[i][j] + weight[i - 1][j] + weight[i][j - 1] - weight[i - 1][j - 1];
		}
	}
	//now move vector v onto path
	path = std::move(temp);
}

//WEIGHT FUNCTION
//return weight value of rectangle formed by (x1,y1) to (x2,y2)
//O(1) RUNTIME
int Field::Weight(int x1, int y1, int x2, int y2) const
{
	//IF INPUT IS GREATER THAN THE 2D-ARRAY SIZE, THROW OUT OF RANGE EXCEPTION
	if (x1 >= cols || y1 >= rows || x2 >= cols || y2 >= rows)
		throw std::out_of_range("Coordinate doesn't exist");

	int rt{ weight[std::max(y1, y2)][std::max(x1, x2)] };  //BOTTOM-RIGHT CORNER VALUE

	//EXTRA WEIGHT VALUE LEFT OF RECTANGLE
	int extra_one{ 0 };
	//EXTRA WEIGHT VALUE ABOVE RECTANGLE
	int extra_two{ 0 };

	//IF BOTH X INPUTS ARE NOT IN FIRST COL, REMOVE RIGHT MOST VALUE NOT INCLUDED LOCATED LEFT OF RECTANGLE
	if (std::min(x1,x2) > 0)
		extra_one = weight[std::max(y1,y2)][std::min(x2,x1) - 1];

	//IF BOTH Y INPUTS ARE NOT IN FIRST ROW, REMOVE RIGHT MOST VALUE NOT INCLUDED LOCATED ABOVE THE RECTANGLE
	if (std::min(y1,y2) > 0)
		extra_two = weight[std::min(y1,y2) - 1][std::max(x1,x2)]; 

	//VALUE THAT MAY HAVE BEEN REMOVED TWICE FROM WEIGHT
	int add_extras{ 0 };
	//IF EXTRA_ONE AND EXTRA_TWO > 0, AN EXTRA VALUE HAS BEEN REMOVED FROM WEIGHT, ADD EXTRA VALUE LOCATED ONE LEFT DIAGONAL ABOVE RECTANGLE
	if (extra_one > 0 && extra_two > 0)
		add_extras = weight[std::min(y1 - 1, y2 - 1)][std::min(x1 - 1, x2 - 1)];

	rt = rt - extra_one - extra_two + add_extras;
	return rt;
}

//FUNCTION PATH COST RETURNS THE COST OF THE CHEAPEST PATH FROM TOP-LEFT CORNER TO THE BOTTOM-RIGHT CORNER
//O(N) time where N is the number of values in 2d array
//FUNCTION CAN'T BE CONST BECAUSE VALUES ARE BEING MODIFIED
int Field::PathCost()
{
	//EDGE CASES
	//if vector doesn't exist, return 0
	if (path.size() == 0)
		return 0;
	//if vector is empty, return 0
	if (path.size() > 0 && path[0].size() == 0)
		return 0;
	//if vector contains only one element, return element
	if (path.size() == 1 && path[0].size() == 1)
		return path[0][0];

	//[1 2 3 4 5 6] -> [1 3 6  10 15 21]
	//[1 2 3 4 5 6] -> [2 4 7  11 16 22]
	//[1 2 3 4 5 6] -> [3 5 8  12 17 23]
	//[1 2 3 4 5 6] -> [4 6 9  13 18 24]
	//[1 2 3 4 5 6] -> [5 7 10 14 19 25]

	//set each element in the first row adding the original value + previous value
	for (int i{ 1 }; i < cols; i++)
		path[0][i] += path[0][i - 1];
	//from the second row, set first value in each row to the first value in the previous row
	for (int i{ 1 }; i < rows; i++)
	{
		path[i][0] += path[i - 1][0];
		//starting from second value in each row, set each value to the minimum of the previous value in row or the value in the previous row
		for (int j{ 1 }; j < cols; j++)
			path[i][j] += std::min(path[i][j - 1], path[i - 1][j]);
	}
	//return bottom right corner of array
	return path[rows - 1][cols - 1];
}
