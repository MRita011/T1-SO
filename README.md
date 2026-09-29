# T1-SO
- Nomes: Gabriel Kowaleski, Jully Anne Seifert, Maria Rita Rodrigues e Mayra Bordin.

## Versão sequencial

A versão sequencial realiza a contagem de objetos em uma matriz binária utilizando conectividade 8.

### Compilação

No Linux, macOS ou WSL:

```bash
gcc -std=c89 -Wall -Wextra -pedantic src/conta-objetos-sequencial.c -o sequencial
```
e depois:

```
./sequencial
```

## Versão Paralela

A versão paralela utiliza Pthreads para dividir o processamento da matriz em faixas de linhas.

Cada thread recebe uma região específica da matriz e realiza a identificação dos componentes locais utilizando flood_fill_regiao, que limita a busca às linhas atribuídas àquela thread.

Atualmente, a implementação realiza a contagem dos componentes locais de cada região. A etapa de consolidação dos componentes que atravessam as fronteiras entre as threads será realizada posteriormente.

### Compilação

No Linux, macOS ou WSL:

```bash
gcc -std=c89 -Wall -Wextra -pedantic -pthread \
src/conta-objetos-paralelo.c -o paralelo
```
e depois:

```
./paralelo
```