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

using namespace std;

int numberOfLines, numberOfColumns;
int a[5000][5000], b[5000][5000], c[5000][5000];


void sumGranularity(int a[][5000], int b[][5000], int c[][5000], int numberOfLines, int numberOfColumns, int maxNumberOfElementsPerThread, int threadId)
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
}

void initialiseMatrix(int matrix[][5000], int numberOfLines, int numberOfColumns)
{
    for (int i=0; i<numberOfLines; i++)
    {
        for (int j=0; j<numberOfColumns; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

double mainFunc(int numberOfLines, int numberOfColumns, int numberOfThreads)
{
    initialiseMatrix(a, numberOfLines, numberOfColumns);
    initialiseMatrix(b, numberOfLines, numberOfColumns);
    
    int maxNumberOfElementsPerThread;
    int numberOfElements = numberOfLines * numberOfColumns;
    thread threads[numberOfThreads + 1];

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
        threads[i] = thread(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i);
    }

    for (int i=0; i<numberOfThreads; i++)
    {
        threads[i].join();
    }
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;

    return elapsed.count();
}

void statistic(int numberOfLines, int numberOfColumns, int numberOfThreads){
    double nr = 0;
    for(int i = 0; i < 20; ++i)
        nr += mainFunc(numberOfLines, numberOfColumns, numberOfThreads);
    cout<<"------------------------------------------\n";
    cout<<numberOfLines<<"   |  "<<numberOfColumns<<"   |  "<<numberOfThreads<<"  |  "<<nr/20<<"\n";
}

int main()
{
    cout<<"Lines | Columns | Threads | Time\n";
//    statistic(3, 5, 2);
//    statistic(3, 5, 2);
//    statistic(5000, 5000, 1);
//    statistic(5000, 5000, 2);
//    statistic(5000, 5000, 4);
//    statistic(5000, 5000, 8);
//    statistic(5000, 5000, 10);
    for(int i = 1; i <= 5000*5000; ++i)
        statistic(5000, 5000, i);
//    mainFunc(5000, 5000, 80);
    return 0;
}
