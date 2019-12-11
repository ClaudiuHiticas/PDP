//
//  main.cpp
//  Bank_Lab1
//
//  Created by Claudiu Hiticas on 11/4/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//

#include <iostream>
#include "Bank.hpp"
#include <thread>
#include <ctime>
#include <numeric>
#include <vector>
#include <assert.h>
#include <chrono>
#include <ratio>


using namespace std;
using namespace std::chrono;

Bank* initializeBank(int noOfBankAccounts)
{
    Bank* bank = new Bank();
    for(int i = 0; i < noOfBankAccounts; ++i)
    {
        BankAccount* bankAccount = new BankAccount(i+10, double(rand()%100000+1)/100);
        bank->saveAccount(bankAccount);
    }
    return bank;
}

void transfer(Bank* bank, int noOfBankAccounts, int transferNumbers)
{
    for(int i = 0; i <= transferNumbers; ++i)
    {
        int id1 = rand() % noOfBankAccounts + 10;
        int id2 = rand() % noOfBankAccounts + 10;
        while(id1 == id2)
        {
            id2 = rand() % noOfBankAccounts + 10;
        }
        double amount = double(rand() % 1000 + 1) / 100;
        bank->transfer(bank->bankAccountMap[id1], bank->bankAccountMap[id2], amount);
    }
}

double transferTest(int noOfBankAccounts, int transferNumbers, int noOfThreads)
{
    Bank* bank = initializeBank(noOfBankAccounts);
    thread thr[noOfThreads+1];
    
    auto start = chrono::high_resolution_clock::now();
    
    for(int i = 0; i < noOfThreads; ++i)
    {
        thr[i] = thread(transfer, bank, noOfBankAccounts, transferNumbers/noOfThreads);
    }
    
    for(int i = 0; i < noOfThreads; ++i)
    {
        thr[i].join();
    }
    auto finish = chrono::high_resolution_clock::now();
    
    chrono::duration<double> elapsed = finish - start;
    
    return elapsed.count();
}

void statistic(int noOfBankAccounts, int transferNumber, int noOfThreads)
{
    double nr = 0;
    int nrTest = 5;
    transferTest(noOfBankAccounts, transferNumber, noOfThreads);
    for(int i = 0; i < nrTest; ++i)
    {
        double x = transferTest(noOfBankAccounts, transferNumber, noOfThreads);
        nr += x;
        //cout << x << ", ";
    }
    cout << "----------------------------------------\n";
    cout << noOfThreads << " \t  | " << nr / 5 << "\n";
   
}

int main(int argc, const char * argv[])
{
    srand(time_t(NULL));
    cout<<"threads | seconds\n";
//    statistic(100000, 200000, 1);
//    statistic(100000, 200000, 2);
//    statistic(100000, 200000, 4);
//    statistic(100000, 200000, 8);
//    statistic(100000, 200000, 16);
    statistic(50000,2000,8);
    statistic(50000,2000,50);
    statistic(50000,2000,100);
    statistic(50000,2000,500);
    statistic(50000,2000,1000);
    return 0;
}
