#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

void Worker(int index)
{
    std::stringstream ss;
    ss << "Thread № " << index << " is working now\n";
    std::cout << ss.str();
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        std::cerr << "Использование: " << argv[0] << " <N>\n"
                  << "  N — целое положительное число потоков.\n";
        return 1;
    }

    int n = 0;
    try 
    {
        std::size_t pos = 0;
        n = std::stoi(argv[1], &pos);

        if (pos != std::string(argv[1]).size() || n <= 0) 
        {
            throw std::invalid_argument("не положительное целое");
        }
    } 
    catch (const std::exception&) 
    {
        std::cerr << "Ошибка: аргумент '" << argv[1]
                  << "' не является целым положительным числом.\n";
        return 2;
    }

    std::vector<std::jthread> threads;
    threads.reserve(static_cast<std::size_t>(n));

    for (int i = 1; i <= n; i++) 
    {
        threads.emplace_back(Worker, i);
    }

    return 0;
}