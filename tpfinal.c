//Patrick Araujo de Almeida - 19.2.4117 - Ciência da Computação - UFOP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cores.h"
#include "aloca.h"

void newGame(int linha, int coluna, int bombas, int argc, const char *arg);// função para gerar um jogo
void preencheCampo(char **campo, int linha, int coluna);//  coloca uma as dicas ao redor das bombas
int dentroDoCampo(int linha,int coluna,int i2, int j2);// verifica se a posição encolida na matriz esta dentro do campo
void jogar(char **campo, int linha, int coluna,int bombas, int argc, const char *arg);// da as opções para o usuario durante uma partida 
int converteArgumentos(char argumento1);// converte o argumrnto do tipo char que o usuario digitou para um argumento tipoo int
void abreCampo(char **campo, char **campo2, int linha, int coluna, int argumento1, int argumento2);
int verificaBomba(char **campo, int argumento1, int argumento2);// abre a casa que o usuario escolheu
void resolveJogo(char **campo, int linha, int coluna);// imprime o jogo resolvido caso o usuario abra uma bomba ou selecione essa opção
void imprimeJogo(char **campo, int linha , int coluna);// imprime as alterações no campo minado durante uma partida
void salvaJogo(char **campo2, char **campo, char *salvar,int linha, int coluna);// salva o jogo em arquivos texto quando o usuario digita essa opção 
void marcaBomba(char **campo2, int linha, int coluna, int argumento1, int argumento2);// função que marca uma bomba no campo quando o usuario digita "x" e as coordenadas
int verificaEspacos(int espacosVazios, char **campo2, int linha, int coluna);// verifica quantos espaços foram abertos durante a partida
void retomaJogo(char **campo, char **campo2, int linha, int coluna, int bombas, const char *arg);// rotoma o jogo que o usuario salvou
void abreAoRedor(char **campo, char **campo2, int linha, int coluna, int i, int j);// abre as casas adjacentes às adjacentes 


typedef struct{ // estrutura para ler os dados do campo minado de um arquivo texto
	int linha;
	int coluna;
	int bombas;
}CampoMinado;

int main(int argc, char const *argv[]){
	int dificuldade;
	CampoMinado resolvido, jogo;
	resolvido.bombas = 0;
	jogo.bombas = 0;
	char **campoTxt, **resolvidoTxt;
	if(argc == 1){
		printf(TAB_TL); for (int i = 0; i < 63; ++i) printf(TAB_HOR); printf(TAB_TR"\n");
		printf(TAB_VER);printf(BOLD(" \t\t ESCOLHA A DIFICULDADE DO NOVO JOGO: \t\t"));printf(TAB_VER"\n");
		printf(TAB_VER);printf(BOLD(" \t\t  1 - FÁCIL - 8x8 COM 10 MINAS \t\t\t"));printf(TAB_VER"\n");
		printf(TAB_VER);printf(BOLD("\t\t  2 - MÉDIO - 10x10 COM 20 MINAS\t\t"));printf(TAB_VER"\n");
		printf(TAB_VER);printf(BOLD("\t\t  3 - DIFÍCIL -  15x13 COM 30 MINAS \t\t"));printf(TAB_VER"\n");
		printf(TAB_BL); for (int i = 0; i < 63; ++i) printf(TAB_HOR); printf(TAB_BR"\n");
		printf("\n");
		printf(BOLD("ESCOLHA: "));
		scanf("%d", &dificuldade);
		getchar();
		system("clear");
		switch (dificuldade){
			case 1:
				newGame(8, 8, 10, argc, argv[0]);
				break;
			case 2:
				newGame(10, 10, 20, argc, argv[0]);
				break;
			case 3:
				newGame(15, 13, 30, argc, argv[0]);
				break;
		}
	}
	else if (argc == 2){ // le o campo do arquivo em texto (.jogo) e o transfor em uma partida 
		FILE *entrada = fopen(argv[1], "r"); //verifica se o segundo argumento é um arquivo em texto

		if(entrada == NULL){ // verifica se o arquivo foi aberto e/ou se ele existe
			printf(BOLD("O ARQUIVO NÃO FOI ABERTO")"\n");
			exit(0);	
		} 

		fscanf(entrada, "%d %d \n\n", &jogo.linha, &jogo.coluna);
		 
		alocaCampo(&campoTxt, jogo.linha, jogo.coluna);
		for(int i = 0; i < jogo.linha; i++){
			for (int j = 0; j < jogo.coluna; j++){
				fscanf(entrada, "%c ", &campoTxt[i][j]);
				if(campoTxt[i][j] == 'x') jogo.bombas++;
			}
		}
		fclose(entrada);
		jogar(campoTxt, jogo.linha, jogo.coluna, jogo.bombas, argc, argv[0]);
	}
	else if(argc == 3){
		FILE *entrada = fopen(argv[1], "r"); //verifica se o segundo argumento é um arquivo em texto

		if(entrada == NULL){ // verifica se o arquivo foi aberto e/ou se ele existe
			printf(BOLD("O ARQUIVO NÃO FOI ABERTO")"\n");
			exit(0);	
		} 

		fscanf(entrada, "%d %d \n\n", &resolvido.linha, &resolvido.coluna);
		 
		alocaCampo(&resolvidoTxt, resolvido.linha, resolvido.coluna);
		for(int i = 0; i < resolvido.linha; i++){
			for (int j = 0; j < resolvido.coluna; j++){
				fscanf(entrada, "%c ", &resolvidoTxt[i][j]);
				if(resolvidoTxt[i][j] == 'x') resolvido.bombas++;
			}
		}
		fclose(entrada);
		jogar(resolvidoTxt, resolvido.linha, resolvido.coluna, resolvido.bombas, argc, argv[2]);
	}
	return 0;
}

