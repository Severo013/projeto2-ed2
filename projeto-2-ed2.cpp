#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

int main() {

	const char* nomeArquivoDados = "dados.bin";
	const char* nomeArquivoIndice = "indice.bin";

	inicializarArquivo(nomeArquivoDados);
	inicializarArquivo(nomeArquivoIndice);

	Registro* vetorInsercoes;
	int numeroRegistros = carregarRegistros(&vetorInsercoes);

	BuscaChavePrimaria* vetorChavesPrimarias;
	int numeroChavesPrimarias = carregarBuscaChavePrimaria(&vetorChavesPrimarias);


	//FILE* indice_secundario = fopen("indice_secundario_nome.bin", "rb+");
	//FILE* lista_invertida = fopen("lista_invertida.bin", "rb+");

	//if (!dados || !indice_primario || !indice_secundario || !lista_invertida) {
	//    printf("Erro ao abrir arquivos!\n");
	//    return 1;
	//}

	//// Exemplo de uso: Inserir um novo registro
	//Registro novo_registro = { "001", "ED2", "Joao Silva", "Estruturas de Dados", 8.5, 80.0 };
	//inserir_registro(dados, indice_primario, indice_secundario, novo_registro);

	// Exemplo de uso: Buscar por chave primária

	Registro resultado = buscaChavePrimaria(vetorChavesPrimarias[1], nomeArquivoDados, nomeArquivoIndice);
	if (strlen(resultado.idAluno) > 0) {
		printf("Registro encontrado: %s %s\n", resultado.idAluno, resultado.nomeAluno);
	}
	else {
		printf("Registro não encontrado.\n");
	}

	//// Fechar arquivos
	//fclose(dados);
	//fclose(indice_primario);
	//fclose(indice_secundario);
	//fclose(lista_invertida);

	//return 0;
}
