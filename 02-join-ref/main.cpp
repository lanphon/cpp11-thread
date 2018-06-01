#include<iostream>
#include<thread>

void acc(int& i)
{
	++i;
}

int main()
{
	int i = 0;
	std::cout << "i is " << i << std::endl;
	std::thread t(acc, std::ref(i));
	t.join();
	std::cout << "i is " << i << std::endl;
	return 0;
}
