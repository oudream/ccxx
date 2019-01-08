#include "account_service.h" 

AccountService::AccountService() 
{ 
    this->pAccountManager = NULL; 
} 

void AccountService::setAccountManager(AccountManager* pManager) 
{ 
    this->pAccountManager = pManager; 
} 

void AccountService::transfer(const std::string& senderId, 
        const std::string& beneficiaryId, long amount) 
{ 
    Account sender = this->pAccountManager->findAccountForUser(senderId); 

    Account beneficiary = this->pAccountManager->findAccountForUser(beneficiaryId); 

    sender.debit(amount); 

    beneficiary.credit(amount); 

    this->pAccountManager->updateAccount(sender); 

    this->pAccountManager->updateAccount(beneficiary); 
}
