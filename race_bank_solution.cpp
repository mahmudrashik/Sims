
#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <mutex>

using namespace std;

int account_balance = 2000;
mutex cout_mutex;

void wait_some_time() {
    this_thread::sleep_for(chrono::milliseconds(rand() % 100 + 50));
}

void botAlpha() {
    for (int i = 0; i < 10; i++) {
        int temp = account_balance;
        temp += 50;
        wait_some_time();
        account_balance = temp;

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "BotAlpha deposited 50" << endl;
        }
    }
}

void botBeta() {
    for (int i = 0; i < 10; i++) {
        int temp = account_balance;
        temp -= 30;
        wait_some_time();
        account_balance = temp;

        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "BotBeta withdrew 30" << endl;
        }
    }
}

int main() {
    srand(time(0));

    thread alpha(botAlpha);
    thread beta(botBeta);

    alpha.join();
    beta.join();

    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Final Account Balance: " << account_balance << endl;
    }

    return 0;
}
