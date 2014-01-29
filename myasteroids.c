//Lily Peng
// 11/15/12

#include "myLib.h"
#include "bigcookie.h"
#include "smallcookie.h"
#include "smallercookie.h"
#include "mint.h"
#include "lives3.h"
#include "lives2.h"
#include "lives1.h"
#include "north.h"
#include "northeast.h"
#include "east.h"
#include "southeast.h"
#include "south.h"
#include "southwest.h"
#include "west.h"
#include "northwest.h"
#include "title.h"
#include "gameover.h"
#include "story.h"
#include "win.h"

#include <stdlib.h>

//global variables
Asteroid asteroid[MAXASTEROIDS];
Bullet bullet[MAXBULLETS];
Ship player;

int bulletDelay = 0;
int playerDelay = 0;
int astDelay = 0;
int state = 0;

const int xdir[8]= { 0, 1,1,1,0,-1,-1,-1};	//x coordinates of directions
const int ydir[8]= {-1,-1,0,1,1, 1, 0,-1};	//y coordinates of directions

// Initialize player struct
void initPlayer() 
{
	player.dir = 0;		//facing up
	player.vc = 0;		//no initial velocity
	player.vr = 0;	
	player.lives = 3;
	player.c = 110;		//starting location in the middle. player.c = column = x coordinate
	player.r = 60;		//player.r = row = y coordinate
	player.turncounter = 0;
}

void initAsteroids() 
{
	int i;
	for (i = 0; i < MAXASTEROIDS; i++) 
	{
		(asteroid[i]).c = 0;
		(asteroid[i]).r = 0;
		(asteroid[i]).alive = 0;
		(asteroid[i]).dir = 0;
		(asteroid[i]).size = 0;
	}
}

// Initialize bullet struct
void initBullets() 
{
	int i;
	for (i = 0; i < MAXBULLETS; i++) 
	{
		(bullet[i]).c = 0;
		(bullet[i]).r = 0;
		(bullet[i]).life = 0;	//all bullets dead
		(bullet[i]).dir = 0;
	}
}

//only happens once at the very beginning
//asteroid[0-9] active
void fillAsteroids()
{
	for (int i = 0; i < STARTASTEROIDS; i++)
	{
		int r = rand() % 2; //0 or 1
		if (r == 0) //either 0 or 1: 0 is left
		{
			(asteroid[i]).c = (rand() % 11) + 10; //random 10-20
			(asteroid[i]).r = (rand() % 141) + 10; //10-150
		}
		else if (r == 1) //right
		{
			(asteroid[i]).c = (rand() % 11) + 220; //random 220-230
			(asteroid[i]).r = (rand() % 141) + 10; //10-150
		}
		
		(asteroid[i]).alive = 1;
		(asteroid[i]).dir = rand() % 8; //random 0-7
		(asteroid[i]).size = rand() % 2;	//size 1 is big, size 0 is small
	}
}

//make smaller asteroids when a big one is destroyed
void replaceAsteroid(int x, int y)
{
	int count = 0;
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		if ((asteroid[i]).alive == 0)	//finds an empty space in the array for an asteroid
		{
			count++;
			(asteroid[i]).alive = 1;
			(asteroid[i]).dir = rand() % 8; //random 0-7
			(asteroid[i]).c = x;
			(asteroid[i]).r = y;
			(asteroid[i]).size = 0;	//size 1 is big, size 0 is small
			if (count == 2)
				break;
		}
	}

}

//returns 0 if no collision, 1 if collision
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	int c1 = x1 + w1 - 1 >= x2;
	int c2 = x2 + w2 - 1 >= x1;
	int c3 = y1 + h1 - 1 >= y2;
	int c4 = y2 + h2 - 1 >= y1;
	
	return c1 && c2 && c3 && c4;
}

