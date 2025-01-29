#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex balance_mutex;  // Mutex for synchronizing access to balance
int balance = 100;    // Shared account balance

// Function for BotAlpha (Depositor)
void deposit(int amount, int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(balance_mutex);  // Locks automatically, unlocks when out of scope
        balance += amount;
        cout << "BotAlpha deposited " << amount << endl;
    }
}

// Function for BotBeta (Withdrawer)
void withdraw(int amount, int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(balance_mutex);  // Locks automatically, unlocks when out of scope
        if (balance >= amount) {  // Ensure sufficient balance before withdrawal
            balance -= amount;
            cout << "BotBeta withdrew " << amount << endl;
        } else {
            cout << "BotBeta tried to withdraw " << amount << " but insufficient balance!" << endl;
        }
    }
}

int main() {
    int deposit_amount = 50, withdraw_amount = 30;
    int transactions = 5;  // Number of times each bot performs its action

    thread t1(deposit, deposit_amount, transactions);
    thread t2(withdraw, withdraw_amount, transactions);

    t1.join();
    t2.join();

    cout << "Final Account Balance: " << balance << endl;
    return 0;
}
