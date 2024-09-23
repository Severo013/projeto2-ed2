#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

#pragma region Funções Auxiliares

int getTamanhoArquivo(FILE* arquivo)
{
	fseek(arquivo, 0, SEEK_END);
	int tamanhoArquivo = ftell(arquivo);
	fseek(arquivo, 0, SEEK_SET);

	return tamanhoArquivo;
}

Registro lerRegistro(FILE* dados, int offset)
{
	Registro registro;
	fseek(dados, offset, SEEK_SET);
	fread(&registro, sizeof(Registro), 1, dados);
	return registro;
}

#pragma endregion

void inicializarArquivo(const char* nomeArquivo) {

	FILE* arquivo = fopen(nomeArquivo, "rb+");

	if (arquivo == NULL) {
		// Se o arquivo não existir, crie-o e inicialize o cabeçalho
		arquivo = fopen(nomeArquivo, "wb");
		if (arquivo == NULL) {
			perror("Erro ao criar o arquivo");
			exit(EXIT_FAILURE);
		}
	}
	fclose(arquivo);
}

int carregarRegistros(Registro** registros) {

	FILE* arquivoInsercoes = fopen("insere.bin", "rb");

	if (arquivoInsercoes == NULL) {
		perror("Erro ao abrir insere.bin");
		return 0;
	}

	int tamanhoArquivo = getTamanhoArquivo(arquivoInsercoes);

	int numeroRegistros = tamanhoArquivo / sizeof(Registro);
	*registros = (Registro*)malloc(tamanhoArquivo);

	if (*registros == NULL) {
		perror("Erro ao alocar memória para inserções");
		fclose(arquivoInsercoes);
		return 0;
	}

	fread(*registros, sizeof(Registro), numeroRegistros, arquivoInsercoes);
	fclose(arquivoInsercoes);

	return numeroRegistros;
}

int carregarBuscaChavePrimaria(BuscaChavePrimaria** chaves) {

	FILE* arquivoBuscaChavePrimaria = fopen("busca_p.bin", "rb");

	if (arquivoBuscaChavePrimaria == NULL) {
		perror("Erro ao abrir busca_p.bin");
		return 0;
	}

	int tamanhoArquivo = getTamanhoArquivo(arquivoBuscaChavePrimaria);

	int numeroRegistros = tamanhoArquivo / sizeof(BuscaChavePrimaria);
	*chaves = (BuscaChavePrimaria*)malloc(tamanhoArquivo);

	if (*chaves == NULL) {
		perror("Erro ao alocar memória para busca por chave primária");
		fclose(arquivoBuscaChavePrimaria);
		return 0;
	}

	fread(*chaves, sizeof(BuscaChavePrimaria), numeroRegistros, arquivoBuscaChavePrimaria);
	fclose(arquivoBuscaChavePrimaria);

	return numeroRegistros;
}

int carregarBuscaChaveSecundaria(BuscaChaveSecundaria** chaves) {

	FILE* arquivoBuscaChaveSecundaria = fopen("busca_s.bin", "rb");

	if (arquivoBuscaChaveSecundaria == NULL) {
		perror("Erro ao abrir busca_s.bin");
		return 0;
	}

	int tamanhoArquivo = getTamanhoArquivo(arquivoBuscaChaveSecundaria);

	int numeroRegistros = tamanhoArquivo / sizeof(BuscaChaveSecundaria);
	*chaves = (BuscaChaveSecundaria*)malloc(tamanhoArquivo);

	if (*chaves == NULL) {
		perror("Erro ao alocar memória para busca por chave secundária");
		fclose(arquivoBuscaChaveSecundaria);
		return 0;
	}

	fread(*chaves, sizeof(BuscaChaveSecundaria), numeroRegistros, arquivoBuscaChaveSecundaria);
	fclose(arquivoBuscaChaveSecundaria);

	return numeroRegistros;
}

void inserirRegistro(Registro registro, FILE* dados, FILE* indicePrimario) {

	// Escrever o registro no final do arquivo de dados
	fseek(dados, 0, SEEK_END);
	fwrite(&registro, sizeof(Registro), 1, dados);
	int offset = ftell(dados);

	// Atualizar o índice primário
	IndicePrimario indice;
	strcpy(indice.idAluno, registro.idAluno);
	strcpy(indice.siglaDisciplina, registro.siglaDisciplina);
	indice.offset = offset;

	fseek(indicePrimario, 0, SEEK_END);
	fwrite(&indice, sizeof(IndicePrimario), 1, indicePrimario);

	// Atualizar o índice secundário (lista invertida)
}

Registro buscaChavePrimaria(BuscaChavePrimaria chaveBusca, const char* nomeDados, const char* nomeIndicePrimario) {

	FILE* dados = fopen(nomeDados, "rb+");
	FILE* indicePrimario = fopen(nomeIndicePrimario, "rb+");

	IndicePrimario indice;
	fseek(indicePrimario, 0, SEEK_SET);

	// Busca sequencial no índice primário
	while (fread(&indice, sizeof(IndicePrimario), 1, indicePrimario)) {
		if (strcmp(indice.idAluno, chaveBusca.idAluno) == 0 && strcmp(indice.siglaDisciplina, chaveBusca.siglaDisciplina) == 0) {
			Registro registro = lerRegistro(dados, indice.offset);
			fclose(dados);
			fclose(indicePrimario);
			return registro;
		}
	}

	fclose(dados);
	fclose(indicePrimario);
	Registro vazio = { 0 };  // Retorna um registro vazio caso não seja encontrado
	return vazio;
}

// Função para buscar registro por chave secundária (nome do aluno)
void buscaChaveSecundaria(FILE* dados, FILE* indice_secundario, FILE* lista_invertida, const char* nome) {
	IndiceSecundario indice_sec;
	fseek(indice_secundario, 0, SEEK_SET);

	// Busca sequencial no índice secundário
	while (fread(&indice_sec, sizeof(IndiceSecundario), 1, indice_secundario)) {
		if (strcmp(indice_sec.nome, nome) == 0) {
			// Encontrado o nome, seguir a lista invertida
			int offset_lista = indice_sec.offset_lista_invertida;
			ListaInvertida entrada;
			while (offset_lista != -1) {
				fseek(lista_invertida, offset_lista, SEEK_SET);
				fread(&entrada, sizeof(ListaInvertida), 1, lista_invertida);

				// Recuperar o registro correspondente à chave primária
				Registro registro = buscaChavePrimaria(dados, indice_secundario, entrada.id, entrada.sigla);
				printf("ID: %s, Sigla: %s, Nome: %s\n", registro.idAluno, registro.siglaDisciplina, registro.nomeAluno);

				// Seguir para o próximo na lista invertida
				offset_lista = entrada.prox_offset;
			}
		}
	}
}