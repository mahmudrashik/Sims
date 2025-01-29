#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

#define NUM_PHILOSOPHERS 5
#define itr_cnt 1

mutex forks[NUM_PHILOSOPHERS]; // Mutexes for forks
mutex cout_mutex;
mutex table_mutex;
condition_variable fork_condition[NUM_PHILOSOPHERS];
bool fork_available[NUM_PHILOSOPHERS] = {true, true, true, true, true};

void think(int i) {
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "\nPhilosopher " << i << " is thinking.\n" << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(500 + rand() % 500));
}

void eat(int i) {
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " is eating." << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(500 + rand() % 500));
}

void pick_up_forks(int i) {
    unique_lock<mutex> lock(table_mutex);
    while (!fork_available[i]) {
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << i << " is waiting for fork " << i << "." << endl;
        }
        fork_condition[i].wait(lock);
    }
    fork_available[i] = false;
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " picked up fork " << i << "." << endl;
    }

    while (!fork_available[(i + 1) % NUM_PHILOSOPHERS]) {
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << i << " is waiting for fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
        }
        fork_condition[(i + 1) % NUM_PHILOSOPHERS].wait(lock);
    }
    fork_available[(i + 1) % NUM_PHILOSOPHERS] = false;
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " picked up fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
    }
}

void put_down_forks(int i) {
    {
        lock_guard<mutex> lock(table_mutex);
        fork_available[i] = true;
        fork_available[(i + 1) % NUM_PHILOSOPHERS] = true;
        fork_condition[i].notify_one();
        fork_condition[(i + 1) % NUM_PHILOSOPHERS].notify_one();
    }

    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " put down fork " << i << "." << endl;
        cout << "Philosopher " << i << " put down fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
    }
    think(i);
}

void philosopher(int i) {
    for (int x = 0; x < itr_cnt; x++) {
        think(i);
        pick_up_forks(i);
        eat(i);
        put_down_forks(i);
    }
}

int main() {
    vector<thread> philosophers;
    srand(time(NULL));

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.push_back(thread(philosopher, i));
    }

    for (auto &t : philosophers) {
        t.join();
    }
    cout<<"ALL DONE";
    return 0;
}
