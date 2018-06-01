#include<iostream>
#include<thread>
#include<unistd.h>

class hierarchy_mutex_t
{
        static uint32_t thread_local m_current_level;

        const uint32_t m_level;
        uint32_t m_prev_level;
        std::mutex m_mutex;

        void check()
        {
            if (m_prev_level < m_level)
            {
                /// try to comment this line out, which may cause dead-lock
                //throw std::runtime_error("wrong hierarcyh");
            }
        }

        void update()
        {
            m_prev_level = m_current_level;
            m_current_level = m_level;
        }
        void restore()
        {
            m_current_level = m_prev_level;
        }
    public:
        hierarchy_mutex_t(uint32_t level)
            : m_level(level)
        {}

        void lock()
        {
            check();
            m_mutex.lock();
        }

        void unlock()
        {
            m_current_level = m_prev_level;
            m_mutex.unlock();
        }

        bool try_lock()
        {
            check();
            if (!m_mutex.try_lock())
            {
                return false;
            }
            return true;
        }
};

thread_local uint32_t hierarchy_mutex_t::m_current_level = UINT_MAX;

hierarchy_mutex_t g0_mutex(1000);
hierarchy_mutex_t g1_mutex(500);
hierarchy_mutex_t g2_mutex(30);

void func0()
{
    usleep(random() % 130);
    std::lock_guard<hierarchy_mutex_t> lk0(g0_mutex);
    usleep(random() % 73);
    std::lock_guard<hierarchy_mutex_t> lk1(g1_mutex);
}

void func1()
{
    usleep(random() % 138);
    std::lock_guard<hierarchy_mutex_t> lk1(g1_mutex);
    usleep(random() % 74);
    std::lock_guard<hierarchy_mutex_t> lk0(g0_mutex);
}

int main(int argc, char **argv)
{
    std::thread t0(func0);
    std::thread t1(func1);

    t0.join();
    t1.join();

    return 0;
}
