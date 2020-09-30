/*
双线程互锁（次序）执行
*/
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include<windows.h>

std::mutex data_mutex;
std::condition_variable data_var;
bool flag = true;
int k = 0;

void printA()
{
    while (1)
    {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        Sleep(1000);

        std::unique_lock<std::mutex> lck(data_mutex);
        data_var.wait(lck, [] {return flag;});
        std::cout << "thread: " << std::this_thread::get_id() << "   printf: " << ++k << std::endl;
        flag = false;
        data_var.notify_one();
    }
}

void printB()
{
    while (1)
    {
        std::unique_lock<std::mutex> lck(data_mutex);
        data_var.wait(lck, [] {return !flag;});
        std::cout << "thread: " << std::this_thread::get_id() << "   printf: " << --k << std::endl;
        flag = true;
        data_var.notify_one();
    }
}

int main()
{
    std::thread tA(printA);
    std::thread tB(printB);
    tA.join();
    tB.join();

    return 0;
}
