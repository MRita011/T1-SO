# T1-SO: Contagem paralela de objetos em uma matriz binária

Trabalho desenvolvido para a disciplina de **Sistemas Operacionais: 2026/2**.

## Integrantes

- Gabriel Kowaleski
- Jully Anne Seifert
- Maria Rita Rodrigues
- Mayra Bordin

## Descrição

O projeto implementa a contagem de objetos em uma matriz binária utilizando **conectividade 8**.

Na matriz:

- `0` representa o fundo;
- `1` representa uma célula pertencente a um objeto.

Duas células de valor `1` podem fazer parte do mesmo objeto quando estão conectadas:

- horizontalmente;
- verticalmente;
- diagonalmente.

Foram desenvolvidas duas versões do programa:

- **versão sequencial**, utilizada como referência de correção;
- **versão paralela**, utilizando POSIX Threads (Pthreads).

Na versão paralela, a matriz é dividida em faixas horizontais de linhas. Cada thread identifica os componentes existentes em sua própria região. Depois que todas as threads terminam, os componentes que atravessam as fronteiras são consolidados utilizando **Union-Find**.

---

## Estrutura do projeto

```text
T1-SO/
├── Makefile
├── README.md
├── RELATORIO_TECNICO.md
├── results/
│   ├── desempenho.md
│   ├── medicoes.csv
│   ├── grafico-tempo.png
│   ├── grafico-aceleracao.png
│   └── grafico-eficiencia.png
├── src/
│   ├── conta-objetos-sequencial.c
│   ├── conta-objetos-paralelo.c
│   └── lib/
│       ├── contagem.c
│       ├── contagem.h
│       ├── flood-fill.c
│       ├── flood-fill.h
│       ├── union-find.c
│       └── union-find.h
└── tests/
    ├── teste-desempenho.c
    └── gerar-graficos.py
```

---

# Compilação e execução

O projeto possui um `Makefile`, portanto **não é necessário digitar manualmente os comandos completos do GCC**.

## 1. Entre na pasta do projeto

Abra o terminal e entre na pasta do repositório:

```bash
cd T1-SO
```

Se você já estiver dentro da pasta `T1-SO`, pode seguir diretamente para o próximo passo.

---

## 2. Limpe compilações anteriores

Execute:

```bash
make clean
```

Esse comando remove executáveis antigos:

```text
sequencial
paralelo
desempenho
```

---

## 3. Compile o projeto

Execute:

```bash
make
```

Esse comando compila automaticamente os três executáveis:

```text
sequencial  → versão sequencial
paralelo    → versão paralela
desempenho  → testes de desempenho
```

Durante a compilação, o terminal mostrará comandos semelhantes a:

```bash
gcc -std=c89 -Wall -Wextra -pedantic -pthread src/conta-objetos-sequencial.c src/lib/contagem.c src/lib/flood-fill.c src/lib/union-find.c -o sequencial

gcc -std=c89 -Wall -Wextra -pedantic -pthread src/conta-objetos-paralelo.c src/lib/contagem.c src/lib/flood-fill.c src/lib/union-find.c -o paralelo

gcc -std=c89 -Wall -Wextra -pedantic -pthread tests/teste-desempenho.c src/lib/contagem.c src/lib/flood-fill.c src/lib/union-find.c -o desempenho
```

Se nenhum erro for exibido, a compilação foi concluída.

---

## 4. Execute a versão desejada

### Versão sequencial

Execute:

```bash
./sequencial
```

O programa executará as cinco matrizes obrigatórias e mostrará:

- dimensão;
- resultado esperado;
- resultado obtido;
- status.

Exemplo:

```text
+--------+----------+----------+--------+----------+
| Matriz | Dimensao | Esperado | Obtido | Status   |
+--------+----------+----------+--------+----------+
| 1      |  5x5     | 3        | 3      | OK       |
```

---

### Versão paralela

Para executar todas as configurações previstas no programa:

```bash
./paralelo
```

Nesse caso, cada matriz será testada com:

```text
2 threads
3 threads
4 threads
```

Também é possível escolher a quantidade de threads.

Com 2 threads:

