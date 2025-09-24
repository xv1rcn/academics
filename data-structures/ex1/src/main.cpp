#include <fstream>
#include <iostream>

#include "ordered_array.h"
#include "ordered_array_stl.h"
#include "ordered_list.h"
#include "ordered_list_stl.h"

#include "cf_ostream.cpp"
#include "profile.cpp"
#include "test.cpp"

int main() {
    test<ordered_array<int>>();
    test<ordered_array_stl<int>>();
    test<ordered_list<int>>();
    test<ordered_list_stl<int>>();
    std::cout << "All container tests finished!" << std::endl << std::endl;

    std::ofstream fout("profile.txt");
    cf_ostream dout(std::cout, fout);

    for (size_t n = 0; n <= 100'000; n += 1'000) {
        dout << "Profiling with n = " << n << std::endl;
        profile<ordered_array<int>, int>(n, dout);
        profile<ordered_array_stl<int>, int>(n, dout);
        profile<ordered_list<int>, int>(n, dout);
        profile<ordered_list_stl<int>, int>(n, dout);
        dout << "Finished profiling for n = " << n << std::endl << std::endl;
    }
    dout << "All container profiles finished!" << std::endl << std::endl;

    return 0;
}