void newGame(int linha, int coluna, int bombas, int argc, const char *arg){
	int contBomb, bombLinha, bombColuna;
	char **campo;
	alocaCampo(&campo, linha, coluna);
	srand(time(NULL));
	
	for (contBomb = 0; contBomb < bombas; ){ //coloca bombas em posições aleatorias 
		bombLinha = rand()%linha;
		bombColuna = rand()%coluna;
		if(campo[bombLinha][bombColuna] != 'x'){
			campo[bombLinha][bombColuna] = 'x';
			contBomb++;
		}
	}

	for (int i = 0; i < linha; i++){ // as posições que não frem bomba são inicializadas com 0
		for(int j = 0; j < coluna; j++){
			if(campo[i][j] != 'x')
				campo [i][j] = '0';
		}
	}
	preencheCampo(campo, linha, coluna);
	jogar(campo, linha, coluna, bombas, argc, arg);
}

void preencheCampo(char **campo, int linha, int coluna){
	int j, i, j2, i2;

	//coloca os valores ao redor das casas fora das bordaas do campo
	for (i = 0; i < linha; i++){
		for(j = 0; j < coluna; j++){
			if(campo[i][j] == 'x'){
				for (i2 = i - 1; i2 <= i + 1; i2++){
					for (j2 = j - 1; j2 <= j + 1; j2++){
						if (dentroDoCampo(linha, coluna, i2, j2)){
							if (campo[i2][j2] != 'x'){
								campo[i2][j2] += 1;
							}
						}
					}
				}
			}
		}
	}
}

