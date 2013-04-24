/*
 * Test.cpp
 *
 *  Created on: 2012-5-2
 *      Author: pc5
 */
#include "SingularTuning.h"
int main()
{
	SingularTuning st(2);
	st.ReadInfo();
	st.InitX();
	st.Solution();
	st.Output();
	return 0;
}
