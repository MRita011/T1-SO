# Testes de desempenho

## Configuração dos testes

- 5 repetições por teste;
- mesma matriz utilizada pelas versões sequencial e paralela;
- versão paralela testada com 2, 3 e 4 threads;
- o tempo apresentado corresponde à média das 5 execuções;
- Flood Fill implementado de forma iterativa;
- speedup calculado por:

```text
Speedup = Tempo sequencial / Tempo paralelo
```

## Matriz 1000x1000

| Versão | Threads | Objetos | Tempo médio | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | - | 45472 | 0.014655 s | 1.00 |
| Paralela | 2 | 45472 | 0.012014 s | 1.22 |
| Paralela | 3 | 45472 | 0.009519 s | 1.54 |
| Paralela | 4 | 45472 | 0.011542 s | 1.27 |

**Melhor resultado:** 3 threads, com speedup de **1.54**.

## Matriz 2000x2000

| Versão | Threads | Objetos | Tempo médio | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | - | 181340 | 0.058888 s | 1.00 |
| Paralela | 2 | 181340 | 0.046245 s | 1.27 |
| Paralela | 3 | 181340 | 0.037831 s | 1.56 |
| Paralela | 4 | 181340 | 0.040353 s | 1.46 |

**Melhor resultado:** 3 threads, com speedup de **1.56**.

## Matriz 4000x4000

| Versão | Threads | Objetos | Tempo médio | Speedup |
|---|---:|---:|---:|---:|
| Sequencial | - | 724346 | 0.224660 s | 1.00 |
| Paralela | 2 | 724346 | 0.221963 s | 1.01 |
| Paralela | 3 | 724346 | 0.193788 s | 1.16 |
| Paralela | 4 | 724346 | 0.210604 s | 1.07 |

**Melhor resultado:** 3 threads, com speedup de **1.16**.

# Conclusão

A versão paralela apresentou a mesma quantidade de objetos da versão
sequencial em todas as configurações testadas, confirmando a correção
dos resultados.

Os melhores speedups observados foram:

- **1.54** com 3 threads na matriz 1000x1000;
- **1.56** com 3 threads na matriz 2000x2000;
- **1.16** com 3 threads na matriz 4000x4000.

Nos testes realizados, a configuração com **3 threads apresentou o
melhor desempenho nos três tamanhos de matriz**.

Os resultados também mostram que aumentar a quantidade de threads não
produz necessariamente um ganho proporcional. A utilização de 4 threads
não superou a configuração com 3 threads em nenhum dos testes finais.

Esse comportamento pode ser explicado pelos custos associados à criação
e sincronização das threads, ao acesso concorrente à memória e à etapa
sequencial de consolidação dos componentes encontrados nas fronteiras
entre as regiões.

Na matriz 4000x4000, por exemplo, a utilização de 2 threads apresentou
speedup de apenas 1.01, enquanto 3 threads atingiram 1.16. Com 4 threads,
o speedup caiu para 1.07.

Portanto, os testes demonstram que a paralelização pode reduzir o tempo
de execução, mas o melhor número de threads depende da relação entre o
trabalho paralelo realizado e os custos adicionais introduzidos pela
execução concorrente.