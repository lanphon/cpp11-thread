#include<iostream>
#include<thread>
#include<mutex>
#include<unistd.h>

class Y
{
    int m_v;
    mutable std::mutex m_mutex;

    int retrieve_value() const
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_v;
    }

    public:
    Y(int v)
        : m_v(v)
    {}

    void set(int v)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_v = v;
    }

    friend bool operator==(const Y& lhs, const Y& rhs)
    {
        if (&lhs == &rhs) return true;
        auto v0 = lhs.retrieve_value();
        auto v1 = rhs.retrieve_value();

        return v0 == v1;
    }
};

Y g0(10);
Y g1(10);

void compare()
{
    // the result would be equal/non equal randomly, because
    // the retrieved value may be changed, since the protection
    // granule is too small
    // try to run it about 100~300 times
    if (g0 == g1)
    {
        std::cout << "equal" << std::endl;
    }
    else
    {
        std::cout << "not equal" << std::endl;
    }
}

int main()
{
    std::thread t0(compare);

    usleep(random() % 13);
    g0.set(11);

    t0.join();
}
