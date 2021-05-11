//Bibliotecas padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TAM_POPULACAO 10   //tamanio da populaçao
#define INICIO_EIXO_X -500 //inicio do eixo "X"
#define FIM_EIXO_X +500    //fim do eixo "X"
#define TAM_EIXO_X FIM_EIXO_X-INICIO_EIXO_X//tamanho do eixo x
#define TAXA_MUTACAO 0.02  //esta taxa de mutaçao é de tudo o eixo completo, não é de um gen
                           //neste caso a mutaçao é 2% e é fixo


int populacao[TAM_POPULACAO];//array que salva a populaçao
int fitness[TAM_POPULACAO];// array que salva o fitness de cada nova populaçao

int best_fitness;// melhor fitness do populacao en cada novo geraçao
int average_fitness;//media dos fitness do populacao en cada novo geraçao
int contador_geracoes=1 ;// conta o número atual de geraçoes, aumenta a cada nova geraçao,começa em 1

void inicializar_populacao(){
    srand(time(NULL));//geraçao da semente

    //geramos a primeira populaçao aleatoriamente entre 0 e 1000
    for(int i=0; i<TAM_POPULACAO;i++){
        populacao[i]= (rand()%(FIM_EIXO_X-INICIO_EIXO_X))+INICIO_EIXO_X;
    }
}

void avalicao(){
    FILE* meuarquivo;
    FILE* apenas_funcao;
    FILE* arquivo_gnuplot; //arquivo o qual tem o proceso de gnuplot
    if((meuarquivo=fopen("meuarquivo.txt","w"))!=NULL){
        
        for(int i=0; i<TAM_POPULACAO;i++){
            //criamos uma funçao qualquer pra prenchar o fitness
            fitness[i] = (2*cos(0.039*populacao[i]) + 5*sin(0.05*populacao[i]) + 0.5*cos(0.01*populacao[i]) + 10*sin(0.07*populacao[i]) + 5*sin(0.1*populacao[i]) + 5*sin(0.035*populacao[i]))*10+500;
            //fitness[i]= 1000-abs(populacao[i]);
            //escrevemos os datos por console
            printf("%d. x=%d \t y=%d\n",i+1,populacao[i],fitness[i]);
            fprintf(meuarquivo,"%d \t %d \n",populacao[i],fitness[i]);
        }
        fflush(meuarquivo);
        //arquivo_gnuplot=popen("gnuplot -persist","w"); --> com esto abrimos o gnuplo no modo persistente
        arquivo_gnuplot=popen("gnuplot","w");//comando <gnuplot> é escreto na terminal, ainda nao estamos no QT
        
        //agora recien escrevemos no QT pra poder garficar
        fprintf(arquivo_gnuplot,"set title 'Algoritmo Evolutivo\n set xlabel'populaçao'\n set ylabel 'fitness'\n");
        fprintf(arquivo_gnuplot,"set xrange[%d:%d]\n",INICIO_EIXO_X,FIM_EIXO_X);
        //fprintf(arquivo_gnuplot,"plot 'meuarquivo.txt' title 'Pontos da populaçao' with points, 1000-abs(x) title 'Funçao doida' with lines\nrefresh\n");
        fprintf(arquivo_gnuplot,"plot 'meuarquivo.txt' title 'Pontos da populaçao' with points, (2*cos(0.039*x) + 5*sin(0.05*x) + 0.5*cos(0.01*x) + 10*sin(0.07*x) + 5*sin(0.1*x) + 5*sin(0.035*x))*10+500 title 'Funçao doida' with lines\nrefresh\n");
        fprintf(arquivo_gnuplot,"pause mouse any\n");
        
        //fechamos os arquvios e o processo
        pclose(arquivo_gnuplot);
        fclose(meuarquivo);
    }
    else{
        printf("Nao pudimos abrir o aquivo chamado meuarquivo");
    }
}

void mutacao(int position_best_fitness){
    /*Recomenda-se a mutação de 1 gene para cada indivíduo, mas neste caso só temos um gene para 
    cada indivíduo, portanto, sempre faremos mutação nesse gene*/

    int neg_o_pos[2]={1,-1};
    //sempre temos que mutar todos os individuos
    for (int i = 0; i < TAM_POPULACAO; i++)
    {   if (position_best_fitness!=i)
        {
            //mutamos cada individuo
            populacao[i] = populacao[i] + ( neg_o_pos[rand()%2] * (rand()%TAM_EIXO_X) * TAXA_MUTACAO );

            //certificamo-nos de que a mutação não torna o indivíduo maior do que o eixo permitido
            if (populacao[i] > FIM_EIXO_X)
            {
                populacao[i] -=TAM_EIXO_X;
            }
            //certificamo-nos de que a mutação não torna o indivíduo menor do que o eixo permitido
            else if ( populacao[i] < INICIO_EIXO_X)
            {
                populacao[i] +=TAM_EIXO_X;
            }
        }
    }
}

void elitismo(){//o melhor transa com tudos

    int position_best_fitness=0;

    //procura o individuo com melhor fitness
    int maior=fitness[0];
    for (int i = 1; i < TAM_POPULACAO; i++)//Nao tem sentido comparar com fitness[0] asim que comença desde i=1
    {
        if (fitness[i]>maior)
        {
            maior=fitness[i];
            position_best_fitness=i;
        }
    }
    printf("best of all : %d %d \n",populacao[position_best_fitness],fitness[position_best_fitness]);

    //fazemos o crossover
    for (int i = 0; i <TAM_POPULACAO; i++)
    {
        /*uma vez que sempre pasa que" (x+x)/2 = x" entao o melhor fitness nao vai mudar,
          por tanto nao preciso proteger o melhor individuo */
        populacao[i] = (populacao[position_best_fitness] + populacao[i])/2;
    }
    
    //mutamos os individuos
    mutacao(position_best_fitness);
}

void selecao(){
    elitismo();
}

void rearranjo(){
    
}

int main(void){

    /*FILE* meuarquivo;
    FILE* gnuplot;
    //FILE* comandos;

    meuarquivo=fopen("meuarquivo.txt","w");
    //comandos=popen("export DISPLAY=localhost:0","w");
    //fclose(comandos);
    gnuplot=popen("gnuplot -persist","w");
    
    fprintf(meuarquivo,"1 1\n2 2\n3 3\n5 5\n8 8\n13 13\n");
    fflush(meuarquivo);

    fprintf(gnuplot,"set title 'Fibonacci e seno'\nset key above\nset xlabel 'Valores de x'\nset ylabel 'Valores de y'\n");
    fprintf(gnuplot,"plot 'meuarquivo.txt' title 'Fibo' with lines, sin(x) title 'Sen' with lines \n");

    pclose(gnuplot);*/
    
    printf("tamaño del eje X : %d \n",TAM_EIXO_X);
    inicializar_populacao();
    while (true)
    {   
        printf("Numero de geraçao : %d \n",contador_geracoes);     
        avalicao();
        selecao();
        //mutacao
        rearranjo();
    }

    return 0;
}