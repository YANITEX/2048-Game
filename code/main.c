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
    int delay = 1000 , count = 0 ;
    int mat[4][4] , MachineMat[4][4];  RESET_Matrix(mat); RESET_Matrix(MachineMat) ;
    int CanMove = 1 ;
    int stop = 0 , score = 0 , possible , machineScore = 0 ;
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
                        SDL_RenderPresent(renderer);

                    }
                    
                }
            }
            else if(event.type == SDL_KEYDOWN) {

                if(ModePlay == PLAYER){
                    if (jeuTermine(mat)==0){
                        move(mat,event.key.keysym.sym,&score,&CanMove);
                        if(CanMove){
                            AjoutNombreAl(mat , CheckVide(mat)) ;
                            tempJouer = (SDL_GetTicks() - count)/1000 ;
                            Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight ,tempJouer,font ,score);
                        }
                    }
                }
                else if(ModePlay == PLAYERvsMACHIN){
                    if(jeuTermine(mat)==0){
                        move(mat,event.key.keysym.sym,&score,&CanMove);
                        if(CanMove){
                            AjoutNombreAl(mat , CheckVide(mat)) ;
                            gameRect.x = windowWidth / 2 + 5 ;
                            gameRect.y = windowHeight * 0.2 ;
                            gameRect.w  = windowWidth / 2 - 5 ;
                            gameRect.h = windowHeight * 0.8 ;
                            SDL_RenderCopy(renderer,Mat_Texture(renderer,mat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                        }
                    }
                    
                }
            }
        }
        // Machine play after delay
        if((SDL_GetTicks() - count)>delay && ModePlay == MACHINE && !jeuTermine(mat) ){
            count = SDL_GetTicks() ;
            tempJouer = (SDL_GetTicks() - count2)/1000;
            void encadre(SDL_Renderer *rend , int mat[4][4] , SDL_KeyCode MoveDir);
            move(mat,machine(mat , &CanMove) ,&score , &CanMove) ;
            AjoutNombreAl(mat , CheckVide(mat)) ;
            Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight ,tempJouer,font ,score);
            
        }
        else if(ModePlay == PLAYER){
            if(jeuTermine(mat)==0){
                tempJouer = (SDL_GetTicks() - count2)/1000 ;
                Affiche_ModePlayer(renderer,Mat_Texture(renderer,mat,windowWidth * 0.8,windowHeight,colors,font) ,windowWidth,windowHeight ,tempJouer,font ,score);
            }
            else{
                ModePlay == GAMEOVER ;
            }
        }
        else if(ModePlay == PLAYERvsMACHIN){
            if(!jeuTermine(MachineMat)&& SDL_GetTicks() - count > delay){
                count = SDL_GetTicks() ;
                move(MachineMat,machine(MachineMat , &possible),&machineScore , &possible) ;
                AjoutNombreAl(MachineMat , CheckVide(mat)) ;
                gameRect.x = 0;
                SDL_RenderCopy(renderer,Mat_Texture(renderer,MachineMat,windowWidth/2-5,windowHeight*0.8,colors,font),NULL,&gameRect);
                SDL_RenderPresent(renderer);
                
            }
            if(!jeuTermine(mat) || !jeuTermine(MachineMat)){
                // ajouter le temps dans le mode joueur vs machine
                tempJouer = (SDL_GetTicks() - count2)/1000 ;
                // afficher le temps dans le midle up de la fenetre
                SDL_Surface *textSurface = temps(renderer,tempJouer,font);
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect Rect = {0,0,0,0} ;
                Rect.w = textSurface->w + 10  ; //0.8 * 0.2
                Rect.h = textSurface->h + 6;
                Rect.x = (windowWidth - textSurface->w )  / 2 ;
                Rect.y = windowHeight * 0.1 ;
                SDL_Rect rect2 = {5,3 , textSurface->w , textSurface->h};
                // Copier la texture de text dans le dans le texture qui a le background noire
                SDL_Texture *textureTemp = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,Rect.w,Rect.h);
                SDL_SetRenderTarget(renderer,textureTemp);
                SDL_SetRenderDrawColor(renderer,205, 193, 180,255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, textTexture, NULL, &rect2);
                SDL_SetRenderTarget(renderer,NULL);
                SDL_RenderCopy(renderer,textureTemp,NULL,&Rect);
                // Libérer la surface et la texture
                SDL_DestroyTexture(textureTemp);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                // afficher le score de joueur
                textSurface = scoreSurface(renderer,score,font) ;
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                Rect.w = textSurface->w + 10  ;
                Rect.h = textSurface->h + 6;
                Rect.x = (windowWidth * 1.5 - textSurface->w )  / 2 ;
                Rect.y = windowHeight * 0.1 ;
                rect2.w =  textSurface->w;
                rect2.h =  textSurface->h ;
                // Copier la texture de text dans le dans le texture qui a le background noire
                textureTemp = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,Rect.w,Rect.h);
                SDL_SetRenderTarget(renderer,textureTemp);
                SDL_SetRenderDrawColor(renderer,205, 193, 180,255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, textTexture, NULL, &rect2);
                SDL_SetRenderTarget(renderer,NULL);
                SDL_RenderCopy(renderer,textureTemp,NULL,&Rect);
                // Libérer la surface et la texture
                SDL_DestroyTexture(textureTemp);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                // afficher le score de machine
                textSurface = scoreSurface(renderer,machineScore,font) ;
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                Rect.w = textSurface->w + 10  ;
                Rect.h = textSurface->h + 6;
                Rect.x = (windowWidth * 0.5 - textSurface->w - 5)  / 2 ;
                Rect.y = windowHeight * 0.1 ;
                rect2.w =  textSurface->w;
                rect2.h =  textSurface->h ;
                // Copier la texture de text dans le dans le texture qui a le background noire
                textureTemp = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB888,SDL_TEXTUREACCESS_TARGET,Rect.w,Rect.h);
                SDL_SetRenderTarget(renderer,textureTemp);
                SDL_SetRenderDrawColor(renderer,205, 193, 180,255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, textTexture, NULL, &rect2);
                SDL_SetRenderTarget(renderer,NULL);
                SDL_RenderCopy(renderer,textureTemp,NULL,&Rect);
                // Libérer la surface et la texture
                SDL_DestroyTexture(textureTemp);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                SDL_RenderPresent(renderer);
            }
            else{
                ModePlay = GAMEOVER ;
            }

            
        }
        else if(ModePlay == GAMEOVER){
            displayLoose(window,renderer,font,0) ;
        }
            
            
    } 


    return fin(window,renderer,font,EXIT_SUCCESS) ;
}


