#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <typeinfo>
#include <vector>

template <typename C, typename T>
void profile(size_t n, std::ostream &out = std::cout) {
    std::vector<T> vals(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<T> dist(0, n * 10);
    for (auto &x : vals) x = dist(rng);

    C a;
    std::string name = typeid(C).name();
    auto t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) a.push_back(vals[i]);
    auto t1 = std::chrono::high_resolution_clock::now();
    out << name << " push_back: " << std::chrono::duration<double>(t1 - t0).count() << "s" << std::endl;

    t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) a.ordered_insert(vals[i]);
    t1 = std::chrono::high_resolution_clock::now();
    out << name << " ordered_insert: " << std::chrono::duration<double>(t1 - t0).count() << "s" << std::endl;

    t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i) a.contains(vals[i]);
    t1 = std::chrono::high_resolution_clock::now();
    out << name << " contains: " << std::chrono::duration<double>(t1 - t0).count() << "s" << std::endl;

    t0 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n / 10; ++i) a.remove(vals[i]);
    t1 = std::chrono::high_resolution_clock::now();
    out << name << " remove: " << std::chrono::duration<double>(t1 - t0).count() << "s" << std::endl;

    t0 = std::chrono::high_resolution_clock::now();
    a.sort();
    t1 = std::chrono::high_resolution_clock::now();
    out << name << " sort: " << std::chrono::duration<double>(t1 - t0).count() << "s" << std::endl;

    t0 = std::chrono::high_resolution_clock::now();
    a.clear();
    t1 = std::chrono::high_resolution_clock::now();
    out << name << " clear: " << std::chrono::duration<double>(t1 - t0).count() << "s" << std::endl;

    C a1, a2;
    for (size_t i = 0; i < n; ++i) {
        a1.push_back(dist(rng));
        a2.push_back(dist(rng));
    }
    a1.sort();
    a2.sort();
    auto t_merge = std::chrono::high_resolution_clock::now();
    a1.merge(a2);
    auto t_merge_end = std::chrono::high_resolution_clock::now();
    out << name << " merge: " << std::chrono::duration<double>(t_merge_end - t_merge).count() << "s" << std::endl;
}
