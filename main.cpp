#include <iostream>
#include <cassert>
#include <thread>
#include <vector>

using namespace std;

#define BIG_NUM 100000000

class futexA {
    atomic<size_t> pid;
    hash<thread::id> hasher;
    
public:
    futexA() : pid(0) {}
    
    void lock() {
        size_t id = hasher(this_thread::get_id());
        size_t tmp = 0;
        while (!pid.compare_exchange_weak(tmp, id)) {
            this_thread::yield();
            tmp = 0;
        }
    }
    
    void unlock() {
        size_t id = hasher(this_thread::get_id());
        assert(id == pid);
        pid.store(0);
    }
};

class futexB {
    atomic<size_t> pid;
    hash<thread::id> hasher;
    
public:
    futexB() : pid(0) {}
    
    void lock() {
        size_t id = hasher(this_thread::get_id());
        size_t tmp = 0;
        while (!pid.compare_exchange_weak(tmp, id, memory_order_acquire)) {
            this_thread::yield();
            tmp = 0;
        }
    }
    
    void unlock() {
        size_t id = hasher(this_thread::get_id());
        assert(id == pid);
        pid.store(0, memory_order_release);
    }
};

class futexC {
    atomic<size_t> pid;
    hash<thread::id> hasher;
    
public:
    futexC() : pid(0) {}
    
    void lock() {
        size_t id = hasher(this_thread::get_id());
        size_t tmp = 0;
        while (!pid.compare_exchange_weak(tmp, id, memory_order_relaxed)) {
            this_thread::yield();
            tmp = 0;
        }
    }
    
    void unlock() {
        size_t id = hasher(this_thread::get_id());
        assert(id == pid);
        pid.store(0, memory_order_relaxed);
    }
};

template<typename MutexClass>
void increment(int &var, MutexClass &f) {
    while (true) {
        lock_guard<MutexClass> g(f);
        if (var < BIG_NUM) {
            var++;
        } else {
            break;
        }
    }
}

class myInt {
public:
    int x;
    myInt() {
        x = 0;
    }
};

myInt x;

template<typename MutexClass>
void countTime(MutexClass &m, const string &mutexType, int threadNumber) {
    cout << mutexType << "\n";
    vector <thread> th;
    x.x = 0;
    chrono::time_point<std::chrono::system_clock> start, end;
    start = chrono::system_clock::now();
    for (int i = 0; i < threadNumber; i++) {
        th.push_back(thread(increment<MutexClass>, ref(x.x), ref(m)));
    }
    for (int i = 0; i < threadNumber; i++) {
        th[i].join();
    }
    end = chrono::system_clock::now();
    cout << "result: " << x.x << "\n";
    long long tm = chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    cout << "time: " << tm << "\n";
}


int main() {
    futexA fa;
    futexB fb;
    futexC fc;
    countTime<futexA>(fa, "Slow futex", 4);
    countTime<futexB>(fb, "Normal futex", 4);
    countTime<futexC>(fc, "Volatile futex", 4);
    
}
