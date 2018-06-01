#include<iostream>
#include<thread>
#include<cassert>
#include<unistd.h>

int a  = 0;
int b = 0;

void func0()
{
    usleep(random() % 173);
    a = 1;
    usleep(random() % 173);
    b = 2;
}

void func1()
{
    usleep(random() % 103);

    // on X86, this is ensurd, that if b's value is seen by this thread,
    // then a's update must have been done. X86 is seq_lst implicitt
    while(b != 2);
    assert(a == 1);
}

int main()
{
    std::thread t0(func0);
    std::thread t1(func1);

    t0.join();
    t1.join();

    return 0;
}
