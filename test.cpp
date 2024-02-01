#include "raylib.h"
//#include <ifstream>
#include <cmath>
#include <iostream>
#include <vector>

struct player{
	float xPos,yPos;
	float dirX,dirY,planeX,planeY;
	float dir;
	float angle;
	int pitch;

}player;

const int tilesize= 20;
struct player user;
std::vector<std::vector<int>> map(tilesize,std::vector<int>(tilesize));

int keymap[3][3] = {{1,0,1},
	{0,1,0},
	{1,0,1}};

void populateMap(){

	for(int i=0;i<tilesize;i+=1){
		for(int j=0;j<tilesize;j+=1){
			map[i][j]=0;
		}
	}


}

void drawMap(){
	for(int i=0;i<tilesize;i+=1){
		for(int j=0;j<tilesize;j+=1){
			if(map[i][j] ==0){DrawRectangleLines(i*20,j*20,20,20,MAROON);}
			if(map[i][j] ==1){DrawRectangle(i*20,j*20,20,20,MAROON);}	
			if(map[i][j] ==2){DrawRectangle(i*20,j*20,20,20,GREEN);}
			if(map[i][j] == 3) {DrawRectangle(i*20,j*20,20,20,GREEN);}
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
	double rotationconst =.1;
	if(user.angle >= 2*PI){
	user.angle=0;

	}

	if(IsKeyDown(KEY_A)){
	user.pitch+=18;
	}
	if(IsKeyDown(KEY_Z)){
	user.pitch-=18;
	}

	if(IsKeyDown(KEY_RIGHT)) 
	{
	  Vector2 newDir = rotateVector({user.dirX,user.dirY},-rotationconst);	
	  Vector2 newPlaneDir =  rotateVector({user.planeX,user.planeY},-rotationconst);
	  user.dirX=newDir.x;
	  user.dirY=newDir.y;
	  user.planeX=newPlaneDir.x;
	  user.planeY=newPlaneDir.y;
	  user.angle-=rotationconst;
	}	

	if(IsKeyDown(KEY_LEFT))
  	{
	  Vector2 newDir = rotateVector({user.dirX,user.dirY},rotationconst);	
	  Vector2 newPlaneDir =  rotateVector({user.planeX,user.planeY},rotationconst);
	  user.dirX=newDir.x;
	  user.dirY=newDir.y;
	  user.planeX=newPlaneDir.x;
	  user.planeY=newPlaneDir.y;	
	  user.angle+=rotationconst;
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
void drawImage(int xStart,int drawStart,int xEnd,int drawEnd){
	int step= (drawEnd-drawStart)/3;

	for(int i=drawStart; i<drawEnd;i+=step)
	{
		
		DrawRectangleV({},{},BLACK);



	}




}
void drawscreen(float perpwalldist,int ray, float NumberofRays,int axislineHit){
		int width =400;
		int maxheight =400;
		int xdiststart = (ray/NumberofRays)*width;
		ray++;
		int xdistend = (ray/NumberofRays)*width;	

		int lineHeight = (int)( maxheight/ (perpwalldist/tilesize));
      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + maxheight / 2 +user.pitch;
      int drawEnd = lineHeight / 2 + maxheight / 2 +user.pitch;
      if(drawStart < 0){drawStart = 0;}
      if(drawEnd >= maxheight){drawEnd = maxheight - 1;}
		DrawRectangleV({width+xdiststart,0},{xdistend-xdiststart,drawStart},BLUE);

		int additive = drawStart + (drawEnd-drawStart);
		int tempg = 400-additive;
		DrawRectangleV({width+xdiststart,drawStart},{xdistend-xdiststart,drawEnd+tempg},GREEN);

		//drawImage(width+xdiststart,drawStart,xdistend-xdiststart,drawEnd-drawStart);
		if(axislineHit == 0){
		DrawRectangleV({width+xdiststart,drawStart},{xdistend-xdiststart,drawEnd-drawStart},BROWN);
		}
		if(axislineHit == 1){
		DrawRectangleV({width+xdiststart,drawStart},{xdistend-xdiststart,drawEnd-drawStart},DARKBROWN);
		}
}
void drawRay(){ //example of drawing a singular ray using DDA algorithim.
	int rays=60;
	for(int iter=0;iter<rays;iter++)
	{
	double cameraX = (2*iter/double(rays))-1;

	double rayX = user.dirX +user.planeX *cameraX;
	double rayY = user.dirY +user.planeY *cameraX;
	//DDA in future will try Beseneham
	// heavy influence from lodev.org

	double changeInXdist= sqrt(1+((rayY*rayY)/(rayX*rayX))); 
	double changeInYdist= sqrt(1+((rayX*rayX)/(rayY*rayY))); 
	
	//double changeInXdist= std::abs(1/rayX); 
	//double changeInYdist= std::abs(1/rayY);

	int xStep;
	int yStep; //indicates wether x step and y step will be in the positive or negative direction. Essentially describes the quadrant in cartesian coordinates that the line is moving towards.
	int mapX = int(user.xPos/20);//20;
	int mapY = int(user.yPos/20);///20;
	double xxPos = (double) ((user.xPos))/20.0;
	double yyPos = (double) ((user.yPos))/20.0;

	double interiorYdist;
	double interiorXdist;
	int hit=0;
	if(rayX < 0)
	{
		xStep=-1;
		interiorXdist =(xxPos-mapX) * changeInXdist; //- user.xPos;
	}
	if(rayX >= 0)
	{
		xStep=1;
		interiorXdist = (mapX- xxPos+1.0)* changeInXdist;

	}
	if(rayY >= 0)
	{
		yStep=1;
		interiorYdist = (mapY-yyPos +1.0) *  changeInYdist;

	}
	if(rayY < 0)
	{
		yStep=-1;
		interiorYdist =   changeInYdist* (yyPos-mapY); //- user.yPos;

	}
	int count=0;
	int axisLineHit=0;
	while(hit == 0 && count <250)
	{
		if(interiorXdist > interiorYdist)
		{
			mapY+=yStep;
			interiorYdist += changeInYdist;
 			axisLineHit=0;
		}
		else
		{
			mapX+=xStep;
			interiorXdist += changeInXdist;
 			axisLineHit=1;
		}
		count++;
		if(map[mapX][mapY] >0){
			if(axisLineHit==0){interiorYdist-=changeInYdist;}
			if(axisLineHit==1){interiorXdist-=changeInXdist;}
			hit=1;
			map[mapX][mapY]=2;
		}
	}
	interiorXdist*=20;
	interiorYdist*=20;
	double angle = atan2(rayY,rayX);
	if(axisLineHit ==0){
		float xdist=interiorYdist*cos(user.angle);
		float ydist= interiorYdist*sin(user.angle);
		Vector2 newvect = rotateVector({xdist,ydist},(angle-user.angle));
		DrawLineEx({user.xPos,user.yPos},{user.xPos+newvect.x ,user.yPos+newvect.y},1,ORANGE);
		float xdist2=interiorYdist*cos(angle-user.angle);
		drawscreen(xdist2,iter,rays,axisLineHit);
	}
	if(axisLineHit ==1){
		float xdist=interiorXdist*cos(user.angle);
		float ydist= interiorXdist*sin(user.angle);
		Vector2 newvect = rotateVector({xdist,ydist},(angle-user.angle));
		DrawLineEx({user.xPos,user.yPos},{user.xPos+newvect.x ,user.yPos+newvect.y},1,BROWN);
		float xdist2=std::abs(interiorXdist*cos(angle-user.angle));
		drawscreen(xdist2,iter,rays,axisLineHit);
	}

	}
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
void readFromFile(){
/*	ifstream file;
	
	assert(file.is_open();
	while(getLine(


*/

	}



int main(void)
{
	 user.xPos=200;
	 user.yPos=200;
	 user.dir=0.0f;
	 user.dirX = -1;
	 user.dirY = 0; // direction player is facing, naturally should be orthog to plane
	 user.planeX = 0;
	 user.planeY = 2.0/3.0;
	 user.angle=PI;	


    InitWindow(800, 400, "raylib [core] example - basic window");
	 SetTargetFPS(30);

	 populateMap();
	 map[5][5]=1;
	 map[5][6]=1;
	 map[7][15]=1;
	 map[8][13]=1;
	 createBoundary();
//	 drawMap();
//	 drawRay();
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
				drawMap();
				createBoundary();
				DrawFPS(0,0);
				updateUser();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
