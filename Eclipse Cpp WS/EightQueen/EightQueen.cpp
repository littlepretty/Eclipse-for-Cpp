/*
 * EightQueen.cpp
 *
 *  Created on: 2012-3-25
 *      Author: pc5
 */

/** @file
 *  @brief Definition of EightQueen class
 *  @author: yjq
 *  @date 2012-3-25
 */

#include "EightQueen.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <ostream>

/*
 * @brief Constructor of EightQueen with no parameters
 */
EightQueen::EightQueen()
: QueenNumber(8),
  Count(0),
  Position()
{
	// TODO Auto-generated constructor stub
}

EightQueen::EightQueen(int qn, int c)
{
	QueenNumber = qn;
	Count = c;
	for (int i = 0; i <= QueenNumber - 1; i++) {
		Position.push_back(1);
	}
}


/*
 * @brief Destructor of EightQueen, doing nothing
 */
EightQueen::~EightQueen() {
	// TODO Auto-generated destructor stub
}

/*
 * @brief Select every queen's position so that eight-queen problem is solved
 * @param index consider from which number of the queen
 */
std::vector<int> EightQueen::SafePut(int index) {
	for (int i = 0; i < QueenNumber; i++) {
		Position.at(index) = i + 1;
		if (CheckSafe(index)) {
			if (index == QueenNumber - 1) {
				Count = Count + 1;
				PrintQueen();
			} else
				SafePut(index + 1);
		}
	}
	return Position;

}

/*
 * @brief print every queen's column number, each on a row, and draw a O to represent a queen
 *
 */
void EightQueen::PrintQueen() {
	for (std::vector<int>::iterator iter = Position.begin(); iter
			!= Position.end(); iter++)
		std::cout << *iter;
	std::cout << " " << Count << '\n';

	for (int j = 0; j < QueenNumber; j++) {
		for (int i = 0; i < Position.at(j); i++) {
			std::cout << " ";
		}
		std::cout << "O";

		for (int i=Position.at(j)+1;i<QueenNumber;i++)
		{
			std::cout << " ";
		}
		std::cout<<'\n';
	}
	std::cout<<'\n';
}

/*
 * @brief check whether a queen's position is good considering all the located queens before
 * @param index consider from which number of the queen
 */
bool EightQueen::CheckSafe(int index) {
	for (int i = 0; i < index; i++) {
		if (Position.at(i) - Position.at(index) == i - index || Position.at(i)
				- Position.at(index) == index - i || Position.at(i)
				== Position.at(index))
			return false;
	}
	return true;

}

