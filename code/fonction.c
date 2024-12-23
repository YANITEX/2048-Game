#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bibio.h"
#include <math.h>

//intialisation de SDL
void Initial_SDL(SDL_Window **window , SDL_Renderer **renderer ){
    if(0 != SDL_Init(SDL_INIT_VIDEO)){
    fprintf(stderr,"ERROR de initialisation : %s",
    SDL_GetError());
    fin(*window,*renderer,NULL , EXIT_FAILURE);
    }
    SDL_DisplayMode display ;
    SDL_GetCurrentDisplayMode(0,&display);
    *window = SDL_CreateWindow("MINI PROJET",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,display.w * 0.8, display.h * 0.8,SDL_WINDOW_SHOWN);
    if (*window == NULL)
    {
        fprintf(stderr,"ERROR de creation fenetre : %s",
        SDL_GetError()) ;
        fin(*window , *renderer,NULL , EXIT_FAILURE) ;
    }
    *renderer = SDL_CreateRenderer(*window,-1,SDL_RENDERER_ACCELERATED) ;
    if (*renderer == NULL)
    {
        fprintf(stderr,"ERROR de creation gpu renderer : %s",SDL_GetError());
        *renderer = SDL_CreateRenderer(*window , -1 ,SDL_RENDERER_SOFTWARE) ;
        if (*renderer == NULL)
        {
            fprintf(stderr,"ERROR de creation renderer : %s",SDL_GetError());
            fin(*window , *renderer,NULL , EXIT_FAILURE);
        }
        
    }
    TTF_Init();
}
//fin main
int fin(SDL_Window *window , SDL_Renderer *renderer ,TTF_Font* font, int statut){
        if(font != NULL){
            TTF_CloseFont(font);
            TTF_Quit();
        }
        if(renderer != NULL){
            SDL_DestroyRenderer(renderer) ;
        }
        if (window != NULL)
        {
            SDL_DestroyWindow(window);
        }
        
        SDL_Quit();
        if(statut == EXIT_FAILURE){
            exit(statut);
        }
        else return statut;
}


// returne le nombre de case vide
int CheckVide(int mat[4][4]){
    int compt = 0 ;

    for (int i = 0; i < 4; i++)
    {
        for(int j = 0 ; j < 4 ; j++ ){
            if(mat[i][j] == 0){
                compt++ ;
            }
        }
    }
    return compt ;
}

// fonction pour ajouter aleatoirment dans une case vide un nombre 2 ou 4 sachant que la probabilité de 2 est 0.8 et 0.2 pour 4
void AjoutNombreAl(int mat[4][4] , int NombreCaseVide){
    if(NombreCaseVide == 0){
        return ;
    }
    int pos = rand() % NombreCaseVide ;
    int value = rand() % 5 ;
    value = (value == 0) ? 4 : 2 ;
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0 ; j < 4 ; j++ ){
            if(mat[i][j] == 0){
                if(pos == 0){
                    mat[i][j] = value ;
                    return ;
                }
                pos-- ;
            }
        }
    }
}



// dessiner la matrice sur la fenetre de SDL

