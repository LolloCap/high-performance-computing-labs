# Slurm Cluster Evidence

| File | What it shows |
| --- | --- |
| `virtualbox-controller-memory.png` | 6 GB assigned to the controller VM |
| `virtualbox-controller-cpu.png` | One virtual CPU assigned to the controller |
| `virtualbox-controller-network-nat.png` | Adapter 1 attached through NAT |
| `virtualbox-controller-network-internal.png` | Adapter 2 attached to the internal network |
| `virtualbox-controller-disk.png` | The controller's 50 GB virtual disk |
| `worker1-network-and-hosts.png` | `worker1` at `192.168.10.11` and the four-node host map |
| `ssh-connectivity.png` | Shell sessions used to validate inter-node connectivity |
| `nfs-shared-file.png` | A shared file visible through the NFS mount |
| `munge-status-and-test.png` | Munge service state and credential test output |
| `slurm-node-definition.png` | The verified three-worker Slurm node and partition fragment |
| `slurm-services-running.png` | Active controller and worker Slurm services |
| `sview-cluster-overview.png` | Cluster state in Sview |
| `multi-terminal-cluster-overview.png` | Controller and worker terminals alongside Sview |
| `slurm-srun-test-command.png` | The final `srun -N3 hostname` validation command |

The filenames and descriptions follow the configuration step visible in each screenshot.
