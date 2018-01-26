#include "account.h" 

Account::Account() 
{ 
} 

Account::Account(const std::string& accountId, long initialBalance) 
{ 
    this->accountId = accountId; 
    this->balance = initialBalance; 
} 

void Account::debit(long amount) 
{ 
    this->balance -= amount; 
} 

void Account::credit(long amount) 
{ 
    this->balance += amount; 
} 

long Account::getBalance() const 
{ 
    return this->balance; 
} 

std::string Account::getAccountId() const 
{ 
    return accountId; 
} 
