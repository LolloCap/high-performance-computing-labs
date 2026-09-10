# Methodology and Reproducibility

This page describes the experimental platform, measurement boundaries, and interpretation rules used throughout the repository.

## Experimental platform

- Course: *Sistemas Computadores de Altas Prestaciones*, 2024/2025.
- Processor: Intel Core i7-12700 with 12 physical cores and 20 logical threads.
- Topology: 8 Hyper-Threaded P-cores and 4 single-threaded E-cores.
- Memory: 16 GB single-channel DDR4-3200.
- Shared L3 cache: 25 MB.

The complete hardware summary is available in [hardware.md](hardware.md).

## Benchmark interpretation

### OpenMP

The course loop stored `b * c` in a thread-private variable and never consumed the result. Optimized builds may remove the loop. Results must therefore be interpreted as coursework measurements rather than as a general-purpose CPU benchmark.

The 8P+4E topology makes the logical processors non-equivalent. Performance analysis must account for heterogeneous cores, Hyper-Threading, and Windows scheduling.

### MPI

The MPI timer starts after `mpiexec` has created the processes, so the recorded number does not include process-launch overhead. This is valid for timing the distributed kernel, but it cannot support conclusions about total application startup cost.

### Slurm

The base topology contains a controller and two workers. The additional task introduces a cloned third worker, producing the final four-machine topology documented in the screenshots.

## Repository scope

The repository includes source code, configuration examples, course measurements, selected screenshots, and bilingual assignment summaries. It intentionally excludes:

- Microsoft MPI installers;
- the complete CPU-Z text report, which contains machine identifiers and serial information;
- course handouts whose redistribution rights belong to their original authors;
- machine-specific or temporary files that are not required to reproduce the laboratories.
