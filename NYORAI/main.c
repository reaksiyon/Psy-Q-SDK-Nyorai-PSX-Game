#include "constants.h"
#include "player.h"
#include "monster.h"
#include "misc.h"


typedef enum { false, true } bool;

void initialize();
void update();
void draw();

Color* color;
u_long* data[34]; //++ when new data added
Sprite* sprites[16]; 


bool gameStart = false;
int gameover;
bool change = false;
bool mapclean = false;
bool creditScreen = false;


int hitposx, hitposy, hittempx, hittempy;
int dHit = 0;

int townCount = 0;// count to go town
int randMap; 

bool firstm1 = true;
bool firstm2 = true;
bool firstm3 = true;
bool takeable = false;

int MrandX = 0; //Random Monster X Pos
int MrandY = 0; //Random Monster Y Pos

int MrandDrop = 0; //Random Drop Rate

int DItemX = 255; //Potion X Pos
int DItemY = 255; //Potion Y Pos

char posX[100];
char posY[100];


int main() {
    initialize();
	
  
    while(1) {
		
		if(gameStart==true)
		{
        update();
		drawStats();
        clear_display();
		
		
        draw();
		
		
        display();
		}
		else
		{
			updateMenu();
			clear_display();
			drawMenu();
			display();
			DrawSync(0);
			VSync(0);
			
		}

		
    }
}

void initialize() {
    initialize_heap();
    initialize_screen();
    initialize_pad();
	initialize_debug_font();
	
    color_create(0, 0, 0, &color); // RGB
    set_background_color(color);
	
	
	//
	InitPlayer();
	InitMonster();
	gameover = 0;
	Pexp = 0;
	Ppot = 0;
	dropID = 0;
	Pgold = 0;
	
    cd_open();
    cd_read_file("PD1.TIM", &data[0]);
	cd_read_file("PD2.TIM", &data[1]);
	cd_read_file("PL1.TIM", &data[2]);
	cd_read_file("PL2.TIM", &data[3]);
	cd_read_file("PR1.TIM", &data[4]);
	cd_read_file("PR2.TIM", &data[5]);
	cd_read_file("PU1.TIM", &data[6]);
	cd_read_file("PU2.TIM", &data[7]);
	cd_read_file("SP1.TIM", &data[8]);
	cd_read_file("SP2.TIM", &data[9]);
	cd_read_file("ROTUE1.TIM", &data[10]);
	cd_read_file("ROTUE2.TIM", &data[11]);
	cd_read_file("ROTUE3.TIM", &data[12]);
	cd_read_file("ARRUP.TIM", &data[13]);
	cd_read_file("ARRRIGHT.TIM", &data[14]);
	cd_read_file("ARRLEFT.TIM", &data[15]);
	cd_read_file("ARRDOWN.TIM", &data[16]);
	cd_read_file("PD.TIM", &data[17]); //PLAYER DEATH
	cd_read_file("SP3.TIM", &data[18]); //SPIDER DEATH
	
	cd_read_file("ATTACK.VAG", &data[19]); //PLAYER ATTACK
	cd_read_file("DEATH.VAG", &data[20]); //ENEMY DEATH
	cd_read_file("WALK.VAG", &data[21]); //PLAYER WALK
	
	cd_read_file("POTION.TIM", &data[22]);
	cd_read_file("NPC.TIM", &data[23]);
	cd_read_file("TOWN.TIM", &data[24]);
	cd_read_file("GOLD.TIM", &data[25]);
	
	cd_read_file("EMPTY.TIM", &data[26]); //REMOVE ITEM
	
	cd_read_file("MENUBG.TIM", &data[27]); //MAIN MENU
	cd_read_file("MENUCR.TIM", &data[28]); //MAIN MENU CREDITS
	cd_read_file("ARROW.TIM", &data[29]); //MAIN MENU ARROW
	
	cd_read_file("MENUBGM.wav", &data[30]); //MAIN MENU BGM
	
	
	
    cd_close();



    sprite_create((u_char *)data[0], Px, Py, &sprites[0]); //PLAYER
	sprite_create_menu((u_char *)data[27], 0, 0, &sprites[9]); // MAIN MENU
	sprite_create_arrow((u_char *)data[29], 70, 128, &sprites[10]); // MENU ARROW



    audio_init();
    audio_transfer_vag_to_spu((u_char *)data[30], SECTOR * 21, SPU_01CH);
    audio_play(SPU_01CH);



	
	

	
}

