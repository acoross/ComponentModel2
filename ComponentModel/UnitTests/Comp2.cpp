#include "UnitTests/Comp2.h"
#include "UnitTests/Comp1.h"

int UnitTest::Component2::getComp1Val()
{
	return _comp1->val;
}
