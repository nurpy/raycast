#include "raylib.h"
#include <cmath>
#include <iostream>
#include <vector>
//temp
//
//
//
//
//
//
//

struct player{
	int xPos,yPos;
	double dirX,dirY,planeX,planeY;
	float dir;

}player;

struct player user;
std::vector<std::vector<int>> map(20,std::vector<int>(20));
const int tilesize= 20;

void populateMap(){

	for(int i=0;i<20;i+=1){
		for(int j=0;j<20;j+=1){
			map[i][j]=0;
		}
	}


}

void drawMap(){
	for(int i=0;i<20;i+=1){
		for(int j=0;j<20;j+=1){
			if(map[i][j] ==0){DrawRectangleLines(i*20,j*20,20,20,MAROON);}
			if(map[i][j] ==1){DrawRectangle(i*20,j*20,20,20,MAROON);}
		}
	}
}

Vector2 applyTransformation(Vector2 start, Vector2 end, float angle){
	Vector2 newEnd;
	newEnd.x = (end.x-start.x) * cos(angle) - (end.y-start.y) * sin(angle) + start.x;	
	newEnd.y = (end.x-start.x) *sin(angle) + (end.y-start.y) * cos(angle) + start.y;
	return newEnd;
}

void controlPlayerInput(){
	if(IsKeyDown(KEY_RIGHT)) {user.dir+= .15;} 
	if(IsKeyDown(KEY_LEFT)) {user.dir -= .15;}
	if(IsKeyDown(KEY_DOWN)) {user.yPos+= 10*sin(user.dir); user.xPos+=10*cos(user.dir);}
	if(IsKeyDown(KEY_UP)) {user.yPos-= 10*sin(user.dir); user.xPos -= 10*cos(user.dir);}
}

void updateUser(){
				controlPlayerInput();
				Vector2 userpos = {user.xPos,user.yPos};
				Vector2 userpos2= {user.xPos,user.yPos};	
				DrawCircle(user.xPos,user.yPos,10,MAROON);
				Vector2 newEnd = applyTransformation(userpos,{user.xPos-25,user.yPos},user.dir);
				DrawLineV(userpos,newEnd,MAROON);
//				Vector2 mousepos = GetMousePosition();
//				DrawCircle(mousepos.x,mousepos.y,10,BLACK);
//				DrawLineV(mousepos,{user.xPos,user.yPos},GREEN);
}
void drawRay(){
	double cameraX = 1;
	double rayX = user.dirX +user.planeX *cameraX;
	double rayY = user.dirY +user.planeY *cameraX;
	
	DrawLineV({user.xPos,user.yPos},{user.xPos+rayX,user.yPos+rayY},BLACK);


}

void checkBoudaries(){







}

int main(void)
{
	 user.xPos=200;
	 user.yPos=200;
	 user.dir=0.0f;
	 user.dirX = -1;
	 user.dirY = 0;
	 user.planeX = 0;
	 user.planeY = 10;



    InitWindow(800, 400, "raylib [core] example - basic window");
	 SetTargetFPS(30);

	 populateMap();
	 map[5][5]=1;
	 map[5][6]=1;

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
				drawMap();
				DrawFPS(0,0);
				updateUser();
				drawRay();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
