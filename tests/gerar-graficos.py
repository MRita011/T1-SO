import csv
import os

import matplotlib.pyplot as plt


ARQUIVO_CSV = "results/medicoes.csv"
PASTA_RESULTADOS = "results"


def carregar_dados():
    dados = {}

    with open(ARQUIVO_CSV, "r", encoding="utf-8") as arquivo:
        leitor = csv.DictReader(arquivo)

        for linha in leitor:
            matriz = linha["matriz"]
            versao = linha["versao"]
            trabalhadores = int(linha["trabalhadores"])
            tempo = float(linha["tempo_ms"])

            chave = (matriz, versao, trabalhadores)

            if chave not in dados:
                dados[chave] = []

            dados[chave].append(tempo)

    return dados


def media(valores):
    return sum(valores) / len(valores)


def gerar_grafico_tempo(dados):
    matrizes = ["1000x1000", "2000x2000", "4000x4000"]
    trabalhadores = [1, 2, 3, 4]

    plt.figure(figsize=(9, 6))

    for matriz in matrizes:
        medias = []
        erros_inferiores = []
        erros_superiores = []

        for quantidade in trabalhadores:
            if quantidade == 1:
                valores = dados[(matriz, "sequencial", 1)]
            else:
                valores = dados[(matriz, "paralela", quantidade)]

            valor_medio = media(valores)

            medias.append(valor_medio)
            erros_inferiores.append(valor_medio - min(valores))
            erros_superiores.append(max(valores) - valor_medio)

        plt.errorbar(trabalhadores, medias, yerr=[erros_inferiores, erros_superiores], marker="o", capsize=4, label=matriz)

    plt.title("Tempo de execução por quantidade de trabalhadores")
    plt.xlabel("Quantidade de trabalhadores")
    plt.ylabel("Tempo médio (ms)")
    plt.xticks(trabalhadores)
    plt.legend(title="Matriz")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(os.path.join(PASTA_RESULTADOS, "grafico-tempo.png"), dpi=300)
    plt.close()


def gerar_grafico_aceleracao(dados):
    matrizes = ["1000x1000", "2000x2000", "4000x4000"]
    trabalhadores = [1, 2, 3, 4]

    plt.figure(figsize=(9, 6))

    for matriz in matrizes:
        tempo_sequencial = media(dados[(matriz, "sequencial", 1)])
        speedups = [1.0]

        for quantidade in trabalhadores[1:]:
            tempo_paralelo = media(dados[(matriz, "paralela", quantidade)])
            speedups.append(tempo_sequencial / tempo_paralelo)

        plt.plot(trabalhadores, speedups, marker="o", label=matriz)

    plt.plot(trabalhadores, trabalhadores, linestyle="--", label="Aceleração ideal")

    plt.title("Aceleração por quantidade de trabalhadores")
    plt.xlabel("Quantidade de trabalhadores")
    plt.ylabel("Speedup S(p)")
    plt.xticks(trabalhadores)
    plt.legend(title="Matriz")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(os.path.join(PASTA_RESULTADOS, "grafico-aceleracao.png"), dpi=300)
    plt.close()


def gerar_grafico_eficiencia(dados):
    matrizes = ["1000x1000", "2000x2000", "4000x4000"]
    trabalhadores = [1, 2, 3, 4]

    plt.figure(figsize=(9, 6))

    for matriz in matrizes:
        tempo_sequencial = media(dados[(matriz, "sequencial", 1)])
        eficiencias = [1.0]

        for quantidade in trabalhadores[1:]:
            tempo_paralelo = media(dados[(matriz, "paralela", quantidade)])
            speedup = tempo_sequencial / tempo_paralelo
            eficiencias.append(speedup / quantidade)

        plt.plot(trabalhadores, eficiencias, marker="o", label=matriz)

    plt.title("Eficiência paralela por quantidade de trabalhadores")
    plt.xlabel("Quantidade de trabalhadores")
    plt.ylabel("Eficiência E(p)")
    plt.xticks(trabalhadores)
    plt.ylim(0, 1.1)
    plt.legend(title="Matriz")
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(os.path.join(PASTA_RESULTADOS, "grafico-eficiencia.png"), dpi=300)
    plt.close()


def main():
    dados = carregar_dados()

    gerar_grafico_tempo(dados)
    gerar_grafico_aceleracao(dados)
    gerar_grafico_eficiencia(dados)

    print("Graficos gerados com sucesso:")
    print("results/grafico-tempo.png")
    print("results/grafico-aceleracao.png")
    print("results/grafico-eficiencia.png")


if __name__ == "__main__":
    main()