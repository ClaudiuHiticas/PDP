//
//  main.cpp
//  Sum_Matrix
//
//  Created by Claudiu Hiticas on 11/4/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//
  
#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <future>
#include <vector>
#include "ThreadPool.h"

using namespace std;

const int NORMAL_WAY = 1;
const int ASYNC_WAY = 2;
const int POOL_WAY = 3;

int numberOfLines, numberOfColumns;
int a[1000][1000], b[1000][1000], c[1000][1000];
bool isDone[1000];

void sumGranularity(int a[][1000], int b[][1000], int c[][1000], int numberOfLines, int numberOfColumns, int maxNumberOfElementsPerThread, int threadId)
{
    int counter = 0;
    int startsFrom = threadId * maxNumberOfElementsPerThread;
    int lineStart = startsFrom / numberOfColumns;
    int columnStart = startsFrom % numberOfColumns;
    for (int i=lineStart; i<numberOfLines && counter < maxNumberOfElementsPerThread; i++)
    {
        for (int j=columnStart; j<numberOfColumns && counter < maxNumberOfElementsPerThread; j++)
        {
            c[i][j] = a[i][j] + b[i][j];
            counter++;
        }
        columnStart = 0;
    }
    isDone[threadId] = true;
}

void initialiseMatrix(int matrix[][1000], int numberOfLines, int numberOfColumns)
{
    for (int i=0; i<numberOfLines; i++)
    {
        for (int j=0; j<numberOfColumns; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

double mainFunc(int numberOfLines, int numberOfColumns, int numberOfThreads, int theWay)
{
    initialiseMatrix(a, numberOfLines, numberOfColumns);
    initialiseMatrix(b, numberOfLines, numberOfColumns);
    
    int maxNumberOfElementsPerThread;
    int numberOfElements = numberOfLines * numberOfColumns;
    thread threads[numberOfThreads + 1];

    vector<future<void>> futures;
    ThreadPool pool(numberOfThreads);
    for(int i = 0; i <numberOfThreads; ++i)
        isDone[i] = false;
    
    if (numberOfElements % numberOfThreads == 0)
    {
        maxNumberOfElementsPerThread = numberOfElements / numberOfThreads;
    }
    else
    {
        maxNumberOfElementsPerThread = numberOfElements / numberOfThreads + 1;
    }

    auto start = chrono::high_resolution_clock::now();
    for (int i=0; i<numberOfThreads; i++)
    {

        if(theWay == POOL_WAY)
        {
            futures.push_back(pool.enqueue(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i));
        }
        else
        if(theWay == ASYNC_WAY)
        {
            futures.push_back(async(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i));
        }
        else
        if(theWay == NORMAL_WAY)
        {
            threads[i] = thread(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i);
        }
        
    }

    for (int i=0; i<numberOfThreads; i++)
    {
        if(theWay == POOL_WAY)
            futures[i].get();
        else
        if(theWay == NORMAL_WAY)
            threads[i].join();
        else
        if(theWay == ASYNC_WAY)
        {
            while(true)
            {
                bool toBreak = true;
                for(int i = 0; i < futures.size(); ++i)
                   if(isDone[i] == false)
                       toBreak = false;
                if(toBreak)
                   break;
            }
        }
    }
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;

    return elapsed.count();
}

void statistic(int numberOfLines, int numberOfColumns, int numberOfThreads){
    double nrNormal = 0;
    double nrAsync = 0;
    double nrPool = 0;
    int iterationNumber = 20;
    for(int i = 0; i < iterationNumber; ++i)
    {
        nrNormal += mainFunc(numberOfLines, numberOfColumns, numberOfThreads, NORMAL_WAY);
        nrPool += mainFunc(numberOfLines, numberOfColumns, numberOfThreads, POOL_WAY);
        nrAsync += mainFunc(numberOfLines, numberOfColumns, numberOfThreads, ASYNC_WAY);
    }
    
    cout<<"----------------------------\n";
    cout<<"Async   | "<<numberOfThreads<<"  |  "<<nrAsync/iterationNumber<<"\n";
    cout<<"Pool    | "<<numberOfThreads<<"  |  "<<nrPool/iterationNumber<<"\n";
    cout<<"Normal  | "<<numberOfThreads<<"  |  "<<nrNormal/iterationNumber<<"\n";
}

int main()
{
    cout<<"Way     | Threads | Time\n";
    statistic(1000, 1000, 1);
    statistic(1000, 1000, 2);
    statistic(1000, 1000, 4);
    statistic(1000, 1000, 8);
    statistic(1000, 1000, 16);
//    for(int i = 1; i <= 1000*1000; ++i)
//        statistic(1000, 1000, i);

    return 0;
}
