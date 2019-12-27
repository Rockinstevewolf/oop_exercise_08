//================================================//
// M8O-201Б                                       //
// Баранников С.А.                                //
// Вариант 1: Треугольник, Прямоугольник, Квадрат //
//================================================//

#include <iostream>
#include <fstream>
#include <string>

#include <exception>
#include <vector>
#include <memory>

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Shape.h"
#include "Factory.h"
using namespace std;

using WorkingType = int; 

std::vector< std::shared_ptr<Shape<WorkingType>> > shapes;

std::mutex              mtx;
std::condition_variable sync;

int GetNum(void)
{
    int  k;
    bool got = false;
    while (!got)
    {
        cin >> k;
        if (!cin.good())
        {
            cout << "Bad input, ignored...\n" << flush;
            cin.clear();
            cin.ignore(256, '\n');
        }
        else got = true;
    }
    return k;
}

void save(const std::string& filePath)
{
    std::ofstream outfile;

    outfile.open(filePath, std::ios::out);
    if (outfile.fail())
    {
        throw std::runtime_error("File open operation failed");
    }

    std::vector< std::pair<WorkingType,WorkingType> > points;
    outfile << shapes.size() << '\t';
    for(std::shared_ptr<Shape<WorkingType>> e : shapes)
    {
        points = e->getPoints();
        outfile << e->getType() << '\t';
        switch(e->getType())
        {
            case 0:
                for (int i = 0; i < 3; ++i)
                {
                    outfile << points[i].first << '\t' << points[i].second << '\t';
                }
                break;
            
            case 1:

            case 2:
                for (int i = 0; i < 4; ++i)
                {
                    outfile << points[i].first << '\t' << points[i].second << '\t';
                }
                break;
        }
    }
    outfile.close();
}
    
void printer (int& work, const int size)
{
    std::string path;
    int saveNum = 1, figNum = 1;
    
    while(true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        sync.wait(lock, [&] { return (shapes.size() == size || !work); });
        if (!work) break;
        std::cout << "//=========================\\\\\n|| Beginning buffer output ||\n\\\\=========================//\n" << std::endl;
        path = "Saves/Buffer" + std::to_string(saveNum);
        try {
            save(path);
        }
        catch (std::runtime_error &e) {
            std::cout << "DURING SAVING: File failed to open" << std::endl;
        }

        ++saveNum;

        for (std::shared_ptr< Shape<WorkingType> > e : shapes)
        {
            std::cout << figNum++ << ". " << *e << "\n";
        }
        figNum = 1;

        shapes.clear();
        sync.notify_one();
    }
}

void getMenu()
{
    cout
      << "\n1. Add shape\n"
        << "2. Print all elements\n"
        << "3. Delete element\n"
        << "4. Clear shapes\n"
        << "5. Menu\n"
        << "0. Exit" << endl;
}

int main(int argc, char* argv[])
{

    int size = 5;
    
    if (argc > 1)
    {
        try 
        {
            size = std::stoi(argv[1]);
            if (size <= 0) throw std::invalid_argument("stoi");
        }
        catch (std::invalid_argument& e)
        {
            std::cerr << "Invalid parameter value given" << std::endl;
            return 0;
        }
    }

    std::pair<WorkingType, WorkingType> points[4];

    int figNum = 1, k = 1, work = 1;
    std::thread worker (printer, std::ref(work), size);

    std::cout << "\nMax buffer size is set to " << size;
    getMenu();

    do
    {
        std::unique_lock<std::mutex> lock(mtx);

        cout << "-----------------------\n> ";
        try
        {
            k = GetNum();
            if (!cin) throw runtime_error("Failed to get input");
            switch (k)
            {
            case 1:
				cout
                    << "1. Triangle\n"
					<< "2. Rectangle\n"
					<< "3. Square\n"
					<< "(Any other number will lead you back to menu)\n"
					<< "Pick the type of shape: ";
				k = GetNum();
				if (k >= 1 && k <= 3)
				{
                    --k;
                    switch (k)
                    {
                    case 0:
                        std::cout << "Input coordinates of points A,B,C like 'Xcord Ycord' with space\nA: ";
                        cin >> points[0].first >> points[0].second;
                        std::cout << "B: ";
                        cin >> points[1].first >> points[1].second;
                        std::cout << "C: ";
                        cin >> points[2].first >> points[2].second;
                        break;
                    
                    case 1:
                        // Intentionally empty

                    case 2:
                        std::cout << "Input coordinates of points A,B,C,D like 'Xcord Ycord' with space\nA: ";
                        std::cin >> points[0].first >> points[0].second;
                        std::cout << "B: ";
                        std::cin >> points[1].first >> points[1].second;
                        std::cout << "C: ";
                        std::cin >> points[2].first >> points[2].second;
                        std::cout << "D: ";
                        std::cin >> points[3].first >> points[3].second;
                        break;
                    
                    default:
                        break;
                    }
                    shapes.push_back(Factory<WorkingType>::create(k, points));
                    
                    if (shapes.size() == size) {
                        std::cout << "\n";
                        sync.notify_one();
                        sync.wait(lock, [&] { return shapes.empty(); });
                    }
				}
				k = 1;
				break;
            case 2:
                if (shapes.empty())
                {
                    cout << "No shapes are inserted" << endl;
                }
                else
                {
                    for (std::shared_ptr< Shape<WorkingType> > e : shapes)
                    {
                        std::cout << figNum++ << ". " << *e << "\n";
                    }
                    figNum = 1;
                }
                break;
            case 3:
                if (shapes.empty()) throw runtime_error("No shapes to delete");
                cout << "Number of the shape: ";
                k = GetNum();
                if (k < 1 || k > shapes.size()) throw runtime_error("Incorrect index");
                shapes.erase(shapes.begin() + k - 1);
                break;
            case 4:
                shapes.clear();
                std::cout << "All shapes are deleted" << std::endl;
                break;
            case 5:
                getMenu();
                break;
            case 0:
                work = 0;
                break;
            default:
                cout << "No such number" << endl;
            }

        }
        catch (runtime_error& err)
        {
            cerr << err.what() << endl;
        }
        catch (logic_error& err)
        {
            cerr << err.what() << endl;
        }
        catch (...)
        {
            cerr << "Exception thrown, but undefined." << endl;
        }
    } while (work);

    sync.notify_one();
    worker.join();
    return 0;
}