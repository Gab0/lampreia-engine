#include "ev_chess.h"

void cord2pos (char out[]) {
    char keymap[8] = {'a','b','c','d','e','f','g','h'};
    int i = out[0];
    int j = out[1];
    
    out[0] = keymap[j];
    out[1] = 8 - i + '0';
}
void pos2cord (char out[]) {
    char keymap[8] = {'a','b','c','d','e','f','g','h'};
   
    char let = out[0];
    char num = out[1];
    
    int i = 0;
    while ( i < 8 && keymap[i] != let ) i++;
    
    int j = num -'0';
    
    out[0] = 8 - j;
    out[1] = i;
}

int parse_move (struct move *target, char *s, int P) {

    
    if (strstr(s,"O-O-O") != NULL) {castle(&board, 1, 1-machineplays, 0); return 1;}
    if (strstr(s, "O-O") != NULL ) {castle(&board,1,1-machineplays,1); return 1;}
    
    if (!isalpha(s[0]) || !isalpha(s[2])) return 0;
    if (!isdigit(s[1]) || !isdigit(s[3])) return 0;
    if (s[0] < 'a' || s[0] > 'h' || s[2] < 'a' || s[2] > 'h') return 0;
    if (s[1] < '1' || s[1] > '8' || s[3] < '1' || s[3] > '8') return 0;
    
    
    
    else {
        target->from[0] = s[0];
        target->from[1] = s[1];
        target->to[0] = s[2];
        target->to[1] = s[3];

        pos2cord(target->from);
        pos2cord(target->to);           
        
        
        
        if(s[4]=='q') {
            if (s[1]<s[3])target->promoteto='Q';
            if (s[1]>s[3])target->promoteto='q';

        }
        
        else target->promoteto=0;
        
        if (target->from[1] == 4){
            if (target->from[0] == 0 || target->from[0] == 7)
            if (target->to[1] == 6 || target->to[1] <= 2){printf("castle? p=%i\n",board.castle[P][1]);
                if (board.castle[P][1]) {target->iscastle=1; printf("castle.\n");}}
        
        }
        

        return 1;
        }
    
}

