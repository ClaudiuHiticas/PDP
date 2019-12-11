//
//  BankAccount.cpp
//  Bank_Lab1
//
//  Created by Claudiu Hiticas on 11/4/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//

#include "BankAccount.hpp"

BankAccount::BankAccount()
{
    
}


BankAccount::BankAccount(unsigned int id, double amount)
{
    this->id = id;
    this->amount = amount;
    this->log.push_back(amount);
}

void BankAccount::addAmount(double amount)
{
    this->amount += amount;
}

void BankAccount::substractAmount(double amount)
{
    this->amount -= amount;
}

void BankAccount::addLog(double operation)
{
    this->log.push_back(operation);
}

unsigned int BankAccount::getId()
{
    return this->id;
}

double BankAccount::getAmount()
{
    return this->amount;
}

vector<double> BankAccount::getLogs()
{
    return this->log;
}

BankAccount::~BankAccount()
{
    
}
