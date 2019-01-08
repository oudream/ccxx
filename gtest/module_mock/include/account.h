#pragma once

#include <string> 

class Account 
{ 
    private: 
        std::string accountId; 

        long balance; 

    public: 
        Account(); 

        Account(const std::string& accountId, long initialBalance); 

        void debit(long amount); 

        void credit(long amount); 

        long getBalance() const; 

        std::string getAccountId() const; 
}; 
