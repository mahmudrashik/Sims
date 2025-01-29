#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 5; // Fixed buffer size

queue<int> buffer; // Shared buffer (FIFO queue)
mutex mtx; // Mutex to protect shared buffer
condition_variable buffer_not_full, buffer_not_empty; // Condition variables
int in = 0, out = 0; // Buffer index tracking

void producer() {
    for (int item = 0; item < 10; item++) {
        unique_lock<mutex> lock(mtx);
        buffer_not_full.wait(lock, [] { return buffer.size() < BUFFER_SIZE; }); // Wait if buffer is full

        // Produce item
        buffer.push(item);
        cout << "Produced: " << item << " at index " << in << endl;
        in = (in + 1) % BUFFER_SIZE;

        buffer_not_empty.notify_one(); // Notify consumer that an item is available
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(500)); // Simulate delay
    }
}

void consumer() {
    for (int i = 0; i < 10; i++) {
        unique_lock<mutex> lock(mtx);
        buffer_not_empty.wait(lock, [] { return !buffer.empty(); }); // Wait if buffer is empty

        // Consume item
        int item = buffer.front();
        buffer.pop();
        cout << "Consumed: " << item << " from index " << out << endl;
        out = (out + 1) % BUFFER_SIZE;

        buffer_not_full.notify_one(); // Notify producer that space is available
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(800)); // Simulate delay
    }
}

int main() {
    thread t1(producer);
    thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
