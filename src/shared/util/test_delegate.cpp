#include "delegate.h"
#include <iostream>

using namespace std;
using namespace srdgame;

class A
{
public:
	void a_fun(int)
	{
		cout << " A function" << endl;
	}
};
class B
{
public:
	void b_fun(int)
	{
		cout << " B function " << endl;
	}
};

int main()
{
	A a;
	B b;

	Delegate<void, int> d[2];
	d[0] = Delegate<void, int>::from_method<A, &A::a_fun>(&a);
	d[1] = Delegate<void, int>::from_method<B, &B::b_fun>(&b);

	d[0](0);
	d[1](0);
}
