#include<iostream>
#include<thread>

struct s_t
{
	int i = 0;
	int j = 0;
};

void acc(s_t& s)
{
	++ s.i;
}

void dec(s_t& s)
{
	-- s.j;
}

int main()
{
	s_t s;
	std::cout << "s is (" << s.i << ", " << s.j << ")" << std::endl;
	std::thread t0(acc, std::ref(s));
	std::thread t1(dec, std::ref(s));
	t0.join();
	t1.join();
	std::cout << "s is (" << s.i << ", " << s.j << ")" << std::endl;
	return 0;
}
