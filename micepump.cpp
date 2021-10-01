// micepump.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>


struct Runner
{
    
    Runner(const std::string &fname) : m_thread{ nullptr }, m_fileName{ fname } 
    {
        std::ofstream m{ fname };
        m << "!!BEGIN!!";
        m.close();
        srand(time(NULL));
    }

    virtual ~Runner()
    {
        if (m_thread && m_thread->joinable()) {
            m_thread->join();
            delete m_thread;
        }        
        
        m_thread = nullptr; 
    }


    int Run()
    {
        m_thread = new std::thread{ [&] {
            for (;;)
            {
                if (readall()) {
                    break;
                }
                sendev();
                ::Sleep(10000);
            }
        } };
        m_thread->join(); 
        return 0; 
    }

protected:

    int randcoord()
    {
        return rand() % 1000 + 1;
    }

    void sendev()
    {
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.dx = randcoord();
        input.mi.dy = randcoord();

        input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP);
        input.mi.mouseData = 0; 
        input.mi.dwExtraInfo = NULL;
        input.mi.time = 0;
        SendInput(1, &input, sizeof(input));
    }


    bool readall(void)
    {
        std::ifstream file;
        file = std::ifstream(m_fileName);
        std::string line;
        while (std::getline(file, line))
        {
            if (strstr(line.c_str(), "stop"))
            {
                file.close();
                std::ofstream of{ m_fileName };
                of << "!!END!!";
                of.close();
                return true;
            }
        }
        return false;
    }

    std::thread* m_thread;
    std::string m_fileName;
};


int main(int argc, char** argv)
{
    Runner rnr{ std::string{"ivz.pipe"} };
    return rnr.Run();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
