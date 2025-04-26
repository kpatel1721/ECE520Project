#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc,char **argv){
    FILE *fisc,*fvec,*ffau,*fres;             //file pointers used for .isc file, .vec file, .faults file and resultfile
    int Max,Opt,Npi,Npo,Total,Tfs;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults
    NODE graph[Mnod];                         //structure used to store the ckt information in .isc file 
    PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file 

    FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
    int a,b,c,d;                             //random variables


    //Read the .isc file and store the information in graph structure
    fisc=fopen(argv[1],"r");                           //file pointer to open .isc file 
    Max=0; 
    Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
    fclose(fisc);                            //close file pointer for .isc file
    PrintCircuit(graph,Max);                 //print all members of graph structure


    int num_input = count_inputs(graph,Max);

    int out_list[Mout];
    int num_output = count_outputs(graph,Max,out_list);

    //Read the .vec file and store the information in  vector structure
    fvec = fopen(argv[2],"r"); 
    int pattern_idx = ReadVec(fvec,vector);
    fclose(fvec);

    ffau = fopen(argv[3],"r");
    int fault_idx = ReadFault(ffau,stuck);
    fclose(ffau);

    //insert all inputs in array
    fres = fopen(argv[4],"w"); // Open the file once before the loop
    for(a = 0; a < pattern_idx; a++){
        sim(graph,Max,num_input,vector,a);
        out_write_sim(fres,graph,vector,out_list,num_output,num_input,a,Max);
        fault_sim(graph,Max,num_input,vector,a,stuck,fault_idx,fres);
    }
    fclose(fres); // Close the file after the loop
    ClearCircuit(graph,Mnod);                                      //clear memeory for all members of graph
    return;
}//end of main
/****************************************************************************************************************************/

