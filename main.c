#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
	#include <stdio.h>
#include <math.h>

	
volatile int pixel_buffer_start; // global variable 

typedef struct Ball{
	int x;
	int y; 
	int radius; 
	int dx;
	int dy; 
	short color;
}Ball; 

enum gameStatus{
	menu,
	help,
	game,
	gameover,
	paused,
	difficultySelect,
	gamewon
}; 

enum difficulty{
	easy,
	medium,
	hard
};

enum gameStatus status; 
enum difficulty diff; 

void clear_screen();
void drawBall(const Ball *ball, short int color); 
void wait_for_vsync(); 
void plot_pixel(int x, int y, short int line_color); 
void readKeyboard(unsigned char *clickedKey);
void redrawPlayer(Ball *ball, unsigned char var);
void draw_string(int x, int y, char str[]);
void clear_text();
int generateRandomNum(int lower, int upper);
void redrawRandomBall(Ball *ball,Ball *previousRandomBall);
void resetPlayerBall (Ball *ball);

Ball playerBall = {160,120,10, 0,0};
Ball previousBall =  {160,120,10, 0,0};
Ball randomBall = {0,0,0,0,0};
Ball previousRandomBall = {0,0,0,0,0};
	
int main(void) {
	
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	pixel_buffer_start = *pixel_ctrl_ptr;
	
	int count=0;
	int consumedNum = 0;

	unsigned char clickedKey = 0; 
	clear_screen();
	clear_text();


	status = menu;
	diff = easy;
	Ball randomBallArray[10];
	Ball previousBallArray[10];
	drawBall (&playerBall,0x07E0); 
	while(true){
		//------------------------------------------------menu------------------------------------------------
		if (status == menu){
			clear_screen();
			clear_text();
			count = 0;
			
			draw_string(31, 10, "!!!Arcade Agario!!!");
			draw_string(22, 40, "press space to start or 1 for help");

			resetPlayerBall(&playerBall);
			readKeyboard(&clickedKey);
				if (clickedKey == 0x29) {
					clear_screen();
					clear_text();
					status = difficultySelect;
				}else if (clickedKey == 0x16){
					clear_screen();
					clear_text();
					status = help; 
				}
		}//-------------------------------------------difficultty-select----------------------------------------
		else if (status == difficultySelect){
			clear_screen(); 
			draw_string(30, 15, "Select Difficulty");
			draw_string(20, 20, "Press E for Easy M for Medium or H for Hard");
			readKeyboard(&clickedKey); 
			if (clickedKey == 0x24){
				clear_screen();
				clear_text();
				diff = easy;
				status = game;
			}
			else if (clickedKey == 0x3A){
				clear_screen();
				clear_text();
				diff = medium;
				status = game;
			}
			else if (clickedKey == 0x33){
				clear_screen();
				clear_text();
				diff = hard;
				status = game;
			}
				
		}//------------------------------------------------help------------------------------------------------
		else if (status == help){
			draw_string(30,15, "How to Play/Rules:");
			draw_string(20,20, "1. Use the arrow keys to control your player");
			draw_string(10,25, "2. As long as your player is bigger than enemy you can eat them");
			draw_string(10,30, "3. if enemy is bigger than you and you hit them you will be eaten");
			draw_string(24,35, "----->Press Space to start<-----");
			
			readKeyboard(&clickedKey);
			if (clickedKey == 0x29){
					clear_screen();
					clear_text();
					status = menu;
			}
		}//------------------------------------------------game------------------------------------------------
		else if (status == game){
			if (count==0)draw_string(1, 1, "Balls Consumed: 0");
			wait_for_vsync();
			//draw previous balls to refresh screen
			drawBall(&previousBall,0x0000);
		
			//create random balls
			if (diff == medium){
				if (count ==0){
					for (int i=0; i<5; i++){			
						randomBallArray[i].x = generateRandomNum(10,100);
						randomBallArray[i].y = generateRandomNum(10,90);
						randomBallArray[i].radius = generateRandomNum(5,20);
						randomBallArray[i].dx = generateRandomNum(-2,2);
						randomBallArray[i].dy = generateRandomNum(-2,2);
						//randomize colour
						short color_array [10] = {0xf800,0x001f,0xffe0};
						randomBallArray[i].color = color_array [rand()%3];
						drawBall(&randomBallArray[i],randomBallArray[i].color);				
					}		
					for (int i=5; i<10; i++){			
						randomBallArray[i].x = generateRandomNum(200,310);
						randomBallArray[i].y = generateRandomNum(140,230);
						randomBallArray[i].radius = generateRandomNum(5,20);
						randomBallArray[i].dx = generateRandomNum(-2,2);
						randomBallArray[i].dy = generateRandomNum(-2,2);
						//randomize colour
						short color_array [10] = {0xf800,0x001f,0xffe0};
						randomBallArray[i].color = color_array [rand()%3];
						drawBall(&randomBallArray[i],randomBallArray[i].color);				
					}
				}
			}
			else if (diff == easy){
				if (count ==0){
					for (int i=0; i<5; i++){			
						randomBallArray[i].x = generateRandomNum(10,100);
						randomBallArray[i].y = generateRandomNum(10,90);
						randomBallArray[i].radius = generateRandomNum(5,15);
						randomBallArray[i].dx = generateRandomNum(-1,1);
						randomBallArray[i].dy = generateRandomNum(-1,1);
						//randomize colour
						short color_array [10] = {0xf800,0x001f,0xffe0};
						randomBallArray[i].color = color_array [rand()%3];
						drawBall(&randomBallArray[i],randomBallArray[i].color);				
					}		
					for (int i=4; i<10; i++){			
						randomBallArray[i].x = generateRandomNum(200,310);
						randomBallArray[i].y = generateRandomNum(140,230);
						randomBallArray[i].radius = generateRandomNum(5,15);
						randomBallArray[i].dx = generateRandomNum(-1,1);
						randomBallArray[i].dy = generateRandomNum(-1,1);
						//randomize colour
						short color_array [10] = {0xf800,0x001f,0xffe0};
						randomBallArray[i].color = color_array [rand()%3];
						drawBall(&randomBallArray[i],randomBallArray[i].color);				
					}
				}
			}
			else if (diff == hard){
				if (count ==0){
					for (int i=0; i<5; i++){			
						randomBallArray[i].x = generateRandomNum(10,100);
						randomBallArray[i].y = generateRandomNum(10,90);
						randomBallArray[i].radius = generateRandomNum(5,25);
						randomBallArray[i].dx = generateRandomNum(-3,3);
						randomBallArray[i].dy = generateRandomNum(-3,3);
						//randomize colour
						short color_array [10] = {0xf800,0x001f,0xffe0};
						randomBallArray[i].color = color_array [rand()%3];
						drawBall(&randomBallArray[i],randomBallArray[i].color);				
					}		
					for (int i=5; i<10; i++){			
						randomBallArray[i].x = generateRandomNum(200,310);
						randomBallArray[i].y = generateRandomNum(140,230);
						randomBallArray[i].radius = generateRandomNum(5,25);
						randomBallArray[i].dx = generateRandomNum(-3,3);
						randomBallArray[i].dy = generateRandomNum(-3,3);
						//randomize colour
						short color_array [10] = {0xf800,0x001f,0xffe0};
						randomBallArray[i].color = color_array [rand()%3];
						drawBall(&randomBallArray[i],randomBallArray[i].color);				
					}
				}
			}
		//draw previous ball
		if (count !=0){
			for (int i=0; i<10; i++){
			drawBall(&previousBallArray[i],0x0000);	
			}
		}else if (count ==0){
			for (int i=0; i<10; i++){
			drawBall(&randomBallArray[i],0x0000);	
				count++;
			}
		}
			
		//redraw player ball
		readKeyboard(&clickedKey);	
		redrawPlayer(&playerBall, clickedKey);
		drawBall(&playerBall,0x07E0);
			
		//redraw random ball
		for (int i=0; i<10; i++){
			redrawRandomBall(&randomBallArray[i], &previousBallArray[i]);
			drawBall(&randomBallArray[i],randomBallArray[i].color);
		}
		
		
		int player_x1, player_x2, player_y1, player_y2;
		int random_x1, random_x2, random_y1, random_y2;
			

		char score[] = "Balls Consumed: ";
		char num_str[10];
			for (int i=0; i<10; i++){
				if (consumedNum>=10)status = gamewon;
			player_x1 = playerBall.x-playerBall.radius;
			player_x2 = playerBall.x+playerBall.radius;
			player_y1 = playerBall.y-playerBall.radius;
			player_y2 = playerBall.y+playerBall.radius;
			random_x1 = randomBallArray[i].x-randomBallArray[i].radius;
			random_x2 = randomBallArray[i].x+randomBallArray[i].radius;
			random_y1 = randomBallArray[i].y-randomBallArray[i].radius;
			random_y2 = randomBallArray[i].y+randomBallArray[i].radius;
			if (player_x1 < random_x2 && player_x2 > random_x1 && player_y1 < random_y2 && player_y2 > random_y1){
				if (playerBall.radius>=randomBallArray[i].radius){	
					clear_text();
					consumedNum++;
					playerBall.radius+=1;
					randomBallArray[i].radius=0;
					randomBallArray[i].x=0;
					randomBallArray[i].y=0;
					randomBallArray[i].dx=0;
					randomBallArray[i].dy=0;
					sprintf(num_str, "%d", consumedNum);
					strcat(score, num_str);
					draw_string(1, 1, score);
				}else status = gameover;
			}
		
		}
		//------------------------------------------------gameover------------------------------------------------
		}else if (status == gameover){
			clear_screen();
			draw_string(35, 30, "Gameover");
			draw_string(30, 25, "Press enter to restart");
				
			readKeyboard(&clickedKey);
			if (clickedKey == 0x5A){
				status = menu; 
				count =0;
			}
		}else if (status ==gamewon){
			clear_screen();
			draw_string(25, 25, "All Balls Consumed! Gamewon!");
			draw_string(30, 30, "Press enter to restart");
				
			readKeyboard(&clickedKey);
			if (clickedKey == 0x5A){
				status = menu; 
				count =0;
			}
		}
			
	}
}

