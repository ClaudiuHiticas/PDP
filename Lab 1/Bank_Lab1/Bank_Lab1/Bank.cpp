//
//  Bank.cpp
//  Bank_Lab1
//
//  Created by Claudiu Hiticas on 11/4/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//

#include "Bank.hpp"

Bank::Bank()
{
    
}

void Bank::saveAccount(BankAccount* bankAccount)
{
    this->bankAccountMap[bankAccount->getId()] = bankAccount;
}

bool Bank::transfer(BankAccount* sender, BankAccount *receiver, double amount)
{
    if(sender->getId() > receiver->getId())
    {
        sender->m.lock();
        receiver->m.lock();
    }
    else
    {
        receiver->m.lock();
        sender->m.lock();
    }
    
    if(sender->getAmount() < amount)
    {
        if(sender->getId() > receiver->getId())
        {
            sender->m.unlock();
            receiver->m.unlock();
        }
        else
        {
            receiver->m.unlock();
            sender->m.unlock();
        }
        
        return false;
    }
    
    sender->substractAmount(amount);
    receiver->addAmount(amount);
    sender->addLog(0-amount);
    receiver->addLog(amount);
    
    if(sender->getId() < receiver->getId())
    {
        sender->m.unlock();
        receiver->m.unlock();
    }
    else
    {
        receiver->m.unlock();
        sender->m.unlock();
    }
    return true;
}
