#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>


//structs
typedef struct dicotomia{
    float erro;
    float eq;
    float min;
    float max;
    int nTermos;
    int k;
}dicotomia;

typedef struct lagrange{
    float fx;
    float px;
    float Lx;
    float x;
    int nTermos;
}lagrange;

typedef struct trapezios{
	int n;
	float a;
    float b;
    float h;
    float it;
    float x;
    float fx;
}trapezios; 

//prot�tipo de fun��es utilizadas pelo programa em geral
void cabecalho();
void menu(dicotomia *p, lagrange *q, trapezios *z);
void alocaFormula(float **p, int tam);

//prot�tipo de fun��es a serem utilizadas para dicotomia
float calculaDicotomia(dicotomia *p, float ponto);
void recebeDadosD(dicotomia *p);
void alocaDicotomia(dicotomia **p, int tam);

//prot�tipo de fun��es a serem utilizadas para lagrange
float calculaLagrange(lagrange *q);
float calcularLx(int n, int k, float val, lagrange *q);
void recebeDadosL(lagrange *q);
void alocaLagrange(lagrange **q, int tam);

//prot�tipo de fun��es a serem utilizadas para trapezios
void alocaTrapezios(trapezios **p, int tam);
void recebeDadosT(trapezios *p);
void calculaTrapezios(trapezios *info, float *formula, int grau);


//main
int main(void){
    setlocale(LC_ALL, "portuguese");
    
	cabecalho();
    
	dicotomia *dicotomia = NULL;
    alocaDicotomia(&dicotomia, 1);
    
	lagrange *lagrange = NULL;
    alocaLagrange(&lagrange, 1);
    
    trapezios *trapezios = NULL;
    alocaTrapezios(&trapezios, 1);
    
	menu(dicotomia, lagrange, trapezios);

    return 0;
}


//rotina para aloca��o dos dados/struct dicotomia
void alocaDicotomia(dicotomia **p, int tam){
    if ((*p = (dicotomia *)realloc(*p, tam * sizeof(dicotomia))) == NULL){
        printf("Erro ao alocar. \nEncerrando...");
        exit(1);
    }
}
//rotina para aloca��o dos dados/struct lagrange
void alocaLagrange(lagrange **q, int tam){
    if ((*q = (lagrange *)realloc(*q, tam * sizeof(lagrange))) == NULL){
        printf("Erro ao alocar. \nEncerrando...");
        exit(1);
    }
}

//rotina para aloca��o dos dados/struct trapezios
void alocaTrapezios(trapezios **p, int tam){
    if ((*p = (trapezios *)realloc(*p, tam * sizeof(trapezios))) == NULL){
        printf("Erro ao alocar. \nEncerrando...");
        exit(1);
    }
}

void alocaFormula(float **p, int tam){
    if ((*p = (float *)realloc(*p, tam * sizeof(float))) == NULL){
        printf("Erro ao alocar. \nEncerrando...");
        exit(1);
    }
}

//rotina para receber os dados dicotomia
void recebeDadosD(dicotomia *p){
    int i    = 0,
        cont = 1,
        tam;
    float resultado;

    printf("\nDigite a quantidade de termos que a equa��o apresenta...: ");
    scanf("%i", &p->nTermos);
    fflush(stdin);

    printf("\nDigite o valor do ponto A...............................: ");
    scanf("%f", &p->min);
    fflush(stdin);

    printf("\nDigite o valor do ponto B...............................: ");
    scanf("%f", &p->max);
    fflush(stdin);

    printf("\nDigite o valor do erro..................................: ");
    scanf("%f", &p->erro);
    fflush(stdin);

//forma de receber o coeficiente e a pot�ncia em um array para que depois ele possa ser tratado e realizada a substitu�do em X para calcular as equa��es necess�rias no m�todo da dicotomia
    tam = p->nTermos * 2;

    for(i=0; i<tam; i+=2){
        printf("\nDigite o coeficiente da equa��o e o grau dele........: ", cont);
        scanf("%f %f", &(p+i)->eq, &(p+i+1)->eq);
		//scanf("%f %f", &p->eq[i], &p->eq[i+1]);
        fflush(stdin);

        cont++;
    }
    	
//chamada da fun��o que come�ar� a dicotomia para o primeiro intervalo
calculaDicotomia(p, p->min);
}

