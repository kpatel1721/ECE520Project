#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc,char **argv){
    FILE *fisc,*fvec,*ffau,*fres; //file pointers used for .isc file, .vec file, .faults file and resultfile
    int Max,Opt,Npi,Npo,Total,Tfs; //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults
    NODE graph[Mnod]; //structure used to store the ckt information in .isc file 
    PATTERN vector[Mpt]; //structure used to store the input vectors information in .vec file 

    FAULT stuck[Mft]; //structure used to store the faults information in .faults file
    int a,b,c,d; //random variables

    //Read the .isc file and store the information in graph structure
    fisc=fopen(argv[1],"r"); //file pointer to open .isc file 
    Max=0; 
    Max=ReadIsc(fisc,graph); //read .isc file and return index of last node in graph formed
    fclose(fisc); //close file pointer for .isc file
    PrintCircuit(graph,Max); //print all members of graph structure


    int num_input = count_inputs(graph,Max); //count the number of inputs in the circuit

    int out_list[Mout]; //array to store the output node ids
    int num_output = count_outputs(graph,Max,out_list); //count the number of outputs in the circuit

    //Read the .vec file and store the information in  vector structure
    fvec = fopen(argv[2],"r"); 
    int pattern_idx = ReadVec(fvec,vector);
    fclose(fvec);

    //Read the .faults file and store the information in  stuck structure
    ffau = fopen(argv[3],"r");
    int fault_idx = ReadFault(ffau,stuck);
    fclose(ffau);

    //Print to result file while simulating
    fres = fopen(argv[4],"w"); // Open the file once before the loop
    for(a = 0; a < pattern_idx; a++){
        sim(graph,Max,num_input,vector,a); //simulate the circuit with the input vector
        out_write_sim(fres,graph,vector,out_list,num_output,num_input,a,Max); //write the output to the result file
        fault_sim(graph,Max,num_input,vector,a,stuck,fault_idx,fres); //simulate the circuit with the faults and write ouputs to the result file
    }
    fclose(fres); //Close the file after the loop
    ClearCircuit(graph,Mnod); //clear memeory for all members of graph
    return;
}//end of main
/****************************************************************************************************************************/