void InitGAME()
{
    audio_free(SPU_00CH); // STOP MAIN MENU BGM
	
	
	sprite_create_menu((u_char *)data[26], 0, 0, &sprites[9]); // DELETE MENU
	sprite_create_arrow((u_char *)data[26], 0, 0, &sprites[10]); // DELETE ARROW
	
	/*
	sprite_create((u_char *)data[8], 100, 100, &sprites[1]); // ENEMY1
	sprite_create((u_char *)data[8], 100, 100, &sprites[14]); // ENEMY2
	sprite_create((u_char *)data[8], 100, 100, &sprites[15]); // ENEMY3
	*/
	
	sprite_create_map((u_char *)data[10], 0, 0, &sprites[2]); // MAP
	//ARROWS
	sprite_create((u_char *)data[13], 136, -4, &sprites[3]); // ARROW
	sprite_create((u_char *)data[14], 290, 96, &sprites[4]); // ARROW
	sprite_create((u_char *)data[15], -4, 96, &sprites[5]); // ARROW
	sprite_create((u_char *)data[16], 136, 210, &sprites[6]); // ARROW
	
	
}

void mapTeleportCheck()
{
	
		/*
UP: Px>=-8 && Px<=292	Y<=-11
DOWN: Px<=292 && Px>=-8	Y>219
RIGHT: X>=291 Py>=-8 && Py<=210
LEFT: X<=-7 Py<=210 && Py>=-8
	*/
	
	
		if(((Px>=-8 && Px<=292) && Py<=-11) && Prot == 1) //UP ENTERANCE
		{
			sprites[0]->y = 218;
			newMap();
			
		}
		
		
		if(((Px<=292 && Px>=-8) && Py>=219) && Prot == 4 ) //DOWN ENTERANCE
		{
			sprites[0]->y = -10;
			newMap();
			
		}
		
		
		if(((Py>=-8 && Py<=210) && Px>=291) && Prot == 3) //RIGHT ENTERANCE
		{
			sprites[0]->x = -6;
			newMap();
			
		}
		
		if(((Py<=210 && Py>=-8) && Px<=-7) && Prot == 2) //LEFT ENTERANCE
		{
			sprites[0]->x = 290;
			newMap();
			
		}
	
	
}

void newMap()
{
		sprite_create((u_char *)data[26], 500, 500, &sprites[7]); //CLEAR ITEMS ON MAP
		firstm1 = true;
		firstm2 = true;
		firstm3 = true;
	
		townCount++;
		
		
		randMap=1+rand()%4;
			
		
		if(townCount<15)
		{
		
		
		sprite_create((u_char *)data[26], 255, 255, &sprites[8]); 
		if(randMap == 1)
		{
			
			sprite_create_map((u_char *)data[10], 0, 0, &sprites[2]);
			respawnMonster();
		}
		else if(randMap == 2)
		{
			
			sprite_create_map((u_char *)data[11], 0, 0, &sprites[2]);
			respawnMonster();
		}
		else if(randMap == 3)
		{
			
			sprite_create_map((u_char *)data[12], 0, 0, &sprites[2]);
			respawnMonster();
		}
		else
		{
			
			sprite_create_map((u_char *)data[10], 0, 0, &sprites[2]);
			respawnMonster();
		}
		}
		else
		{
			
			sprite_create_map((u_char *)data[24], 0, 0, &sprites[2]);
			respawnNPC();
			townCount = 0;
		}
		
		
	

	
	
	
}

void respawnNPC()
{
	
	mapclean=true; //town
	sprite_create((u_char *)data[23], 196, 36, &sprites[8]); 
	
	
}

