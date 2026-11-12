#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARACTERES 256
#define  MAX_NOS 511
#define MAX_TEXTO 500
#define MAX_CODIGO 256 
#define MAX_BITS (MAX_TEXTO * 255 + 1 ) 

//montar estrutura do nó da arvore de Haffman

typedef struct{
	char caractere;
	int frequencia;
	int esquerda;
	int direita;
	int pai;
}No;

//VETOR que armazena todos os nós da arvore
No nos[MAX_NOS];
//quantidade de nos utilizados
int quantidadeNos = 0;

//gerar uma tabela que guarda código de cada caracter
char codigos[ MAX_CARACTERES][MAX_CODIGO];


/*Função encontrarMenor
procura o nó de menor frequencia que ainda não possui pai.
parametro ignorar impede que o mesmo nó seja escolhido duas vezes*/

int encontrarMenor(int ignorar){
	int i, menor;
	for(i = 0; i<quantidadeNos;i++){
	//o nó precisa estar livre, ou seja, sem pai
	if(nos[i].pai == -1 && i != ignorar){
		if(menor == -1 || nos[i].frequencia < nos[menor].frequencia){
			menor = i;
		}
	}
	}
	return menor;
}

/*Função gerar código
percorrer a árvore recursivamente
esq = 0
dir = 1
quando encontra uma folha, salva o caminho percorrido como código do caracter*/
void gerarCodigo(int indice, char caminho[], int nivel){
	//verificar se o indice é válido
	if(indice == -1){
		return;
	}
	//verificar se o nó é uma folha
	if(nos[indice].esquerda == -1 && nos[indice].direita == -1){
		caminho[nivel] ='\0';
		//caso especial : somente um caractere
		if(nivel == 0){
			strcpy(codigos[nos[indice].caractere],"0");
		}else{
			strcpy(codigos[nos[indice].caractere],caminho);
		}
		return;
	}
	//percorer o filho esq, apresentar 0
	caminho[nivel] = '0';
	gerarCodigo(nos[indice].esquerda, caminho, nivel +1);
	
	//percorre filho da direita
	caminho[nivel] = '1';
	gerarCodigo(nos[indice].direita, caminho, nivel +1);
}

/*função mostrar tabela
	exibir os caracteres, suas frequencias*/
	
void mostrarTabela(int frequencia[]){
	printf("\n========Tabela de Codificação de Haffman=======\n");
	printf("%-10s %-11s %-10s\n","Caractere", "Frequencia", "Código");
	printf("-------------------------------------------------------");
	int i;
	for (i=0; i< MAX_CARACTERES; i++){
		if(frequencia[i]>0){
			//identificar o espaço em branco
			if(i== ' '){
				printf("%-10s %-11s %-10s\n","[ESPACO]", frequencia[i], codigos[i]);
			}else{
				printf("%-10s %-11s %-10s\n",i, frequencia[i],codigos[i]);
			}
		}
		
	}
}

//função construirArvore
//cria os nós iniciais e constroi a árvore 
//cada caracter diferente será um no inicial
// depois, os dois nos de menor frequencia serao unidos ate restar somente a raiz

int construirArvore(int frequencia[]){
	int i;
	quantidadeNos = 0;
	//criar um nó para cada caracter existente
	for(i=0;i< MAX_CARACTERES; i++){
		if(frequencia[i]>0){
			nos[quantidadeNos].caractere = i;
			nos[quantidadeNos].frequencia = frequencia[i];
			nos[quantidadeNos].direita = -1;
			nos[quantidadeNos].esquerda = -1;
			nos[quantidadeNos].pai = -1;
			quantidadeNos++;
		}
	}
	//verificar se existem caracteres para cumprir
	if(quantidadeNos == 0){
		return -1;
	}
	//caso exista somente um caracter diferente a arvore tera apenas um no
	if(quantidadeNos == 1){
		return 0;
	}
	//calcular quem é o menor - repetir a união dos dois nós menores
	while(1){
		int menor1 = encontrarMenor(-1);
		if(menor1 == -1){
			break;
		}
		int menor2 = encontrarMenor(menor1);
		if(menor2 == -1){
			break;
		}
		//criar um novo nó interno, sua frequencia sera a soma dos filhos
		nos[quantidadeNos].frequencia = nos[menor1].frequencia + nos[menor2].frequencia;
		
		nos[quantidadeNos].esquerda = menor1;
		nos[quantidadeNos].direita = menor2;
		nos[quantidadeNos].pai = -1;
		//pegar os nós anteriores agora possuem um pai
		nos[menor1].pai = quantidadeNos;
		nos[menor2].pai = quantidadeNos;
		//avaça para a próxima posição do vetor
		quantidadeNos++;
	}
	//O último nó criado será a raiz da árvore
	return quantidadeNos -1;
}

//Programa principal
int main(){
	//escolher frase de exemplo
	char frase[] = "DADOSGERAMINFORMACAO";
	//criar um vetor de frequencia dos caracteres
	int frequencia[MAX_CARACTERES] = {0};
	//criar um vetor que armazena o caminho durante a geração dos códigos
	char caminho[MAX_CODIGO];
	//Vetor da mensagem comprimida
	char comprimida[MAX_BITS];
	
	//etapa 1 - contar as frequencias
	int totalCaracteres = 0;
	int totalBits = 0;
	int raiz, i;
	printf("Frase original: \n");
	printf("%s\n", frase);
	for(i=0; frase[i] != '\0'; i++){
		char c = (char) frase[i];
		frequencia[c]++;
		totalCaracteres++;
	}
	//etapa 2 - construir a árvore
	raiz = construirArvore(frequencia);
	
	
	//etapa 3 - gerar os códigos
	gerarCodigo(raiz, caminho,0);
	
	//etapa 4 - mostrar tabela
	mostrarTabela(frequencia);
}
