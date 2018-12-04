#ifndef MATRIZ_ESPARSA_H

    #define MATRIZ_ESPARSA_H

    typedef struct celula CELULA;

    typedef struct matriz_esparsa MATRIZ_ESPARSA;

    MATRIZ_ESPARSA *criar_matriz (int , int );

    int set_matriz(MATRIZ_ESPARSA *, int , int , double );

    double get_matriz(MATRIZ_ESPARSA *, int , int );

    void print_matriz(MATRIZ_ESPARSA  *);
	
	void apagar_matriz(MATRIZ_ESPARSA *);

	float determinante_matriz(MATRIZ_ESPARSA *);
		float calculo_determinante(MATRIZ_ESPARSA *);
		int triangularizar_matriz(MATRIZ_ESPARSA *);
	
	MATRIZ_ESPARSA *somar_matriz( MATRIZ_ESPARSA *,  MATRIZ_ESPARSA *);
	
	MATRIZ_ESPARSA *transposta_matriz(MATRIZ_ESPARSA  *);

	MATRIZ_ESPARSA  *multiplicar_matriz( MATRIZ_ESPARSA *, MATRIZ_ESPARSA *);
	
	void resumo_matriz(MATRIZ_ESPARSA *);
	
	MATRIZ_ESPARSA *ler_matriz(char array[50]);
#endif
