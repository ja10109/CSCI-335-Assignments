//SquareMatrix class
#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <stdexcept>

template<typename T>
class SquareMatrix
{
	private:
		T** _ptr;
		size_t _size;
		
	public:
		//DEFAULT CONSTRUCTOR
		SquareMatrix():_ptr(nullptr),_size(0) 
		{}

		//PARAMATIZED CONSTRUCTOR used in +operator function
		SquareMatrix(size_t input_size) :_size(input_size) 
		{
			//allocate array
			_ptr = new T * [input_size];
			for (size_t i{ 0 }; i < _size; i++)
			{
				_ptr[i] = new T[input_size];
			}
		}

		//******Big Five*****//

		//DESTRUCTOR
		//deallocates all memory
		~SquareMatrix()
		{
			//first delete the dynamically allocated rows
			for (size_t i{ 0 }; i < _size; i++)
			{
				if(_ptr[i] != nullptr)
					delete[] _ptr[i];
			}
			//delete ptr
			if (_ptr != nullptr)
				delete[] _ptr;
		}

		//COPY CONSTRUCTOR
		//copies all elements from the SquareMatirx A = SquareMatrixB
		SquareMatrix(const SquareMatrix& trg):_ptr(trg._ptr),_size(trg._size)
		{
			//return if SquareMatrix ptr is nullptr
			if (trg._ptr == nullptr)
			{
				return;
			}

			//create a new dynamic array
			_ptr = new T * [_size];
			for (size_t i { 0 }; i < _size; i++)
			{
				//create array for each row, creating the 2D array
				_ptr[i] = new T[_size];
				for (size_t j{ 0 }; j < _size; j++)
				{
					//copy SquareMatrix values
					_ptr[i][j] = trg._ptr[i][j];
				}
			}
		}

		//MOVE CONSTRUCTOR
		//Moves Ptr to SquareMatrix B and MOVES it to A
		//moves trg._ptr to _ptr and sets trg._ptr to null
		SquareMatrix(SquareMatrix&& trg) :_ptr(trg._ptr),_size(trg._size)
		{
			//assign trg members to default values to prevent double-frees
			trg._ptr = nullptr;
			trg._size = 0;
		}

		//COPY ASSIGNMENT
		//sets Square Matrix A = Square Matrix B by copying B values into A
		SquareMatrix& operator=(const SquareMatrix& rhs)
		{
			//if SquareMatrix A == Square Matrix B return
			if (this == &rhs)
		 		return *this;
	
			//deallocate all memory of SquareMatrix A
			for (size_t i{ 0 }; i < _size; i++)
			{
				//deallocate rows
				if (_ptr[i] != nullptr)
					delete[] _ptr[i];
			}
			//deallocate ptr
			if(_ptr != nullptr)
				delete[] _ptr;
			

			//recreate SquareMatrix A dynamic array based off SquareMatrix B size
			_ptr = new T*[rhs._size];
			//set size to SquareMatrix B size
			_size = rhs._size;

			//allocate new memory for 2D Matrix
			for (size_t i{ 0 }; i < rhs._size; i++)
			{
				//create pointers for each row
				_ptr[i] = new T[rhs._size];
				for (size_t j{ 0 }; j < rhs._size; j++)
				{
					//set SquareMatrix A values to be same as SquareMatrix B values
					_ptr[i][j] = rhs._ptr[i][j];
				}
			}
			return *this;
		}

		//MOVE ASSIGNMENT
		//sets SquareMatrix A = SquareMatrix B by deallocating B and moving B ptr to A
		//if called by OPERATOR+ (C = A+B) move r-value SquareMatrix temp to C and deallocate temp
		SquareMatrix& operator=(SquareMatrix&& rhs)
		{
			//if both Matrixes are equal return
			if (this == &rhs)
			{
				return *this;
			}
			//deallocate R-value memory
			for (size_t i{ 0 }; i < _size; i++)
			{	
				//delete rows
				if(_ptr[i] != nullptr)
					delete[] _ptr[i];
			}
			//delete ptr
			if(_ptr != nullptr)
				delete[] _ptr;
		
			//move rhs ptr and size to current ptr 
			_ptr = rhs._ptr;
			_size = rhs._size;

			//assign rhs members to default to prevent double frees
			rhs._ptr = nullptr;
			rhs._size = 0;

			return *this;
		}

		//****END BIG FIVE*****//
		
		//RESIZE FUNCTION
		//Destroys all memory and content of the matrix and creates a new dynamic array based of new size input
		void resize(size_t new_size)
		{
			//destroy matrix
			for (size_t i{ 0 }; i < _size; i++)
			{
				//delete rows
				if(_ptr[i] != nullptr)
					delete[] _ptr[i];
			}
			//delete ptr
			if(_ptr != nullptr)
				delete[] _ptr;

			//create new dynamic array
			_ptr = new T*[new_size];
			for (size_t i{ 0 }; i < new_size; i++)
			{
				//create 2D array
				_ptr[i] = new T[new_size];
			}
			
			//set new_size
			_size = new_size; 

		}
		
		//OPERATOR==
		//returns true if Matrix A == Matrix B meaning size and values are equal
		bool operator==(const SquareMatrix& a)
		{
			//if A.size != B.size return false
			if (_size != a._size)
			{
				return false;
			}
			//else check both SquareMatrix elements for equality
			for (size_t i{ 0 }; i < _size; i++)
			{
				for (size_t j{ 0 }; j < _size; j++)
				{
					//if not equal return false
					if (_ptr[i][j] != a._ptr[i][j])
					{
						return false;
					}
				}
			}
			//size and all elements are equal return true
			return true;
		}

		//OPERATOR+ takes two SquareMatrix of same size A and B and returns SquareMatrix C with the sum of elements from A and B
		//SquareMatrix C = SquareMatrix A + SquareMatrix B,
		//A calls +operator with B as parameter resulting temp matrix C= this gets called by move assignment
		SquareMatrix operator+(const SquareMatrix& a) const
		{
			//call empty paramatized constructor to create temp matrix
			SquareMatrix<T> temp(a._size);
			for (size_t i{ 0 }; i < _size; i++)
			{
				for (size_t j{ 0 }; j < _size; j++)
				{
					//temp SquareMatrix takes the sum of A+B
					temp._ptr[i][j] = (a._ptr[i][j] + _ptr[i][j]);
				}
			}
			//return temp SquareMatrix to be used as a temporary r-value by move assignment
			return temp;
		}
		
		//AT function retuns _ptr to row,column coordinates
		T& at(size_t row, size_t column)
		{
			//if row or column doesnt exist throw exception
			if (row >= _size || column >= _size)
			{
				throw std::out_of_range("Row or column doesn't exist");
			}
			else
			{
				//return SquareMatrix coordinates
				return _ptr[row][column];
			}
		}
		
		//return SquareMatrix size
		size_t size() const
		{
			return _size;
		}
}; 
#endif