int dentroDoCampo(int linha, int coluna, int i2, int j2){
	// caso a posição esteja dentro do campo a função retorna 1, ela é utilizada para evitar erros de segmentação 
	// quando o programa tenta acessar uma posição que esta fora do campo
	if(i2 >= 0 && i2 <= linha - 1 && j2 >= 0 && j2 <= coluna - 1) 
		return 1;
	else 
		return 0;
}
void jogar(char **campo, int linha, int coluna, int bombas, int argc, const char *arg){
	int espacosVazios = (linha * coluna) - bombas;
	char escolha[9], arg1, arg2, **campoFechado, salvar[20];
	int convertido1, convertido2, termina = 1;
	alocaCampo(&campoFechado, linha, coluna); // função da biblioteca "aloca.h"
	for (int i = 0; i < linha; ++i){ // inicisliza o campo que o usuario verá durante a partida com '-'
		for (int j = 0; j < coluna; j++){
			campoFechado[i][j] = '-';
		}
	}
	if(argc == 3){ // verifica se a quantidade de argumentos passados durante a execução é 3 para dar continuidade ao jogo salvo
		retomaJogo(campo, campoFechado, linha, coluna, bombas, arg);
	}
	imprimeJogo(campoFechado, linha, coluna);

	do{
		for (int i = 0; i < 105; ++i) printf(TAB_HOR);
		printf("\n");

		printf(BOLD("x           	AC 	   \t Indica que a célula da linha A e coluna C contém uma mina.\n"));
		printf(BOLD("o           	EC 	   \t Revela o valor da célula da linha E e coluna C.\n"));
		printf(BOLD("salvar      	out	   \t Salva o jogo tal como est´a no momento.\n"));
		printf(BOLD("resolver      		   \t Resolve o jogo, marcando automaticamente as células com o valor certo.\n"));
		printf(BOLD("sair          		   \t Encerra o programa (sem salvar as últimas alterações)\n"));

		for (int i = 0; i < 105; ++i) printf(TAB_HOR);
		printf("\n");
		printf("O que deseja fazer? \n");
		scanf("%s%*c", escolha); // %*c iginora um caractere, no caso o \n
		if(strcmp(escolha, "x") == 0){
			printf("Digite a linha  e a coluna: ");
			scanf("%c%c%*c", &arg1, &arg2);
			convertido1 = converteArgumentos(arg1);
			convertido2 = converteArgumentos(arg2);
			marcaBomba(campoFechado, linha, coluna, convertido1, convertido2);
			system("clear");
			imprimeJogo(campoFechado, linha, coluna);
		}
		else if(strcmp(escolha, "o") == 0){
			printf("Digite a linha  e a coluna: ");
			scanf("%c%c%*c", &arg1, &arg2);
			convertido1 = converteArgumentos(arg1);
			convertido2 = converteArgumentos(arg2);
			system("clear");
			if(verificaBomba(campo, convertido1, convertido2)){ // verifica se a posição que o usuario quer abrir é uma bomba 
				for (int i = 0; i < 10; i++){
					printf(BOLD(RED("\tGAME OVER!!! ")));printf(BOLD(RED("\tGAME OVER!!! ")));printf(BOLD(RED("\tGAME OVER!!! ")));printf(BOLD(RED("\tGAME OVER!!! ")));printf(BOLD(RED("\tGAME OVER!!! ")));printf(BOLD(RED("\tGAME OVER!!!\n")));
				}
				resolveJogo(campo, linha, coluna);
				liberaCampo(&campo, linha, coluna);// função da biblioteca "aloca.h"
				liberaCampo(&campoFechado, linha, coluna);
				termina = 0;
			}
			else {
				abreCampo(campo, campoFechado, linha, coluna, convertido1, convertido2);
				if(verificaEspacos(espacosVazios, campoFechado, linha, coluna)){
					for (int i = 0; i < 10; i++){
					printf(BOLD(BLUE("\tYOU ARE THE CHAMPION!!! ")));printf(BOLD(BLUE("\tYOU ARE THE CHAMPION!!! ")));printf(BOLD(BLUE("\tYOU ARE THE CHAMPION!!! ")));printf(BOLD(BLUE("\tYOU ARE THE CHAMPION!!! ")));printf(BOLD(BLUE("\tYOU ARE THE CHAMPION!!! ")));printf(BOLD(BLUE("\tYOU ARE THE CHAMPION!!!\n")));
				}
					termina = 0;
				}
			}
		}
		else if(strcmp(escolha, "resolver") == 0){
			system("clear");
			resolveJogo(campo, linha, coluna);
			liberaCampo(&campo, linha, coluna);
			liberaCampo(&campoFechado, linha, coluna);
			termina = 0;
		}
		else if(strcmp(escolha, "sair") == 0){
			liberaCampo(&campo, linha, coluna);
			liberaCampo(&campoFechado, linha, coluna);
			system("clear");
			termina = 0;
		}
		else if(strcmp(escolha, "salvar") == 0){
			printf("Dogite o nome do arquivo para salvar o jogo: ");
			scanf("%s", salvar);
			salvaJogo(campoFechado, campo, salvar, linha, coluna);
			liberaCampo(&campo, linha, coluna);
			liberaCampo(&campoFechado, linha, coluna);
			termina = 0;
		}
	}while(termina != 0);
}
int converteArgumentos(char argumento){
	if(argumento >= 97 && argumento <= 122)// verifica se o aargumento passado é uma letra minuscula e a converte para uma letra maiuscula
		argumento -= 32;
	return (argumento - 'A');
}
void abreCampo(char **campo, char **campo2, int linha, int coluna, int argumento1, int argumento2){
	// troca o '-' do campo que aparece para o usuario durante o jogo para os valor do campo que contem as respostas 
	if(campo2[argumento1][argumento2] != '-'){// verifica se a casa ja foi aberta
		printf(BOLD("\t\t\tCASA JA ABERTA\n"));
	}
	else if(campo[argumento1][argumento2] == '0'){// abre as casas adjacentes caso a casa escolhida seja 0
		for (int i2 = argumento1 - 1; i2 <= argumento1 + 1; i2++){
			for (int j2 = argumento2 - 1; j2 <= argumento2 + 1; j2++){
				if (dentroDoCampo(linha, coluna, i2, j2)){
					if (campo[i2][j2] >= '0' && campo[i2][j2] != 'x'){
						campo2[i2][j2] = campo[i2][j2];
						abreAoRedor(campo, campo2, linha, coluna, i2, j2);
					}
				}
			}
		}
	}
	else{
		campo2[argumento1][argumento2] = campo[argumento1][argumento2];// abrer somente a casa escolhida caso ela não seja 0
	}
	imprimeJogo(campo2, linha , coluna);
}

