#include "graph.h"
int INPT_out[5] = {0,1,X,D,Db};
                    
int OR_out[5][5] = {{0,1,X,D,Db},
                    {1,1,1,1,1},
                    {X,1,X,X,X},
                    {D,1,X,D,1},
                    {Db,1,X,1,Db}};

int AND_out[5][5] = {{0,0,0,0,0}, 
                     {0,1,X,D,D}, 
                     {0,X,X,X,X},
                     {0,D,X,D,0},
                     {0,D,X,0,Db}};

int XOR_out[5][5] = {{0,1,X,D,Db},
                     {1,0,X,D,Db},
                     {X,X,X,X,X},
                     {D,D,X,0,1},
                     {Db,Db,X,1,0}};

int NOT_out[5] = {1,0,X,Db,D};

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
                graph[i].Cval = OR_out[graph[i].Cval][graph[temp->id].Cval];
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
                graph[i].Cval = OR_out[graph[i].Cval][graph[temp->id].Cval];
                temp = temp->next;
            }
            graph[i].Cval = NOT_out[graph[i].Cval];
        }
        else if(graph[i].Type==XOR){
            LIST *temp = graph[i].Fin;
            graph[i].Cval = graph[temp->id].Cval;
            temp = temp->next;
            while(temp != NULL){
                graph[i].Cval = XOR_out[graph[i].Cval][graph[temp->id].Cval];
                temp = temp->next;
            }
        }
        else if(graph[i].Type == XNOR){
            LIST *temp = graph[i].Fin;
            graph[i].Cval = graph[temp->id].Cval;
            temp = temp->next;
            while(temp != NULL){
                graph[i].Cval = XOR_out[graph[i].Cval][graph[temp->id].Cval];
                temp = temp->next;
            }
            graph[i].Cval = NOT_out[graph[i].Cval];
        }
        else if(graph[i].Type == BUFF || graph[i].Type == FROM){
            LIST *temp = graph[i].Fin;
            graph[i].Cval = graph[temp->id].Cval;
            
        }
        else if(graph[i].Type == NOT){
            LIST *temp = graph[i].Fin;
            graph[i].Cval = NOT_out[graph[temp->id].Cval];
        }
    }
    //PrintCircuit(graph, Max);
    return;     
}

void out_write_sim(FILE* result, NODE* graph, PATTERN *vector, int out_list[],int num_output, int num_input, int current_idx, int max){
	int i;
    fprintf(result, "Inputs: ");
    for(i=0;i<num_input;i++){
        if(vector[current_idx].matrix[i] == 2){
            fprintf(result,"x");
        }
        else{
            fprintf(result,"%d",vector[current_idx].matrix[i]);
        }
    }
    fprintf(result, "\nOutputs: ");
    i = 0; 
    for(i=0;i<=max;i++){
        if(graph[i].Type!=0){
            if(graph[i].Po == 1){
		        if(graph[i].Cval != 2){
                    fprintf(result,"%d",graph[i].Cval);		
        	    }
		        else{
		            fprintf(result,"x");
 		        }
    	    }
    	}
    }
    return;
}

int ReadFault(FILE *ffau, FAULT *f){
    char input[Mlin];
    int idx = 0; 
    int cur_id;
    int cur_val;
    while(fgets(input, sizeof(input), ffau)){
	    sscanf(input,"%d/%d",&cur_id,&cur_val);
	    f[idx].id = cur_id;
	    f[idx].val = cur_val;
        idx++;
    }
    return idx;
}

void fault_sim(NODE *graph, int Max, int num_input, PATTERN *p, int cur_idx, FAULT *f, int fault_idx, FILE *fres){
    int i,j,k; 
    int input_count = 0;
    for(j = 0; j < fault_idx; j++){
        for(i=0;i<=Max;i++){
            if(graph[i].Type==0) {continue;}
            if(graph[i].Type == INPT){
                graph[i].Cval = p[cur_idx].matrix[input_count];
                input_count++;
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
                    graph[i].Cval = OR_out[graph[i].Cval][graph[temp->id].Cval];
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
                    graph[i].Cval = OR_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
                graph[i].Cval = NOT_out[graph[i].Cval];
            }
            else if(graph[i].Type==XOR){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = XOR_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
            }
            else if(graph[i].Type == XNOR){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
                temp = temp->next;
                while(temp != NULL){
                    graph[i].Cval = XOR_out[graph[i].Cval][graph[temp->id].Cval];
                    temp = temp->next;
                }
                graph[i].Cval = NOT_out[graph[i].Cval];
            }
            else if(graph[i].Type == BUFF || graph[i].Type == FROM){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = graph[temp->id].Cval;
            }
            else if(graph[i].Type == NOT){
                LIST *temp = graph[i].Fin;
                graph[i].Cval = NOT_out[graph[temp->id].Cval];
            }
            if(i == f[j].id){
                if((graph[i].Cval == 1) && (f[i].val == 0)){
                    graph[i].Cval = D;
                }
                else if((graph[i].Cval == 0) && (f[i].val == 1)){
                    graph[i].Cval = Db;
                }
            }
        }
        fprintf(fres, "\nFault %d/%d: ",f[j].id,f[j].val);
        i = 0;
        int detect = 0; 	
        for(i=0;i<=Max;i++){
            if(graph[i].Po != 1){
	    	    continue;
        	}
            if(graph[i].Cval == 2){
                fprintf(fres,"x");		
            }
            else if(graph[i].Cval == 3){
                fprintf(fres,"D");
		detect = 1;
            }
            else if(graph[i].Cval == 4){
                fprintf(fres,"Db");
		detect = 1;
            }
	    else{
	    	fprintf(fres,"%d",graph[i].Cval);
 	    }
        }
        if(detect == 1){
            fprintf(fres, "\nFault Detected");
        }
        else if(detect == 0){
            fprintf(fres, "\nFault Not Detected");
        }
    }
    fprintf(fres, "\n\n");
    return;     
}
