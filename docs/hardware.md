# Laboratory Machine

The hardware summary below contains only the fields needed to interpret the experiments. Identifying serial numbers and UUIDs are intentionally omitted.

| Component | Verified value |
| --- | --- |
| Processor | 12th Gen Intel Core i7-12700 |
| Physical cores | 12 |
| Logical threads | 20 |
| P-core topology | 8 cores, 16 threads |
| E-core topology | 4 cores, 4 threads |
| L3 cache | 25 MB shared |
| Memory | 16 GB DDR4-3200 |
| Memory channels | Single channel, 64-bit |

## Why the topology matters

The logical processors are not equivalent. The first sixteen logical threads belong to eight Hyper-Threaded P-cores, while four additional threads belong to the E-cores. A thread-count sweep therefore measures a combination of application scaling, Windows scheduling decisions, core type, cache and memory pressure, and frequency changes.

For that reason, a simple ideal curve based only on `speedup = N` is insufficient. A useful analysis should distinguish at least these regions:

- low thread counts, where work is likely placed on P-cores;
- the transition where the scheduler begins using additional P-core siblings and/or E-cores;
- full occupancy at 20 logical threads;
- oversubscription beyond 20, where several software threads or processes share each logical processor.
