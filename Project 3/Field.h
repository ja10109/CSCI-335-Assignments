//Field H FILE
#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

class Field
{
	private:
		//2d array representing WEIGHTS
		std::vector<std::vector<int>> weight;
		//2d array representing QUICKEST PATH
		std::vector<std::vector<int>> path;
		//ROW and COL VALUES
		int rows;
		int cols;
		
	public:
		//move constructor in O(N) COMPLEXITY and MEMORY
		Field(const std::vector<std::vector<int>> &rhs);
		//move assignment in O(N) COMPLEXITY AND MEMORY
		Field(std::vector<std::vector<int>> &&rhs);
		
		//in O(1) CONSTANT run-time return value from WEIGHT vector
		int Weight(int x1, int y1, int x2, int y2) const;
		
		//O(N) runtime return value from PATH vector
		int PathCost();
};

#endif
