#include "ev_chess.h"


#define DUMP(B) if (B!=NULL){free(B);B=NULL;}
       





int think (struct move *out, int PL, int DEEP, int verbose) {
    int i=0; int r=0;
    int last[2]={0,-32760};

    int bI = 0;
    
    time_t startT = time(NULL);

    struct board *_board = makeparallelboard(&board);  
    Vb printf("Master Address: %p\n", (void *)_board); 
    
    long Alpha = -1690000;
    long Beta = 1690000;
    
    struct movelist moves;
    legal_moves(_board, &moves,PL, 0); 
    
    reorder_movelist(&moves);
    
    
    Vb printf("thinking r:%i  k:%i DEEP:%i.\n",r,moves.k,DEEP);
    if (moves.k == 0) return -1;
    Vb printf("value of k is %i.\n",moves.k);
    
    
    
    for (i=0;i<moves.k;i++) {
    //infoMOVE = (char *)malloc(sizeof(char)*128);
        if(i==0)
           if(!canNullMove(DEEP, _board, moves.k, PL))
               continue;       
        
        
        //show_board(board.squares);
     Vb printf("new tree branching. i=%i\n",i);
     Vb print_movement(&moves.movements[i],0);
     printf("Alpha = %i\n", Alpha);
     
     move_pc(_board, &moves.movements[i]);    
     moves.movements[i].score = thinkiterate(_board, 1-PL, DEEP-1, 0,
             Alpha, Beta);
     
     if (moves.movements[i].score > Alpha) {
         Alpha = moves.movements[i].score;
         r=i;
     }
     
     //show_board(_board->squares);
     undo_move(_board, &moves.movements[i]); 

     
     Vb printf("analyzed i=%i; score is %li.\n",i, moves.movements[i].score);
     Vb if (show_info) printf(">>>>>>>>\n");
     if (show_info) eval_info_move(&moves.movements[i],DEEP, startT, PL);              
             
             

     }

    


    if (r==0) r++;
    replicate_move(out, &moves.movements[r]);
    print_movement(out,1);
   
   Vb printf("r = %i\n", r);

   DUMP(_board);
   return r;
   
}

long thinkiterate(struct board *feed, int PL, int DEEP, int verbose,
        long Alpha, long Beta) {

    int i=0, j=0, t=0, W=0, r=0;
    
    int enemy_score=0,machine_score=0;

    struct board *_board = makeparallelboard(feed);

    int ABcutoff = 0;

    //struct move result;
   
    long score=0;
    
    struct movelist moves;
    legal_moves(_board, &moves, PL, 0);
    
    Vb printf("DEEP = %i; K=%i Address: %p\n", DEEP,moves.k ,(void *)_board); 
    Vb printf("NullMove!\n"); 
    
     //If in checkmate/stalemate situation;
     if (moves.k == 1) {
         if (ifsquare_attacked(_board->squares, findking(_board->squares, 'Y', PL), 
                 findking(_board->squares, 'X', PL), PL, 0)) {
            score = 13000 - 50*(Brain.DEEP-DEEP); 
            if (PL == machineplays) score = -score;
         }
          else score = 0;
        
       Vb printf("check/stalemate.\n");
       return score;
         
     }
    
    


    Vb show_board(_board->squares);
   if (DEEP>0) {
    reorder_movelist(&moves); 
    
   //Movelist iteration.    
   for(i=0;i<moves.k;i++) {
       //NULL MOVE: guaranteed as long as if PL is not in check,
       //and its not K+P endgame.
       if(i==0)
           if(!canNullMove(DEEP, _board, moves.k, PL))
               continue;
       
       move_pc(_board, &moves.movements[i]);  

       moves.movements[i].score = thinkiterate(_board, 1-PL, DEEP-1, verbose, 
               Alpha, Beta);
       
       //eval_info_move(&_board->movelist[i],DEEP, PL);
       //show_board(_board->squares);
       undo_move(_board, &moves.movements[i]);
       
       //if (show_info) eval_info_move(&_board->movelist[i],DEEP, PL);  
       
       if (PL==machineplays) 
           if (moves.movements[i].score > Alpha) {
               //printf("*.\n");
               Alpha = moves.movements[i].score; r=i;
           
           if(Beta<=Alpha) ABcutoff=1;}

       if (PL!=machineplays)
            if (moves.movements[i].score < Beta) {
               //printf("*.\n");
               Beta = moves.movements[i].score; r=i;
            
            if(Beta<=Alpha) ABcutoff=1;
            }
              
        if (ABcutoff) {
            score = moves.movements[i].score;

            return score;
        }
     }
     if (r==0)r++;  
     score = moves.movements[r].score;  
     return score;       
   }
     
     else {
     machine_score = evaluate(_board, &moves, machineplays);
     enemy_score = evaluate(_board, &moves, 1-machineplays);
             
     score = machine_score - enemy_score * Brain.presumeOPPaggro;
     //printf("S=%i\n", score);
     //printf(">>>>>>%i.\n", PL);
    //printf("score of %i //     machine = %i     enemy = %i;\n", score, 
   //machine_score, enemy_score);
     Vb if (show_info) eval_info_move(&moves.movements[i],DEEP, 0, PL);  
     

     return score;
     }

}

