#include <stdio.h>
#include <iostream>
#include <queue>
#include "globalVariables.h"

template<class T>
void printQueue(T& q)
{
    while (!q.empty())
    {
        std::cout << q.top() << std::endl;
        q.pop();
    }
}

void SamplePriorityQueue()
{
    std::priority_queue<int, std::vector<int>, std::greater<int> > q;

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q.push(n);

    printQueue(q);
}

void SamplePriorityQueueWithLamda() {
    auto compare = [](int lhs, int rhs) {
        return lhs < rhs;
    };

    std::priority_queue<int, std::vector<int>, decltype(compare)> q(compare);

    for(int n : {1,8,5,6,3,4,0,9,7,2})
        q.push(n);

    printQueue(q);
}

struct CustomCompare
{
    bool operator()(const int& lhs, const int& rhs) {
        return lhs < rhs;
    }
};

void SamplePriorityQueueWithCustomComparator()
{
    std::priority_queue<int,std::vector<int>, CustomCompare > pq;

    pq.push(3);
    pq.push(5);
    pq.push(1);
    pq.push(8);

    printQueue(pq);
}




int main(int argc, char const *argv[])
{   
    vector<LpaStarCell> cellVector = {
        LpaStarCell v1


    }


    SamplePriorityQueue();
    std::cout << std::endl;
    SamplePriorityQueueWithLamda();
    std::cout << std::endl;
    SamplePriorityQueueWithCustomComparator();
    return 0;
}
