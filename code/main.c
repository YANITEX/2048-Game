#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 
#include "bibio.h"
int main(int argc, char *argv[])
{   
    srand(time(NULL));
    SDL_Window* window = NULL ;
    SDL_Renderer *renderer = NULL ;
    SDL_Color colors[] = {
        {205, 193, 180,255},  // 0
        {238, 228, 218, 255}, // 2
        {237, 224, 200, 255}, // 4
        {242, 177, 121, 255}, // 8
        {245, 149, 99, 255},  // 16
        {246, 124, 95, 255},  // 32
        {246, 94, 59, 255},   // 64
        {237, 207, 114, 255}, // 128
        {237, 204, 97, 255},  // 256
        {237, 200, 80, 255},  // 512
        {237, 197, 63, 255},  // 1024
        {237, 194, 46, 255},
        {0,0,0,255},
        {0,0,0,255} ,
        {0,0,0,255} ,
        {0,0,0,255} ,
        {0,0,0,255} ,
        {0,0,0,255} ,

         // 2048
};
    SDL_Texture* Game_Matrix = NULL ;
    SDL_Rect gameRect = {0,0,555,555} ;
    Mode ModePlay = HOME ;
    SDL_Event event ;
    int delay = 2000 , count = 0 ;
    int mat[4][4] , MachineMat[4][4];  RESET_Matrix(mat); RESET_Matrix(MachineMat) ;
    int CanMove = 1 ;
    int stop = 0 , score = 0;
    int tempJouer = 0 , count2 = 0;
    Initial_SDL(&window,&renderer);
    TTF_Font* font = TTF_OpenFont("arial.ttf",24) ;
    int windowWidth = 600 , windowHeight = 500 ; SDL_GetWindowSize(window,&windowWidth,&windowHeight);
    SDL_SetRenderDrawColor(renderer,255,255,255,255) ; // background color
    SDL_RenderClear(renderer);  // set background
    dessin_Logo(window);
    Button buttons[4] = {
    {{(windowWidth-200)/2, windowHeight*0.2, 200, 50}, {200, 50, 50, 255}, "Player"},
    {{(windowWidth-200)/2, windowHeight*0.2+100, 200, 50}, {50, 50, 200, 255}, "Machine"}, 
    {{(windowWidth-200)/2, windowHeight*0.2+150, 200, 50}, {50, 200, 50, 255}, "Player vs Machine"},
    {{(windowWidth-200)/2, windowHeight*0.2+200, 200, 50}, {200, 200, 50, 255}, "Classement"},
    };
    Dessin_Home(renderer,buttons,windowWidth,windowHeight,font) ;
    //SDL_AffichMat(renderer,mat,colors) ;

    
    while (stop == 0)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                stop = 1;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){
                int x = event.button.x , y = event.button.y ;
                if(ModePlay == HOME){
                    int state = isInsideButton(buttons,4,x,y) ;
                    ModePlay =  state == -1 ? HOME : state ;
                    if(ModePlay==PLAYER){
                        count2 = SDL_GetTicks() ;
                        SDL_SetRenderDrawColor(renderer,255,255,255,255) ; // background color
                        SDL_RenderClear(renderer);  // set background
                        Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight , 0 , font , score);
                    }
                    if (ModePlay==MACHINE){
                        count2 = SDL_GetTicks() ;
                        SDL_SetRenderDrawColor(renderer,255,255,255,255) ; // background color
                        SDL_RenderClear(renderer);  // set background                        
                        Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight , 0 , font , score);
                    }
                    if(ModePlay == PLAYERvsMACHIN){
                        SDL_SetRenderDrawColor(renderer,255,255,255,255) ; // background color
                        SDL_RenderClear(renderer);  // set background
                        gameRect.x = windowWidth / 2 + 5 ;
                        gameRect.y = windowHeight * 0.2 ;
                        gameRect.w  = windowWidth / 2 - 5 ;
                        gameRect.h = windowHeight * 0.8 ;
                        SDL_RenderCopy(renderer,Mat_Texture(renderer,mat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                        gameRect.x = 0;
                        SDL_RenderCopy(renderer,Mat_Texture(renderer,MachineMat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                        SDL_RenderPresent(renderer);

                    }
                    
                }
            }
            else if(event.type == SDL_KEYDOWN) {

                if(ModePlay == PLAYER){
                    
                    move(mat,event.key.keysym.sym,&score,&CanMove);
                    
                    if(CanMove){
                        AjoutNombreAl(mat , CheckVide(mat)) ;
                        tempJouer = (SDL_GetTicks() - count)/1000 ;
                        Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight ,tempJouer,font ,score);
                    }
                }
                else if(ModePlay == PLAYERvsMACHIN){
                    move(mat,event.key.keysym.sym,&score,&CanMove);
                    if(CanMove){
                        AjoutNombreAl(mat , CheckVide(mat)) ;
                        gameRect.x = windowWidth / 2 + 5 ;
                        gameRect.y = windowHeight * 0.2 ;
                        gameRect.w  = windowWidth / 2 - 5 ;
                        gameRect.h = windowHeight * 0.8 ;
                        SDL_RenderCopy(renderer,Mat_Texture(renderer,mat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                    }
                    move(MachineMat,machine(MachineMat , &CanMove),&score , &CanMove) ;
                    if(CanMove){
                        AjoutNombreAl(MachineMat , CheckVide(mat)) ;
                        gameRect.x = 0;
                        SDL_RenderCopy(renderer,Mat_Texture(renderer,MachineMat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                        SDL_RenderPresent(renderer);
                    }
                    
                }
            }
        }
        // Machine play after delay
        if((SDL_GetTicks() - count)>delay && ModePlay == MACHINE){
            count = SDL_GetTicks() ;
            tempJouer = (SDL_GetTicks() - count2)/1000;
            if(CanMove){
                move(mat,machine(mat , &CanMove) ,&score , &CanMove) ;
                AjoutNombreAl(mat , CheckVide(mat)) ;
                Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight ,tempJouer,font ,score);
            }
            
        }
        else if(ModePlay == PLAYER){
            tempJouer = (SDL_GetTicks() - count2)/1000 ;
            Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight ,tempJouer,font ,score);
        }
        else if(ModePlay == PLAYERvsMACHIN){
            if(jeuTermine(MachineMat) == 0){
                move(MachineMat,machine(MachineMat , &CanMove),&score , &CanMove) ;
                if(CanMove){
                    AjoutNombreAl(MachineMat , CheckVide(mat)) ;
                    gameRect.x = 0;
                    SDL_RenderCopy(renderer,Mat_Texture(renderer,MachineMat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                    SDL_RenderPresent(renderer);
                }
            }
            
        }
            
            
    } 


    return fin(window,renderer,font,EXIT_SUCCESS) ;
}


