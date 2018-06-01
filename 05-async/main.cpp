#include<future>
#include<iostream>

int acc()
{
	int a = 0;
	for (unsigned int i = 0; i < 100; ++i) {
		a += i;
	}
	return a;
}

int main()
{
	std::future<int> f0 = std::async(acc);
	std::cout << "do some usuefully work" << std::endl;
	std::cout << "acc result is " << f0.get() << std::endl;
	return 0;
}