int verificaBomba(char **campo, int argumento1, int argumento2){
	if (campo[argumento1][argumento2] == 'x')
		return 1;
	else 
		return 0;
}
void resolveJogo(char **campo, int linha, int coluna){
	char c ='A';
 	for (int z = 0; z < coluna; z++){
 		printf(BOLD("\t%c "), c);
 		c++;
 	}
 	c ='A';
 	printf("\n");printf("   ");printf(TAB_TL);
 	for (int z = 0; z < coluna  * 2; z++){
 		printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR);
 	}
 	printf("\n");
	for (int i2 = 0; i2 < linha; i2++){
		printf(" %c "TAB_VER, c);
		for(int j2 = 0; j2< coluna; j2++){ 
			if (campo[i2][j2] == 'x')printf( BOLD(RED("\t%c")), campo[i2][j2]);
			else if(campo[i2][j2] == '0')printf(BLACK("\t%c"), campo[i2][j2]);
			else if(campo[i2][j2] == '1')printf(CYAN("\t%c"), campo[i2][j2]);
			else if(campo[i2][j2] == '2')printf(GREEN("\t%c"), campo[i2][j2]);
			else if(campo[i2][j2] == '3')printf(BLUE("\t%c"), campo[i2][j2]);
			else printf(RED("\t%c"), campo[i2][j2]);
		}
		printf("\n   "TAB_VER"\n");
 		c++;
	}
}
void imprimeJogo(char **campo2, int linha , int coluna){
	char c ='A';
 	for (int z = 0; z < coluna; z++){
 		printf(BOLD("\t%c "), c);
 		c++;
 	}
 	c ='A';
 	printf("\n");printf("   ");printf(TAB_TL);
 	for (int z = 0; z < coluna  * 2; z++){
 		printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR);
 	}
 	printf("\n");
	for (int i = 0; i < linha; ++i){
		printf(" %c "TAB_VER, c);
		for (int j = 0; j < coluna; j++){
			if (campo2[i][j] == '-')printf( BOLD("\t%c"), campo2[i][j]);
			else if(campo2[i][j] == 'x')printf(BOLD("\t%c"), campo2[i][j]);
			else if(campo2[i][j] == '0')printf(BLACK("\t%c"), campo2[i][j]);
			else if(campo2[i][j] == '1')printf(CYAN("\t%c"), campo2[i][j]);
			else if(campo2[i][j] == '2')printf(GREEN("\t%c"), campo2[i][j]);
			else if(campo2[i][j] == '3')printf(BLUE("\t%c"), campo2[i][j]);
			else printf(RED("\t%c"), campo2[i][j]);
		}
		printf("\n   "TAB_VER"\n");
 		c++;
	}	
}
void salvaJogo(char **campo2, char **campo, char *salvar,int linha, int coluna){
	char salva2[20];
	strcpy(salva2, salvar);
	strcat(salva2, ".jogo");
	strcat(salvar, ".txt");
	FILE *saveUser = fopen(salvar, "w");
	fprintf(saveUser, "%d %d \n\n", linha, coluna);
		 
	for(int i = 0; i < linha; i++){
		for (int j = 0; j < coluna; j++){
			if(campo2[i][j] != '-' && campo2[i][j] != 'x') fprintf(saveUser, "o ");
			else if(campo2[i][j] == 'x') fprintf(saveUser, "x ");
			else fprintf(saveUser, "%c ", campo2[i][j]);
		}
		fprintf(saveUser,"\n");
	}
	fclose(saveUser);
	FILE *resolvido = fopen(salva2, "w");
	fprintf(resolvido, "%d %d \n\n", linha, coluna);
	 
	for(int i = 0; i < linha; i++){
		for (int j = 0; j < coluna; j++){
			fprintf(resolvido, "%c ", campo[i][j]);
		}
		fprintf(resolvido,"\n");
	}
	fclose(resolvido);
}
void marcaBomba(char **campo2, int linha, int coluna, int argumento1, int argumento2){
	campo2[argumento1][argumento2] = 'x';
}
int verificaEspacos(int espacosVazios, char **campo2, int linha, int coluna){
	int contaEspacos = 0;
	for (int i = 0; i < linha; i++){
		for (int j = 0; j < coluna; j++){
			if(campo2[i][j] != '-' && campo2[i][j] != 'x')
				contaEspacos ++;
		}
	}
	if(contaEspacos == espacosVazios)
		return 1;
	else return 0;
}
void retomaJogo(char **campo, char **campo2, int linha, int coluna, int bombas, const char *arg){
	CampoMinado retomada;
	char **retomaCampo;
	FILE *retorno = fopen(arg, "r"); //verifica se o segundo argumento é um arquivo em texto

		if(retorno == NULL){ // verifica se o arquivo foi aberto e/ou se ele existe
			printf(BOLD("O ARQUIVO NÃO FOI ABERTO")"\n");
			exit(0);	
		} 

		fscanf(retorno, "%d %d \n\n", &retomada.linha, &retomada.coluna);
		alocaCampo(&retomaCampo, retomada.linha, retomada.coluna);
		for(int i = 0; i < retomada.linha; i++){
			for (int j = 0; j < retomada.coluna; j++){
				fscanf(retorno, "%c ", &retomaCampo[i][j]);
			}
		}
		fclose(retorno);

	for (int i = 0; i < linha; i++){
		for (int j = 0; j < coluna; j++){
			if(retomaCampo[i][j] == 'o') campo2[i][j] = campo[i][j];
			else if(retomaCampo[i][j] == 'x') campo2[i][j] = 'x';
		}
	}
	liberaCampo(&retomaCampo, retomada.linha, retomada.coluna);
}
void abreAoRedor(char **campo, char **campo2, int linha, int coluna, int i, int j){
	// abre as casas adjacentes às adjacentes apenas uma vez
	int i2, j2;
	if(campo[i][j] == '0'){
		for (i2 = i - 1; i2 <= i + 1; i2++){
			for (j2 = j - 1; j2 <= j + 1; j2++){
				if (dentroDoCampo(linha, coluna, i2, j2)){
					if (campo[i2][j2] >= '0' && campo[i2][j2] != 'x'){
						campo2[i2][j2] = campo[i2][j2];
					}
				}
			}
		}
	}
}