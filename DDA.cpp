// C++ program for DDA line generation 
#include "raylib.h"
#include <bits/stdc++.h> 
using namespace std; 

// function for rounding off the pixels 
int round(float n) 
{ 
	if (n - (int)n < 0.5) 
		return (int)n; 
	return (int)(n + 1); 
} 

// Function for line generation 
void DDALine(int x0, int y0, int x1, int y1) 
{ 

	// Calculate dx and dy 
	int dx = x1 - x0; 
	int dy = y1 - y0; 

	int step; 

	// If dx > dy we will take step as dx 
	// else we will take step as dy to draw the complete 
	// line 
	if (abs(dx) > abs(dy)) 
		step = abs(dx); 
	else
		step = abs(dy);
	dx*=20;
   dy*=20;	
	// Calculate x-increment and y-increment for each step 
	float x_incr = (float)dx / step; 
	float y_incr = (float)dy / step; 
	
	// Take the initial points as x and y 
	float x = x0; 
	float y = y0; 

	for (int i = 0; i < step; i++) { 

		// putpixel(round(x), round(y), WHITE); 
		cout << x << " " << y << "\n"; 
		DrawCircle(x,y,2,RED);
		x += x_incr; 
		y += y_incr; 
		// delay(10); 
	} 
} 
void drawMap(){
	for(int i=0; i<800;i+=20)
	{
		for(int j=0; j<800;j+=20)
		{
			DrawRectangleLines(i,j,20,20,BLACK);




		}



	}


}
// Driver code 
int main() 
{ 

	int x0 = 510, y0 = 690, x1 = 180, y1 = 160;
//	:DDALine(x0, y0, x1, y1); 
  
	InitWindow(800, 800, "raylib [core] example - basic window");
   SetTargetFPS(30);
	// Function call 
 while (!WindowShouldClose())
     {
          BeginDrawing();
              ClearBackground(RAYWHITE);
               DrawFPS(0,0);
					drawMap();
					DDALine(x0, y0, x1, y1); 
           EndDrawing();
       
	  }   
       CloseWindow();

	return 0; 
} 

// all functions regarding to graphichs.h are commented out 
// contributed by hopelessalexander
