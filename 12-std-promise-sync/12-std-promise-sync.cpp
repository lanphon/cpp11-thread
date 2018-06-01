#include<iostream>
#include<future>
#include<thread>
#include<cstdint>

using namespace std;

int myacc(int num, std::promise<int>& p0)
{
    int sum = 0;
    for (int i = 0; i <  num; ++i)
    {
        sum += i;
        if (i == num / 2)
        {
            p0.set_value(sum);
        }
    }

    return sum;
}

int main(int argc, char **argv)
{
    std::promise<int> p0;

    auto f = p0.get_future();

    auto k = myacc(100, p0);

    cout << "promise-future get value " << f.get() << endl;
    cout << "function retur value " << k << endl;
}
