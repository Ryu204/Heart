#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>
#include <thread>
#include <Windows.h>
#include <fstream>
#include <conio.h>

/* Time classes */
typedef std::chrono::duration<float> Time;
typedef std::chrono::time_point<std::chrono::steady_clock> Moment;

class Clock
{
public:
    Clock()
        : m_elapsed(m_clock.now())
    { };
    Time restart()
    {
        Moment tmp = m_elapsed;
        m_elapsed = m_clock.now();  
        return m_elapsed - tmp;
    }
private:
    std::chrono::steady_clock m_clock;
    Moment m_elapsed;
};

/* Canvas */
const std::size_t WIDTH = 120;
const std::size_t HEIGHT = 30;

class Canvas
{
public:
    Canvas()
        : m_np_run(false)
    {
        m_vec.resize(HEIGHT);
        for (auto& i : m_vec)
            i.resize(WIDTH);
    };
    void fill(const char& a)
    {
        for (auto& i : m_vec)
            for (auto& j : i)
                j = a;
    };
    void draw(const char& a, std::size_t row, std::size_t col)
    {
        assert(row >= HEIGHT || col >= WIDTH);
        m_vec[row][col] = a;
    }
    void render()
    {
        m_writer.open("tmp.txt");
        for (const auto& i : m_vec)
        {
            for (const auto& j : i)
                m_writer << j;
            // std::cout << '\n';
        }
        m_writer.close();
        system("start notepad tmp.txt");
        Sleep(1000);
        system("taskkill /F /im notepad.exe");
    }
private:
    std::vector<std::vector<char>> m_vec;
    std::ofstream m_writer;
    bool m_np_run;
};

/* Testing environment */

int tmain()
{
    Canvas can;
    Clock c;
    Time dt = c.restart();
    std::string s = "Hello World", u = "";
    while (true)
    {
        if (_kbhit())
            return 0;
        dt += c.restart();
        while (dt.count() > 1/20.f)
        {
            dt -= Time(1/20.f);
            if (!s.empty())
            {
                u += s[0];
                s = s.substr(1, s.size() - 1);
            }
        }

        can.fill('0');
        can.render();
        // Sleep(100);
    }
}