void respawnMonster()
{
	Mhealth = 10;
	Mhealth2 = 10;
	Mhealth3 = 10;

	MrandX = 8+rand()%280;
	MrandY = 8+rand()%200;
	
	Mx = MrandX; //Monster Spawn PosX
	My = MrandY; //Monster Spawn PosY
	
	mapclean=false; //Map has monsters
	sprite_create((u_char *)data[8], Mx, My, &sprites[1]); 
	
	MrandX = 8+rand()%280;
	MrandY = 8+rand()%200;
	
	Mx2 = MrandX; //Monster Spawn PosX
	My2 = MrandY; //Monster Spawn PosY
	
	sprite_create((u_char *)data[8], Mx2, My2, &sprites[14]);
	
	MrandX = 8+rand()%280;
	MrandY = 8+rand()%200;
	
	Mx3 = MrandX; //Monster Spawn PosX
	My3 = MrandY; //Monster Spawn PosY
	
	sprite_create((u_char *)data[8], Mx3, My3, &sprites[15]);
	
}

void playerAttack()
{


	PRdmg = 1+rand()%6;
	Pdmg = PRdmg + (Plevel/2);
	
	if((abs(Py-My)<=20 && abs(Px-Mx)<=20))
	{
		Mhealth-=Pdmg;
		hit_font(sprites[1]->x,sprites[1]->y);
		drawHit();
		
	}
	
	if((abs(Py-My2)<=20 && abs(Px-Mx2)<=20))
	{
		Mhealth2-=Pdmg;
		hit_font(sprites[14]->x,sprites[14]->y);
		drawHit();
		
	}
	
	if((abs(Py-My3)<=20 && abs(Px-Mx3)<=20))
	{
		Mhealth3-=Pdmg;
		hit_font(sprites[15]->x,sprites[15]->y);
		drawHit();
		
	}
	//AUDIO
	audio_init();
    audio_transfer_vag_to_spu((u_char *)data[19], SECTOR * 21, SPU_00CH);
    audio_play(SPU_00CH);
	wait();
    //free3(data[19]);
	
	
	mobDead();
	
}

void useItem()
{
	
	if(Ppot>0)
	{
		Phealth += 70;
		if(Phealth > PmaxHealth)
			Phealth = PmaxHealth;
		
		Ppot--;
		wait();
	}
	
	
}


void takeItem()
{
	absRangeX = abs(Px-DItemX);
	absRangeY = abs(Py-DItemY);
	
	if(absRangeX<20 && absRangeY<20 && takeable == true)
	{
		if(dropID == 3)
		{
		Ppot++;
		sprite_create((u_char *)data[26], Mx, My, &sprites[7]); // ITEM REMOVE
		}
		if(dropID == 5)
		{
		Pgold+=10;
		sprite_create((u_char *)data[26], Mx, My, &sprites[7]); //ITEM REMOVE
		}
		
		takeable == false;
		
	}
	
	
	
}

