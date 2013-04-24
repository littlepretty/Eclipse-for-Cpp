/*
 * EightQueen.h
 *
 *  Created on: 2012-3-25
 *
 */

/** @file
 *  @brief Declare EightQueen class
 *  definition available at @link EightQueen.cpp @endlink
 *  @author: yjq
 *  @date 2012-3-25
 */

#ifndef EIGHTQUEEN_H_
#define EIGHTQUEEN_H_
#include <vector>
class EightQueen
{
public:
	EightQueen();
	EightQueen(int qn, int c);
	virtual ~EightQueen();
	std::vector<int> SafePut(int index);
	void PrintQueen();
	bool CheckSafe(int index);
private:
	int QueenNumber;
	int Count;
	std::vector<int> Position;
};

#endif /* EIGHTQUEEN_H_ */