```bash
./paralelo 2
```

Com 3 threads:

```bash
./paralelo 3
```

Com 4 threads:

```bash
./paralelo 4
```

Exemplo:

```bash
./paralelo 3
```

Saída esperada:

```text
+--------+----------+---------+----------+--------+----------+
| Matriz | Dimensao | Threads | Esperado | Obtido | Status   |
+--------+----------+---------+----------+--------+----------+
| 1      |  5x5     | 3       | 3        | 3      | OK       |
```

Além das matrizes obrigatórias, a versão paralela também executa testes adicionais de:

- conexão diagonal;
- objeto atravessando três regiões;
- matriz zerada.

---

### Testes de desempenho

Execute:

```bash
./desempenho
```

O programa testa automaticamente matrizes:

```text
1000x1000
2000x2000
4000x4000
```

Para cada tamanho são executadas:

```text
versão sequencial
versão paralela com 2 threads
versão paralela com 3 threads
versão paralela com 4 threads
```

Cada configuração é executada **5 vezes**.

São registrados:

- tempo de cada repetição;
- média;
- mínimo;
- máximo;
- speedup;
- eficiência;
- quantidade de objetos;
- status de correção.

Os dados brutos são salvos automaticamente em:

```text
results/medicoes.csv
```

---

## Resumo rápido

### Compilar tudo e executar a versão sequencial

```bash
make clean
make
./sequencial
```

### Compilar tudo e executar a versão paralela com 3 threads

```bash
make clean
make
./paralelo 3
```

### Compilar tudo e executar os testes de desempenho

```bash
make clean
make
./desempenho
```

---

## Compilação individual

Caso seja necessário compilar somente uma parte do projeto:

### Somente sequencial

```bash
make sequencial
```

Depois:

```bash
./sequencial
```

### Somente paralela

```bash
make paralelo
```

Depois:

```bash
./paralelo
```

ou:

```bash
./paralelo 3
```

### Somente desempenho

```bash
make desempenho
```

Depois:

```bash
./desempenho
```

---

## Requisitos

O projeto foi desenvolvido e testado com:

```text
Linux/Ubuntu via WSL
GCC
Make
Pthreads
ANSI C C89/C90
```

As flags utilizadas na compilação são:

```text
-std=c89 -Wall -Wextra -pedantic -pthread
```

---

# Versão sequencial

A versão sequencial percorre toda a matriz e identifica os objetos utilizando **Flood Fill iterativo**.

Foi utilizada uma pilha explícita para evitar o uso excessivo de recursão.

Para cada célula de valor `1` ainda não visitada:

1. um novo objeto é contabilizado;
2. o Flood Fill é iniciado;
3. todas as células conectadas são visitadas;
4. são verificados os oito vizinhos possíveis.

A conectividade utilizada é:

```text
↖ ↑ ↗
←   →
↙ ↓ ↘
```

Arquivos principais:

```text
src/conta-objetos-sequencial.c
src/lib/contagem.c
src/lib/flood-fill.c
```

---

# Versão paralela

A versão paralela utiliza **POSIX Threads (Pthreads)**.

A matriz é dividida estaticamente em faixas horizontais de linhas.

Cada thread recebe uma região específica e executa o Flood Fill somente dentro dos limites dessa região.

A divisão é realizada por:

```c
linha_inicio = i * linhas/num_threads;
linha_fim = ((i + 1) * linhas/num_threads) - 1;
```

Cada thread:

1. percorre sua região;
2. identifica os componentes locais;
3. atribui rótulos aos componentes;
4. registra a quantidade de objetos encontrados.

Depois que todas as threads terminam, a thread principal analisa as fronteiras entre as regiões.

São verificadas conexões:

- verticais;
- diagonais à esquerda;
- diagonais à direita.

Os componentes que pertencem ao mesmo objeto global são consolidados utilizando **Union-Find**.

Arquivos principais:

```text
src/conta-objetos-paralelo.c
src/lib/contagem.c
src/lib/flood-fill.c
src/lib/union-find.c
```

---

# Sincronização

A principal sincronização utilizada é:

```c
pthread_join
```

