#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

static void sumVectors(double sumOfVectors, const std::vector<int> newVector1, const std::vector<int> newVector2)
{
    for (int i = 0; i < newVector1.size(); ++i)
    {
        sumOfVectors += newVector1.at(i) + newVector2.at(i);
    }
}

int main() 
{
    system("chcp 1251");
    std::cout << "Количество аппаратных ядер: " << std::thread::hardware_concurrency() << "\n\n";
    std::cout << std::setw(12) << "\t\t   1000" << "\t\t    10000" << "\t       100000" << "\t     1000000" << "\n";

    std::vector<int> vector1;
    std::vector<int> vector2;
    std::vector<int> numOfThreads{ 1, 2, 4, 8, 16 };
    std::vector<int> sizeOfVector{ 1000, 10000, 100000, 1000000 };

    for (auto& countThread : numOfThreads)
    {
        if (countThread == 1)
        {
            std::cout << std::endl << countThread << " поток";
        }
        else if (countThread == 2 || countThread == 4)
        {
            std::cout << std::endl << countThread << " потока";
        }
        else
        {
            std::cout << std::endl << countThread << " потоков";
        }

        for (auto& countVector : sizeOfVector)
        {
            vector1.resize(countVector, 1);
            vector2.resize(countVector, 2);
            std::vector<std::thread> threads;
            double sumOfVectors = 0;

            int partOfTheSize = static_cast<int>(countVector / countThread);

            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < countThread; i++)
            {
                std::vector<int> newVector1;
                std::vector<int> newVector2;
                int rightBorder = 0;

                if (i != countThread - 1)
                {
                    rightBorder = partOfTheSize * (i + 1);
                }
                else
                {
                    rightBorder = countVector;
                }

                for (int j = partOfTheSize * i; j < rightBorder; ++j)
                {
                    newVector1.push_back(vector1.at(j));
                    newVector2.push_back(vector2.at(j));
                }
                threads.push_back(std::thread(sumVectors, std::ref(sumOfVectors), newVector1, newVector2));
            }
            for (auto& it : threads)
            {
                it.join();
            }

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> totalTime = end - start;
            std::cout << std::setw(16) << totalTime.count() << "ms";
        }
        std::cout << "\n\n";
    }
    return 0;
}