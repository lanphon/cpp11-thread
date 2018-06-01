#include<iostream>
#include<thread>

#include<vector>
#include<cstdint>
#include<mutex>

template<typename T>
class ring_buf
{
	public:
		using buf_len_t = uint32_t;
		const static buf_len_t BUF_LEN = 32;
		ring_buf()
		{}
		~ring_buf()
		{
			std::cout << "Release" << std::endl;
		}

		void push(const T& t)
		{
			while(is_full());
			//std::lock_guard<std::mutex> guard(m_mutex);
			m_rb[m_start] = t;
			acc_counter(m_start);
		}

		T pop()
		{
			while(is_empty());
			//std::lock_guard<std::mutex> guard(m_mutex);
			T result = m_rb[m_end];
			acc_counter(m_end);
			return result;
		}
	protected:
		static void acc_counter(buf_len_t& counter)
		{
			counter ++;
			if (counter >= BUF_LEN) {
				counter -= BUF_LEN;
			}
		}

		bool is_full()
		{
			//std::lock_guard<std::mutex> guard(m_mutex);
			return ((m_end + 1) % BUF_LEN) == m_start;
		}

		bool is_empty()
		{
			//std::lock_guard<std::mutex> guard(m_mutex);
			return m_start == m_end;
		}
	protected:
		std::vector<T> m_rb = std::vector<T>(BUF_LEN, T());
		buf_len_t m_start = 0;
		buf_len_t m_end = 0;
		std::mutex m_mutex;
};

using rb_type = ring_buf<uint32_t>;
const static uint32_t LOOP_TIME = 100;

void producer(rb_type& rb);
void consumer(rb_type& rb);

rb_type rb;
int main()
{
	std::thread prod(producer, std::ref(rb));
	std::thread cons(consumer, std::ref(rb));
	if (prod.joinable())
		prod.join();
	if (cons.joinable())
		cons.join();
	std::cout << "A\n";
	return 0;
}

void producer(rb_type& rb)
{
	for (uint32_t i = 0; i < LOOP_TIME; ++i)
	{
		rb.push(i);
	}
	std::cout << "Output Ok\n";
	return;
}

void consumer(rb_type& rb)
{
	for (uint32_t i = 0; i < LOOP_TIME; ++i) {
		auto result = rb.pop();
		std::cout << result << std::endl;
	}
	std::cout << "Receive OK\n";
	return;
}
