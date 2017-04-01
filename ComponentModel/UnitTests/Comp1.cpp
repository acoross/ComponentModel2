#include "Comp1.h"
#include "./Comp2.h"

UnitTest::Component1::Component1()
{
}

float UnitTest::Component1::GetComp2Val()
{
	return _comp2->val;
}
