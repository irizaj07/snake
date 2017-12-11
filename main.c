#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_ttf.h>

SDL_Rect randomPosition(void);
void blitScore(SDL_Surface *screen, int score);
void blitObjective(SDL_Surface *screen,int x,int y);
void blitText(SDL_Surface *screen, SDL_Rect test);

int main(int argc, char *argv[]){
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	int interval;
	SDL_Rect test;
	SDL_Rect tabPosSnake[200];
	SDL_Surface *screen;
	screen = SDL_SetVideoMode(600,600,32,SDL_HWSURFACE);
	SDL_Surface *plan;
	plan = IMG_Load("plan.jpg");
	SDL_Rect positionPlan;
	positionPlan.x = 0;
	positionPlan.y = 0;
	SDL_BlitSurface(plan,NULL,screen,&positionPlan);
	SDL_Flip(screen);
	SDL_Surface *snakeF;
	SDL_Surface *snakeD;
	snakeF = SDL_CreateRGBSurface(SDL_HWSURFACE,18,18,32,0,0,0,0);
	snakeD = SDL_CreateRGBSurface(SDL_HWSURFACE,20,20,32,0,0,0,0);
	SDL_FillRect(snakeF,NULL,SDL_MapRGB(screen->format,0,0,250));
	SDL_FillRect(snakeD,NULL,SDL_MapRGB(screen->format,250,0,0));
	SDL_Rect positionSnakeF;
	positionSnakeF.x = 1;
	positionSnakeF.y = 1;
	SDL_Rect positionSnake;
	positionSnake.x = 280;
	positionSnake.y = 280;
	SDL_BlitSurface(snakeF,NULL,snakeD,&positionSnakeF);
	SDL_BlitSurface(snakeD,NULL,screen,&positionSnake);
	test = randomPosition();
	SDL_Rect positionJetable;
	blitObjective(screen,test.x,test.y);
	blitText(screen,test);
	SDL_Flip(screen);
	int longSnake = 0;
	int tempNow = SDL_GetTicks();
	int allerVers = 0;
	int lose = 0;
	int score = 0;
	while(1){
		SDL_Event event;
		if(positionSnake.x == test.x && positionSnake.y == test.y){
			score +=1;
			test = randomPosition();
			blitObjective(screen,test.x,test.y);
			longSnake += 1;
			tabPosSnake[longSnake].x = positionSnake.x;
			tabPosSnake[longSnake].y = positionSnake.y;
		}

		if(positionSnake.x == 600 & allerVers == 4){
			positionSnake.x = -20;
		}
		if(positionSnake.y == 600 & allerVers == 2){
			positionSnake.y = -20;
		}
		if(positionSnake.x == 0 & allerVers == 3){
			positionSnake.x = 600;
		}
		if(positionSnake.y == 0 & allerVers == 1){
			positionSnake.y = 600;
		}
		if(SDL_PollEvent(&event)){
		if(event.type == SDL_KEYDOWN){
			switch(event.key.keysym.sym){
				case SDLK_UP:
					allerVers = 1;
					break;
				case SDLK_DOWN:
					allerVers = 2;
					break;
				case SDLK_LEFT:
					allerVers = 3;
					break;
				case SDLK_RIGHT:
					allerVers = 4;
					break;
			}
		}
		}
		if(SDL_GetTicks() -  tempNow > 100  && allerVers != 0){
			tempNow = SDL_GetTicks();
			switch(allerVers){
                                case 1:
                                        positionSnake.y -=20;
                                        break;
                                case 2:
                                        positionSnake.y +=20;
                                        break;
                                case 3:
                                        positionSnake.x -=20;
                                        break;
                                case 4:
                                        positionSnake.x +=20;
                                        break;
                        }
                        SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));
			SDL_BlitSurface(plan,NULL,screen,&positionPlan);
                        for(int i=longSnake;i>0;i--){
				tabPosSnake[i] = tabPosSnake[i-1];
			}
                        tabPosSnake[0].x = positionSnake.x;
                        tabPosSnake[0].y = positionSnake.y;
                        for(int i=0;i<longSnake-1;i++){
                        SDL_BlitSurface(snakeD,NULL,screen,&(tabPosSnake[i]));
                        }
                        SDL_BlitSurface(snakeD,NULL,screen,&positionSnake);
                        blitObjective(screen,test.x,test.y);
			blitScore(screen,score);
                        SDL_Flip(screen);
			for(int i=longSnake-1;i>0;i--){
				if(tabPosSnake[0].x == tabPosSnake[i].x && tabPosSnake[0].y == tabPosSnake[i].y){
					lose = 1;
				} 
			}

                }
		if(lose == 1){
		break;
		}
		if(event.type== SDL_QUIT){
			break;
		}
		interval = SDL_GetTicks() - tempNow;
		SDL_Delay(10);
	}
	SDL_FreeSurface(plan);
	SDL_FreeSurface(screen);
	TTF_Quit();
	SDL_Quit();
	return 0;
}

SDL_Rect randomPosition(void) {
	int x,y;
	time_t t;
	SDL_Rect pos;
	srand((unsigned) time(&t));
	do{
 	x = rand()%1000;
	}while(x>580 || x<20 || x%20 !=0);
	do{
        y = rand()%1000;
        }while(y>580 || y<20 || y%20 !=0);
	pos.x = x;
	pos.y = y;
	return pos;
}

void blitObjective(SDL_Surface *screen,int x,int y){
	SDL_Surface *objective;
	objective = SDL_CreateRGBSurface(SDL_HWSURFACE,20,20,32,0,0,0,0);
	SDL_FillRect(objective,NULL,SDL_MapRGB(screen->format,0,255,0));
	SDL_Rect positionObjective;
	positionObjective.x = x;
	positionObjective.y = y;
	SDL_BlitSurface(objective,NULL,screen,&positionObjective);
	SDL_FreeSurface(objective);
}

void blitScore(SDL_Surface *screen, int score){
	SDL_Surface *Sscore;
	TTF_Font *textFont;
	textFont = TTF_OpenFont("font.ttf",22);
	SDL_Color colorFont = {0,0,255};
	char x[20];
	sprintf(x,"%d",score);
	Sscore = TTF_RenderText_Solid(textFont,x,colorFont);
	SDL_Rect positionScore;
	positionScore.x = 10;
	positionScore.y = 10;
	SDL_BlitSurface(Sscore,NULL,screen,&positionScore);
	SDL_FreeSurface(Sscore);
}

void blitText(SDL_Surface *screen, SDL_Rect test){
	SDL_Surface *text;
	TTF_Font *textFont;
	textFont = TTF_OpenFont("font.ttf",23);
	SDL_Color colorFont = {255,0,0};
	char  x[20],y[20],a[50];
	sprintf(x,"%d",test.x);
	sprintf(y,"%d",test.y);
	strcat(a,"x");
	strcat(a," = ");
	strcat(a,x);
	strcat(a," y = ");
	strcat(a,y);
	text = TTF_RenderText_Solid(textFont,a,colorFont);
	SDL_Rect textPosition;
	textPosition.x = 100;
	textPosition.y = 100;
	SDL_BlitSurface(text,NULL,screen,&textPosition);
	SDL_FreeSurface(text);
}
