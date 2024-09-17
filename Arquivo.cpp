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

#pragma endregion

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

// Função para inserir registro no arquivo de dados e atualizar os índices
void inserir_registro(FILE* dados, FILE* indice_primario, FILE* indice_secundario, Registro registro) {
    // Buscar o final do arquivo de dados
    fseek(dados, 0, SEEK_END);
    int offset = ftell(dados);

    // Escrever o registro no arquivo de dados
    fwrite(&registro, sizeof(Registro), 1, dados);

    // Atualizar o índice primário
    IndicePrimario indice;
    strcpy(indice.id, registro.idAluno);
    strcpy(indice.sigla, registro.siglaDisciplina);
    indice.offset = offset;
    fwrite(&indice, sizeof(IndicePrimario), 1, indice_primario);

    // Atualizar o índice secundário (lista invertida)
    // (Processo de verificação se o nome já existe, e em caso afirmativo, atualizar a lista invertida)
}

// Função para buscar registro por chave primária
Registro buscar_por_chave_primaria(FILE* dados, FILE* indice_primario, const char* id, const char* sigla) {
   
    IndicePrimario indice;
    fseek(indice_primario, 0, SEEK_SET);

    // Busca sequencial no índice primário
    while (fread(&indice, sizeof(IndicePrimario), 1, indice_primario)) {
        if (strcmp(indice.id, id) == 0 && strcmp(indice.sigla, sigla) == 0) {
            fseek(dados, indice.offset, SEEK_SET);
            Registro registro;
            fread(&registro, sizeof(Registro), 1, dados);
            return registro;
        }
    }

    Registro vazio = { 0 };  // Retorna um registro vazio caso não seja encontrado
    return vazio;
}

// Função para buscar registro por chave secundária (nome do aluno)
void buscar_por_chave_secundaria(FILE* dados, FILE* indice_secundario, FILE* lista_invertida, const char* nome) {
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
                Registro registro = buscar_por_chave_primaria(dados, indice_secundario, entrada.id, entrada.sigla);
                printf("ID: %s, Sigla: %s, Nome: %s\n", registro.idAluno, registro.siglaDisciplina, registro.nomeAluno);

                // Seguir para o próximo na lista invertida
                offset_lista = entrada.prox_offset;
            }
        }
    }
}