void moveAsteroid()
{
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		if (((asteroid[i]).alive) == 1)
		{
			(asteroid[i]).c += (xdir[(asteroid[i]).dir]);
			(asteroid[i]).r += (ydir[(asteroid[i]).dir]);
			
			//bouncing asteroids
			if ((asteroid[i]).c < 0)
			{	
				(asteroid[i]).c = 0;
				if (((asteroid[i]).dir == 0) || ((asteroid[i]).dir == 2) || ((asteroid[i]).dir == 4) || ((asteroid[i]).dir == 6))
				{
					(asteroid[i]).dir += 4;
				}  
				else if (((asteroid[i]).dir == 1) || ((asteroid[i]).dir == 5))
				{
					(asteroid[i]).dir += 6;
				}
				else if ((asteroid[i]).dir == 7 || ((asteroid[i]).dir == 3))
				{
					(asteroid[i]).dir += 2;
				}

				if ((asteroid[i]).dir > 7)
				{
					(asteroid[i]).dir -= 8;	
				}
			}
			else if ((asteroid[i]).size == 0)
			{
				if (((asteroid[i]).c + SMALLERCOOKIE_WIDTH) > (GBAWIDTH))
				{
					(asteroid[i]).c -= 1;
					if (((asteroid[i]).dir == 0) || ((asteroid[i]).dir == 2) || ((asteroid[i]).dir == 4) || ((asteroid[i]).dir == 6))
					{
						(asteroid[i]).dir += 4;
					}  
					else if (((asteroid[i]).dir == 1) || ((asteroid[i]).dir == 5))
					{
						(asteroid[i]).dir += 6;
					}
					else if ((asteroid[i]).dir == 7 || ((asteroid[i]).dir == 3))
					{
						(asteroid[i]).dir += 2;
					}
	
					if ((asteroid[i]).dir > 7)
					{
						(asteroid[i]).dir -= 8;	
					}
				}
			}
			else if ((asteroid[i]).size == 1)
			{
				if (((asteroid[i]).c+ SMALLCOOKIE_WIDTH) > (GBAWIDTH))
				{
					(asteroid[i]).c -= 1;
					if (((asteroid[i]).dir == 0) || ((asteroid[i]).dir == 2) || ((asteroid[i]).dir == 4) || ((asteroid[i]).dir == 6))
					{
						(asteroid[i]).dir += 4;
					}  
					else if (((asteroid[i]).dir == 1) || ((asteroid[i]).dir == 5))
					{
						(asteroid[i]).dir += 6;
					}
					else if ((asteroid[i]).dir == 7 || ((asteroid[i]).dir == 3))
					{
						(asteroid[i]).dir += 2;
					}
	
					if ((asteroid[i]).dir > 7)
					{
						(asteroid[i]).dir -= 8;	
					}	
				}
			}
	
			if ((asteroid[i]).r < 0) 
			{
				(asteroid[i]).r = 0;
				if (((asteroid[i]).dir == 0) || ((asteroid[i]).dir == 2) || ((asteroid[i]).dir == 4) || ((asteroid[i]).dir == 6))
				{
					(asteroid[i]).dir += 4;
				}  
				else if (((asteroid[i]).dir == 3) || ((asteroid[i]).dir == 7))
				{
					(asteroid[i]).dir += 6;
				}
				else if ((asteroid[i]).dir == 1 || ((asteroid[i]).dir == 5))
				{
					(asteroid[i]).dir += 2;
				}

				if ((asteroid[i]).dir > 7)
				{
					(asteroid[i]).dir -= 8;	
				}

			}
			else if ((asteroid[i]).size == 0)
			{
				if (((asteroid[i]).r + SMALLERCOOKIE_HEIGHT) > (GBAHEIGHT))
				{
					(asteroid[i]).r -= 1;
					if (((asteroid[i]).dir == 0) || ((asteroid[i]).dir == 2) || ((asteroid[i]).dir == 4) || ((asteroid[i]).dir == 6))
					{
						(asteroid[i]).dir += 4;
					}  
					else if (((asteroid[i]).dir == 3) || ((asteroid[i]).dir == 7))
					{
						(asteroid[i]).dir += 6;
					}
					else if ((asteroid[i]).dir == 1 || ((asteroid[i]).dir == 5))
					{
						(asteroid[i]).dir += 2;
					}
	
					if ((asteroid[i]).dir > 7)
					{
						(asteroid[i]).dir -= 8;	
					}
				}
			}
			else if ((asteroid[i]).size == 1)
			{
				if (((asteroid[i]).r+ SMALLCOOKIE_HEIGHT) > (GBAHEIGHT))
				{
					(asteroid[i]).r -= 1;
					if (((asteroid[i]).dir == 0) || ((asteroid[i]).dir == 2) || ((asteroid[i]).dir == 4) || ((asteroid[i]).dir == 6))
					{
						(asteroid[i]).dir += 4;
					}  
					else if (((asteroid[i]).dir == 3) || ((asteroid[i]).dir == 7))
					{
						(asteroid[i]).dir += 6;
					}
					else if ((asteroid[i]).dir == 1 || ((asteroid[i]).dir == 5))
					{
						(asteroid[i]).dir += 2;
					}
	
					if ((asteroid[i]).dir > 7)
					{
						(asteroid[i]).dir -= 8;	
					}
				}
			}
			
			//unit collision with bullet
			for (int i = 0; i < MAXBULLETS; i++)
			{
				if ((bullet[i]).life > 0)
				{
					for (int j = 0; j < MAXASTEROIDS; j++)
					{
						if ((asteroid[j]).alive == 1)
						{
							if ((asteroid[j]).size == 0)
							{
								if (collision((asteroid[j]).c, (asteroid[j]).r, SMALLERCOOKIE_WIDTH, SMALLERCOOKIE_HEIGHT, (bullet[i]).c, (bullet[i]).r, MINT_WIDTH, MINT_HEIGHT) == 1)
								{
									(asteroid[j]).alive = 0; //small asteroids DIE
									(bullet[i]).life = 0;
								}
							}
							else if ((asteroid[j]).size == 1)
							{
								if (collision((asteroid[j]).c, (asteroid[j]).r, SMALLCOOKIE_WIDTH, SMALLCOOKIE_HEIGHT,(bullet[i]).c, (bullet[i]).r, MINT_WIDTH, MINT_HEIGHT) == 1)
								{
									(asteroid[j]).alive = 0;
									(bullet[i]).life = 0;
									replaceAsteroid((asteroid[j]).c, (asteroid[j]).r);
								}
							}
						
						}
					}
				}
			
			}

			//unit collision with player
			if ((asteroid[i]).size == 0)
			{
				if (collision(player.c, player.r, NORTH_WIDTH, NORTH_HEIGHT, (asteroid[i]).c, (asteroid[i]).r, SMALLERCOOKIE_WIDTH, SMALLERCOOKIE_HEIGHT) == 1)
				{
					playerDelay--;	
					if (playerDelay % 15 == 0)
					{
						player.lives--;
						if (player.lives <= 0)
						{
							state = 2;	//gameover
						}
					}
				}
			}
			else if ((asteroid[i]).size == 1)
			{
				if (collision(player.c, player.r, NORTH_WIDTH, NORTH_HEIGHT, (asteroid[i]).c, (asteroid[i]).r, SMALLCOOKIE_WIDTH, SMALLCOOKIE_HEIGHT) == 1)
				{
					playerDelay--;	
					if (playerDelay % 15 == 0)
					{
						player.lives--;
						if (player.lives <= 0)
						{
							state = 2;	//gameover
						}
					}
				}
			}	
		}
	}
}



