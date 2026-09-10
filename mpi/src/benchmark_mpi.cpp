#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

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

struct WorkRange {
    long long begin;
    long long end;
};

WorkRange balanced_range(long long operations, int rank, int process_count) {
    const long long base = operations / process_count;
    const long long remainder = operations % process_count;
    const long long extra_before = std::min<long long>(rank, remainder);
    const long long count = base + (rank < remainder ? 1 : 0);
    const long long begin = rank * base + extra_before;
    return {begin, begin + count};
}

double execute_kernel(const WorkRange range) {
    double local_sum = 0.0;
    for (long long i = range.begin; i < range.end; ++i) {
        const double perturbation = static_cast<double>(i % 1024) * 1.0e-9;
        local_sum += (2.0 + perturbation) * (3.0 - perturbation);
    }
    return local_sum;
}

}  // namespace

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int process_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    long long operations = 0;
    int repetitions = 0;
    int input_valid = 1;

    if (rank == 0) {
        try {
            if (argc != 3) {
                throw std::invalid_argument(
                    "usage: benchmark_mpi OPERATIONS REPETITIONS");
            }
            operations = parse_positive(argv[1], "OPERATIONS");
            repetitions = static_cast<int>(parse_positive(argv[2], "REPETITIONS"));
        } catch (const std::exception& error) {
            std::cerr << "Error: " << error.what() << '\n';
            input_valid = 0;
        }
    }

    MPI_Bcast(&input_valid, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (!input_valid) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    MPI_Bcast(&operations, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&repetitions, 1, MPI_INT, 0, MPI_COMM_WORLD);

    const WorkRange range = balanced_range(operations, rank, process_count);
    const WorkRange warmup_range =
        balanced_range(std::min<long long>(operations, 100000), rank, process_count);
    const double warmup_checksum = execute_kernel(warmup_range);

    double global_warmup_checksum = 0.0;
    MPI_Reduce(&warmup_checksum, &global_warmup_checksum, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);

    if (rank == 0) {
        if (!std::isfinite(global_warmup_checksum)) {
            std::cerr << "Error: the warm-up produced a non-finite checksum\n";
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        std::cout << "processes,run,seconds,checksum\n";
        std::cout << std::setprecision(12);
    }

    for (int run = 1; run <= repetitions; ++run) {
        MPI_Barrier(MPI_COMM_WORLD);
        const double start = MPI_Wtime();
        const double local_checksum = execute_kernel(range);
        const double local_seconds = MPI_Wtime() - start;

        double maximum_seconds = 0.0;
        double global_checksum = 0.0;
        MPI_Reduce(&local_seconds, &maximum_seconds, 1, MPI_DOUBLE, MPI_MAX, 0,
                   MPI_COMM_WORLD);
        MPI_Reduce(&local_checksum, &global_checksum, 1, MPI_DOUBLE, MPI_SUM, 0,
                   MPI_COMM_WORLD);

        if (rank == 0) {
            std::cout << process_count << ',' << run << ',' << maximum_seconds << ','
                      << global_checksum << '\n';
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