//rotina para receber os dados lagrange
void recebeDadosL(lagrange *q){
    int i    = 0,
        cont = 1,
        tam;
    float resultado;

    printf("\nDigite a quantidade de termos que a equa��o apresenta...: ");
    scanf("%i", &q->nTermos);
    tam = q->nTermos;
    fflush(stdin);

    printf("\nDigite os valores dos pontos x...............................: \n\n");
    for (i = 0; i < tam; i++) {
            printf("x (%d): ", i);
            scanf("%f", &(q+i)->x);
            printf("------\n");
        }
    fflush(stdin);

    printf("\nDigite os valores dos pontos fx...............................: \n\n");
    for (i = 0; i < tam; i++) {
            printf("fx (%d): ", i);
            scanf("%f", &(q+i)->fx);
            printf("------\n");
        }
    fflush(stdin);
    
    //chamada da fun��o que calcular� o lagrange
    calculaLagrange(q);
}

//rotina para receber os dados Trapezios
void recebeDadosT(trapezios *p){
	int grau,
		i=0;
	
	float *formula = NULL;
	
	char op, op1, op2;
	
	printf("\nDigite o grau do polin�mio: ");
    scanf("%d", &grau);
	
	alocaFormula(&formula, grau);
	
	printf("\n");
	
    for(i=0;i<=grau;i++){
        printf("%c^%d", i+97, grau-i);

        if (i != (grau)){
            printf(" + ");
        }
    }
    
    printf("\n\n");
    
    for (i=0; i<=grau; i++){
        printf("Insira o valor de %c: ", i+97);
        fflush(stdin);
        scanf("%f",(formula+i));
    }

    printf("\nIntervalo - Calculo h\n");
    printf("[a]: ");
	scanf("%f", &p->a);
    fflush(stdin);
    
	printf("[b]: ");
    scanf("%f", &p->b);
    fflush(stdin);

    if (p->b <= p->a){        
		float c;
        c = p->a;
        p->a = p->b;
        p->b = c;
        
     	printf("\nAten��o - Intervalo alterado: \nOrdem correta: [a]= %.3f e [b]= %.3f\n\n", p->a, p->b);
    }
    	
	//calculo trapezio
	do{
		printf("\nDigite o n�mero de divis�es do intervalo [%.3f, %.3f]: ", p->a, p->b);
    	scanf("%d", &p->n);
    	fflush(stdin);
    	
		calculaTrapezios(p, formula, grau);
		
		printf("\n\nDeseja calcular novamente com outra quantidade de trap�zios? (s/n) ");
		scanf("%c", &op);
		fflush(stdin);
		
	}while(op == 's' || op == 'S');
	fflush(stdin);
	
	
	printf("\nDeseja realizar outra integra��o num�rica? (s/n) ");
	scanf("%c", &op2);
	fflush(stdin);
	
	if(op2 == 's' || op2 == 'S'){
		recebeDadosT(p);
	}
	
//	printf("\n\nDeseja voltar ao menu inicial? <S/N> ");
//	scanf("%c", &op1);
//	fflush(stdin);
//	
//	if (op1 == 's' || op1 == 'S'){
//		//system("cls");
//		main();		
//	}
//	else{
//		exit(1);
//	}
}


