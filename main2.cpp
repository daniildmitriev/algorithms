#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#define MAX_VALUE 1e9

struct S1 {
    int v1;
    int v2;
};

struct S2 {
    int v1;
    int v3[10000];
    int v2;
};

void inc(int& v) {
    for (int i = 0; i < MAX_VALUE; i++) {
        v++;
    }
}

int main() {
    S1 s1;
    S2 s2;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    std::thread t1(inc, std::ref(s1.v1));
    std::thread t2(inc, std::ref(s1.v2));
    
    t1.join();
    t2.join();
    
    end = std::chrono::system_clock::now();
    long long t = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "with: " << t << std::endl;
    
    start = std::chrono::system_clock::now();
    
    std::thread t3(inc, std::ref(s2.v1));
    std::thread t4(inc, std::ref(s2.v2));
    
    t3.join();
    t4.join();
    
    end = std::chrono::system_clock::now();
    
    t = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    
    std::cout << "without: " << t << std::endl;
    return 0;
}
