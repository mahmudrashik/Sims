#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <semaphore.h>

using namespace std;

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

mutex buffer_mutex;
sem_t empty_slots, full_slots;

void wait_some_time() {
    this_thread::sleep_for(chrono::milliseconds(rand() % 150 + 50));
}

void producer() {
    for (int item = 0; item < 10; item++) {
        sem_wait(&empty_slots);
        buffer_mutex.lock();

        buffer[in] = item;
        cout << "Produced: " << item << " at index " << in << endl;
        in = (in + 1) % BUFFER_SIZE;

        buffer_mutex.unlock();
        sem_post(&full_slots);
        wait_some_time();
    }
}

void consumer() {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full_slots);
        buffer_mutex.lock();

        int item = buffer[out];
        cout << "Consumed: " << item << " from index " << out << endl;
        out = (out + 1) % BUFFER_SIZE;

        buffer_mutex.unlock();
        sem_post(&empty_slots);
        wait_some_time();
    }
}

int main() {
    srand(time(0));

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);

    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);

    return 0;
}
