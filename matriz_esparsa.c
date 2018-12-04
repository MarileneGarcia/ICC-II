#include <stdio.h>
#include <stdlib.h>
#include "matriz_esparsa.h"


struct celula{
    
    // identificação da célula (coordenadas da linha e da coluna da matriz)
    int linha;
    int coluna;

    // valor armazenado na célula
    float valor;

    // ponteiros para as céulas a direita e a abaixo
    CELULA *cel_direita; 
    CELULA *cel_abaixo;
};

struct matriz_esparsa{

    // ponteiros para os vetores de ponteiros de celulas
    CELULA **linha;
    CELULA **coluna;
    
    // número de linhas e colunas
    int nL;
    int nC;
};

MATRIZ_ESPARSA *criar_matriz (int n_linhas, int n_colunas){

	int aux;
    MATRIZ_ESPARSA *matriz_criada = (MATRIZ_ESPARSA *) malloc(sizeof(MATRIZ_ESPARSA));

    matriz_criada->nL = n_linhas;
    matriz_criada->nC = n_colunas;
    matriz_criada->linha = (CELULA **) malloc ( (sizeof (CELULA *)) * n_linhas); // array de ponteiros para célula para as linhas
    matriz_criada->coluna = (CELULA **) malloc ( (sizeof (CELULA *)) * n_colunas); // array de ponteiros para célula para as colunas

    for (aux=0; aux < n_linhas; aux++){
  		matriz_criada->linha[aux] = (CELULA *) malloc (sizeof(CELULA));
	  	matriz_criada->linha[aux]->cel_direita = NULL;
    }

    for (aux=0; aux < n_colunas; aux++){
      	matriz_criada->coluna[aux] = (CELULA *) malloc (sizeof(CELULA));
  	  	matriz_criada->coluna[aux]->cel_abaixo = NULL;
    }

    return matriz_criada;
}

int set_matriz (MATRIZ_ESPARSA *matriz, int linha_set, int coluna_set, double valor_set){

    // Matrizes em C consideram o 0 como a primeira linha e coluna, mas para melhor interpretação do usuário, a linha e a coluna começarão em 1
    --linha_set;
    --coluna_set;

    if ( (linha_set < matriz->nL) && (linha_set >= 0) && (coluna_set < matriz->nC) && (coluna_set >=0) ){ // Verifica se a posição dada não é inválida
 
        // Celula a ser inserida
        CELULA *celula_set = (CELULA *) malloc (sizeof(CELULA));
        celula_set->linha=linha_set;
        celula_set->coluna=coluna_set;
        celula_set->valor=valor_set;

        CELULA *p_aux = matriz->linha[linha_set]; // ponteiro auxiliar para manipulação da matriz
        while ( (p_aux->cel_direita != NULL) && (p_aux->cel_direita->coluna <= coluna_set) ){
            p_aux = p_aux->cel_direita; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }
        if (p_aux->cel_direita == NULL){
            celula_set->cel_direita = NULL; 
            p_aux->cel_direita = celula_set;
        }
        else{
            celula_set->cel_direita = p_aux->cel_direita; 
            p_aux->cel_direita = celula_set;
        }
        p_aux = matriz->coluna[coluna_set];
        while ( (p_aux->cel_abaixo != NULL) && (p_aux->cel_abaixo->linha <= linha_set) ){
            p_aux = p_aux->cel_abaixo; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }
        if (p_aux->cel_abaixo == NULL){
            celula_set->cel_abaixo = NULL; 
            p_aux->cel_abaixo = celula_set;
        }
        else{
            celula_set->cel_abaixo = p_aux->cel_abaixo; 
            p_aux->cel_abaixo = celula_set;
        }

        return 1;
    }
    return 0;
}

double get_matriz(MATRIZ_ESPARSA *matriz, int linha_get, int coluna_get){
    --linha_get;
    --coluna_get;

    if ( (linha_get < matriz->nL) && (linha_get >= 0) && (coluna_get < matriz->nC) && (coluna_get >=0) ){ // Verifica se a posição dada não é inválida
    
        CELULA *celula_get = matriz->linha[linha_get]; // ponteiro auxiliar que recebe o endereço do nó cabeça

        while (celula_get->cel_direita != NULL && celula_get->cel_direita->coluna <= coluna_get)
            celula_get = celula_get->cel_direita;

        if (celula_get->coluna == coluna_get)
            return celula_get->valor;        
    }
    return 0;
}