//rotina para fazer o calculo do m�todo da dicotomia
float calculaDicotomia(dicotomia *p, float ponto){
	//rotina para calcular o k (n�mero de itera��es)
	int cont = 0;
	float k = (log10(p->max - p->min) - log10(p->erro))/log10(2),
		  rFB,
		  rFM,
		  fM;
	p->k = k;
	printf("\n k = %f", fabs(k));
	
	float somaA = 0,
		  somaB = 0,
		  somaM = 0;
	
	int i = 0,
	    tam;
	
	char op;
	
	tam = p->nTermos * 2;
	
	float a, b, m, fA, fB;
	   	a = p->min;
		b = p->max;
		m = (a + b)/2;
	do{
//garantir que durante o looping os valores das somas sejam inicialmente 0, pois sen�o correria o risco do valor da itera��o passada ser somado
		somaA = 0;
		somaB = 0;
		somaM = 0;

//logica utilizada para substituir os valores em X para a equa��o e ao mesmo tempo soma-los entre s�, garantindo que qualquer equa��o de qualquer grau possa ser utilizada
		for(i=0; i<tam; i+=2){
			somaA += ((p+i)->eq * pow(a, (p+i+1)->eq));
			somaB += ((p+i)->eq * pow(b, (p+i+1)->eq));
			somaM += ((p+i)->eq * pow(m, (p+i+1)->eq));
		}
		
//verifica��o para saber se existe raiz para o intervalo inserido
		if(cont == 1 && somaA * somaB > 0){
			printf("\nN�o � poss�vel realizar a dicotomia para o intervalo inserido, insira novos valores\n\n");
			main();
		}
	    
	    printf("\n\n ---------------------------------------------------------------------------------- "
		         "\n                                Tabela de Itera��es                                 "
		         "\n ---------------------------------------------------------------------------------- "
		         "\n     i     |     a     |     m     |     b     |   f(a)    |   f(m)    |   f(b)     "
		         "\n     %i    |  %.3f     |    %.3f  |    %.3f  |   %.3f  |    %.3f  |    %.3f   \n    "
		         "\n ---------------------------------------------------------------------------------- ", cont, a, m, b, somaA, somaM, somaB);
	    

//analisar qual ser� o novo ponto m�dio para a pr�xima itera��o
		if(somaA * somaM < 0){
	    	b = m;
	    	m = (a + b)/2;
		}
		else{
			a = m;
			m = (a + b)/2;
		}
		cont++;
	}while(cont<k);
	
	printf("\n\nO ponto procurado � %.3f pois seu resultado na itera��o � %.3f, ou seja menor que a margem de erro estipulado de %.2f", m, somaM, p->erro);
	
	printf("\n\nDeseja voltar ao menu inicial? <S/N> ");
	scanf("%c", &op);
	fflush(stdin);
	
	if (op != 's' && op != 'S')
		recebeDadosD(p);
	else{
		system("cls");
		main();
	}

	return m;
}

//rotina para fazer o calculo do m�todo de lagrange
float calculaLagrange(lagrange *q){
    int pc;
    char op1;
	char op2;
    int ai;

        do{
            ai = 0;
            do {
                printf("\nPor favor digite um ponto entre os valores de x a ser interpolado: \n");
				scanf("%i", &pc);
				fflush(stdin);
            }while(pc <= q->x && pc >= q->nTermos );
			
			float soma = 0;

            int i;
            for (i = 0; i < q->nTermos; i++)
            {
            (q+i)->Lx = calcularLx(q->nTermos, i, pc, q);
            soma += (q+i)->fx * (q+i)->Lx;
            printf("\n------\n\nL(%d) = %5.3lf\n", i, (q+i)->Lx);
            }

            printf("\nO resultado de f(%i) � igual a %f\n", pc, soma);
            printf("\nDeseja interpolar outro ponto? <S/N>\n\n");
			scanf("%c", &op1);
			fflush(stdin);
            if (op1 == 's' || op1 == 'S'){
		    	ai = 1;
	        }else{
                printf("\n\nDeseja voltar ao menu inicial? <S/N> ");
	            scanf("%c", &op2);
	            fflush(stdin);
	            if (op2 == 's' || op2 == 'S'){
					system("cls");
					main();
				}else{
		        system("pause");
		        exit(1);
	            }  
            };
	    }while(ai == 1);
};