bool is_in(char val, char arr[], int size){
    int i = 0;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

bool is_legal(struct move *play, int P) {
    int i = 0;
    for (i=0; i < board.k; i++) {
        if (comp_arr(board.movelist[i].from, play->from) &&
            comp_arr(board.movelist[i].to, play->to)) return true;
    
    }
    return false;
}

int append_move(struct board *board, int i,int j, int mod_i, int mod_j, int P) {
    
    
    
    
    if (i==16) {
        
        int I = 7 * (1-P);
        
        board->movelist[board->k].from[0] = I;
        board->movelist[board->k].from[1] = 4;
        
        board->movelist[board->k].to[0] = I;
        board->movelist[board->k].to[1] = j;
        
        board->movelist[board->k].promoteto = 0;
        board->movelist[board->k].casualty = 'x';
        
        board->movelist[board->k].iscastle = 1;
        board->movelist[board->k].lostcastle = 1;
        

    
    }
    
    else {
    board->movelist[board->k].from[0] = i;
    board->movelist[board->k].from[1] = j;
    
    board->movelist[board->k].to[0]=i+mod_i;
    board->movelist[board->k].to[1]=j+mod_j;
    
    board->movelist[board->k].casualty = board->squares[i+mod_i][j+mod_j];
    board->movelist[board->k].promoteto = 0;
    board->movelist[board->k].iscastle = 0;
    board->movelist[board->k].lostcastle = 0;
    
    //printf("testing check.\n");
    //print_play_cord(move);
    
    if (P>3) {printf("fodeumlk\n");exit(0);}
    
    if (P==3) {board->movelist[board->k].promoteto = 'q'; P=1;}
    if (P==2) {board->movelist[board->k].promoteto = 'Q'; P=0;}
    
    
    
    if (check_move_check(board, &board->movelist[board->k], P)) return 0;        
        
       
  
    }
     
    //replicate_move(board->movelist[board->k],move);

    /*printf("appending[k=%i]: ", k);*/
    /*print_movement(k,P);*/
                    board->k++;
                    return 1;
}

void erase_moves(struct board *tgt, int eraseall) {
    int i = 0;
    tgt->k=0;
    tgt->kad = 0;
    tgt->hindex = 0;
/*    for (i=0; i < 128; i++) {
        tgt->movelist[i][0][0] = 0;
        tgt->movelist[i][0][1] = 0;
        tgt->movelist[i][1][0] = 0;
        tgt->movelist[i][1][1] = 0;
        tgt->movelist[i][2][0] = 0;
        tgt->movelist[i][2][1] = 0;
        
        if (eraseall) {
        
        tgt->movehistory[i][0][0] = 0;
        tgt->movehistory[i][0][1] = 0;
        tgt->movehistory[i][1][0] = 0;
        tgt->movehistory[i][1][1] = 0;
        tgt->movehistory[i][2][0] = 0;
        tgt->movehistory[i][2][1] = 0;
        }
    }
    
    if (eraseall) for (i=0; i < 64; i++) {
        tgt->attackers[i][0] = 0;
        tgt->attackers[i][1] = 0;
        
        tgt->defenders[i][0] = 0;
        tgt->defenders[i][1] = 0;
        tgt->defenders[i][2] = 0;
        
    }
        
    tgt->k=0;
    tgt->kad = 0;
    tgt->hindex = 0;*/
}

void print_movement (struct move *move) {
    char play[2][2] = {{move->from[0], move->from[1]},
                        {move->to[0], move->to[1]}};
    
    cord2pos(play[0]);
    cord2pos(play[1]);
    
    print_play(play);
    
}

int ifsquare_attacked (struct board *tg_board, int TGi, int TGj, int P, int verbose) {
    int i = 0;
    int j =0;
    int offender = 0;
    int target[2] = {TGi, TGj};
    
    int z=0;
    int n=0;
    
    int result = 0;
    
    int aim_x = 0;
    int aim_y =0;    
    
    int matrix[10][2]= {{0,0},{-1,-1},{-1,0},{-1,1},{0,-1},{0,0},{0,1},{1,-1},{1,0},{1,1}};
    int horse_matrix[2][2] = {{-1,1},{-2,2}};    
    

    
    for(z=0;z<10;z++) {
        if (z==0||z==5) continue;
        i=1;
        j=1;
        
        aim_y=target[0]+i*matrix[z][0];
        aim_x=target[1]+j*matrix[z][1];
        
        
        
        while (onboard(aim_y,aim_x) && tg_board->squares[aim_y][aim_x] == 'x') {i++; aim_y=target[0]+i*matrix[z][0]; j++; aim_x=target[1]+j*matrix[z][1];}
        
        Vb printf("checking %i%i\n", aim_y,aim_x);
        
        
        if ((onboard(aim_y,aim_x)) && (is_in(tg_board->squares[aim_y][aim_x], pieces[1-P],6))) {
            offender = getindex(tg_board->squares[aim_y][aim_x], pieces[1-P],6);
            Vb printf("y");
            //printf("kpos %i%i x=%i\n",aim_y,aim_x,x);
            
            
            if (offender==1||offender==4) {if (z==2||z==4||z==6||z==8) result = 1;} else Vb printf("granted. aim=%i%i z=%i offender=%i\n", aim_y,aim_x,z,offender);
                    
            if (offender==3||offender==4) if (z==1||z==3|z==7||z==9) result = 1;
            
            if (offender==5) if (i==1) result = 1;
            
            if (offender==0) if (i==1) if (P==0) if (z==1||z==3)  result = 1;
            
            if (offender==0) if (i==1) if (P==1) if (z==7||z==9) result = 1;
              

            

        } }
    
    

    
    
    for(z=0;z<2;z++) {
    for(n=0;n<2;n++) {

        
        aim_y=target[0]+horse_matrix[1][n];
        aim_x=target[1]+horse_matrix[0][z];
        
        if ((onboard(aim_y,aim_x)) && (tg_board->squares[aim_y][aim_x] == pieces[1-P][2])) result = 1;
             
  
        aim_y=target[0]+horse_matrix[0][n];
        aim_x=target[1]+horse_matrix[1][z];
                
        if ((onboard(aim_y,aim_x)) && (tg_board->squares[aim_y][aim_x] == pieces[1-P][2])) result = 1;
        


  
    }        
            
            
            
            
        }    
    
    
    
    
    return result;
}

int check_move_check (struct board *tg_board, struct move *move, int P) {
    

    int kpos[2];

    int check=-1;
    
    int i=0;
    int j=0;
    
    int verbose = 0;
    //if (movement[1][0] == 5 && movement[1][1]==7) verbose = 1;
    
    
    
    move_pc(tg_board, move);
    
    for (i=0;i<8;i++) {
        for (j=0;j<8;j++) {
            if (tg_board->squares[i][j] == pieces[P][5]) {kpos[0]=i; kpos[1]=j;check++;}
            
        }
    }
    
    //printf("checking check kpos= %i%i\n", kpos[0],kpos[1]);
    
    
    if (check==-1) {
        //printf("er-r [king not found].\n"); show_board(squares);
        undo_move(tg_board, move);return 0;}
    
    if (ifsquare_attacked(tg_board, kpos[0],kpos[1], P, verbose)) check = 1;
    
    
        


    //printf("check done.\n");
    undo_move(tg_board, move);
    return check;
}

int fehn2board (char str[]) {
    char *fstring;
    int z=0;
    int i=0;
    int j=0;
    int n=0;
    int number=0;
    int PL=0;
    fstring = strtok(str, " ");
    strtok(NULL, " ");

    
    
    for (z=0;z<=64;z++) {
        
    
     number = fstring[z]-'0';
        if (is_in(fstring[z],pieces[0],6)||is_in(fstring[z],pieces[1],6)) {
            board.squares[i][j] = fstring[z];
            j++;
        }
        if (fstring[z]=='/') {
            i++;
            j=0;        
        }
        if (0<number && number<9) {
            for (n=0;n<(fstring[z]-'0');n++) {
                board.squares[i][j] = 'x';
                j++;
                           }
        }
        if (i==7 && j==7) {
          if (fstring[z] == 'b') PL=1;  
        }
}
    return PL;
}

int read_movelines (char txt[128], int verbose) {
    char *movement = strtok(txt, " ");
    struct move move;
    int x=0;

    //movement = strtok(NULL, " "); 

        while ( movement != NULL) {

            if (parse_move(&move, movement, 1-machineplays)) {
                Vb printf("moving from input: %i%i %i%i\n",move.from[0],move.from[1],move.to[0],move.to[1]);
                move.casualty = board.squares[move.to[0]][move.to[1]]; 
                
                move_pc(&board, &move);

                history_append(&move);
                x++;
            }
        movement = strtok(NULL, " ");        
                
        }
    Vb printf("%i moves read.\n",x);

    
    
    return x;
}

int getindex (char x, char array[],int size) {
    int i=0;
    for (i=0;i<size;i++) {
        if (array[i] == x) return i;
        
    }
    return -1;
}

struct board *makeparallelboard (struct board *model) {
    int i=0;
    int j=0;
    int k=0;
    
    

    /*if (!cuda)*/ struct board *_board = (struct board *)malloc(sizeof (struct board));

    //else {struct board *_board = cudaMalloc()

    //_board->attackers = board->attackers;
    _board->k = 0;//model->k;
    _board->kad = 0;//model->kad;
    _board->evaltable = (long long *)malloc(128 * sizeof(long long));

    for (i=0;i<64;i++) {
        _board->attackers[i][0] = '0';
        _board->attackers[i][1] = '0';
    
        _board->defenders[i][0] = '0';
        _board->defenders[i][1] = '0';
        _board->defenders[i][2] = '0';
    }
    
    
    for (i=0;i<8;i++){
        for (j=0;j<8;j++){
            _board->squares[i][j] = model->squares[i][j];
            
            
        }
    }
    
    for(i=0;i<3;i++) {_board->castle[0][i] = model->castle[0][i]; _board->castle[1][i] = model->castle[1][i];}
    
    return _board;
}
 
void freeboard (struct board *target) {
    
    if (target != NULL) {

    //free (target->squares);
    //free (target->movelist);
    //free (target->defenders);
    //free (&target->k);
    //free (&target->kad);
    
    
    //free (target->attackers);
    free(target->evaltable);
    free (target);
    
    }
}




void select_top (long long *array, int size, int target[], int quant) {
    int i = 0;
    int qu=0;
    int win[16][2]={0};
    char forbid[16]={};
    int f_index=0;
    
    
    for (qu=0;qu<quant;qu++) {
        win[qu][1] = -16700;
        
    for (i=0;i<size;i++) {
        if (!is_in(i,forbid,f_index+1)) {
        

        if (array[i] > win[qu][1]){ 
            win[qu][1] = array[i];
            win[qu][0] = i;
            forbid[f_index] = i;  
            
        }       
        
        }
        
        
    }
        f_index++;
    }
    

        for (i=0;i<quant;i++) {
        target[i] = win[i][0];
        //target[i][1] = win[i][1];
                

    }
        
       
               
    
}

void replicate_move(struct move *target, struct move *source) {
    target->from[0] = source->from[0];
    target->from[1] = source->from[1];

    target->to[0] = source->to[0];
    target->to[1] = source->to[1];

        target->casualty = source->casualty;
        target->promoteto = source->promoteto;
        
        target->iscastle = source->iscastle;
        target->lostcastle = source->lostcastle;
    
}

int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
 }
