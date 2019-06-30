//  Copyright 2019 Richard Herrera <henriqueherrera9@gmail.com>
// 
//  Permission is hereby granted, free of charge, to any person obtaining a 
//  copy of this software and associated documentation files (the "Software"), 
//  to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//  and/or sell copies of the Software, and to permit persons to whom the 
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
//  DEALINGS IN THE SOFTWARE.

#include<iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <SDL/SDL_mixer.h>
using namespace std;

SDL_Surface *screen, *imgFly, *imgBackGroung[2], *text, *placar, *imgPerson = NULL;

SDL_Rect movBackGround[3], movGround[2],rectFly[4], movFly, rectPonto;

SDL_Rect rectBackGroung[2], rectGround, rectPerson[3], movPerson;

Mix_Music *mscMusic = NULL;
Mix_Chunk *mscFly = NULL;
Mix_Chunk *mscColision = NULL;
TTF_Font *font;

Uint32 color, color2, start;

SDL_Event event;

int mov_ground[3] = {0,620,1220}, fly = 810, animaPerson = 0, numberSpriteFly, numberSpritePerson, animaFly = 0, sort = 1, score =0;

const int FPS = 60;

bool saltPressed = false, running = true, gameOver = false;

string pontos = "Pontos: 0";

void delayBlitPerson() {
    while( running ) {
	this_thread::sleep_for(chrono::milliseconds(150));
	animaPerson++;
	animaFly++;
	
	if( animaPerson == 3 ) {
	    animaPerson = 0;
	}
	
	if( animaFly == 4 ) {
	   animaFly = 0;
	}
    }
}

void setFly() {
    int x[4] = {6,70,130,195}, y[4] = {73,68,70,73}, h[4] = {54,55,55,54}, w = 58;
    imgFly = IMG_Load("./fly.png");
    for( int i = 0; i<4; i++ ) {
        rectFly[i].x = x[i];
        rectFly[i].y = y[i];
        rectFly[i].h = h[i];
        rectFly[i].w = w;
    }
    movFly.x = 810;
    movFly.y = 50;
}
void setSound() {
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
	cout<<"SDL_Error: \n"<<SDL_GetError()<<endl;	
    }
    
    mscMusic = Mix_LoadMUS("snd.wav");
    
    if( mscMusic == NULL) {
	cout<<"Couldn't load snd.wav: "<<Mix_GetError()<<endl;
    }
    
    mscFly = Mix_LoadWAV("sndfly.wav");
    mscColision = Mix_LoadWAV("sndcolision.wav");
    if( mscFly == NULL || mscColision == NULL ) {
	cout<<"Couldn't load msc: "<<Mix_GetError()<<endl;
    }
}
void setBackGroung() {
    int movBg[3] = {0, 1280, 2520};
    imgBackGroung[0] = SDL_DisplayFormat(SDL_LoadBMP("fundo.bmp"));
    imgBackGroung[1] = SDL_DisplayFormat(SDL_LoadBMP("fundo2.bmp"));
    
    for( int i = 1; i<= 3; i++ ) {
    	rectBackGroung[i-1].w = 1280;
	rectBackGroung[i-1].h = 620;
        rectBackGroung[i-1].x = 0;
        rectBackGroung[i-1].y = 0;
    }
    for( int i = 0; i<3; i++ ) {
    	movBackGround[i].x = movBg[i];
	movBackGround[i].y = 0;
    }
    movGround[0].x = 0;
    movGround[0].y = 0;
    movGround[1].x = 1240;
    movGround[1].y = 0;
}
void setPerson() {
    imgPerson = IMG_Load("./drag.png");
    int x[3] = {8,153,297}, y[3] = { 134,163,168}, h[3] = {123,60,57}, w[3] = {112,112,182};
    for( int i = 0; i<3; i++ ) {
	rectPerson[i].x = x[i];
	rectPerson[i].y = y[i];
	rectPerson[i].h = h[i];
	rectPerson[i].w = w[i];
    }	
    movPerson.x = 200;
    movPerson.y = 100;
}

void blitScreen() {
    SDL_FillRect(screen, &screen->clip_rect, color2);
    SDL_BlitSurface(imgBackGroung[0],&rectBackGroung[0], screen, &movBackGround[0]);
    SDL_BlitSurface(imgBackGroung[1],&rectBackGroung[0], screen, &movBackGround[1]);
    SDL_BlitSurface(imgFly,&rectFly[animaFly], screen, &movFly);
    SDL_BlitSurface(imgPerson,&rectPerson[animaPerson], screen, &movPerson); 
    SDL_BlitSurface(placar, NULL, screen, &rectPonto);
    SDL_Flip(screen);
}

