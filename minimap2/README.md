
### Optimization methods
| Programs | GCC      | Huge Page| NUMACTL  |SEE2Scalar | Thread Affinity|
|----------|----------|----------|----------|----------|----------|
| minimap2 |&#10004;  |&#10004;  |&#10004;  |          |          |


### Results
Optimization results of Minimap2 on seq3(seconds):
| #Threads | Ori      |GCC       |NUMACTL  |HugePage   |           
|----------|----------|----------|----------|----------|
| 1        | 40194   | 31877     | 30897    | 30221    |
| 16       | 2822    | 2249      | 2057     | 2050     |
| 32       | 1456    | 1249      | 1073     | 1068     |
| 64       | 4559    | 3992      | 828      | 577      |