void redrawPlayer(Ball *ball, unsigned char var){
	//for arrow key inputs 
	if (var == 0x72)
		ball->dy = 3; 
	else if (var == 0x75)
		ball->dy = -3;
	else if (var == 0x74)
		ball->dx = 3;
	else if (var == 0x6B)
		ball->dx = -3;
	else{
		ball->dx = 0;
		ball->dy = 0;
	}
	//add the delta 
	(ball->y) += (ball->dy); 
	(ball->x) += (ball->dx);
	//border cases for x 
	if (ball->x - ball->radius < 0 || ball->x + ball->radius > 320) {
		ball->x = ball->x - ball->dx;
		ball->dx = 0;}
	//border cases for y 
	if (ball->y - ball->radius < 0 || ball->y + ball->radius > 240) {
		ball->y = ball->y - ball->dy;
		ball->dy = 0;}
	previousBall = *ball;
}

void redrawRandomBall(Ball *ball, Ball *previousRandomBall){
	(ball->y) += (ball->dy);
	(ball->x) += (ball->dx);
	//border cases for x
	if (ball->x - ball->radius < 0 || ball->x + ball->radius > 320) {
		ball->x = ball->x - ball->dx;
		ball->dx = -1*ball->dx;}
	//border cases for y 
	if (ball->y - ball->radius < 0 || ball->y + ball->radius > 240) {
		ball->y = ball->y - ball->dy;
		ball->dy = -1*ball->dy;}
	
	*previousRandomBall = *ball;
}


