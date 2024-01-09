#include "raylib.h"
#include <cmath>
#include <iostream>
#include <vector>

struct player{
	float xPos,yPos;
	float dirX,dirY,planeX,planeY;
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
			if(map[i][j] ==2){DrawRectangle(i*20,j*20,20,20,GREEN);}
		}
	}
}

Vector2 applyTransformation(Vector2 start, Vector2 end, float angle){
	Vector2 newEnd;
	newEnd.x = (end.x-start.x) * cos(angle) - (end.y-start.y) * sin(angle) + start.x;	
	newEnd.y = (end.x-start.x) *sin(angle) + (end.y-start.y) * cos(angle) + start.y;
	return newEnd;
}
Vector2 rotateVector(Vector2 input, float angle){
	Vector2 newVector;
	newVector.x= (input.x) * cos(angle) - (input.y) * sin(angle);
	newVector.y= (input.x) *sin(angle) + (input.y) * cos(angle);
	return newVector;

}
void controlPlayerInput(){
	double rotationconst =.15;
	if(IsKeyDown(KEY_RIGHT)) 
	{
	  Vector2 newDir = rotateVector({user.dirX,user.dirY},rotationconst);	
	  Vector2 newPlaneDir =  rotateVector({user.planeX,user.planeY},rotationconst);
	  user.dirX=newDir.x;
	  user.dirY=newDir.y;
	  user.planeX=newPlaneDir.x;
	  user.planeY=newPlaneDir.y;
	}	

	if(IsKeyDown(KEY_LEFT))
  	{
	  Vector2 newDir = rotateVector({user.dirX,user.dirY},-rotationconst);	
	  Vector2 newPlaneDir =  rotateVector({user.planeX,user.planeY},-rotationconst);
	  user.dirX=newDir.x;
	  user.dirY=newDir.y;
	  user.planeX=newPlaneDir.x;
	  user.planeY=newPlaneDir.y;	
	}
	if(IsKeyDown(KEY_DOWN))
  	{
		user.yPos-= 10* user.dirY;
	  	user.xPos-=10* user.dirX;
	}
	if(IsKeyDown(KEY_UP))
  	{
		user.yPos+= 10*user.dirY;
	  	user.xPos += 10*user.dirX;
	}
}
void drawRay(){ //example of drawing a singular ray using DDA algorithim.
	double cameraX = 0;
	double rayX = user.dirX +user.planeX *cameraX;
	double rayY = user.dirY +user.planeY *cameraX;
	DrawLineEx({user.xPos,user.yPos},{user.xPos+rayX*30,user.yPos+rayY*30},3,GREEN);
	//DDA in future will try Beseneham
	// heavy influence from lodev.org
	
	double changeInXdist= std::abs(1/rayX); 
	double changeInYdist= std::abs(1/rayY);
	
	int xStep;
	int yStep; //indicates wether x step and y step will be in the positive or negative direction. Essentially describes the quadrant in cartesian coordinates that the line is moving towards.
	int mapX = int(user.xPos/20);//20;
	int mapY = int(user.yPos/20);///20;
	//mapY+=1;
	double xxPos = (double) (int(user.xPos)%20)/20.0;
	double yyPos = (double) (int(user.yPos)%20)/20.0;

	double interiorYdist;
	double interiorXdist;
	int hit=0;
	if(rayX < 0)
	{
		xStep=-1;
		interiorXdist =(user.xPos-mapX) * changeInXdist; //- user.xPos;
	}
	if(rayX >= 0)
	{
		xStep=1;
		interiorXdist = (mapX- user.xPos+1.0)* changeInXdist;

	}
	if(rayY >= 0)
	{
		yStep=1;
		interiorYdist = (mapY-user.yPos +1.0) *  changeInYdist;

	}
	if(rayY < 0)
	{
		yStep=-1;
		interiorYdist =   changeInYdist* (user.yPos-mapY); //- user.yPos;

	}
	int count=0;
	std::cout << "intXdist " << interiorXdist << '\n';
	std::cout << "intYdist " << interiorYdist << '\n';
	while(hit == 0 && count <250)
	{
		if(interiorXdist > interiorYdist)
		{
			mapY+=yStep;
			interiorYdist += changeInYdist;


		}
		else
		{
			mapX+=xStep;
			interiorXdist += changeInXdist;
		}
		count++;
		std::cout<< "mapX: "<< mapX<< "mapY: " << mapY<<'\n';
		if(map[mapX][mapY] >0){hit=1;map[mapX][mapY]=2;}
	}
	std::cout<< "mapX: " << mapX << "   " << "mapY: " << mapY;	

}
void updateUser(){
				controlPlayerInput();
				Vector2 userpos = {user.xPos,user.yPos};
				Vector2 userpos2= {user.xPos,user.yPos};	
				DrawCircle(user.xPos,user.yPos,10,MAROON);

				DrawLineV(userpos,{user.xPos+(user.dirX*15),user.yPos+(user.dirY*15)},BLACK); // direction vector
				DrawLineV({user.xPos+(user.dirX*15),user.yPos+(user.dirY*15)},{user.xPos+(user.dirX*15)+user.planeX*10,user.yPos+(user.dirY*15)+user.planeY*10},PINK); // half of camera plane
				DrawLineV({user.xPos+(user.dirX*15),user.yPos+(user.dirY*15)},{user.xPos+(user.dirX*15)-user.planeX*10,user.yPos+(user.dirY*15)-user.planeY*10},PINK); // camerplane
				drawRay();
//				Vector2 mousepos = GetMousePosition();
//				DrawCircle(mousepos.x,mousepos.y,10,BLACK);
//				DrawLineV(mousepos,{user.xPos,user.yPos},GREEN);
}

void createBoundary(){
// create a box 
	for(int i=0; i <20;i++)
	{
		map[i][0]=1;
	}
	for(int i=0; i <20;i++)
	{
		map[19][i]=1;
	}
	for(int i=19; i >= 0;i--)
	{
		map[i][19]=1;
	}
	for(int i=19; i >= 0;i--)
	{
		map[0][i]=1;
	}

}
void checkBoudaries(){







}

int main(void)
{
	 user.xPos=200;
	 user.yPos=200;
	 user.dir=0.0f;
	 user.dirX = -1;
	 user.dirY = 0; // direction player is facing, naturally should be orthog to plane
	 user.planeX = 0;
	 user.planeY = 1;



    InitWindow(800, 400, "raylib [core] example - basic window");
	 SetTargetFPS(30);

	 populateMap();
	 map[5][5]=1;
	 map[5][6]=1;
	 createBoundary();
//	 drawMap();
//	 drawRay();
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
				drawMap();
				DrawFPS(0,0);
				updateUser();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
