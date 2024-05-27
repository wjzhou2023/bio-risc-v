# bio-risc-v
A repository for optimized bioinformatics programs performing on SG2042. 

## Optimization methods
| Programs | GCC | Huge Page | NUMACTL | SSE2Scalar | Thread Affinity |
| :---- | :----: | :----: | :----: | :----: | :----: |
| BWA | &#10004; | &#10004; | &#10004; | | |
| Bowtie2 | &#10004; | | &#10004; | &#10004; | &#10004; |
| Minimap2 | &#10004; |&#10004; | &#10004; |  | &#10004; |

## Optimization results
Genome reference data: 

    human_g1k_v37.fasta, https://ftp.ncbi.nih.gov/1000genomes/ftp/technical/reference/human_g1k_v37.fasta.gz
    
Genome sequencing data: 
    
    The next generation sequence(seq2): Genome in a Bottle, NA12878, ftp://ftp-trace.ncbi.nih.gov/giab/ftp/data/NA12878/Garvan_NA12878_HG001_HiSeq_Exome/NIST7035_TAAGGCGA_L001_R1_001.fastq.gz

    The third generation sequence(seq3): Pacbio, SRR1772721, https://www.ncbi.nlm.nih.gov/sra/SRX852869[accn]

Results:

Optimization results of BWA on seq2 (seconds):
| #Threads | Ori | GCC | Huge Page | NUMACTL |
| :----: | :----: | :----: | :----: | :----: |
| 1 | 9,573 | 8,894 | 5,838 | 5,778|
| 16| 850 | 789 | 459 | 382 |
| 32 | 1,211 | 1,102 | 345 | 220 |
| 64 | 1,807 | 1,626 | 539 | 214 |

Optimization results of BWA on seq3 (seconds):
| #Threads | Ori | GCC | Huge Page | NUMACTL |
| :----: | :----: | :----: | :----: | :----: |
| 1 | 60,040 | 44,903 | 36,797 | 36,657|
| 16| 4,061 | 3,001 | 2,394 | 2,353 |
| 32 | 2,282 | 1,747 | 1,242 | 1,194 |
| 64 | 9,826 | 9,270 | 3,722 | 631 |


Optimization results of Bowtie2 on seq2(seconds):
|#Thread|Ori|GCC|SSE2Scalar|NUMACTL|
| :----: |:----:|:----:|:----:|:----:|
|1|79,502|79,052|78,075|-|
|16|5,076|5,022|4,622|4,374|
|32|2,612|2,646|2,481|2,216|
|64|1,947|1,812|1,783|1,143|

Optimization results of Minimap2 on seq3(seconds):
|#Thread|Ori|GCC|NUMACTL|Huge Page|Thread Affinity|
| :----: |:----:|:----:|:----:|:----:|:----:|
|1|40,194|31,877|30,897|30,403|30,221|
|16|2,822|2,249|2,057|2,050|2,041|
|32|1,456|1,249|1,073|1,068|1,056|
|64|4,559|3,922|828|577|567|

