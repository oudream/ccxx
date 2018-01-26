#include <map> 
#include <string> 

#include <gtest/gtest.h> 
#include <gmock/gmock.h> 

#include "account.h" 
#include "account_service.h" 
#include "account_manager.h" 

// MockAccountManager, mock AccountManager with googlemock 
class MockAccountManager : public AccountManager 
{ 
    public: 
        MOCK_METHOD1(findAccountForUser, Account(const std::string&)); 

        MOCK_METHOD1(updateAccount, void(const Account&)); 
}; 

// A facility class acts as an external DB 
class AccountHelper 
{ 
    private: 
        std::map<std::string, Account> mAccount; 
        // an internal map to store all Accounts for test 

    public: 
        AccountHelper(std::map<std::string, Account>& mAccount); 

        void updateAccount(const Account& account); 

        Account findAccountForUser(const std::string& userId); 
}; 

AccountHelper::AccountHelper(std::map<std::string, Account>& mAccount) 
{ 
    this->mAccount = mAccount; 
} 

void AccountHelper::updateAccount(const Account& account) 
{ 
    this->mAccount[account.getAccountId()] = account; 
} 

Account AccountHelper::findAccountForUser(const std::string& userId) 
{ 
    if (this->mAccount.find(userId) != this->mAccount.end()) 
        return this->mAccount[userId]; 
    else 
        return Account(); 
} 

// Test case to test AccountService 
TEST(AccountServiceTest, transferTest) 
{ 
    std::map<std::string, Account> mAccount; 
    mAccount["A"] = Account("A", 3000); 
    mAccount["B"] = Account("B", 2000); 
    AccountHelper helper(mAccount); 

    MockAccountManager* pManager = new MockAccountManager(); 

    // specify the behavior of MockAccountManager 
    // always invoke AccountHelper::findAccountForUser 
    // when AccountManager::findAccountForUser is invoked 
    EXPECT_CALL(*pManager, findAccountForUser(testing::_)).WillRepeatedly( 
            testing::Invoke(&helper, &AccountHelper::findAccountForUser)); 

    // always invoke AccountHelper::updateAccount 
    //when AccountManager::updateAccount is invoked 
    EXPECT_CALL(*pManager, updateAccount(testing::_)).WillRepeatedly( 
            testing::Invoke(&helper, &AccountHelper::updateAccount)); 

    AccountService as; 
    // inject the MockAccountManager object into AccountService 
    as.setAccountManager(pManager); 

    // operate AccountService 
    as.transfer("A", "B", 1005); 

    // check the balance of Account("A") and Account("B") to 
    //verify that AccountService has done the right job 
    EXPECT_EQ(1995, helper.findAccountForUser("A").getBalance()); 
    EXPECT_EQ(3005, helper.findAccountForUser("B").getBalance()); 

    delete pManager; 
} 
