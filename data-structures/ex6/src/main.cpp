#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/resource.h>

#include "matrix.h"

double now_seconds() {
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

size_t get_peak_rss_kb() {
    struct rusage r;
    if (getrusage(RUSAGE_SELF, &r) == 0) {
        return (size_t)r.ru_maxrss;
    }
    return 0;
}

struct BenchmarkResult {
    double avg_time = 0.0;
    double peak_mb = 0.0;
    std::string notes = "";
    bool oom = false;
    bool skipped = false;
};

static double mean(const std::vector<double> &v) {
    if (v.empty())
        return 0.0;
    double s = 0.0;
    for (double x : v)
        s += x;
    return s / v.size();
}

BenchmarkResult run_benchmark_for_N(size_t N, int repeats) {
    BenchmarkResult res;

    try {
        if (N > (size_t)1e8) {
            res.notes = "N too large (sanity)";
            res.skipped = true;
            return res;
        }

        Matrix A(N, N), B(N, N);
        A.fill(1.0);
        B.fill(1.0);

        std::vector<double> times;
        for (int r = 0; r < repeats; ++r) {
            Matrix C(N, N);

            double t0 = now_seconds();
            C = A * B;
            double t1 = now_seconds();

            times.push_back(t1 - t0);

            size_t peak_kb = get_peak_rss_kb();
            double peak_now_mb = peak_kb / 1024.0;
            if (peak_now_mb > res.peak_mb)
                res.peak_mb = peak_now_mb;
        }

        res.avg_time = mean(times);

    } catch (const std::bad_alloc &e) {
        res.notes = std::string("OOM: ") + e.what();
        res.oom = true;
    } catch (const std::exception &e) {
        res.notes = std::string("Error: ") + e.what();
    }

    return res;
}

int main(int argc, char **argv) {
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " START END STEP REPEATS OUTPUT_CSV\n";
        std::cerr << "Example: " << argv[0] << " 100 500 100 3 results.csv\n";
        return 1;
    }

    size_t start = std::stoull(argv[1]);
    size_t end = std::stoull(argv[2]);
    size_t step = std::stoull(argv[3]);
    int repeats = std::stoi(argv[4]);
    std::string out_csv = argv[5];
    if (step == 0) {
        std::cerr << "STEP must be > 0\n";
        return 1;
    }

    std::ofstream fout(out_csv);
    if (!fout) {
        std::cerr << "Cannot open output file " << out_csv << "\n";
        return 1;
    }

    fout << "N,avg_time_s,peak_rss_MB,notes\n";

    for (size_t N = start; N <= end; N += step) {
        std::cout << "Testing N=" << N << "\n";

        BenchmarkResult res = run_benchmark_for_N(N, repeats);

        if (res.skipped) {
            std::cout << "Skipping N=" << N << " (sanity check)\n";
            fout << N << ",," << "0.00," << res.notes << "\n";
            fout.flush();
            continue;
        }

        if (res.oom) {
            std::cout << "Allocation failed for N=" << N << "\n";
            fout << N << ",, ," << res.notes << "\n";
            fout.flush();
            break;
        }

        if (!res.notes.empty()) {
            std::cout << res.notes << "\n";
        }

        fout << std::format("{},{:.2f},{:.2f},{}\n", N, res.avg_time, res.peak_mb, res.notes);
        fout.flush();
    }

    std::cout << "Benchmark finished. Results in " << out_csv << "\n";
    return 0;
}
