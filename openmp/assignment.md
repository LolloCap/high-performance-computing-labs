# OpenMP Assignment Summary

## Resumen en español

La práctica introduce la programación multihilo con OpenMP en Visual Studio. Incluye la creación de un programa Hello World con cuatro hilos, el estudio de variables compartidas y `firstprivate`, y una traza del reparto explícito de veinte iteraciones entre cuatro hilos.

La parte experimental pide:

1. ejecutar cinco veces el benchmark secuencial proporcionado con 10 000 operaciones y calcular la media;
2. implementar una versión OpenMP distribuyendo explícitamente las iteraciones;
3. analizar críticamente la arquitectura del PC del laboratorio;
4. estimar y representar la aceleración teórica en función del paralelismo y del overhead;
5. ejecutar cinco veces el benchmark paralelo con 1 000 000 de operaciones por configuración, aumentando el número de hilos de uno en uno;
6. observar el balanceo de carga en el Administrador de tareas;
7. representar la aceleración medida y compararla críticamente con la estimación teórica.

La práctica no fija un máximo universal de hilos. El límite debe justificarse a partir de la máquina analizada. Los experimentos por encima de los veinte hilos fueron una extensión realizada durante el trabajo, no un requisito explícito.

## English version

The laboratory introduces multithreaded programming with OpenMP in Visual Studio. It includes a four-thread Hello World program, an exercise on shared variables and `firstprivate`, and a trace of the explicit distribution of twenty iterations across four threads.

The experimental section requires the student to:

1. run the supplied sequential benchmark five times with 10,000 operations and calculate the mean;
2. implement an OpenMP version with an explicit distribution of iterations;
3. critically analyze the architecture of the laboratory PC;
4. estimate and plot the theoretical speedup as a function of parallelism and overhead;
5. run five measurements per configuration with 1,000,000 operations, increasing the thread count by one each time;
6. observe operating-system load balancing in Task Manager;
7. plot the measured speedup and compare it critically with the theoretical estimate.

The assignment does not prescribe one universal maximum thread count. The chosen range should follow from the analyzed hardware. Runs above twenty threads were an additional exploration, not a stated requirement.
