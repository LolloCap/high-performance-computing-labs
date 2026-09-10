#include <omp.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

long long parse_positive(const char* text, const char* name) {
    std::size_t consumed = 0;
    const std::string value(text);
    const long long parsed = std::stoll(value, &consumed);
    if (consumed != value.size() || parsed <= 0) {
        throw std::invalid_argument(std::string(name) + " must be a positive integer");
    }
    return parsed;
}

std::vector<int> make_thread_plan(int maximum, bool include_oversubscription) {
    std::vector<int> plan;
    plan.reserve(static_cast<std::size_t>(maximum) + 3);
    for (int threads = 1; threads <= maximum; ++threads) {
        plan.push_back(threads);
    }
    if (include_oversubscription) {
        for (const int threads : {60, 120, 240}) {
            if (threads > maximum) {
                plan.push_back(threads);
            }
        }
    }
    return plan;
}

double execute_kernel(long long operations, int& actual_threads) {
    double checksum = 0.0;

#pragma omp parallel reduction(+ : checksum)
    {
        const int thread_id = omp_get_thread_num();
        const int team_size = omp_get_num_threads();

#pragma omp single
        actual_threads = team_size;

        double local_sum = 0.0;
        for (long long i = thread_id; i < operations; i += team_size) {
            const double perturbation = static_cast<double>(i % 1024) * 1.0e-9;
            local_sum += (2.0 + perturbation) * (3.0 - perturbation);
        }
        checksum += local_sum;
    }

    return checksum;
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc < 4 || argc > 5) {
            std::cerr
                << "Usage: benchmark_openmp OPERATIONS MAX_THREADS REPETITIONS "
                   "[--oversubscribe]\n";
            return EXIT_FAILURE;
        }

        const long long operations = parse_positive(argv[1], "OPERATIONS");
        const int maximum_threads =
            static_cast<int>(parse_positive(argv[2], "MAX_THREADS"));
        const int repetitions =
            static_cast<int>(parse_positive(argv[3], "REPETITIONS"));
        const bool include_oversubscription =
            argc == 5 && std::string(argv[4]) == "--oversubscribe";

        if (argc == 5 && !include_oversubscription) {
            throw std::invalid_argument("the only optional flag is --oversubscribe");
        }

        omp_set_dynamic(0);
        std::cout << "requested_threads,actual_threads,run,seconds,checksum\n";
        std::cout << std::setprecision(12);

        for (const int requested_threads :
             make_thread_plan(maximum_threads, include_oversubscription)) {
            omp_set_num_threads(requested_threads);

            int warmup_threads = 0;
            const double warmup_checksum =
                execute_kernel(std::min<long long>(operations, 100000), warmup_threads);
            if (!std::isfinite(warmup_checksum)) {
                throw std::runtime_error("the warm-up produced a non-finite checksum");
            }

            for (int run = 1; run <= repetitions; ++run) {
                int actual_threads = 0;
                const double start = omp_get_wtime();
                const double checksum = execute_kernel(operations, actual_threads);
                const double seconds = omp_get_wtime() - start;

                std::cout << requested_threads << ',' << actual_threads << ',' << run
                          << ',' << seconds << ',' << checksum << '\n';
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
