# MPI Assignment Summary

## Resumen en español

La práctica introduce la programación multiproceso con Microsoft MPI en Visual Studio. Primero se configura y ejecuta un programa Hello World mediante `mpiexec`. Tras diez ejecuciones con el mismo número de procesos, se pide discutir el orden de la salida, por qué los mensajes no se entremezclan, si cada proceso ocupa necesariamente un núcleo distinto y cómo escala el consumo de memoria.

La parte experimental pide:

1. ejecutar cinco veces el benchmark secuencial con 100 millones de operaciones;
2. implementar una versión multiproceso con Microsoft MPI;
3. reutilizar el análisis de la arquitectura realizado en la práctica OpenMP;
4. actualizar la estimación teórica si la capacidad multiproceso difiere de la multihilo;
5. ejecutar cinco veces cada configuración, aumentando el número de procesos de uno en uno;
6. observar el balanceo de carga en el Administrador de tareas;
7. representar la aceleración, compararla con la predicción teórica y discutir las diferencias respecto a OpenMP.

## English version

The laboratory introduces multi-process programming with Microsoft MPI in Visual Studio. It first configures and runs an MPI Hello World program through `mpiexec`. After ten executions with the same process count, the student must discuss output ordering, why complete messages do not interleave, whether each process is guaranteed a separate core, and how memory consumption scales.

The experimental section requires the student to:

1. run the sequential benchmark five times with 100 million operations;
2. implement a multi-process version with Microsoft MPI;
3. reuse the architecture analysis from the OpenMP laboratory;
4. revise the theoretical estimate if multi-process capacity differs from multithreaded capacity;
5. perform five runs per configuration while increasing the process count by one;
6. observe operating-system load balancing in Task Manager;
7. plot the speedup, compare it with the theoretical prediction, and discuss the differences from OpenMP.
