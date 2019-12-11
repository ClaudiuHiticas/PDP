//
//  Bank_Account.h
//  Bank_Lab1
//
//  Created by Claudiu Hiticas on 11/4/19.
//  Copyright © 2019 Claudiu Hiticas. All rights reserved.
//

#ifndef Bank_Account_h
#define Bank_Account_h
#include <string>
#include <vector>
#include <mutex>

using namespace std;

class BankAccount{
private:
    unsigned int id;
    double amount;
    vector<double> log;
public:
    mutex m;
    
    BankAccount();
    BankAccount(unsigned int id, double amount);
    
    void addAmount(double amount);
    void substractAmount(double amount);
    void addLog(double operation);
    unsigned int getId();
    double getAmount();
    vector<double> getLogs();
    ~BankAccount();
};



#endif /* Bank_Account_h */