//rotina para calcular Lx[i]
float calcularLx(int n, int k, float val, lagrange *q){
    int i;
    float resultado = 1;
    for (i = 0; i < n; i++)
    {
        if (i!=k)
            resultado = resultado * (float)((val - (q+i)->x) / ((q+k)->x - (q+i)->x));
    }
    return resultado;
};

//rotina para realizar calculo de trapezios
void calculaTrapezios(trapezios *info, float *formula, int grau){
	int i=0,
		j=0,
		cont = 0;
		
	float soma = 0,
		  fx,
		  z;
		
	float h = (info->b - info->a)/info->n;
	
	for (z=info->a; z<info->b; z+=h){
        fx = 0;
        for(j=0; j<=grau; j++){
            fx = fx + pow(z, (grau-j)) * (*(formula+j));
    	}

        (info+cont)->x = z;
        (info+cont)->fx = fx;
        cont++;
    }
    
    fx = 0;
    for(j=0; j<=grau; j++){
        fx = fx + pow(info->b, (grau-j)) * (*(formula+j));
    }

    (info+cont)->x = info->b;
    (info+cont)->fx = fx;

	if(info->n == 1){
		info->it = (h * (info->fx + (info+1)->fx)) / 2;
	}
	else{
		for(i=1; i<info->n; i++){
			soma += (info+i)->fx;
		}
		float aux = (info->fx) + ((info+info->n)->fx) + (2*soma);
		info->it = (h * aux)/2;
	}
	
	printf("\nh = (%.4f - %.4f) / %i", info->b, info->a, info->n);
	printf("\nh: %.4f", h);
	
	printf("\n\n\nTabela de Valores");
	
	printf("\n\n   x      f(x)");
	
	for(i=0;i<=info->n;i++){
		printf("\n%.4f   %.4f", (info+i)->x, (info+i)->fx);
	}
	
	printf("\n\nITR = %.4f", info->it);
}

//cabe�alho do programa
void cabecalho(){
	printf("\n\n**********************************************************************************************************************\n"
		   "\n* Este � um programa realizado pelos alunos Diogo, Vitor e Murilo para realiza��o do m�todo da Dicotomia e Lagrange! *\n"
		   "\n* Importante se atentar a forma correta de adicionar os n�meros da equa��o, onde por exemplo ax� + bx + c possu� 3  *\n"
		   "\n* termos e ser� referente a: primeira entrada = a 2, segunda entrada = b 1 e terceira entrada = c 0, j� que x^0=1.   *\n"
		   "\n* O teclado est� configurado para pt-BR, ent�o as entradas flutuantes ser�o com virgulas! Por exemplo: 2,3/ 1,2 e n�m *\n"
		   "\n* eros do g�nero. Aproveite!                                                                                         *\n"
		   "\n**********************************************************************************************************************\n\n");
}


//menu
void menu(dicotomia *p, lagrange *q, trapezios *z){
	int op;
	printf("\n-------------------------------Menu-------------------------------");
	printf("\nAplicar Dicotomia		[1]");
	printf("\nAplicar LaGrange		[2]");
	printf("\nAplicar Trapezios               [3]");
	printf("\nSair				[4]\n");
	printf("\nEscolha a sua op��o: ");
	scanf("%i", &op);
	switch(op){
		case 1: system("cls");
				recebeDadosD(p);
				break;
				
		case 2: system("cls");
				recebeDadosL(q);
				break;
		
		case 3: system("cls");
				recebeDadosT(z);
				break;
				
		case 4: exit(1);
				break;
		 
		default:system("clr");
				printf("Op��o inv�lida \n\n\n");
				main();
				break;
	}
}
