#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

void exibirMenu() {
	printf("Selecione uma op��o:\n");
	printf("1. Inserir um registro\n");
	printf("2. Buscar um registro por chave prim�ria\n");
	printf("3. Buscar um registro por chave secund�ria\n");
	printf("0. Sair\n");
}

void exibirRegistrosDisponiveis(Registro* registros, int numeroRegistros) {
	printf("Registros dispon�veis:\n");
	for (int i = 0; i < numeroRegistros; i++) {
		printf("%d. ", i);
		printRegistro(registros[i]);
	}
}

void exibirChavesPrimarias(BuscaChavePrimaria* chaves, int numeroChaves) {
	printf("Chaves prim�rias dispon�veis:\n");
	for (int i = 0; i < numeroChaves; i++) {
		printf("%d. %s - %s\n", i, chaves[i].idAluno, chaves[i].siglaDisciplina);
	}
}

void exibirChavesSecundarias(BuscaChaveSecundaria* chaves, int numeroChaves) {
	printf("Chaves secund�rias dispon�veis:\n");
	for (int i = 0; i < numeroChaves; i++) {
		printf("%d. %s\n", i, chaves[i].nomeAluno);
	}
}

int main() {

	const char* nomeDados = "dados.bin";
	const char* nomeIndicePrimario = "indice_p.bin";
	const char* nomeIndiceSecundario = "indice_s.bin";
	const char* nomeListaInvertida = "lista_invertida.bin";

	inicializarArquivo(nomeDados);
	inicializarArquivo(nomeIndicePrimario);
	inicializarArquivo(nomeIndiceSecundario);
	inicializarArquivo(nomeListaInvertida);

	Registro* vetorInsercoes;
	int numeroRegistros = carregarRegistros(&vetorInsercoes);

	BuscaChavePrimaria* vetorChavesPrimarias;
	int numeroChavesPrimarias = carregarBuscaChavePrimaria(&vetorChavesPrimarias);

	BuscaChaveSecundaria* vetorChavesSecundarias;
	int numeroChavesSecundarias = carregarBuscaChaveSecundaria(&vetorChavesSecundarias);

	int opcao;
	do {
		exibirMenu();
		printf("Op��o: ");
		scanf("%d", &opcao);

		switch (opcao) {
		case 1:
			exibirRegistrosDisponiveis(vetorInsercoes, numeroRegistros);
			printf("Selecione um registro para inserir: ");
			int indiceInsercao;
			scanf("%d", &indiceInsercao);
			Registro registroInsercao = vetorInsercoes[indiceInsercao];
			inserirRegistro(registroInsercao);
			break;
		case 2:
			exibirChavesPrimarias(vetorChavesPrimarias, numeroChavesPrimarias);
			printf("Selecione uma chave prim�ria para buscar: ");
			int indiceBusca;
			scanf("%d", &indiceBusca);
			BuscaChavePrimaria chaveBusca = vetorChavesPrimarias[indiceBusca];
			Registro registroBusca = buscaChavePrimaria(chaveBusca);
			printRegistro(registroBusca);
			break;
		case 3:
			exibirChavesSecundarias(vetorChavesSecundarias, numeroChavesSecundarias);
			printf("Digite o nome do aluno para buscar: ");
			char nomeBusca[50];
			scanf("%s", nomeBusca);
			Registro registroBuscaSecundaria = buscaChaveSecundaria(nomeBusca);
			printRegistro(registroBuscaSecundaria);
			break;
		case 0:
			printf("Saindo...\n");
			break;
		default:
			printf("Op��o inv�lida. Tente novamente.\n");
			break;
		}
	} while (opcao != 0);

	return 0;
}