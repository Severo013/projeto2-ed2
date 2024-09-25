#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

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

	for (int i = 0; i < numeroRegistros; i++) {
		inserirRegistro(vetorInsercoes[i]);
	}

	for (int i = 0; i < numeroChavesPrimarias; i++) {

		Registro resultado = buscaChavePrimaria(vetorChavesPrimarias[i]);
		if (strlen(resultado.idAluno) > 0)
		{
			printRegistro(resultado);
		}
	}

	for (int i = 0; i < numeroChavesSecundarias; i++) {

		Registro resultado = buscaChaveSecundaria(vetorChavesSecundarias[i].nomeAluno);
		if (strlen(resultado.idAluno) > 0)
		{
			printRegistro(resultado);
		}
	}

	return 0;
}
