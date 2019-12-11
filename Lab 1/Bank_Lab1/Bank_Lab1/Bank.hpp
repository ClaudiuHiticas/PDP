//
//  Bank.hpp
//  Bank_Lab1
//
//  Created by Claudiu Hiticas on 11/4/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//

#ifndef Bank_hpp
#define Bank_hpp

#include <stdio.h>
#include "Bank_Account.h"
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Bank{
public:
    map<int, BankAccount*> bankAccountMap;
    
    Bank();
    void saveAccount(BankAccount* bankAccount);
    bool transfer(BankAccount* sender, BankAccount* reciver, double amount);
    BankAccount* getBankAccountById(unsigned int id);
    ~Bank();
    
};


#endif /* Bank_hpp */