A consolidação das fronteiras somente começa depois que todas as threads terminam.

Não são utilizados mutexes durante a identificação local porque cada thread escreve apenas nas linhas pertencentes à sua própria região.

A estrutura Union-Find também não é acessada simultaneamente pelas threads, pois sua utilização ocorre somente após os `pthread_join`.

---

# Consolidação com Union-Find

Um mesmo objeto pode atravessar duas ou mais regiões.

Por exemplo:

```text
Região 1
0 1 0
0 1 0
-----

Região 2
0 1 0
0 1 0
```

Antes da consolidação, cada região pode identificar o objeto separadamente.

Após o processamento local, as fronteiras são comparadas.

Quando dois rótulos pertencem ao mesmo objeto, é executada uma união:

```c
uf_union(...)
```

A contagem global é calculada utilizando:

```text
total_global = total_local - unioes
```

---

# Testes funcionais

As cinco matrizes obrigatórias foram testadas na versão sequencial e na versão paralela.

Resultados esperados:

| Matriz | Dimensão | Objetos |
|---|---:|---:|
| 1 | 5x5 | 3 |
| 2 | 6x8 | 4 |
| 3 | 8x8 | 5 |
| 4 | 9x12 | 6 |
| 5 | 12x12 | 7 |

Na versão paralela, todas foram testadas com:

```text
2 threads
3 threads
4 threads
```

Todos os resultados obtidos foram iguais aos resultados esperados.

Também foram adicionados testes para:

```text
Diagonal
3 regiões
Matriz zerada
```

Todos apresentaram status:

```text
OK
```

---

# Testes de desempenho

Os testes utilizam matrizes:

```text
1000x1000
2000x2000
4000x4000
```

Cada configuração é executada cinco vezes.

O tempo é medido com:

```c
clock_gettime(CLOCK_MONOTONIC, ...)
```

São calculados:

```text
tempo médio
mínimo
máximo
speedup
eficiência
```

O speedup é calculado por:

```text
S(p) = tempo sequencial/tempo paralelo
```

A eficiência é calculada por:

```text
E(p) = S(p)/p
```

---

# Resultados de desempenho

Na coleta final, a configuração com **3 threads** apresentou o melhor tempo médio nos três tamanhos testados.

| Matriz | Sequencial | 3 threads | Speedup |
|---|---:|---:|---:|
| 1000x1000 | 17.890 ms | 9.491 ms | 1.89 |
| 2000x2000 | 63.465 ms | 38.945 ms | 1.63 |
| 4000x4000 | 265.057 ms | 196.809 ms | 1.35 |

Os resultados mostram que aumentar a quantidade de threads não produz necessariamente ganho proporcional.

Entre os fatores que influenciam o desempenho estão:

- criação das threads;
- sincronização;
- acesso à memória;
- balanceamento entre regiões;
- consolidação das fronteiras;
- partes sequenciais do algoritmo.

Os dados completos estão disponíveis em:

```text
results/desempenho.md
```

Os dados brutos estão em:

```text
results/medicoes.csv
```

---

# Gráficos

Os gráficos são gerados a partir de:

```text
results/medicoes.csv
```

Arquivos gerados:

```text
results/grafico-tempo.png
results/grafico-aceleracao.png
results/grafico-eficiencia.png
```

O script responsável pela geração está em:

```text
tests/gerar-graficos.py
```

Para gerar novamente os gráficos, primeiro ative o ambiente virtual:

```bash
source .venv/bin/activate
```

Depois execute:

```bash
python tests/gerar-graficos.py
```

---

# Relatório técnico

A explicação completa da solução está disponível em:

```text
RELATORIO_TECNICO.md
```

O relatório apresenta:

- arquitetura;
- versão sequencial;
- versão paralela;
- decomposição;
- sincronização;
- consolidação;
- Union-Find;
- testes funcionais;
- testes de desempenho;
- speedup;
- eficiência;
- gráficos;
- tratamento de erros;
- limitações da implementação.

---

# Tecnologias utilizadas

- ANSI C C89/C90
- POSIX Threads
- GCC
- Make
- Git
- GitHub
- Python
- Matplotlib