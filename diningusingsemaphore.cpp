#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <semaphore.h>
#include <chrono>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
sem_t forks[NUM_PHILOSOPHERS]; // Semaphores for forks
mutex printMutex; // Mutex for console output

// Function to print synchronized messages
void printAction(const string &action) {
    lock_guard<mutex> lock(printMutex);
    cout << action << endl;
}

// Simulate thinking
void think(int philosopher) {
    printAction("Philosopher " + to_string(philosopher) + " is thinking.");
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Simulate eating
void eat(int philosopher) {
    printAction("Philosopher " + to_string(philosopher) + " is eating.");
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Function to pick up forks
bool pick_up_forks(int philosopher) {
    int firstFork = philosopher;
    int secondFork = (philosopher % 2 == 0) ? (philosopher + 1) % NUM_PHILOSOPHERS : (philosopher - 1 + NUM_PHILOSOPHERS) % NUM_PHILOSOPHERS;

    // Pick up the first fork
    sem_wait(&forks[firstFork]);
    printAction("Philosopher " + to_string(philosopher) + " picked up fork " + to_string(firstFork) + ".");

    // Try to pick up the second fork
    if (sem_trywait(&forks[secondFork]) != 0) {
        // If the second fork is unavailable, put down the first fork
        sem_post(&forks[firstFork]);
        printAction("Philosopher " + to_string(philosopher) + " put down fork " + to_string(firstFork) + " (couldn't get second fork).");
        return false;
    }

    printAction("Philosopher " + to_string(philosopher) + " picked up fork " + to_string(secondFork) + ".");
    return true;
}

// Function to put down forks
void put_down_forks(int philosopher) {
    int firstFork = philosopher;
    int secondFork = (philosopher % 2 == 0) ? (philosopher + 1) % NUM_PHILOSOPHERS : (philosopher - 1 + NUM_PHILOSOPHERS) % NUM_PHILOSOPHERS;

    sem_post(&forks[firstFork]);
    sem_post(&forks[secondFork]);

    printAction("Philosopher " + to_string(philosopher) + " put down forks " + to_string(firstFork) + " and " + to_string(secondFork) + ".");
}

// Philosopher function
void philosopher(int id, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        think(id);
        while (!pick_up_forks(id)) {
            this_thread::sleep_for(chrono::milliseconds(50)); // Wait and retry
        }
        eat(id);
        put_down_forks(id);
    }
}

int main() {
    vector<thread> philosophers;
    int iterations = 2; // Number of times each philosopher eats

    // Initialize semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        sem_init(&forks[i], 0, 1); // Each fork semaphore initialized to 1 (available)
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i, iterations);
    }

    // Join philosopher threads
    for (auto &ph : philosophers) {
        ph.join();
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
