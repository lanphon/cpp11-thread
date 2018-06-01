#include<memory>
#include<queue>
#include<mutex>
#include<thread>
#include<iostream>

template<typename T>
class threadsafe_queue
{
	public:
		threadsafe_queue()
		{}
		threadsafe_queue(const threadsafe_queue& rhs)
		{
			std::lock_guard<std::mutex> lk(m_mut);
			m_data = rhs.m_data;
		}
		threadsafe_queue& operator=(const threadsafe_queue&) = delete;

		void push(T new_value)
		{
			std::lock_guard<std::mutex> lk(m_mut);
			m_data.push(new_value);
			m_cond.notify_one();
		}

		bool try_pop(T& value)
		{
			std::lock_guard<std::mutex> lk(m_mut);
			if (m_data.empty())
				return false;

			value = m_data.front();
			m_data.pop();
			return true;
		}
		std::shared_ptr<T> try_pop()
		{
			std::lock_guard<std::mutex> lk(m_mut);
			if (m_data.empty())
				return std::shared_ptr<T>();

			std::shared_ptr<T> res = std::make_shared<T>(m_data.front());
			m_data.pop();
			return res;
		}

		void wait_and_pop(T& value)
		{
			std::unique_lock<std::mutex> lk(m_mut);
			m_cond.wait(lk, [this]{return !m_data.empty();});
			value = m_data.front();
			m_data.pop();
		}
		std::shared_ptr<T> wait_and_pop()
		{
			std::unique_lock<std::mutex> lk(m_mut);
			m_cond.wait(lk, [this]{return !m_data.empty();});
			std::shared_ptr<T> res(std::make_shared<T>(m_data.front()));
			m_data.pop();
			return res;
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> lk(m_mut);
			return m_data.empty();
		}
	private:
		std::mutex m_mut;
		std::queue<T> m_data;
		std::condition_variable m_cond;
};

std::mutex mut;

template<typename T>
void print(const T& t)
{
	std::lock_guard<std::mutex> lk(mut);
	std::cout << t <<std::endl;
}

using queue_t = threadsafe_queue<int>;

void prepare_data(queue_t& q)
{
	for (int i = 0; i < 100; i++) {
		print(i);
		q.push(i);
	}
	q.push(-1);
}

void process_data(queue_t& q)
{
	int v;
	do
	{
		auto pv = q.wait_and_pop();
		print(*pv);
		v = *pv;
	} while(v != -1);
}

int main()
{
	queue_t q;
	std::thread t0(prepare_data, std::ref(q));
	std::thread t1(process_data, std::ref(q));
	t0.join();
	t1.join();
	return 0;
}