SDL_Texture* Mat_Texture(SDL_Renderer *rend, int mat[4][4], int w, int h, SDL_Color *color, TTF_Font *font) {
    SDL_Texture *texture_matrice = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(rend, texture_matrice);
    SDL_SetRenderDrawColor(rend, 150, 150, 150, 255);
    SDL_RenderClear(rend);
    int SeparateurWidth = w * 0.01 ; // 1%
    int sepateurHeight = h * 0.01 ; // 1%
    int CaseHeight = (h - (4 + 1) * 5) / 4; // dimensions des cases avec 5px entre eux
    int CaseWidth = (w - (4 + 1) * 5) / 4;
    SDL_Rect rec = {5, 5, CaseWidth, CaseHeight};

    for (size_t i = 0; i < 4; i++) {
        rec.y = i * (CaseHeight + 5) + 5;
        for (size_t j = 0; j < 4; j++) {
            rec.x = j * (CaseWidth + 5) + 5;

            // Définir la couleur de fond de la case
            int k = (mat[i][j] > 0) ? log2(mat[i][j]) : 0; // éviter log2(0)
            SDL_SetRenderDrawColor(rend, color[k].r, color[k].g, color[k].b, 255);
            SDL_RenderFillRect(rend, &rec);

            // Si la valeur n'est pas 0, dessiner le texte
            if (mat[i][j] != 0) {
                char value[20];
                sprintf(value, "%d", mat[i][j]);

                SDL_Color textColor = {0, 0, 0, 255}; // Couleur du texte (noir)
                SDL_Surface *textSurface = TTF_RenderText_Blended(font, value, textColor);
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend, textSurface);

                SDL_Rect textRect;
                textRect.w = textSurface->w;
                textRect.h = textSurface->h;
                textRect.x = rec.x + (CaseWidth - textRect.w) / 2; // Centrer le texte
                textRect.y = rec.y + (CaseHeight - textRect.h) / 2;

                SDL_RenderCopy(rend, textTexture, NULL, &textRect);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }
    }

    SDL_SetRenderTarget(rend, NULL);
    return texture_matrice;
}



void dessin_Logo(SDL_Window *window){
    SDL_Surface *surface = SDL_CreateRGBSurface(0,32,32,32,0,0,0,0) ;
    carre rect[4]={{{4,4,10,10},SDL_MapRGB(surface->format,255,0,0)},{{18,4,10,10},SDL_MapRGB(surface->format,0,255,0)},{{4,18,10,10},SDL_MapRGB(surface->format,255,255,0)},{{18,18,10,10},SDL_MapRGB(surface->format,2,2,255)}};
    SDL_Rect recton = {0,0,32,32};
    SDL_FillRect(surface,&recton,SDL_MapRGB(surface->format,255,255,255));
    for(int i=0 ; i<4 ; i++){
        SDL_FillRect(surface,&(rect[i].rect),rect[i].couleur);
    }
    SDL_SetWindowIcon(window,surface) ;
}

void RESET_Matrix(int mat[4][4]){
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mat[i][j] = 0;
        }
    }
     for(int k = 0 ; k < 2 ; k++)
    { 
        AjoutNombreAl(mat,CheckVide(mat)) ;
    }
}



void Dessin_Home(SDL_Renderer* renderer,Button* buttons , int width , int height , TTF_Font *font){
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Couleur de fond
    SDL_RenderClear(renderer);
    
        // Dessiner les boutons
    for (int i = 0; i < 4; i++) {
        buttons[i].rect.x = (width - 200)/2  ; 
        buttons[i].rect.y = (height)/4 + i*100 ;
        SDL_SetRenderDrawColor(renderer, buttons[i].color.r, buttons[i].color.g, buttons[i].color.b, buttons[i].color.a);
        SDL_RenderFillRect(renderer, &buttons[i].rect);

        // Ajouter le texte des boutons 
        SDL_Color textColor = {255, 255, 255, 255}; // Couleur du texte (blanc)
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, buttons[i].label, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = buttons[i].rect.x + (buttons[i].rect.w - textRect.w) / 2;
        textRect.y = buttons[i].rect.y + (buttons[i].rect.h - textRect.h) / 2;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

        SDL_RenderPresent(renderer);
}

int isInsideButton(Button* buttons,int N, int x, int y) {
    for(int i = 0 ; i < N ; i++) {
        if (x > buttons[i].rect.x && x < buttons[i].rect.x + buttons[i].rect.w && y > buttons[i].rect.y && y < buttons[i].rect.y + buttons[i].rect.h){
            return i + 2 ;
        }
    }
    return -1 ;
}

void Affiche_ModePlayer(SDL_Renderer* renderer,SDL_Texture* texture, int windowWidth,int windowHeight,int timeInSeconds , TTF_Font *font , int score){
    SDL_Rect Rect ;
    Rect.x = 0 ;
    Rect.y = 0;
    Rect.w  = windowWidth * 0.8;
    Rect.h = windowHeight;
    SDL_RenderCopy(renderer,texture,NULL,&Rect);
    SDL_Surface *textSurface = temps(renderer,timeInSeconds,font);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }
    Rect.w = textSurface->w + 10  ; //0.8 * 0.2
    Rect.h = textSurface->h + 6;
    Rect.x = windowWidth * 0.9 - textSurface->w  / 2 ;
    Rect.y = windowHeight * 0.9 ;
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
    SDL_DestroyTexture(texture);
    
    textSurface = scoreSurface(renderer,score,font) ;
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }
    Rect.w = textSurface->w + 10  ; 
    Rect.h = textSurface->h + 6;
    Rect.x = windowWidth * 0.9 - textSurface->w  / 2 ;
    Rect.y = windowHeight * 0.8 ;
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
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
    

}

