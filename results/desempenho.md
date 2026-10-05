# Testes de desempenho

## Metodologia

Os testes de desempenho foram realizados utilizando três tamanhos de matriz:

- 1000x1000;
- 2000x2000;
- 4000x4000.

Para cada tamanho foram executadas:

- a versão sequencial;
- a versão paralela com 2 threads;
- a versão paralela com 3 threads;
- a versão paralela com 4 threads.

Cada configuração foi executada 5 vezes utilizando exatamente a mesma
matriz binária.

O tempo foi medido com `clock_gettime(CLOCK_MONOTONIC, ...)`.

A medida representativa utilizada foi a média das cinco execuções.

Como medida de dispersão foram registrados os valores mínimo e máximo.

O speedup foi calculado por:

```text
S(p) = Tsequencial / Tparalelo(p)
```

A eficiência foi calculada por:

```text
E(p) = S(p) / p
```

Os dados brutos das execuções estão disponíveis em:

```text
results/medicoes.csv
```

---

## Matriz 1000x1000

Quantidade de objetos encontrada: **45472**

| Versão | Threads | Média (ms) | Mínimo (ms) | Máximo (ms) | Speedup | Eficiência |
|---|---:|---:|---:|---:|---:|---:|
| Sequencial | 1 | 17.890 | 16.319 | 20.442 | 1.00 | 1.00 |
| Paralela | 2 | 12.336 | 11.190 | 15.041 | 1.45 | 0.73 |
| Paralela | 3 | 9.491 | 9.361 | 9.671 | 1.89 | 0.63 |
| Paralela | 4 | 9.591 | 8.335 | 11.413 | 1.87 | 0.47 |

A configuração com **3 threads** apresentou o menor tempo médio,
9.491 ms, correspondendo a um speedup de **1.89**.

---

## Matriz 2000x2000

Quantidade de objetos encontrada: **181340**

| Versão | Threads | Média (ms) | Mínimo (ms) | Máximo (ms) | Speedup | Eficiência |
|---|---:|---:|---:|---:|---:|---:|
| Sequencial | 1 | 63.465 | 60.904 | 66.935 | 1.00 | 1.00 |
| Paralela | 2 | 47.781 | 43.299 | 58.032 | 1.33 | 0.66 |
| Paralela | 3 | 38.945 | 35.523 | 49.799 | 1.63 | 0.54 |
| Paralela | 4 | 40.287 | 32.459 | 46.855 | 1.58 | 0.39 |

Novamente, a configuração com **3 threads** apresentou o melhor tempo
médio, 38.945 ms, com speedup de **1.63**.

---

## Matriz 4000x4000

Quantidade de objetos encontrada: **724346**

| Versão | Threads | Média (ms) | Mínimo (ms) | Máximo (ms) | Speedup | Eficiência |
|---|---:|---:|---:|---:|---:|---:|
| Sequencial | 1 | 265.057 | 250.119 | 292.157 | 1.00 | 1.00 |
| Paralela | 2 | 254.603 | 210.847 | 291.411 | 1.04 | 0.52 |
| Paralela | 3 | 196.809 | 186.442 | 205.183 | 1.35 | 0.45 |
| Paralela | 4 | 198.839 | 184.956 | 235.890 | 1.33 | 0.33 |

A configuração com **3 threads** também apresentou o melhor resultado
na maior matriz, com tempo médio de 196.809 ms e speedup de **1.35**.

---

## Análise

Todos os testes produziram exatamente a mesma quantidade de objetos nas
versões sequencial e paralela, independentemente da quantidade de
threads utilizada.

Os resultados demonstram que a paralelização reduziu o tempo médio de
execução nas três matrizes avaliadas.

A melhor configuração observada foi a utilização de **3 threads**:

- speedup de 1.89 na matriz 1000x1000;
- speedup de 1.63 na matriz 2000x2000;
- speedup de 1.35 na matriz 4000x4000.

A utilização de 4 threads não produziu ganho adicional em relação a
3 threads.

Esse comportamento mostra que o aumento da quantidade de threads não
resulta necessariamente em aceleração proporcional.

Entre os fatores que influenciam esse resultado estão:

- custo de criação e finalização das threads;
- acesso concorrente à memória;
- diferenças de carga entre as regiões da matriz;
- custo da consolidação das fronteiras;
- partes da implementação que continuam sequenciais;
- comportamento da hierarquia de memória e cache.

Também é possível observar a redução da eficiência conforme o número de
threads aumenta. Na matriz 4000x4000, por exemplo, a eficiência foi de
0.52 com 2 threads, 0.45 com 3 threads e 0.33 com 4 threads.

Portanto, para o ambiente utilizado nos testes, a configuração com
3 threads apresentou o melhor equilíbrio entre paralelismo e sobrecarga.