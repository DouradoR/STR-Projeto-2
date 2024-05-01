#include <stdio.h>
#include <json-c/json.h>

#define MAX_TAREFAS 100

// Estrutura para uma tarefa
struct Tarefa{
    int prioridade;
    int periodo;
    int tempo_execucao;
    const char *id;
    int prox_execucao;
};

typedef struct Tarefa Tarefa;

//Função geral para calculo MDC
int mdc(int a, int b){
    while (b != 0){
	int temp = b;
	b = a % b;
	a = temp;	
    }
    return a;
}

//Calcula o MDC para quantidades variadas de números
int mdc_lista(int numeros[], int tamanho) {
    if (tamanho < 1) {
        printf("A lista de números está vazia.\n");
        return -1;
    }

    int resultado = numeros[0];
    for (int i = 1; i < tamanho; i++) {
        resultado = mdc(resultado, numeros[i]);
    }
    return resultado;
}

//Função geral para cálculo MMC
int mmc(int a, int b) {
    return (a * b) / mdc(a, b);
}

//Calcula o MMC para quantidades variadas de números
int mmc_lista(int numeros[], int tamanho) {
    if (tamanho < 1) {
        printf("A lista de números está vazia.\n");
        return -1;
    }

    int resultado = numeros[0];
    for (int i = 1; i < tamanho; i++) {
        resultado = mmc(resultado, numeros[i]);
    }
    return resultado;
}

//Função para ordenar tarefas - menor tempo de execução 
void ordenar_tarefas( Tarefa *tarefa_c,int num_tarefas){
    int i,j;
    Tarefa temp;
    
    for (i = 0; i < num_tarefas - 1; i++){
        for(j = 0; j < num_tarefas - i - 1; j++){
            if(tarefa_c[j].tempo_execucao > tarefa_c[j+1].tempo_execucao){
              temp = tarefa_c[j];
              tarefa_c[j] = tarefa_c[j+1];
              tarefa_c[j+1] = temp;
            }
        }
    }
}

// Função escalonamento SETF
void escalonamento_SETF(Tarefa *tarefa_c, int num_tarefas, int T_primario, int T_secundario){
    int tempo = 0;
    int ciclo = 0;
    int chave = 0;

    
    printf("\nEscalonamento das tarefas ( SETF ):\n");
    printf("-------------------------------------------\n");
    
    while(tempo < T_primario){
        
        
        //Ciclo atual
        if(tempo%T_secundario == 0){
            printf("\nCiclo %d:\n",ciclo+1);
            ciclo++;
        }
        
        //Executa a tarefa seguindo a ordem caso já o tempo seja maior ou igual ao tempo de prox execucao
        //Após executar altera o tempo para a próxima execucao daquela tarefa fazendo tempo + periodo
        for (int i = 0; i < num_tarefas; i++) {
            
            chave = 0;
            
        
            if(tempo >= tarefa_c[i].prox_execucao){
                printf("- %s: Tempo de execução = %d, período = %d, prioridade = %d \n",
                tarefa_c[i].id, tarefa_c[i].tempo_execucao, tarefa_c[i].periodo, tarefa_c[i].prioridade);
                tarefa_c[i].prox_execucao = tempo + tarefa_c[i].periodo; 
                tempo += tarefa_c[i].tempo_execucao;
                if(tempo%T_secundario == 0){
                    chave = 1;
                    printf("break");
                    break;
                    
                    
                }
                chave = 1;
            }
        }
        if(chave == 0){
            tempo++;
            
        }
        
        
    }  
}

//MAIN
int main() {

    int mmc,mdc;
    float cpu;
    
    FILE *fp;
    
    char buffer[1024];
    
    struct json_object *parsed_json;
    struct json_object *tarefas;
    struct json_object *tarefa;
    struct json_object *id;
    struct json_object *periodo;
    struct json_object *tempo_execucao;
    struct json_object *prioridade;

    size_t n_tarefas;
    size_t i;

    fp = fopen("tarefas.json","r");
    if (fp == NULL){
        fprintf(stderr,"Não foi possível abrir arquivo.\n");
        return 1;
    }

    
    fread(buffer, 1024,1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json, "tarefas", &tarefas);
    n_tarefas = json_object_array_length(tarefas);

    printf("Número de tarefas: %lu\n", n_tarefas);
    
    //Apos saber o números de tarefas podemos criar n variáveis do tipo Tarefa//
    struct Tarefa tarefa_c[n_tarefas];
    int numeros[n_tarefas];
    
    //--------------------------------------------------------//
  
    
    for (i = 0; i < n_tarefas; i++){
        tarefa = json_object_array_get_idx(tarefas,i);

        json_object_object_get_ex(tarefa, "id", &id);
        json_object_object_get_ex(tarefa, "periodo", &periodo);
        json_object_object_get_ex(tarefa, "tempo_execucao", &tempo_execucao);
        json_object_object_get_ex(tarefa, "prioridade", &prioridade);

        //Armazenando os dados do arquivo json na struct criada
        tarefa_c[i].id = json_object_get_string(id);
        tarefa_c[i].periodo = json_object_get_int(periodo);
        tarefa_c[i].tempo_execucao = json_object_get_int(tempo_execucao);
        tarefa_c[i].prioridade = json_object_get_int(prioridade);
        tarefa_c[i].prox_execucao = 0;

        //Periodos para cálculo do mmc e mdc
        numeros[i] = tarefa_c[i].periodo;
        
        //Calcular o uso da CPU
        cpu+=( (float)json_object_get_int(tempo_execucao)/json_object_get_int(periodo));
        
        //Mostrando informações gerais de todas a tarefas que foram lidas
        printf("\nTarefa %lu:\n", i+1);
        printf(" ID: %s\n", json_object_get_string(id));
        printf(" Período: %d\n", json_object_get_int(periodo));
        printf(" Tempo de Execução: %d\n", json_object_get_int(tempo_execucao));
        printf(" Prioridade: %d\n", json_object_get_int(prioridade));
        
    }

    
    //Tempo de Ciclo Primário
    mmc = mmc_lista(numeros, n_tarefas);
    
    //Tempo de Ciclo Secundário
    mdc = mdc_lista(numeros, n_tarefas);
    
    
    //Mostrando resultados dos tempos
    printf("\nCálculo de Ciclos para o Executivo Cíclico:\n");
    printf("-------------------------------------------\n");
    printf("Tempo de Ciclo Primário: %d unidades de tempo\n", mmc);
    printf("Tempo de Ciclo Secundário: %d unidades de tempo\n", mdc);
    
    
    //Função para alterar a ordem das tarefas deixando a com menor tempo na posição 0 e assim por diante
    ordenar_tarefas(&tarefa_c, n_tarefas);
    
    //Função para realizar escalonamento por Shortest Execution Time First
    escalonamento_SETF(&tarefa_c, n_tarefas,mmc,mdc);
    
    //
    
    printf("\nResumo:\n");
    printf("-------\n");
    printf("Total de ciclos: %d\n",mmc/mdc);
    printf("Utilização da CPU: %.2f %\n",cpu*100);
    
    
    return 0;
}


        
        
   