string gameOverTxt(int score) {
    rectPonto.x = 200;
    rectPonto.y = 200;
    string x = "Game Over";
    return x;
}

void setRectPonto() {
    rectPonto.x = 200;
    rectPonto.y = 10;
}


void setGame() {
    setSound();
    setPerson();
    setBackGroung();
    setFly();
    setRectPonto();	
}
void mov() {
    movGround[0].x--;
    movGround[1].x--;
    movBackGround[0] = movGround[0];
    movBackGround[1] = movGround[1];
    if(movGround[0].x == -1280){
    	movGround[0].x = 1240;
    }
    if(movGround[1].x == -1280){
    	movGround[1].x = 1240;
    }
}

void inits() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    font = TTF_OpenFont("air.TTF",32);
    screen = SDL_SetVideoMode(800,600,32,SDL_SWSURFACE);
    color = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
    color2 = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    placar = TTF_RenderText_Solid(font, pontos.c_str(), {210,40,89});
 
}

void colision() {
    if((movFly.x >= movPerson.x && movFly.x <= movPerson.x+105) && (( movFly.y>= movPerson.y && movFly.y <= movPerson.y+60 )|| (movFly.y+55 >= movPerson.y && movFly.y <= movPerson.y+60 ))) {
	Mix_PlayChannel(-1,  mscColision, 0);
	gameOver = true;
    }
    if(movPerson.y >= 520) {
    	movPerson.y = 520;
    }
}

void keyBoard() {
    while(SDL_PollEvent(&event)) {
    
	switch(event.type) {
	
	    case SDL_QUIT:
		running = false;
		break;
	
	    case SDL_KEYDOWN:
    
		if (event.key.keysym.sym == SDLK_SPACE) {
		    saltPressed = true;
		    if (gameOver) {
			pontos = "Pontos:0";
		        fly = 810;
			gameOver = false;
		        score = 0;
			movFly.x = fly;
			movFly.y = rand()%550;
			setRectPonto();
		    	placar = TTF_RenderText_Solid(font, pontos.c_str(), {210,40,89});
		    } else {
			Mix_PlayChannel(-1,  mscFly, 0);
		    }
		}
		if (event.key.keysym.sym == 13) {
		    if(gameOver) {
			running = false;
		    }
		}
		break;

	    case SDL_KEYUP:
  
		if (event.key.keysym.sym == SDLK_SPACE)
		    saltPressed = false;
         
		if (event.key.keysym.sym == SDLK_KP_ENTER) {
		    running = false;
		}
		break;
	    default:
		break;
	}	     
    }
}

void clockOfGame() {
    if( 500/FPS > SDL_GetTicks()-start) SDL_Delay(500/FPS-(SDL_GetTicks()-start));
}

void isGameOver(){
    if(!gameOver) {
    fly-=sort;
    movFly.x = fly;
    if (fly <= -58) {
	score++;
	fly = 810;
	movFly.y = rand()%550;
	sort = rand()%9+1;
	pontos = "Pontos: "+to_string(score);
	placar = TTF_RenderText_Solid(font, pontos.c_str(), {210,40,89});
    }
    if( movFly.y> movPerson.y) {
	movFly.y--;
    } else {
	movFly.y++;
    }		
    if(saltPressed) {
	movPerson.y-=5;
    }
    movPerson.y++;
    } else {
	//cout<<gameOverTxt(score)<<endl;
	//cout<<gameOver<<endl;
	string str = gameOverTxt(score);
	placar = TTF_RenderText_Solid(font,str.c_str(),{210,40,89});
    }
}

void engine(){
    inits();
    thread  delay(delayBlitPerson);
    setGame();
    if (Mix_PlayingMusic() == 0) {
	Mix_PlayMusic(mscMusic, -1);
    }
    
    while(running) {
	cout<<movPerson.y<<endl;
	start = SDL_GetTicks();
	keyBoard();
	colision();
	blitScreen();    
	mov();
	clockOfGame();
	isGameOver();
    } 
    delay.join();
}

void freeScreen() {
    SDL_FreeSurface(text);
    SDL_FreeSurface(imgBackGroung[0]);
    TTF_CloseFont(font);
    SDL_Quit(); 
    Mix_FreeMusic(mscMusic);
    Mix_FreeChunk(mscFly);
}

int main (int argc, char ** argcv) {
    engine();
    freeScreen();
    return 0;
}