void apagar_matriz(MATRIZ_ESPARSA *matriz){
    CELULA *aux;
    int j;
    
    for (j=0; j<matriz->nL; j++){         
        CELULA *prox = matriz->linha[j]->cel_direita;  // Recebe as posições das células a direita do nó cabeça
    
        while(prox != NULL){ // Verifica se não será o final da lista ainda
            aux = prox;
            prox = prox->cel_direita; // Recebe uma nova posição à direita
            free(aux);  //Desaloca memoria do vetor que armazenava posição anterior à direita
        }
        free((matriz)->linha[j]); // Desaloca memoria do nó cabeça das linhas
    }

    free((matriz)->linha); // Desaloca memoria do arranjo que armazenava a quantidade de linhas
    free((matriz)->coluna); // Desaloca memoria do arranjo que armazenava a quantidade de colunas
    free((matriz)); // Desaloca memoria do ponteiro que apontava para a estrutura 
    matriz = NULL; // O ponteiro da matriz passa a apontar para uma posição invalida, de maneira que depois possa ser usado novamente
}

MATRIZ_ESPARSA *somar_matriz( MATRIZ_ESPARSA *M1,  MATRIZ_ESPARSA *M2){
    int k,h;
	if (M1->nL == M2->nL && M1->nC == M2->nC){	//Se ambas as matrizes tiverem a mesma quantidade de linhas e colunas ocorrerá a soma
		MATRIZ_ESPARSA *soma = criar_matriz(M1->nL, M2->nC); //Será criada uma matriz com a mesma quantidade de linhas e de colunas das anteriores
        for(k=1; k<=M1->nL;k++){
            for(h=1;h<=M1->nC;h++){	
                if(get_matriz(M1,k,h)!=0 || get_matriz(M2,k,h)!= 0 || get_matriz(M1,k,h) != - get_matriz(M2,k,h)){ //Testa de os elementos não são opostos, ou se tem algum não nulo
					if(get_matriz(M1,k,h) == 0) //A soma será M2, se M1 for nulo
						set_matriz(soma,k,h,get_matriz(M2,k,h));
					else if(get_matriz(M2,k,h) == 0) //A soma será M1, se M2 for nulo
						set_matriz(soma,k,h,get_matriz(M1,k,h));
					else //Se ambos forem não nulos a soma é feita
						set_matriz(soma,k,h,(get_matriz(M1,k,h) + get_matriz(M2,k,h)));
				}	
			}
		}
		return soma;
	}
	else return NULL; //Se as matrizes tiverem quantidades distintas de linhas e colunas não há como somar
}  

MATRIZ_ESPARSA  *multiplicar_matriz( MATRIZ_ESPARSA *M1, MATRIZ_ESPARSA *M2){
    int k,h,f;
	double aux = 0;
	if(M1->nC == M2->nL){	//Se as matrizes tiverem a mesma quantidade de linhas e colunas ocorre a operação
		MATRIZ_ESPARSA *transpostaM2 = transposta_matriz(M2); //Transpõe a matriz M2 de forma que apenas seja necessário multiplicar linhas
		MATRIZ_ESPARSA *produto = criar_matriz(M1->nL, M2->nC);	//Cria a matriz resultante, com o mesmo número de linhas de M1 e a mesma quantidade de colunas de M2 
        for (h=1;h<=M1->nL;h++){	//Percorre as linhas de M1
            for(f= 1;f<= transpostaM2->nL;f++){ //Percorre as linhas da transposta de M2
                for (k=1;k<=M1->nC;k++){ //Percorre as linhas de M1 e da transposta de M2
					aux += ((get_matriz(M1,h,k))*(get_matriz(transpostaM2,f,k))); //Soma o produto das linhas
				}
				set_matriz(produto,h,f, aux); //Cria o elemento de linha k e coluna f
				aux = 0;
			}
		}
		return produto;		
	}
	else return NULL;
}

void print_matriz(MATRIZ_ESPARSA  *matriz){
	int k,h;
    if (matriz != NULL){ 	//Verifica se o endereço indicado é válido
        for (k=1; k<=matriz->nL; k++){	//Percorre as linhas
            for (h=1; h<=matriz->nC; h++){	//Percorre as colunas
                printf("%.2lf  ", get_matriz(matriz,k,h));	//Imprime os elementos
            }
            printf("\n");
        }
    }
}
	
