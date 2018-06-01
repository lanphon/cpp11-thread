#include<iostream>
#include<thread>
#include<mutex>
#include<unistd.h>

int g_int = 0;
std::mutex g_mutex;
void acc0()
{
    for (uint32_t i = 0; i < 10; ++i)
    {
        /// please try to comment following line and try several times
        //std::lock_guard<std::mutex> lk(g_mutex);
        g_int ++;
        auto t = random();
        t = t%180;
        usleep(t);
    }
}

int main()
{
    std::thread t0(acc0);
    std::thread t1(acc0);


    t0.join();
    t1.join();
    std::cout << "g is " << g_int << std::endl;
}

