#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

const char* nomeDados = "dados.bin";
const char* nomeIndicePrimario = "indice_p.bin";
const char* nomeIndiceSecundario = "indice_s.bin";
const char* nomeListaInvertida = "lista_invertida.bin";

#pragma region Fun��es Auxiliares

Registro nullRegistro() {
    Registro registro = { 0 };
    return registro;
}

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

void printRegistro(Registro registro) {
	printf("%s | %s | %s | %s | %f | %f \n", registro.idAluno, registro.siglaDisciplina, registro.nomeAluno, registro.nomeDisciplina, registro.media, registro.frequencia);
}

void inicializarArquivo(const char* nomeArquivo) {

	FILE* arquivo = fopen(nomeArquivo, "rb+");

	if (arquivo == NULL) {
		// Se o arquivo n�o existir, crie-o e inicialize o cabe�alho
		arquivo = fopen(nomeArquivo, "wb");
		if (arquivo == NULL) {
			perror("Erro ao criar o arquivo");
			exit(EXIT_FAILURE);
		}
	}
	fclose(arquivo);
}

#pragma region Carregamento de Dados de Arquivos

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
		perror("Erro ao alocar mem�ria para inser��es");
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
		perror("Erro ao alocar mem�ria para busca por chave prim�ria");
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
		perror("Erro ao alocar mem�ria para busca por chave secund�ria");
		fclose(arquivoBuscaChaveSecundaria);
		return 0;
	}

	fread(*chaves, sizeof(BuscaChaveSecundaria), numeroRegistros, arquivoBuscaChaveSecundaria);
	fclose(arquivoBuscaChaveSecundaria);

	return numeroRegistros;
}

#pragma endregion

// Fun��o para inserir um registro no arquivo de dados
void inserirRegistro(Registro registro) {

	FILE* arquivoDados = fopen(nomeDados, "rb+");

	// Escrever o registro no final do arquivo de dados
	fseek(arquivoDados, 0, SEEK_END);
	fwrite(&registro, sizeof(Registro), 1, arquivoDados);
	int offset = ftell(arquivoDados);

	// Atualizar o �ndice prim�rio
	atualizarIndicePrimario(&registro, offset);

	fclose(arquivoDados);

	// Atualizar o �ndice secund�rio (lista invertida)
	atualizarIndiceSecundario(registro);

	printf("Registro inserido com sucesso.\n");
}

// Fun��o para buscar registro por chave prim�ria (idAluno + siglaDisciplina)
Registro buscaChavePrimaria(BuscaChavePrimaria chaveBusca) {

	FILE* arquivoDados = fopen(nomeDados, "rb+");
	FILE* arquivoIndicePrimario = fopen(nomeIndicePrimario, "rb+");

	IndicePrimario indice;
	fseek(arquivoIndicePrimario, 0, SEEK_SET);

	// Busca sequencial no �ndice prim�rio
	while (fread(&indice, sizeof(IndicePrimario), 1, arquivoIndicePrimario)) {
		if (strcmp(indice.idAluno, chaveBusca.idAluno) == 0 && strcmp(indice.siglaDisciplina, chaveBusca.siglaDisciplina) == 0) {
			Registro registro = lerRegistro(arquivoDados, indice.offset);
			fclose(arquivoDados);
			fclose(arquivoIndicePrimario);
			return registro;
		}
	}

	fclose(arquivoDados);
	fclose(arquivoIndicePrimario);

	printf("Registro n�o encontrado.\n");

    return nullRegistro();
}

