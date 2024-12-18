#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bibio.h"
#include <math.h>
#define HIGHT 1000
#define WIDTH 1500

void move(int mat[4][4] , SDL_KeyCode MoveDir , int *score , int *CanMove){
    *CanMove = 0 ;
    if (MoveDir == SDLK_UP)
    {
        for (int j = 0; j < 4; j++) // parcourir colomnne par colomnne 
        {
            int i = 0 ;                 
            while (i<3) 
            {
            
                int k = i + 1 ;
                while (mat[k][j] == 0 && k<4) // parcourir jusqu'a le premier element != 0
                {
                    k++;
                }
                if (k == 4)   // si k == 4 signifie que tout les elemment de colomne (apres le premier) sont des zero donc on fait rien 
                {
                    break;
                }
                
                if (mat[i][j] == 0)  // l'element courant est un zero donc on peux le remplacer par un nombre
                {   
                    mat[i][j] = mat[k][j] ;
                    
                }
                else if (mat[i][j] == mat[k][j]) 
                {
                    mat[i][j] += mat[k][j] ;
                    *score += mat[i][j] ;
                    i++;
                }
                else{
                    mat[++i][j] = mat[k][j] ;
                    
                    if (k==i)
                    {
                        continue;
                    }
                }
                
                mat[k][j] = 0 ;
                *CanMove = 1 ;
                    
                    
            }
            
            
        }
    }
    else if (MoveDir == SDLK_LEFT)
    {
        for (int j = 0; j < 4; j++)
        {
            int i = 0 ;
            while (i<3)
            {
            
                int k = i + 1 ;
                while (mat[j][k] == 0 && k<4)
                {
                    k++;
                }
                if (k == 4)
                {
                    break;
                }
                
                if (mat[j][i] == 0)
                {   
                    mat[j][i] = mat[j][k] ;
                }
                else if (mat[j][i] == mat[j][k])
                {
                    mat[j][i] += mat[j][k] ;
                    *score += mat[j][i] ;
                    i++;
                }
                else{
                    mat[j][++i] = mat[j][k] ;
                    
                    if (k==i)
                    {
                        continue;
                    }
                }
                
                mat[j][k] = 0 ;
                *CanMove = 1 ;

            
                    
                    
            }
        
        
        }
    }
    else if (MoveDir == SDLK_DOWN)
    {
        for (int j = 0; j < 4; j++)
        {
            int i = 3 ;
            while (i>0)
            {
            
                int k = i - 1 ;
                while (mat[k][j] == 0 && k>=0)
                {
                    k--;
                }
                if (k == -1)
                {
                    break;
                }
                
                if (mat[i][j] == 0)
                {   
                    mat[i][j] = mat[k][j] ;
                }
                else if (mat[i][j] == mat[k][j])
                {
                    mat[i][j] += mat[k][j] ;
                    *score += mat[i][j] ;
                    i--;
                }
                else{
                    mat[--i][j] = mat[k][j] ;
                    if (k==i)
                    {
                        continue;
                    }
                    
                }
                
                mat[k][j] = 0 ;
                *CanMove = 1 ;
            
                    
                    
            }
        
        
        }
    }
    else if (MoveDir == SDLK_RIGHT)
    {
        for (int j = 0; j < 4; j++)
        {
            int i = 3 ;
            while (i>0)
            {
            
                int k = i - 1 ;
                while (mat[j][k] == 0 && k>=0)
                {
                    k--;
                }
                if (k == -1)
                {
                    break;
                }
                
                if (mat[j][i] == 0)
                {   
                    mat[j][i] = mat[j][k] ;
                }
                else if (mat[j][i] == mat[j][k])
                {
                    mat[j][i] += mat[j][k] ;
                    *score += mat[j][i] ;
                    i--;
                }
                else{
                    mat[j][--i] = mat[j][k] ;
                    if (k==i)
                    {
                        continue;
                    }
                    
                }
                
                mat[j][k] = 0 ;
                *CanMove = 1 ;
        
                    
            }
            
            
        }
    }
      
    
}



void copieMatrice(int mat[4][4],int mat2[4][4]){

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            mat2[i][j] = mat[i][j] ;
        }
        
    }

}




int machine(int mat[4][4] , int *POSSIBLE){
    int temp[4][4] , temp2[4][4] , temp3[4][4];
    int max = 0 , score = 0 , PossibleDeMouve = 0;
    int scoreTemp = 0 , maxTemp = 0;
    int scoreTemp3 = 0 , maxTemp3 = 0 ;
    SDL_KeyCode BestMove = SDLK_RIGHT ;
    *POSSIBLE = 0 ;
    
    for (SDL_KeyCode i = SDLK_RIGHT; i <= SDLK_UP; i++)
    {
        score = 0 ;
        copieMatrice(mat, temp) ;
        move(temp , i , &score , &PossibleDeMouve) ;
        maxTemp = 0 ;
        if (PossibleDeMouve)
        {
            *POSSIBLE = 1 ;
            for(SDL_KeyCode j = SDLK_RIGHT; j <= SDLK_UP; j++){
                scoreTemp  = 0 ;
                copieMatrice(temp,temp2) ;
                move(temp2,j,&scoreTemp,&PossibleDeMouve) ;
                AjoutNombreAl(temp2,CheckVide(temp2));
                if(PossibleDeMouve){
                    
                    maxTemp3 = 0 ;
                    for (SDL_KeyCode k = SDLK_RIGHT; k <= SDLK_UP; k++)
                    {
                        scoreTemp3 = 0 ;
                        copieMatrice(temp2,temp3);
                        move(temp3,k,&scoreTemp3,&PossibleDeMouve);
                        if(PossibleDeMouve && scoreTemp3 > maxTemp3){
                            maxTemp3 = scoreTemp3 ;
                        }
                    }
                    scoreTemp = scoreTemp * 2 + maxTemp3 ;
                    
                }
                if(scoreTemp>maxTemp){
                    maxTemp = scoreTemp ;
                }
            }
            score = score * 4 + maxTemp ;
        }
        
        if(max < score){
            BestMove = i ;
            max = score ;
        }
        
        
    }
    return BestMove ;
    
}


int jeuTermine(int mat[4][4]){
    int temp[4][4] ;
    int score = 0 , can = 0 ;
    for(SDL_KeyCode i = SDLK_RIGHT ; i <= SDLK_UP ; i++){
        copieMatrice(mat,temp);
        move(temp,i,&score,&can);
        if(can == 1){
            return 0 ;
        }
    }
    return 1 ;
}