MATRIZ_ESPARSA *transposta_matriz(MATRIZ_ESPARSA  *matriz){
	MATRIZ_ESPARSA *transposta = criar_matriz( matriz->nC, matriz->nL); //Cria uma matriz com a mesma quantidade de linhas e colunas da matriz enviada como parâmetro
	 int k,h;
        for(k=1; k<=matriz->nL; k++){	//Percorre as linhas
           for(h=1; h<=matriz->nC; h++){	//Percorre as colunas
		      set_matriz(transposta,h,k,get_matriz(matriz,k,h)); //Coloca o elemento na posição transposta
		   }
		}
	return transposta;
}

void resumo_matriz(MATRIZ_ESPARSA *matriz){
	int h, k;
    if (matriz != NULL){    //Verifica se o endereço indicado é válido
        for (h=1; h<=matriz->nL; h++){  //Percorre as linhas
            for (k=1; k<=matriz->nC; k++){  //Percorre as colunas
                if (get_matriz(matriz,h,k) !=0)
                printf("%.2lf  ", get_matriz(matriz,h,k));  //Imprime os elementos não nulos
	        }
  
		}         
		printf("\n");
	}
}

MATRIZ_ESPARSA *ler_matriz(char array[50]){ // Recebe como parâmetro um arquivo cujo nome tenha até 50 caracteres
    int k,h,l,c;
    double aux;
    FILE *file; //Cria um ponteiro para o arquivo
    file = fopen(array, "r");   //Abre o arquivo em modo de leitura 
    fscanf(file, "%d %d", &k, &h);  //Armazena a quantidade de linhas e colunas
    MATRIZ_ESPARSA *matriz = criar_matriz (k,h);    //Usa os valores armazenados para criar uma matriz
    for(l=1; l<=h; l++){    //Percorre as linhas dos valores dos elementos presentes no arquivo
        for (c=1; c<=h; c++){   //Percorre as colunas dos valores dos elementos presentes no arquivo
            fscanf(file ," %lf ",&aux); //Armazena os valores
            set_matriz (matriz,l,c,aux);    //Insere os valores na matriz
        }
    }
            return matriz;
}



float determinante_matriz(MATRIZ_ESPARSA *matriz){
	float b;
	
	// Triangularizar a matriz
	b = triangularizar_matriz(matriz);
	
	// Calculo do determinante
	if(calculo_determinante(matriz) == -0.0)
		return 0;
	else
 		return b*calculo_determinante(matriz);
	
}

int triangularizar_matriz(MATRIZ_ESPARSA *matriz){
	int w,j,b,k,i,h,l,contador;
	float x,z,n,m,y,aux;
	contador = 0;
	for(b=1; b<matriz->nL; b++){ 
		x = get_matriz(matriz,b,b); //Armazena os elementos da diagonal principal na variável x
		for(l=(b+1),h=1; l<=matriz->nL; l++,h++){   //Percorre os elementos nas linhas a baixo do x
			x = get_matriz(matriz,b,b);
			if(x == 0){ //Se o elemento da diagonal principal for zero, será necessário efetuar trocas de linhas
				for(j=b; j<=matriz->nL; j++){
			 		if(get_matriz(matriz,j,b) != 0){    //Verifica uma linha que possa ser trocada					
						for(w=1; w<=matriz->nC; w++){
                            //Efetua a troca de linhas
							aux = get_matriz(matriz,b,w);
							z = get_matriz(matriz,j,w);
							set_matriz(matriz,b,w,z);
							set_matriz(matriz,j,w,aux);	
						}
						contador = contador + 1;
					}
				}
			}
			
            //Zera os elementos que estão a baixo do x e na mesma coluna dele
			y = (-(get_matriz(matriz,l,b)) / (get_matriz(matriz,l-h,b)));
			for(i=b; i<=matriz->nC; i++){
				aux = get_matriz(matriz,l,i) + (y*(get_matriz(matriz,l-h,i)));				
				set_matriz(matriz,l,i,aux);
			}
		}
	}
	b = contador%2; //Verifica quantas vezes as linhas foram trocadas
	if(b == 0)
		return 1;
	else 
		return -1;
}
	
float calculo_determinante(MATRIZ_ESPARSA *matriz){
	int b;
	float x,aux;
	aux = 1.0;
	for(b=1; b<=matriz->nL; b++){ 
        //Multiplica os elementos da diagonal principal para calcular o determinante
		x = get_matriz(matriz,b,b);	
		aux = aux*x;
	}
   	 //Garante que não será retornado um valor inválido(nan) como determinante
	if(aux <= 0)
		return aux;   
	else if(aux > 0)
		return aux;
	else  
		return 0;
}

