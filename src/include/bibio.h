#ifndef BIBIO_H
#define BIBIO_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *label;
} Button;

typedef struct carre{
    SDL_Rect rect ;
    Uint32 couleur ;
}carre ;

typedef enum Mode{
    HOME,
    PAUSE,
    PLAYER,
    MACHINE,
    PLAYERvsMACHIN,
    GAMEOVER,
} Mode ;


void affichMat(int mat[4][4]);
void UpArrow(int mat[4][4] , int *score , int *CanMove);
void DownArrow(int mat[4][4], int *score , int *CanMove);
void RightArrow(int mat[4][4], int *score , int *CanMove);
void LeftArrow(int mat[4][4], int *score , int *CanMove);
int CheckVide(int mat[4][4]);

void AjoutNombreAl(int mat[4][4] , int NombreCaseVide);

void SDL_AffichMat(SDL_Renderer *rend ,int mat[4][4] , SDL_Color *color ) ;
void RobotMove(int mat[4][4],int *POSSIBLE);
int machine(int mat[4][4] , int *POSSIBLE) ;
void move(int mat[4][4] , SDL_KeyCode MoveDir , int *score , int *CanMove);
void Initial_SDL(SDL_Window **window , SDL_Renderer **renderer );
int fin(SDL_Window *window , SDL_Renderer *renderer ,TTF_Font* font, int statut); // function pour finir le program
SDL_Texture* Mat_Texture(SDL_Renderer *rend, int mat[4][4], int w, int h, SDL_Color *color, TTF_Font *font);
void dessin_Logo(SDL_Window *window);
void RESET_Matrix(int mat[4][4]) ;
void Dessin_Home(SDL_Renderer* renderer,Button* buttons , int width , int height , TTF_Font *font);
int isInsideButton(Button* buttons,int N, int x, int y);
void Affiche_ModePlayer(SDL_Renderer* renderer,SDL_Texture* texture, int windowWidth,int windowHeight,int timeInSeconds , TTF_Font *font,int score);
void Affiche_ModeMachine(SDL_Renderer* renderer,SDL_Texture* texture, int windowWidth,int windowHeight);
SDL_Surface *temps(SDL_Renderer* renderer , int timeInSeconds , TTF_Font *font);
SDL_Surface *scoreSurface(SDL_Renderer* renderer , int score , TTF_Font *font);
int jeuTermine(int mat[4][4]) ;
#endif