/* drawimage3
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	int i;
	for (i = 0; i < height; i++)
	{
		DMA[3].cnt = 0;
		DMA[3].src = &image[IMGOFFSET(i, 0,width)];
		DMA[3].dst = &videoBuffer[OFFSET(r+i,c)];
		DMA[3].cnt = DMA_ON | width;
	}
}

void drawLives()
{
	if (player.lives == 3)
		drawImage3(150, 0, LIVES3_WIDTH, LIVES3_HEIGHT, (u16*)lives3);
	else if (player.lives == 2)
		drawImage3(150, 0, LIVES2_WIDTH, LIVES2_HEIGHT, (u16*)lives2);
	else if (player.lives == 1)
		drawImage3(150, 0, LIVES1_WIDTH, LIVES1_HEIGHT, (u16*)lives1);
}

void drawAsteroid()
{
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		if (((asteroid[i]).alive) == 1)
		{
			if ((asteroid[i]).size == 0)
				drawImage3((asteroid[i]).r, (asteroid[i]).c, SMALLERCOOKIE_WIDTH, SMALLERCOOKIE_HEIGHT, (u16*)smallercookie);
			else if ((asteroid[i]).size == 1)
				drawImage3((asteroid[i]).r, (asteroid[i]).c, SMALLCOOKIE_WIDTH, SMALLCOOKIE_HEIGHT, (u16*)smallcookie);
		}
	}
}

void movePlayer() 
{
	player.c += player.vc;
	player.r += player.vr;
	
	if (player.c <= -NORTH_WIDTH) 		//edge scrolling
	{	
		player.c += GBAWIDTH;
	}
	else if (player.c > GBAWIDTH)
	{
		player.c = 0;
	}
	
	if (player.r <= -NORTH_HEIGHT) 
	{
		player.r += GBAHEIGHT;
	}
	else if (player.r > GBAHEIGHT)
	{
		player.r = 0;
	}
}

void moveBullet()
{
	for (int i = 0; i < MAXBULLETS; i++)
	{
		if ((bullet[i]).life > 0)	//if bullet is alive
		{
			//move bullet depending on dir player is facing
			if ((bullet[i]).dir == 0)
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i]).dir == 1)	//1 is northeast
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i]).dir == 2)	//2 is east
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i]).dir == 3)	//3 is southeast
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i]).dir == 4)	//4 is south
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i].dir) == 5)	//5 is southwest
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i]).dir == 6)	//6 is west
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			else if ((bullet[i]).dir == 7)	//7 is northwest
			{
				(bullet[i]).c += (xdir[(bullet[i]).dir]) * 3;
				(bullet[i]).r += (ydir[(bullet[i]).dir]) * 3;
			}
			
			(bullet[i]).life--;
			
			if ((bullet[i]).c <= -5) 		//edge scrolling
			{	
				(bullet[i]).c += GBAWIDTH;
			}
			else if ((bullet[i]).c > GBAWIDTH)
			{
				(bullet[i]).c = 0;
			}
	
			if ((bullet[i]).r <= -5) 
			{
				(bullet[i]).r += GBAHEIGHT;
			}
			else if ((bullet[i]).r > GBAHEIGHT)
			{
				(bullet[i]).r = 0;
			}
		}
	}
}

void drawBullet()
{
	for (int i = 0; i < MAXBULLETS; i++)
	{
		if ((bullet[i]).life > 0)
			drawImage3((bullet[i]).r, (bullet[i]).c, MINT_WIDTH, MINT_HEIGHT, (u16*)mint);
	}
}

//When A or B is pressed, shoot a bullet:
void shoot()
{
	bulletDelay--;	
	if (bulletDelay % 4 == 0)
	{
		for (int i = 0; i < MAXBULLETS; i++)
		{
			if ((bullet[i]).life <= 0)
			{
				(bullet[i]).r = player.r + (NORTH_HEIGHT / 2) - (MINT_HEIGHT / 2);	//mint start at middle
				(bullet[i]).c = player.c + (NORTH_WIDTH / 2) - (MINT_HEIGHT / 2);
				(bullet[i]).life = BULLMAXLIFE;		//lifetime of a bullet
				(bullet[i]).dir = player.dir;
				break;
			}
		}
	}
}

void reset()
{
	state = 0;
	initAsteroids();
	initPlayer();
	initBullets();
	fillAsteroids();
}

void keyPresses()
{
		if(KEY_DOWN_NOW(BUTTON_UP))
		{
			player.vr+= ydir[player.dir];
			if (player.vr > MAXSPEED)
			{
				player.vr = MAXSPEED;
			}
			if (player.vr < -MAXSPEED)
			{
				player.vr = -MAXSPEED;
			}
			
			player.vc+= xdir[player.dir];
			if (player.vc > MAXSPEED)
			{
				player.vc = MAXSPEED;
			}
			if (player.vc < -MAXSPEED)
			{
				player.vc = -MAXSPEED;
			}
		}

		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{	
			player.vr /= 2;
			player.vc /= 2;
		}	
		if(KEY_DOWN_NOW(BUTTON_LEFT))
		{	
			player.turncounter--;	
			if (player.turncounter % 3 == 0)
			{
				if (player.dir == 0)
				{
					player.dir += NUMDIR-1;
				}			
				else
				{
					player.dir -= 1;
				}
			}
		}		
		
		if(KEY_DOWN_NOW(BUTTON_RIGHT))
		{	
			player.turncounter++;	
			if (player.turncounter % 5 == 0)
			{
				if (player.dir == 7)
				{
					player.dir = 0;
				}			
				else
				{
					player.dir += 1;
				}
			}
		}
		
		//shoot
		if((KEY_DOWN_NOW(BUTTON_A)) | (KEY_DOWN_NOW(BUTTON_B)))	
		{
			shoot();
		}
		
		if((KEY_DOWN_NOW(BUTTON_SELECT)))	
		{
			reset();
		}
		
}

void drawPlayer()
{
		if (player.dir == 0)		//0 is north
		{
			drawImage3(player.r, player.c,NORTH_WIDTH,NORTH_HEIGHT,(u16*)north);
		}
		else if (player.dir == 1)	//1 is northeast
		{
			drawImage3(player.r, player.c,NORTHEAST_WIDTH,NORTHEAST_HEIGHT,(u16*)northeast);
		}
		else if (player.dir == 2)	//2 is east
		{
			drawImage3(player.r, player.c,EAST_WIDTH,EAST_HEIGHT,(u16*)east);
		}
		else if (player.dir == 3)	//3 is southeast
		{
			drawImage3(player.r,player.c,SOUTHEAST_WIDTH,SOUTHEAST_HEIGHT,(u16*)southeast);
		}
		else if (player.dir == 4)	//4 is south
		{
			drawImage3(player.r,player.c,SOUTH_WIDTH,SOUTH_HEIGHT,(u16*)south);
		}
		else if (player.dir == 5)	//5 is southwest
		{
			drawImage3(player.r,player.c,SOUTHWEST_WIDTH,SOUTHWEST_HEIGHT,(u16*)southwest);
		}
		else if (player.dir == 6)	//6 is west
		{
			drawImage3(player.r,player.c,WEST_WIDTH,WEST_HEIGHT,(u16*)west);
		}
		else if (player.dir == 7)	//7 is northwest
		{
			drawImage3(player.r,player.c,NORTHWEST_WIDTH,NORTHWEST_HEIGHT,(u16*)northwest);
		}	
}

void checkWin()
{
	int count = 0;
	for (int i = 0; i < MAXASTEROIDS; i++)
	{
		if (((asteroid[i]).alive) == 1)
			count++;
	}
	
	if (count == 0)
		state = 4;
		
}

void gameState()	//state==1
{
	//Input
	keyPresses();
		
	//Calculations
	moveBullet();
	moveAsteroid();
	movePlayer();
	checkWin();
		
	//Draw
	waitForVblank();
	fillScreen(WHITE);
	drawBullet();
	drawPlayer();		//draws kitty based on what direction she's facing
	drawAsteroid();
	drawLives();
}

void titleState()	//state==0
{
	if(KEY_DOWN_NOW(BUTTON_START))
	{
		state = 3;	//goes to story
	}
	if(KEY_DOWN_NOW(BUTTON_SELECT))
	{
		reset();	//restarts game
	}
	
	waitForVblank();
	fillScreen(WHITE);
	drawImage3(0,0,TITLE_WIDTH,TITLE_HEIGHT,(u16*)title);
}

void storyState()
{
	if(KEY_DOWN_NOW(BUTTON_DOWN))
	{
		state = 1;	//goes to game
	}
	if(KEY_DOWN_NOW(BUTTON_SELECT))
	{
		reset();	//restarts game
	}
	
	waitForVblank();
	fillScreen(WHITE);
	drawImage3(0,0,STORY_WIDTH,STORY_HEIGHT,(u16*)story);
}

void gameOverState()	//state==2
{
	if(KEY_DOWN_NOW(BUTTON_SELECT))
	{
		reset();	//restarts game
	}
	waitForVblank();
	fillScreen(WHITE);
	drawImage3(0,0,GAMEOVER_WIDTH,GAMEOVER_HEIGHT,(u16*)gameover);
}

void winState()
{
	if(KEY_DOWN_NOW(BUTTON_SELECT))
	{
		reset();	//restarts game
	}
	waitForVblank();
	fillScreen(WHITE);
	drawImage3(0,0,WIN_WIDTH,WIN_HEIGHT,(u16*)win);
}

int main()
{
	initAsteroids();
	initPlayer();
	initBullets();

	REG_DISPCTL = MODE3 | BG2_ENABLE;
	fillScreen(WHITE);
	fillAsteroids();	
	
	//GAME LOOP
	while(1)
	{		
		if (state == 0)
			titleState();
		else if (state == 1)
			gameState();
		else if (state == 2)
			gameOverState();
		else if (state == 3)
			storyState();
		else if (state == 4)
			winState();
	}
	return 0;


	
}



