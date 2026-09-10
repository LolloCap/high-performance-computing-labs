# Slurm Cluster Assignment Summary

## Resumen en español

El objetivo es instalar, configurar y gestionar un clúster Ubuntu Server con Slurm usando máquinas virtuales de VirtualBox. La configuración base está formada por un nodo gestor, `controller`, y dos nodos de cómputo, `worker1` y `worker2`, conectados mediante una red Ethernet interna.

La práctica guía la configuración de las máquinas virtuales, Ubuntu Server, las interfaces NAT e interna, direcciones estáticas, resolución local de nombres, SSH, almacenamiento compartido NFS, autenticación Munge y servicios Slurm. El estado del clúster se comprueba mediante `sinfo` y Sview.

La tarea adicional consiste en clonar un worker, renombrarlo como `worker3` e incorporarlo al clúster aplicando los cambios necesarios en los demás nodos y en la configuración de Slurm.

## English version

The objective is to install, configure and manage an Ubuntu Server cluster with Slurm using VirtualBox virtual machines. The base configuration consists of one management node, `controller`, and two compute nodes, `worker1` and `worker2`, connected through an internal Ethernet network.

The laboratory covers VM creation, Ubuntu Server installation, NAT and internal interfaces, static addressing, local hostname resolution, SSH, shared NFS storage, Munge authentication and Slurm services. Cluster state is checked through `sinfo` and Sview.

The additional task requires cloning a worker, renaming it `worker3`, and integrating it by applying the necessary hostname, network and Slurm configuration changes across the cluster.
