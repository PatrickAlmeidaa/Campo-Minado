#include <stdio.h>
#include <stdlib.h>
void alocaCampo(char ***campo, int linha, int coluna){// fuanção para alocar um campo dinamicamente
	*campo = malloc(linha * sizeof(char*));
	for (int i = 0; i < linha; i++)
		(*campo)[i] = malloc(coluna * sizeof(char));
}

void liberaCampo(char ***campo, int linha, int coluna){// fuanção para desalocar um campo que foi alocado dinamicamente
	for (int i = 0; i < linha; i++){
		free((*campo)[i]);
	}
	free(*campo);
}