#include <iostream>
#include <chrono>

class Timer {
public:
    // Starts or resets the timer
    void start() {
        m_StartTime = std::chrono::steady_clock::now();
        m_bRunning = true;
    }

    // Stops the timer to freeze the elapsed time
    void stop() {
        m_EndTime = std::chrono::steady_clock::now();
        m_bRunning = false;
    }

    // Returns elapsed milliseconds
    double elapsedMilliseconds() const {
        std::chrono::time_point<std::chrono::steady_clock> endTime;
        
        if (m_bRunning) {
            endTime = std::chrono::steady_clock::now();
        } else {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }

    // Returns elapsed microseconds (higher precision)
    double elapsedMicroseconds() const {
        std::chrono::time_point<std::chrono::steady_clock> endTime;
        
        if (m_bRunning) {
            endTime = std::chrono::steady_clock::now();
        } else {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
    }

    // Returns elapsed seconds as a fractional double
    double elapsedSeconds() const {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_EndTime;
    bool m_bRunning = false;
};