int evaluate(struct board *evalboard, struct movelist *moves, int PL) {
    int score = 0;
    
    int i = 0;
    int j = 0;
    int L=0,Z=0,K=0;
    
    int chaos = rand() % (int)(Brain.randomness);   
  
    int deadpiece = 0;
    int parallelatks = 0;
    int paralleldefenders = 0;
    
    attackers_defenders(evalboard->squares, *moves, PL);

    
    forsquares {
        if (evalboard->squares[i][j] == 'x') continue;
        L = getindex(evalboard->squares[i][j],pieces[PL],6);
        if (L<0) continue;
        K = Brain.pvalues[L];
        
        if (L==0) {
         if (PL) K= K + i * Brain.pawnrankMOD;
         else K = K + (7-i) * Brain.pawnrankMOD;
        }  
            
            
   
     score = score + K * Brain.seekpieces +
        ((-power(j,2)+7*j-5)+(-power(i,2)+7*i-5))*Brain.seekmiddle;
            

}
        
        
        
     for (Z=0;Z<moves->kad;Z++) {
        L= getindex(moves->defenders[Z][0],pieces[1-PL],6); 
        
        parallelatks = ifsquare_attacked
        (evalboard->squares,moves->defenders[Z][1],
                            moves->defenders[Z][2], 1-PL,0);
                
     if (L==5 && Brain.parallelcheck) {
          if (parallelatks>1) 
            score = score + (parallelatks) * Brain.parallelcheck;}
     else {
        paralleldefenders = ifsquare_attacked
        (evalboard->squares,moves->defenders[Z][1],
                            moves->defenders[Z][2], PL,0);
        
        score = score - paralleldefenders * Brain.MODbackup;
         
         
     }       
                
                
         score = score + Brain.pvalues[L]*Brain.seekatk;
         
         L = getindex(moves->attackers[Z][0],pieces[PL],6);
         score = score - Brain.pvalues[L] * Brain.balanceoffense;
         
         
         }
    score = score+chaos;       
    score = score + moves->k * Brain.MODmobility;
    
    
    //printf("score: %i\n", score);
    return score;
    
}

float scoremod (int DEEP, int method) {
    
    float modifier = 0;
    float helper = 0;
    
    if (method>3) method = 0;
    
    
    if (method == 0) modifier = 1;
    
    if (method == 1) modifier = 2*((DEEP+Brain.deviationcalc)/Brain.DEEP);
    
    if (method == 2) {
        modifier = -power(DEEP,2)+Brain.DEEP*DEEP+2*Brain.DEEP;
        

    helper = Brain.DEEP/2;
        
            helper = -power(helper,2)+Brain.DEEP*helper+2*Brain.DEEP;
            
            modifier = modifier/(helper/1.1);
    }
    
    
    if (method == 3) {

        modifier = Brain.TIMEweight[(int)(Brain.DEEP-DEEP)];
        
    }
    
    
    
    
    return modifier;
}


int canNullMove (int DEEP, struct board *board, int K, int P) {
    int i=0,j=0,NullMove=0;
           if (DEEP>Brain.DEEP-2&&K>5) 
       forsquares {
               if (board->squares[i][j]!='x'&& 
                   board->squares[i][j]!=pieces[P][0]&&
                   !is_in(board->squares[i][j],pieces[1-P],6)) NullMove = 1;
               if (board->squares[i][j]==pieces[P][5])
                   if (ifsquare_attacked (board->squares,i,j,P,0)){
                        NullMove=0;break;break;
                   }
       }
        return  NullMove;       
               
}