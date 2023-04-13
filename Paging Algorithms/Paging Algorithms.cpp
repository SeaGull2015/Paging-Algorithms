// Paging Algorithms.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_set>
#include "page.h"

int FIFO(int availablePages, std::string refstr) {
    int pagefaults = 0;
    std::queue<page> q;
    for (int i = 0; i < refstr.size(); i++) {
        q.push(page(refstr[i] - '0')); // we actually don't care if it's the same object in this algorithm
    }

    std::vector<page> v(availablePages);
    int firstIn = 0;
    bool alreadyThere = false;
    int closestEmpty = -1;
    while (!q.empty()) {
        page currPage = q.front();
        for (int i = v.size() - 1; i >= 0; i--) {
            if (v[i].getId() == -1) {
                closestEmpty = i;
            }
            else if (currPage.getId() == v[i].getId()) {
                alreadyThere = true;
            }
        }

        // out
        std::cout << "Pages in ram:";
        for (int i = 0; i < v.size(); i++) {
            std::cout << v[i].getId() << " ";
        }
        std::cout << "\n";
        std::cout << "Putting page by id " << currPage.getId() << '\n';
        //

        if (alreadyThere) {
            std::cout << "Already in\n";
        }
        else if (closestEmpty != -1) {
            v[closestEmpty] = currPage;
            pagefaults++;
            std::cout << "Pagefault, Put page in " << closestEmpty << '\n';
        }
        else {
            v[firstIn] = currPage;
            firstIn++;
            if (firstIn >= v.size()) {
                firstIn = 0;
            }
            pagefaults++;
            std::cout << "Pagefault, Put page in " << firstIn << '\n';
        }

        alreadyThere = false;
        closestEmpty = -1;
        q.pop();
    }

    return pagefaults;
}

int LRU(int availablePages, std::string refstr) {
    int pagefaults = 0;
    std::queue<page> q;
    for (int i = 0; i < refstr.size(); i++) {
        q.push(page(refstr[i] - '0')); // we actually don't care if it's the same object in this algorithm
    }

    std::vector<page> v(availablePages);
    bool alreadyThere = false;
    int closestEmpty = -1;
    int t = 0;
    int LRUpage = 0;
    int LRUt = 0;
    while (!q.empty()) {
        page currPage = q.front();
        for (int i = v.size() - 1; i >= 0; i--) {
            if (v[i].getId() == -1) {
                closestEmpty = i;
            }
            else if (currPage.getId() == v[i].getId()) {
                alreadyThere = true;
                v[i].call(t); /// should move it into later if else block array, but I am not ready to debug it yet
                break;
            }
            else if (LRUt > v[i].getLastCalled()) {
                LRUpage = i;
                LRUt = v[i].getLastCalled();
            }
        }

        // out
        std::cout << "Pages in ram:";
        for (int i = 0; i < v.size(); i++) {
            std::cout << v[i].getId() << " ";
        }
        std::cout << "\n";
        std::cout << "Putting page by id, last called " << currPage.getId() << ";" << currPage.getLastCalled() << '\n';
        //

        if (alreadyThere) {
            std::cout << "Already in\n";
        }
        else if (closestEmpty != -1) {
            v[closestEmpty] = currPage;
            v[closestEmpty].call(t);
            pagefaults++;
            std::cout << "Pagefault, Put page in " << closestEmpty << '\n';
        }
        else {
            v[LRUpage] = currPage;
            v[LRUpage].call(t);
            pagefaults++;
            std::cout << "Pagefault, Put page in " << LRUpage << '\n';
        }

        alreadyThere = false;
        closestEmpty = -1;
        t++;
        LRUpage = 0;
        LRUt = t;
        q.pop();
    }

    return pagefaults;
}

int NFU(int availablePages, std::string refstr) {
    int pagefaults = 0;

    int pageNum = std::unordered_set<char>(std::begin(refstr), std::end(refstr)).size();
    std::vector<page> pageReference;

    for (int i = 0; i < pageNum; i++) { //suboptimal
        pageReference.push_back(page(i));
    }

    std::vector<page> v(availablePages);
    bool alreadyThere = false;
    int closestEmpty = -1;
    int NFUpage = 0;
    int NFUs = INT_MAX - 1;
    for (int k = 0; k < refstr.size(); k++) {
        page currPage = pageReference[refstr[k] - '0'];
        for (int i = v.size() - 1; i >= 0; i--) {
            if (v[i].getId() == -1) {
                closestEmpty = i;
            }
            else if (currPage.getId() == v[i].getId()) {
                alreadyThere = true;
                NFUpage = i;
                break;
            }
            else if (NFUs > v[i].getUses()) {
                NFUpage = i;
                NFUs = v[i].getUses();
            }
        }

        // out
        std::cout << "Pages in ram:";
        for (int i = 0; i < v.size(); i++) {
            std::cout << v[i].getId() << " ";
        }
        std::cout << "\n";
        std::cout << "Putting page by id, uses " << currPage.getId() << ";" << currPage.getUses() << '\n';
        //

        if (alreadyThere) {
            v[NFUpage].call();
            pageReference[refstr[k] - '0'].call();
            std::cout << "Already in\n";
        }
        else if (closestEmpty != -1) {
            v[closestEmpty] = currPage;
            v[closestEmpty].call();
            pageReference[refstr[k] - '0'].call();
            pagefaults++;
            std::cout << "Pagefault, Put page in " << closestEmpty << '\n';
        }
        else {
            v[NFUpage] = currPage;
            v[NFUpage].call();
            pageReference[refstr[k] - '0'].call();
            pagefaults++;
            std::cout << "Pagefault, Put page in " << NFUpage << '\n';
        }

        alreadyThere = false;
        closestEmpty = -1;
        NFUpage = 0;
        NFUs = INT_MAX - 1;
    }

    return pagefaults;
}

int main()
{
    int availablePages = 4;
    std::string refstr = "012301401234"; 
    /*std::cout << "Enter availablePages:\n";
    std::cin >> availablePages;
    std::cout << "Enter reference string:\n";
    std::cin >> refstr;*/
    std::cout << "Pagefaults:" << FIFO(availablePages, refstr) << '\n';
    std::cout << "Pagefaults:" << LRU(availablePages, refstr) << '\n';
    std::cout << "Pagefaults:" << NFU(availablePages, refstr) << '\n';
    std::cout << "Hello World!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