// texture de temps
SDL_Surface *temps(SDL_Renderer* renderer , int timeInSeconds , TTF_Font *font){
    // afficher le temps
    int minutes = timeInSeconds/60 ;
    timeInSeconds = timeInSeconds % 60;
    
    char timeText[20]; // Chaîne pour stocker le temps converte en texte
    snprintf(timeText, sizeof(timeText), "TEMP : %d m %d s", minutes, timeInSeconds); // Convertir le temps en texte
    // Créer une surface à partir du texte
    SDL_Color color = {25,25,25,255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeText, color);
    
    if (!textSurface) {
        return NULL;
    }
    return textSurface ;

    
}
SDL_Surface *scoreSurface(SDL_Renderer* renderer , int score , TTF_Font *font){

    char timeText[20]; // Chaîne pour stocker le temps converte en texte
    snprintf(timeText, sizeof(timeText), "SCORE : %d", score); // Convertir le temps en texte
    // Créer une surface à partir du texte
    SDL_Color color = {0,0,0,255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, timeText, color);
    
    if (!textSurface) {
        return NULL;
    }
    return textSurface ;

    
}

void Affiche_ModeMachine(SDL_Renderer* renderer,SDL_Texture* texture, int windowWidth,int windowHeight){
    SDL_Rect gameRect ;
    gameRect.x = 0;
    gameRect.y = 0;
    gameRect.w  = windowWidth * 0.8;
    gameRect.h = windowHeight;
    SDL_RenderCopy(renderer,texture,NULL,&gameRect);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(renderer);
}



// Fonction pour initialiser une matrice avec des valeurs provenant d'un fichier
void initializeMatrixFromFile(const char *filename, int matrix[4][4]) {
    FILE *file = fopen(filename, "r"); // Ouvrir le fichier en mode lecture
    if (file == NULL) { // Vérifier si le fichier existe
        RESET_Matrix(matrix);
        return;
    }

    int value, count = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (fscanf(file, "%d", &value) == 1) { // Lire une valeur entière depuis le fichier
                matrix[i][j] = value;
                count++;
            } else {
                matrix[i][j] = 0; // Si la lecture échoue, remplir avec 0
            }
        }
    }

    // Si aucune valeur n'a été lue, remplir toute la matrice avec des 0
    if (count == 0) {
        RESET_Matrix(matrix);
    }

    fclose(file); // Fermer le fichier
}


// Fonction pour stocker les valeurs de la matrice dans un fichier
void saveMatrixToFile(const char *filename, int matrix[4][4]) {
    FILE *file = fopen(filename, "w"); // Ouvrir le fichier en mode écriture
    if (file == NULL) { // Vérifier si l'ouverture échoue
        return;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fprintf(file, "%d ", matrix[i][j]); // Écrire chaque valeur dans le fichier
        }
    }

    fclose(file); // Fermer le fichier
}




// desplay game over dans le midle de la fenetre
void displayLoose(SDL_Window *window , SDL_Renderer *renderer ,TTF_Font* font, int statut){
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, "GAME OVER", (SDL_Color){255,0,0,255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect Rect = {0,0,0,0} ;
    Rect.w = 500  ; 
    Rect.h = 300;
    Rect.x = 100 ;
    Rect.y = 100 ;
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
    SDL_RenderPresent(renderer);

}