//------------------------------helper functions-------------------------------------------------
void drawBall(const Ball *ball, short int color){
	for (int i = ball->x - ball->radius; i < ball->x + ball->radius; i++) {
		for (int j = ball->y - ball->radius; j < ball->y + ball->radius; j++) {
			plot_pixel(i, j, color);
		}
	}
}

void resetPlayerBall (Ball *ball){
	ball->radius=10;
	ball->x=160;
	ball->y=120;
	ball->dx=0;
	ball->dy=0;
}


int generateRandomNum(int lower, int upper){
	int num = (rand() % (upper - lower + 1)) + lower;
	return num; 
}

void wait_for_vsync(){
	volatile int *pixel_ctrl_ptr = (int*)0xFF203020; // pixel controller
	register int status;
	
	*pixel_ctrl_ptr = 1; // start the synchronization process
	
	do {
		status = *(pixel_ctrl_ptr + 3);
	} while ((status & 0x01) != 0); // wait for s to become 0
}

void clear_screen(){
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            plot_pixel(x, y, 0x0000);
        }
    }
}

void plot_pixel(int x, int y, short int line_color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void readKeyboard(unsigned char *clickedKey){
	int dataVar; 
	volatile int * PS2_ptr = (int *) 0xFF200100;
	dataVar = *PS2_ptr;
	*clickedKey = dataVar & 0xFF;
	//loop 
	while (dataVar & 0x8000) {
		dataVar = *PS2_ptr;
	}
}

void draw_string(int x, int y, char str[]) {
	for (int i = 0; i < strlen(str); i++) {
			volatile int charBuffer = 0xc9000000;
	*(char *)(charBuffer + (y << 7) + x+i) = str[i];
	}
}

void clear_text() {
	for (int x = 0; x < 80; x++) {
		for (int y = 0; y < 60; y++) {
			volatile int charBuffer = 0xc9000000;
	*(char *)(charBuffer + (y << 7) + x) = ' ';
		}
	}
}