void mobDead()
{

	
		if(Mhealth<=0) //ON ENEMY DEAD
	{
		if(firstm1 == true)
		{
		dHit = 0;
			

			MrandDrop = 1+rand()%8;
			
			if(MrandDrop == 3)
			{
				sprite_create((u_char *)data[22], Mx, My, &sprites[7]); //potion drop
				dropID = 3; //pot ID
				takeable = true;
			}
			
			if(MrandDrop == 5)
			{
				sprite_create((u_char *)data[25], Mx, My, &sprites[7]); //potion drop
				dropID = 5; //gold ID
				takeable = true;
			}

	Pexp += 10;
	checkPlayerLv();
	sprite_create((u_char *)data[26], Mx, My, &sprites[1]);
	DItemX = Mx;
	DItemY = My;
	
	audio_init();
    audio_transfer_vag_to_spu((u_char *)data[20], SECTOR * 21, SPU_00CH);
    audio_play(SPU_00CH);
    //free3(data[20]);
	firstm1 = false;

		

		}
	}
	
	
			if(Mhealth2<=0) //ON ENEMY DEAD
	{
		if(firstm2 == true)
		{
		dHit = 0;
			
		
			MrandDrop = 1+rand()%8;
			
			if(MrandDrop == 3)
			{
				sprite_create((u_char *)data[22], Mx2, My2, &sprites[7]); //potion drop
				dropID = 3; //pot ID
				takeable = true;
			}
			
			if(MrandDrop == 5)
			{
				sprite_create((u_char *)data[25], Mx2, My2, &sprites[7]); //potion drop
				dropID = 5; //gold ID
				takeable = true;
			}

	Pexp += 10;
	checkPlayerLv();
	sprite_create((u_char *)data[26], Mx, My, &sprites[14]);
	DItemX = Mx2;
	DItemY = My2;
	
	audio_init();
    audio_transfer_vag_to_spu((u_char *)data[20], SECTOR * 21, SPU_00CH);
    audio_play(SPU_00CH);
    //free3(data[20]);
	firstm2 = false;
		
		}
	}
	
	
			if(Mhealth3<=0) //ON ENEMY DEAD
	{
		
		if(firstm3 == true)
		{
				
		dHit = 0;
			

			MrandDrop = 1+rand()%8;
			
			if(MrandDrop == 3)
			{
				sprite_create((u_char *)data[22], Mx3, My3, &sprites[7]); //potion drop
				dropID = 3; //pot ID
				takeable = true;
			}
			
			if(MrandDrop == 5)
			{
				sprite_create((u_char *)data[25], Mx3, My3, &sprites[7]); //potion drop
				dropID = 5; //gold ID
				takeable = true;
			}

	Pexp += 10;
	checkPlayerLv();
	sprite_create((u_char *)data[26], Mx, My, &sprites[15]);
	DItemX = Mx3;
	DItemY = My3;
	
	audio_init();
    audio_transfer_vag_to_spu((u_char *)data[20], SECTOR * 21, SPU_00CH);
    audio_play(SPU_00CH);
    //free3(data[20]);
	firstm3 = false;
		

		}
	}
	
	
	if(Mhealth <= 0 && Mhealth2 <= 0 && Mhealth3<= 0)
			mapclean=true;
	
	
}

