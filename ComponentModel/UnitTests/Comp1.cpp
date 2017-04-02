#include "UnitTests/Comp1.h"
#include "UnitTests/Comp2.h"

UnitTest::Component1::Component1()
{
}

float UnitTest::Component1::GetComp2Val()
{
	return _comp2->val;
}
