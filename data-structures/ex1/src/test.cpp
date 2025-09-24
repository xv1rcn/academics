#include <cassert>
#include <iostream>
#include <typeinfo>
#include <vector>

template <typename C>
void test(std::ostream &out = std::cout) {
    out << "Testing " << typeid(C).name() << std::endl;
    
    C a;
    assert(a.empty() && a.size() == 0);

    // 基本插入
    a.push_back(42);
    assert(a.size() == 1 && a[0] == 42);
    a.clear();
    assert(a.empty());

    // 批量插入
    for (int i = 0; i < 10; ++i) a.push_back(i);
    for (int i = 0; i < 10; ++i) assert(a[i] == i);

    // 排序
    a.clear();
    for (int i = 9; i >= 0; --i) a.push_back(i);
    a.sort();
    for (int i = 1; i < 10; ++i) assert(a[i - 1] <= a[i]);

    // 查找
    assert(a.contains(5) && a.find(5) == 5);
    assert(!a.contains(100) && a.find(100) == a.size());

    // 删除
    a.erase(0);
    assert(a[0] == 1);
    try {
        a[100];
        assert(false);
    } catch (...) {
    }

    // ordered_insert 测试
    a.clear();
    a.ordered_insert(5);
    a.ordered_insert(2);
    a.ordered_insert(8);
    a.ordered_insert(5);
    a.ordered_insert(-100);
    a.ordered_insert(100);
    for (size_t i = 1; i < a.size(); ++i) assert(a[i - 1] <= a[i]);

    // ordered_insert + push_back + sort
    a.clear();
    a.ordered_insert(3);
    a.ordered_insert(1);
    a.push_back(10);
    a.push_back(-1);
    a.sort();
    for (size_t i = 1; i < a.size(); ++i) assert(a[i - 1] <= a[i]);

    // 合并后升序
    C b;
    for (int i = 0; i < 5; ++i) b.ordered_insert(i * 2);
    C c;
    for (int i = 0; i < 5; ++i) c.ordered_insert(i * 2 + 1);
    b.merge(c);
    for (size_t i = 1; i < b.size(); ++i) assert(b[i - 1] <= b[i]);

    // sort 后升序
    b.push_back(-1000);
    b.push_back(9999);
    b.sort();
    for (size_t i = 1; i < b.size(); ++i) assert(b[i - 1] <= b[i]);

    // 赋值 / 拷贝构造
    a.clear();
    for (int i = 0; i < 5; ++i) a.push_back(i);
    C d = a;
    C e(a);
    for (size_t i = 0; i < a.size(); ++i) {
        assert(d[i] == a[i]);
        assert(e[i] == a[i]);
    }
    assert(d.size() == a.size() && e.size() == a.size());

    // 自我赋值 / 合并
    d = d;
    for (size_t i = 0; i < d.size(); ++i) assert(d[i] == i);
    e.merge(e);
    for (size_t i = 1; i < e.size(); ++i) assert(e[i - 1] <= e[i]);

    // 多次 clear / resize / merge
    a.clear();
    a.resize(3);
    a.clear();
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    d = a;
    d.merge(a);
    d.clear();
    d.resize(2);
    d.push_back(99);
    assert(d.size() == 3 && d[2] == 99);

    // 边界插入 / 删除
    a.clear();
    a.push_back(10);
    a.insert(0, 5);
    a.insert(a.size(), 20);
    assert(a[0] == 5 && a[a.size() - 1] == 20);
    a.erase(0);
    a.erase(a.size() - 1);
    assert(a[0] == 10);

    // 迭代器遍历
    int sum = 0;
    for (auto it = a.begin(); it != a.end(); ++it) sum += *it;
    int sum2 = 0;
    for (auto x : a) sum2 += x;
    assert(sum == sum2);

    out << "All tests passed for " << typeid(C).name() << std::endl;
}