void checkPlayerLv()
{
	if(Pexp>50 && Plevel == 1)
	{
		Plevel++;
		Pdmg+=2;
		Pexp = 0;
		PmaxHealth += 20;
	}
	
	if(Pexp>60 && Plevel == 2)
	{
		Plevel++;
		Pdmg+=2;
		Pexp=0;
		PmaxHealth += 20;
	}
	
	if(Pexp>70 && Plevel == 3)
	{
		Plevel++;
		Pdmg+=2;
		Pexp=0;
		PmaxHealth += 20;
	}
	
	if(Pexp>80 && Plevel == 4)
	{
		Plevel++;
		Pdmg+=2;
		Pexp=0;
		PmaxHealth += 20;
	}
	
	if(Pexp>90 && Plevel == 5)
	{
		Plevel++;
		Pdmg+=2;
		Pexp=0;
		PmaxHealth += 20;
	}
	
}
void monsterMovement(){

		Mx = sprites[1]->x;
		My = sprites[1]->y;
		
		Mx2 = sprites[14]->x;
		My2 = sprites[14]->y;
		
		Mx3 = sprites[15]->x;
		My3 = sprites[15]->y;
		



				//MONSTER_MOVEMENT
if(change)
{	
//mons1
if(Mhealth>=0)
{
		if(Mx>Px)
		{
		sprites[1]->x -= Mspeed;
		Mx -= Mspeed;
		sprite_create((u_char *)data[8], Mx, My, &sprites[1]);
		}
		else
		{
		sprites[1]->x += Mspeed;
		Mx += Mspeed;
		sprite_create((u_char *)data[8], Mx, My, &sprites[1]);
		}
	
		if(My>Py)
		{
		sprites[1]->y -= Mspeed;
		My -= Mspeed;
		sprite_create((u_char *)data[8], Mx, My, &sprites[1]);
		
		}
		else
		{
		sprites[1]->y += Mspeed;
		My += Mspeed;
		sprite_create((u_char *)data[8], Mx, My, &sprites[1]);
		
		}
		
}

//mons2

if(Mhealth2>=0)
{
		if(Mx2>Px)
		{
		sprites[14]->x -= Mspeed;
		Mx2 -= Mspeed;
		sprite_create((u_char *)data[8], Mx2, My2, &sprites[14]);
		}
		else
		{
		sprites[14]->x += Mspeed;
		Mx2 += Mspeed;
		sprite_create((u_char *)data[8], Mx2, My2, &sprites[14]);
		}
	
		if(My2>Py)
		{
		sprites[14]->y -= Mspeed;
		My2 -= Mspeed;
		sprite_create((u_char *)data[8], Mx2, My2, &sprites[14]);
		
		}
		else
		{
		sprites[14]->y += Mspeed;
		My2 += Mspeed;
		sprite_create((u_char *)data[8], Mx2, My2, &sprites[14]);
		
		}
		
}
		
//mons3

if(Mhealth3>=0)
{
	
		if(Mx3>Px)
		{
		sprites[15]->x -= Mspeed;
		Mx3 -= Mspeed;
		sprite_create((u_char *)data[8], Mx3, My3, &sprites[15]);
		}
		else
		{
		sprites[15]->x += Mspeed;
		Mx3 += Mspeed;
		sprite_create((u_char *)data[8], Mx3, My3, &sprites[15]);
		}
	
		if(My3>Py)
		{
		sprites[15]->y -= Mspeed;
		My3 -= Mspeed;
		sprite_create((u_char *)data[8], Mx3, My3, &sprites[15]);
		
		}
		else
		{
		sprites[15]->y += Mspeed;
		My3 += Mspeed;
		sprite_create((u_char *)data[8], Mx3, My3, &sprites[15]);
		
		}	
		
}
	change=!change;
}
else
{
	if(Mhealth>=0)
	{
		if(Mx>Px)
		{
		sprites[1]->x -= Mspeed;
		Mx -= Mspeed;
		sprite_create((u_char *)data[9], Mx, My, &sprites[1]);
		}
		else
		{
		sprites[1]->x += Mspeed;
		Mx += Mspeed;
		sprite_create((u_char *)data[9], Mx, My, &sprites[1]);
		}
	
		if(My>Py)
		{
		sprites[1]->y -= Mspeed;
		My -= Mspeed;
		sprite_create((u_char *)data[9], Mx, My, &sprites[1]);
		
		}
		else
		{
		sprites[1]->y += Mspeed;
		My += Mspeed;
		sprite_create((u_char *)data[9], Mx, My, &sprites[1]);
		
		}
		
	}
	
	if(Mhealth2>=0)
	{
		//mons2
			if(Mx2>Px)
		{
		sprites[14]->x -= Mspeed;
		Mx2 -= Mspeed;
		sprite_create((u_char *)data[9], Mx2, My2, &sprites[14]);
		}
		else
		{
		sprites[14]->x += Mspeed;
		Mx2 += Mspeed;
		sprite_create((u_char *)data[9], Mx2, My2, &sprites[14]);
		}
	
		if(My2>Py)
		{
		sprites[14]->y -= Mspeed;
		My2 -= Mspeed;
		sprite_create((u_char *)data[9], Mx2, My2, &sprites[14]);
		
		}
		else
		{
		sprites[14]->y += Mspeed;
		My2 += Mspeed;
		sprite_create((u_char *)data[9], Mx2, My2, &sprites[14]);
		
		}
	}
	
	
//mons3

	if(Mhealth3>=0)
	{
		if(Mx3>Px)
		{
		sprites[15]->x -= Mspeed;
		Mx3 -= Mspeed;
		sprite_create((u_char *)data[9], Mx3, My3, &sprites[15]);
		}
		else
		{
		sprites[15]->x += Mspeed;
		Mx3 += Mspeed;
		sprite_create((u_char *)data[9], Mx3, My3, &sprites[15]);
		}
	
		if(My3>Py)
		{
		sprites[15]->y -= Mspeed;
		My3 -= Mspeed;
		sprite_create((u_char *)data[9], Mx3, My3, &sprites[15]);
		
		}
		else
		{
		sprites[15]->y += Mspeed;
		My3 += Mspeed;
		sprite_create((u_char *)data[9], Mx3, My3, &sprites[15]);
		
		}	
		
	}

	
	change=!change;
}

			//GAMEOVER
			if((abs(Py-My)<=20 && abs(Px-Mx)<=20))
			{
				if(Phealth>0)
				{
				Phealth-=5;
				}
				else{
				gameOver();	
					
				}
	
			}
			
			if((abs(Py-My2)<=20 && abs(Px-Mx2)<=20))
			{
				if(Phealth>0)
				{
				Phealth-=5;
				}
				else{
				gameOver();	
					
				}
				
			}
			
			if((abs(Py-My3)<=20 && abs(Px-Mx3)<=20))
			{
				if(Phealth>0)
				{
				Phealth-=5;
				}
				else{
				gameOver();	
					
				}	
			}

	
}



