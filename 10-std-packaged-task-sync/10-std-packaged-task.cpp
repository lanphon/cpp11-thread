#include<iostream>
#include<future>
#include<cstdint>

using namespace std;

int myacc(int idx)
{
    int r = 0;
    for (int i = 0; i < idx; ++i)
    {
        r += i;
    }
    return r;
}

int main(int argc, char **argv)
{
    std::packaged_task<int(int)> a(myacc);

    auto f = a.get_future();

    // here the callable object 'a' is called just before we get the result from the future 'f'
    a(100);

    cout << "acc is " << f.get() << endl;
}
