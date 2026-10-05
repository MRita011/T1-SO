# Testes de desempenho

## Configuração dos testes

- **5 repetições** por teste;
- mesma matriz utilizada pelas versões sequencial e paralela;
- versão paralela testada com **2, 3 e 4 threads**;
- o tempo apresentado corresponde à **média das 5 execuções**;
- o speedup foi calculado por:

```text
Speedup = Tempo sequencial / Tempo paralelo
```

## Matriz 1000x1000

| Versão | Threads | Objetos | Tempo médio | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | - | 45472 | 0.016198 s | 1.00 |
| Paralela | 2 | 45472 | 0.012516 s | 1.29 |
| Paralela | 3 | 45472 | 0.010436 s | 1.55 |
| Paralela | 4 | 45472 | 0.011142 s | 1.45 |

**Melhor resultado:** 3 threads, com speedup de **1.55**.

## Matriz 2000x2000

| Versão | Threads | Objetos | Tempo médio | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | - | 181340 | 0.062060 s | 1.00 |
| Paralela | 2 | 181340 | 0.050653 s | 1.23 |
| Paralela | 3 | 181340 | 0.040664 s | 1.53 |
| Paralela | 4 | 181340 | 0.040417 s | 1.54 |

**Melhor resultado:** 4 threads, com speedup de **1.54**.

## Matriz 4000x4000

| Versão | Threads | Objetos | Tempo médio | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | - | 724346 | 0.268150 s | 1.00 |
| Paralela | 2 | 724346 | 0.252403 s | 1.06 |
| Paralela | 3 | 724346 | 0.212837 s | 1.26 |
| Paralela | 4 | 724346 | 0.213033 s | 1.26 |

**Melhor resultado:** 3 e 4 threads apresentaram speedup de aproximadamente **1.26**.