void gameOver()
{
	sprite_create((u_char *)data[17], Px, Py, &sprites[0]);
	gameover=1;
}


void walkSound()
{
	
	audio_init();
    audio_transfer_vag_to_spu((u_char *)data[21], SECTOR * 21, SPU_00CH);
    audio_play(SPU_00CH);
    //free3(data[21]);
}

void updateMenu()
{
	pad_update();
	if(creditScreen == false)
	{
		
	if(pad_check_pressed(Pad1Up)) {
	sprites[10]->x = 70;
	sprites[10]->y = 128;
	}
	
	if(pad_check_pressed(Pad1Down))
	{
	sprites[10]->x = 70;
	sprites[10]->y = 158;
	}
	
	}
	
	if(pad_check_pressed(Pad1Cross))
	{
		
	if(sprites[10]->x == 70 && sprites[10]->y == 128) //GAME START
	{
			InitGAME();
			gameStart = true;
			
	}else if(sprites[10]->x == 70 && sprites[10]->y == 158) //CREDITS
	{
	creditScreen = true;
	sprite_create_menu((u_char *)data[28], 0, 0, &sprites[9]);
	sprites[10]->x = 100;
	sprites[10]->y = 180;
			
	}else if(sprites[10]->x == 100 && sprites[10]->y == 180)
	{
		sprite_create_menu((u_char *)data[27], 0, 0, &sprites[9]);
		sprites[10]->x = 70;
		sprites[10]->y = 128;
		creditScreen = false;
	}
	
		
	}
	
	
}

void update() {
	Px = sprites[0]->x;
	Py = sprites[0]->y;
	
	
    pad_update();
	
	if(gameover==0)
	{
	
	if(pad_check_pressed(Pad1Cross))
		playerAttack();
	
	
	if(pad_check_pressed(Pad1Circle))
		useItem();
	
	if(pad_check_pressed(Pad1Square))
		takeItem();
	

		
	if(pad_check(Pad1Up)) {
		
		if(Py>-12 && (Px>=-8 && Px<=292))
		{
		dHit = 0;
			
		sprites[0]->y -= PSpeed;
		Py = sprites[0]->y;
		sprite_create((u_char *)data[6], Px, Py, &sprites[0]);
		}
		Prot = 1;
		//-
		if(Mhealth>0)
		monsterMovement();
		walkSound();
		wait();
		clear_display();
		drawStats();
		draw();
		display();
		
		if(Py>=-12 && (Px>=-8 && Px<=292))
		{
		sprites[0]->y -= PSpeed;
		Py = sprites[0]->y;
		sprite_create((u_char *)data[7], Px, Py, &sprites[0]);
		}
		wait();
		
	
    }

	if(pad_check(Pad1Down)) {
		
		if(Py<220 && (Px<=292 && Px>=-8))
		{
		dHit = 0;
			
        sprites[0]->y += PSpeed;
		Py = sprites[0]->y;
		sprite_create((u_char *)data[0], Px, Py, &sprites[0]);
		}
		Prot = 4;
		//-
		if(Mhealth>0)
		monsterMovement();
		walkSound();
		wait();
		clear_display();
		drawStats();
		draw();
		display();
		
		if(Py<220 && (Px<=292 && Px>=-8))
		{
        sprites[0]->y += PSpeed;
		Py = sprites[0]->y;
		sprite_create((u_char *)data[1], Px, Py, &sprites[0]);
		}
		wait();
	
    }
	
	if(pad_check(Pad1Right)) {
		
		if(Px<292 && (Py>=-8 && Py<=210))
		{
		dHit = 0;
			
        sprites[0]->x += PSpeed;
		Px = sprites[0]->x;
		sprite_create((u_char *)data[4], Px, Py, &sprites[0]);
		}
		Prot = 3;
		//-
		if(Mhealth>0)
		monsterMovement();
		walkSound();
		wait();
		clear_display();
		drawStats();
		draw();
		display();
		
		if(Px<292 && (Py>=-8 && Py<=210))
		{
        sprites[0]->x += PSpeed;
		Px = sprites[0]->x;
		sprite_create((u_char *)data[5], Px, Py, &sprites[0]);
		}
		wait();
		
    }
	
	if(pad_check(Pad1Left)) {
		
		if(Px>-8 && (Py<=210 && Py>=-8))
		{
		dHit = 0;
			
        sprites[0]->x -= PSpeed;
		Px = sprites[0]->x;
		sprite_create((u_char *)data[2], Px, Py, &sprites[0]);
		}
		Prot = 2;
		//-
		if(Mhealth>0)
		monsterMovement();
		walkSound();
		wait();
		clear_display();
		drawStats();
		draw();
		display();
		
		if(Px>-8 && (Py<=210 && Py>=-8))
		{
        sprites[0]->x -= PSpeed;
		Px = sprites[0]->x;
		sprite_create((u_char *)data[3], Px, Py, &sprites[0]);
		}
		wait();
    }
	
	
		mapTeleportCheck();

	
	
	}//playerHp>0


}




