# STR-Projeto-2
Executivo cíclico

## Objetivo
O executivo cíclico é uma técnica comumente utilizada para coordenar a execução de tarefas periódicas, garantindo a sincronização entre elas e otimizando o uso dos recursos do sistema.
O objetivo principal deste projeto é fornecer uma implementação prática de um executivo cíclico em linguagem C, abordando os seguintes aspectos:

Cálculo dos Tempos de Ciclo:
- Determinação do Tempo de Ciclo Primário, representando o ciclo maior que sincroniza todas as tarefas em sua periodicidade.
- Cálculo do Tempo de Ciclo Secundário, representando o menor ciclo possível que ainda respeita as periodicidades das tarefas.
  
Aplicação de Heurísticas de Escalonamento:
- Utilização de heurísticas como Menor Tempo de Execução Primeiro ou Maior Taxa de Periodicidade Primeiro para sugerir um escalonamento que otimize a execução das tarefas dentro dos ciclos determinados.

## Implementação
O sistema compreende as seguintes etapas:

- Leitura de dados de entrada: Os detalhes das tarefas a serem escalonadas são lidos a partir de um arquivo JSON.
- Cálculo dos tempos de ciclo: Os tempos de ciclo primário e secundário são calculados com base nos períodos das tarefas.
- Ordenação das tarefas: As tarefas são ordenadas com base no menor tempo de execução, conforme a heurística SETF.
- Escalonamento das tarefas: O escalonamento das tarefas é realizado dentro dos ciclos primários, seguindo a ordem definida pela heurística SETF.

## Código
- Incluimos as bibliotecas necessárias para o programa, sendo <stdio.h> para operações de entrada e saída padrão e <json-c/json.h> para manipulação de JSON em C.
```c
#include <stdio.h>
#include <json-c/json.h>
```

- Definimos uma estrutura Tarefa para armazenar os detalhes de cada tarefa, incluindo sua prioridade, período, tempo de execução, identificador (id) e o próximo momento de execução.
```c
struct Tarefa{
    int prioridade;
    int periodo;
    int tempo_execucao;
    const char *id;
    int prox_execucao;
};
```

- Definimos uma função mdc (Máximo Divisor Comum) que calcula o máximo divisor comum entre dois números usando o algoritmo de Euclides.
```c
int mdc(int a, int b){
    while (b != 0){
        int temp = b;
        b = a % b;
        a = temp;    
    }
    return a;
}
```

- Definimos uma função mdc_lista que calcula o máximo divisor comum de uma lista de números, utilizando a função mdc.
```c
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
```

- Definimos uma função mmc (Mínimo Múltiplo Comum) que calcula o mínimo múltiplo comum entre dois números usando a relação entre o MMC e o MDC.
```c
int mmc(int a, int b) {
    return (a * b) / mdc(a, b);
}
```

- Definimos uma função mmc_lista que calcula o mínimo múltiplo comum de uma lista de números, utilizando a função mmc.
```c
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
```
- Definimos uma função ordenar_tarefas que ordena um array de tarefas com base no tempo de execução.
```c
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
```

- Definimos uma função para o escalonamento SETF
```c
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
```

## Main()

Nela, são realizadas diversas operações que incluem a leitura de um arquivo JSON contendo informações sobre tarefas, o cálculo do Mínimo Múltiplo Comum (MMC) e do Máximo Divisor Comum (MDC), a ordenação das tarefas com base no tempo de execução e o escalonamento das tarefas usando a heurística SETF.

### Leitura do Arquivo JSON

- A função começa abrindo o arquivo "tarefas.json" no modo de leitura. Se o arquivo não puder ser aberto, uma mensagem de erro será exibida.

```c
FILE *fp;
fp = fopen("tarefas.json","r");
if (fp == NULL){
    fprintf(stderr,"Não foi possível abrir o arquivo.\n");
    return 1;
}
```

- Em seguida, os dados do arquivo são lidos para um buffer e depois parseados como um objeto JSON.
```c
char buffer[1024];
struct json_object *parsed_json;
fread(buffer, 1024,1, fp);
fclose(fp);
parsed_json = json_tokener_parse(buffer);
```

- O código então extrai a lista de tarefas do objeto JSON e itera sobre cada tarefa, armazenando suas informações em uma estrutura de dados.
```c
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
```

- Depois que todas as tarefas são lidas e armazenadas, o código calcula o MMC e o MDC dos períodos das tarefas.
```c
int mmc, mdc;
int numeros[n_tarefas];
// Calcula o MMC e o MDC
mmc = mmc_lista(numeros, n_tarefas);
mdc = mdc_lista(numeros, n_tarefas);
```

- Após processar as informações das tarefas, o programa mostra o resultado do cálculo de ciclos para o executivo cíclico, tempo de ciclo primário e tempo de ciclo secundário.
```c
    printf("Cálculo de Ciclos para o Executivo Cíclico:\n");
    printf("-------------------------------------------\n");
    printf("Tempo de Ciclo Primário: %d unidades de tempo\n", mmc);
    printf("Tempo de Ciclo Secundário: %d unidades de tempo\n", mdc);
```

- Chama a função para alterar a ordem das tarefas deixando-a com menos tempo na posição 0 e assim por diante.
```c
ordenar_tarefas(&tarefa_c, n_tarefas);
```

- Chama a função para realizar o escalonamento por Shortest Execution Time First.
```c
escalonamento_SETF(&tarefa_c, n_tarefas,mmc,mdc);
```

- Por fim, mostramos um resumo.
```c
    printf("\nResumo:\n");
    printf("-------\n");
    printf("Total de ciclos: %d\n",mmc/mdc);
    printf("Utilização da CPU: %.2f %\n",cpu*100);
    
    return 0;
```
