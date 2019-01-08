#pragma once 

#include <string> 

#include "account.h" 
#include "account_manager.h" 

class AccountService 
{ 
    private: 
        AccountManager* pAccountManager; 

    public: 
        AccountService(); 

        void setAccountManager(AccountManager* pManager); 
        void transfer(const std::string& senderId, 
                const std::string& beneficiaryId, long amount); 
}; 
