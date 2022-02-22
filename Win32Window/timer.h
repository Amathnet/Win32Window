#include <chrono>
#include <thread>

class Timer
{
public:
    virtual void timer_function() = 0;

    // sleep im milliseconds
    void timer_start(int sleep)
    {
        sleep = m_sleep;
        m_state = 1;
        std::thread(&Timer::logic, this).detach();
    }
    void timer_stop()
    {
        m_state = 0;
    }

private:
    void logic()
    {
        while (m_state)
        {
            timer_function();
            std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep));
        }
    }

    bool m_state; // start/timer_stop
    int m_sleep;
};