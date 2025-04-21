#include "graph.h"
int INPT_out[3] = {0,1,X};

int OR_out[3][3] = {{0,1,X},
                    {1,1,1},
                    {X,1,X}};

int AND_out[3][3] = {{0,0,0}, 
                     {0,1,X}, 
                     {0,X,X}};

int XOR_out[3][3] = {{0,1,X},
                     {1,0,X},
                     {X,X,X}};

int NOT_out[3] = {1,0,X};

int count_inputs(NODE *graph, int Max){
    int i; 
    int num_input = 0; 
    for(i=0;i<=Max;i++){
        if(graph[i].Type!=0){
            if(graph[i].Type == 1){num_input++;}
        }
    }
    return num_input; 
}

int count_outputs(NODE *graph, int Max, int out_list[]){
    int num_output = 0;
    int i = 0; 
    for(i=0;i<=Max;i++){
        if(graph[i].Type!=0){
            if(graph[i].Po == 1){
		        out_list[num_output] = i;
                num_output++;
                }
        }
    }
    return num_output; 
}

void sim(NODE *graph, int Max, int num_input, PATTERN *p, int cur_idx){
        int i,j; 
        int input_count = 0;
        for(i=0;i<=Max;i++){
            if(graph[i].Type==0) {continue;}
            if(graph[i].Type == INPT){
                graph[i].Cval = p[cur_idx].matrix[input_count];
                input_count++;
                printf("%d",graph[i].Cval);
            }
            else if(graph[i].Type==AND){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = AND_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
            }
            else if(graph[i].Type==OR){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = AND_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
            }
            else if(graph[i].Type == NAND){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = AND_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
                graph[i].Cval = NOT_out[graph[i].Cval];
            }
            else if(graph[i].Type == NOR){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = AND_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
                graph[i].Cval = NOT_out[graph[i].Cval];
            }
            else if(graph[i].Type==XOR){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = AND_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
            }
            else if(graph[i].Type == XNOR){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = AND_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
                graph[i].Cval = NOT_out[graph[i].Cval];
            }
            else if(graph[i].Type == BUFF){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                
            }
            else if(graph[i].Type == NOT){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = NOT_out[graph[temp->id].Cval];
            }
            else if(graph[i].Type == FROM){
                LIST *temp = graph[i].Fin;
                while(temp != NULL){
                    graph[i].Cval = graph[temp->id].Cval;
                    temp = temp->next;
                }
            }
        }
    PrintCircuit(graph, Max);
    return;     
}

void out_write(FILE* result, NODE* graph, PATTERN *vector, int out_list[],int num_output, int num_input, int current_idx){
	int i;
    fprintf(result, "Inputs: ");
    for(i=0;i<num_input;i++){
        fprintf(result,"%d ",vector[current_idx].matrix[i]);
    }
    fprintf(result, "Outputs: ");
    for(i = 0; i < num_output; i++){
        fprintf(result,"%d ",graph[out_list[i]].Cval);
    }
    fprintf(result, "\n");
    return;
}
