#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
using namespace std;
#define L -1 //left 
#define R 1 //right
#define S 0 //stay on that state

#define bn 10    //initialise tape with 10 blanks at both ends
#define maxsize 100
int B;
/*Blank is assigned a symbol(integer) not in the given alphabet*/
int tape_head;
int trans_order[maxsize];/*stores the correct option to be chosen in case of a non deterministic machine*/



typedef struct table_input      //to hold a particular input for tranisiton
{
    int new_state;             //and output as a result of transition
    int new_symbol;
    int direction;
}table_input;


// this is fun for writing tape contents after every step in a output file 
void print_order(vector<vector<vector<table_input>>>transition_table,vector<int>tape)
{
    ofstream fout;       //file handling
    fout.open("./out.txt", ios::app);
    vector<int>temp;   //to make copy of tape so that changes can be done in copy for printing
    temp=tape;

    int i=0,head=bn,state=0;
    //head is index in tape vector
    //printing orignal tape
    for (int j=0; j<temp.size(); j++)  //initialse head with bn=10 as 10 blanks are there in starting
    {
        if (temp[j]==B)
        {
            fout<<"B";
        }else
        {
            fout<<temp[j];
        }
        
    } fout<<endl;
//printing tape after each modification
    while(trans_order[i]!=-1)        //executes till final state
    {
        int symbol=temp[head];       //storing current symbol
        temp[head]=transition_table[state][symbol][trans_order[i]].new_symbol;       //changed to new symbol
        for (int j=0; j<temp.size(); j++)
        {
            if (temp[j]==B)
            {
                fout<<"B";
            }else
            {
                fout<<temp[j];
            }
        
        }
        fout<<endl;
        head=transition_table[state][symbol][trans_order[i]].direction+head;
        //changing the index (leftwardsor rightwardsor stay)
        state=transition_table[state][symbol][trans_order[i]].new_state;
        //according to transition table
        i++;
        
    }
    fout.close();
    temp.clear();
}

//to handle multiple paths for same symbol we have to backtrack
int backtrack(int head,int order_pos,int state, int symbol,vector<vector<vector<table_input>>>transition_table,vector<int>tape,int fstate,vector<int>original_tape)
{
    int status=0;//denotes acceptance
    // Base case:if final state return 1
    if (state==fstate)
    {
        status=1;
        trans_order[order_pos]=-1;
        
    }else
    {
        for (int i=0; i<transition_table[state][symbol].size(); i++)
        {
            tape[head]=transition_table[state][symbol][i].new_symbol;
            /*now checking if selected option can lead to the final state*/
            if (backtrack(head+transition_table[state][symbol][i].direction, order_pos+1, transition_table[state][symbol][i].new_state, tape[head+transition_table[state][symbol][i].direction], transition_table, tape, fstate,original_tape)==1)
            {
                status=1;
                trans_order[order_pos]=i;
                break;
              
            }else
            {
                tape[head]=symbol;
            }
        }
    }
    
    
    
    return status;
    
}



int main(int argc, const char * argv[])
{
    int num_states;            //no. of states
    int num_symbols;           //including blank
    table_input temp;         //temporary structure node
    temp.new_state = -1;     
    ifstream fin;              
    
    fin.open("./input");
    if(!fin)
    {
        cout<<"Cannot Open the file\n";
    }
    else
    {
        string str;              //read no. of states and symbols to initialise 2-d vector array of vectors
        getline(fin,str);
        num_states=str[0]-'0';
        num_symbols=str[2]-'0';
        B=num_symbols-1;
        
                                    
        vector< vector< vector< table_input > > >transition_table( num_states , vector< vector<table_input> >(num_symbols));
        /*this is basically vector array having rows(index corresponding to sates) and columns(index corresponding to symbols) having vectors in each cell for transition from that state(row) for that symbol(column)*/

        ofstream fout;
        fout.open("./out.txt");
       /*input in format [current_state][symbol_at_head]-[new_state][new_symbol][direction]*/
        while (!fin.eof())
        {
            getline(fin,str);
            if (str=="end")
            {
                break;
            }
            int curstate=str[0]-'0';
            int cursymbol=str[1]-'0';;
            if (str[1]=='B')
            {
                cursymbol=B;
            }
           
            int nstate=str[3]-'0';
            int nsymbol=str[4]-'0';
            if (str[4]=='B')
            {
                nsymbol=B;
            }
            int dir;
            if (str[5]=='R')
            {
                dir=1;
            }else if (str[5]=='L')
            {
                dir=-1;
            }else
            {
                dir=0;
            }
            temp.direction=dir;
            temp.new_state=nstate;
            temp.new_symbol=nsymbol;
            transition_table[curstate][cursymbol].push_back(temp);
            fout<<curstate<<cursymbol<<"->"<<nstate<<nsymbol<<dir<<endl;
        }
        fout.close();
        //one more file for string
        
        int i,j;
        vector<int> tape,tape_copy;      //tape having blanks ...input string....blanks initially
        
        for(i=0;i<bn;i++)      //pushing blanks in tape initially
        {
            tape.push_back(B);
        }
        
        ifstream fis;
        fis.open("./inputstring");
        
        if(!fis)
        {
            cout<<"Cannot Open the file\n";
        }
        else
        {
            
            
            getline(fis,str);
            /* '/'denotes end of string in file*/
            for(i=0 ; str[i]!='/' ;i++)
            {
                tape.push_back(str[i]-'0');
            }
            
        }
        for(i=0;i<bn;i++)
        {
            tape.push_back(B);
        }
        
        tape_copy=tape;
        tape_head=bn;
        /* q0 is considered as input state and qn as final state */
        if (backtrack(tape_head, 0, 0, tape[tape_head], transition_table, tape, num_states-1, tape_copy)==1)
        {
            ofstream fout;
            fout.open("./out.txt", ios::app);
            fout<<"\n answer exists"<<endl;
            print_order(transition_table, tape_copy);
            fout.close();
            
        }else
        {
            ofstream fout;
            fout.open("./out.txt", ios::app);
            fout<<"given string not accepted by turing machine";
            fout.close();
        }
        
        
    }
    
    return 0;
}