void InitPlayer(){
	Plevel = 1;
	Phealth = 100;
	PmaxHealth = 100;
	Pdmg = 1+rand()%6 + (Plevel/2);
	Px = 136;
	Py = 96;
	PSpeed = 5;
	Prot=4;
}

void InitMonster(){
	Mx = 120;
	My = 160;
	Mx2 = 110;
	My2 = 130;
	Mx3 = 130;
	My3 = 170;
	Mspeed = 2;
	Mhealth = 10;
	Mhealth2 = 10;
	Mhealth3 = 10;
}



void drawHit()
{
	dHit = 1;
	
}

void drawStats() {
	if(dHit == 1)
	{
			FntPrint("- %d", Pdmg);	
			DrawSync(0);
			wait();
			dHit = 0;
	}
	
	
	if(gameover==0)
	{
		
		initialize_debug_font();
		FntPrint("NYORAI Lv. %d",Plevel);
		FntPrint("\n");
		FntPrint("HP: %d",Phealth);
		FntPrint("\n");
		FntPrint("EXP: %d",Pexp);
		FntPrint("\n");
		FntPrint("Gold: %d",Pgold);
		FntPrint("\n\n");
		/*FntPrint("Pot: %d",Ppot);
		FntPrint("\n\n");*/
		
		
	}
	else
	{
		FntPrint("\n\n\n\n\n\n              GAME OVER");

	}
	
	

		
}


drawMenu()
{
	/*
		FntPrint("----DEBUG----\n");
		sprintf(posX, "X: %d",Px);
		FntPrint(posX);
		FntPrint("\n");
		sprintf(posY, "Y: %d",Py);
		FntPrint(posY);
		FntPrint("\n");
		FntPrint("----DEBUG----");
		FntPrint("\n");
		*/

		draw_sprite(sprites[10]); //ARROW
		draw_sprite(sprites[9]);  //MAINMENU
}


void draw() { //layers



	
	
   
   
   draw_sprite(sprites[8]); //NPC
   draw_sprite(sprites[0]); //PLAYER
   draw_sprite(sprites[1]); //MONSTER
   draw_sprite(sprites[14]); //MONSTER2
   draw_sprite(sprites[15]); //MONSTER3
	
	//ARROWS
	
	draw_sprite(sprites[3]);
	draw_sprite(sprites[4]);
	draw_sprite(sprites[5]);
	draw_sprite(sprites[6]);

	draw_sprite(sprites[7]); //DROPITEM

	draw_sprite(sprites[2]); //MAP
	
	
	
}
	







