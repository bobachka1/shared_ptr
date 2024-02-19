#include <iostream>
#include "shared_ptr.h"

int main()
{
	shared_ptr<int> b = new int (1);
	shared_ptr<int> a(b);
	*b = 3;
	return 0;
}