// Fun��o para buscar registro por chave secund�ria (nome do aluno)
Registro buscaChaveSecundaria(const char* nomeBusca) {

	FILE* arquivoDados = fopen(nomeDados, "rb+");
	FILE* arquivoIndicePrimario = fopen(nomeIndicePrimario, "rb+");
	FILE* arquivoIndiceSecundario = fopen(nomeIndiceSecundario, "rb+");
	FILE* arquivoListaInvertida = fopen(nomeListaInvertida, "rb+");

	IndiceSecundario indice_sec;
	fseek(arquivoIndiceSecundario, 0, SEEK_SET);

	// Busca sequencial no �ndice secund�rio
	while (fread(&indice_sec, sizeof(IndiceSecundario), 1, arquivoIndiceSecundario)) {

		if (strcmp(indice_sec.nomeAluno, nomeBusca) == 0) {
			// Encontrado o nome, seguir a lista invertida

			int offset_lista = indice_sec.offsetListaInvertida;
			ListaInvertida entrada;

			fseek(arquivoListaInvertida, offset_lista, SEEK_SET);
			fread(&entrada, sizeof(ListaInvertida), 1, arquivoListaInvertida);

			// Recuperar o registro correspondente � chave prim�ria
			BuscaChavePrimaria chavePrimaria;
			strcpy(chavePrimaria.idAluno, entrada.idAluno);
			strcpy(chavePrimaria.siglaDisciplina, entrada.siglaDisciplina);
			Registro registro = buscaChavePrimaria(chavePrimaria);

			fclose(arquivoDados);
			fclose(arquivoIndicePrimario);
			fclose(arquivoIndiceSecundario);
			fclose(arquivoListaInvertida);

			return registro;
		}
	}

	fclose(arquivoDados);
	fclose(arquivoIndicePrimario);
	fclose(arquivoIndiceSecundario);
	fclose(arquivoListaInvertida);

	printf("Registro n�o encontrado.\n");

	return nullRegistro();
}

// Fun��o para atualizar o �ndice prim�rio com um novo registro
void atualizarIndicePrimario(Registro* registro, int offset)
{
	FILE* arquivoIndicePrimario = fopen(nomeIndicePrimario, "rb+");

	if (!arquivoIndicePrimario) {
		printf("Erro ao abrir arquivo de �ndice prim�rio!\n");
		return;
	}

	IndicePrimario indice;
	strcpy(indice.idAluno, registro->idAluno);
	strcpy(indice.siglaDisciplina, registro->siglaDisciplina);
	indice.offset = offset;

	fseek(arquivoIndicePrimario, 0, SEEK_END);
	fwrite(&indice, sizeof(IndicePrimario), 1, arquivoIndicePrimario);

	fclose(arquivoIndicePrimario);
}

// Fun��o para atualizar o �ndice secund�rio com um novo registro
void atualizarIndiceSecundario(Registro registro) {

	FILE* arquivoIndiceSecundario = fopen(nomeIndiceSecundario, "rb+");
	FILE* arquivoListaInvertida = fopen(nomeListaInvertida, "rb+");

	if (!arquivoIndiceSecundario || !arquivoListaInvertida) {
		printf("Erro ao abrir arquivos de �ndice secund�rio!\n");
		return;
	}

	IndiceSecundario indiceSec;
	long offsetUltimo = -1;

	// Procurar pelo nome do aluno no �ndice secund�rio
	fseek(arquivoIndiceSecundario, 0, SEEK_SET);
	while (fread(&indiceSec, sizeof(IndiceSecundario), 1, arquivoIndiceSecundario)) {
		if (strcmp(indiceSec.nomeAluno, registro.nomeAluno) == 0) {
			break;
		}
		offsetUltimo = ftell(arquivoIndiceSecundario) - sizeof(IndiceSecundario);
	}

	long offsetNovoRegistro = ftell(arquivoListaInvertida);  // Offset para o novo registro na lista invertida

	// Adicionar novo nome ao �ndice secund�rio e criar a lista invertida
	IndiceSecundario novoIndiceSecundario;
	strcpy(novoIndiceSecundario.nomeAluno, registro.nomeAluno);
	novoIndiceSecundario.offsetListaInvertida = offsetNovoRegistro;

	// Inserir novo nome no �ndice secund�rio
	fseek(arquivoIndiceSecundario, 0, SEEK_END);
	fwrite(&novoIndiceSecundario, sizeof(IndiceSecundario), 1, arquivoIndiceSecundario);

	// Criar a primeira entrada da lista invertida para esse nome
	ListaInvertida novoElemento;
	strcpy(novoElemento.idAluno, registro.idAluno);
	strcpy(novoElemento.siglaDisciplina, registro.siglaDisciplina);
	novoElemento.offset = -1;  // Fim da lista

	fseek(arquivoListaInvertida, 0, SEEK_END);
	fwrite(&novoElemento, sizeof(ListaInvertida), 1, arquivoListaInvertida);

	fclose(arquivoIndiceSecundario);
	fclose(arquivoListaInvertida);
}
