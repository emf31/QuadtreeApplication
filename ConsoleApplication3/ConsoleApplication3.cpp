// ConsoleApplication3.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cstdlib>
#include <iostream>
#include <ctime>


#include "Quadtree.h"
#include <iostream>



int main()
{
	//Creating allegro display
	
	ALLEGRO_DISPLAY *display = NULL;
	al_init_primitives_addon();
	al_init_primitives_addon();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	//Creating width and height (modify this if your resolution is not full HD)
	const int width = 1000;
	const int height = 1000;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(width, height);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));

	//Number of circuferences (modify if you want more o less circles to demo)
	const int numCircuferences = 10000;
	
	int x, y, zbuffer;
	char again;


	do{
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "-------------------Quadtree-------------------"<<std::endl;
		std::cout << "----------------------------------------------" << std::endl;
		
		clock_t begin_time = clock();
		
		//Creating quadtree (root)
		Quadtree *q = new Quadtree(std::pair<int, int>(0, 0), std::pair<int, int>(width, height),0);
		srand(time(NULL));

		//Loop to create all circles and insert it into quadtree
		for (int i = 0; i < numCircuferences; i++) {

			//Random position and rando depth
			x = rand() % width;
			y = rand() % height;
			zbuffer = rand() % 100;

			//Creating and inserting into quad
			Circle c(i, zbuffer, std::pair<int, int>(x, y));
			q->insert(&c);

			//Draw in allegro
			al_draw_circle(x,y, c.Radius, al_map_rgb(255, 0, 0),1);
		}

		//Time to create quadtree
		std::cout << "Time to create quadtree in milisecods: " << float(clock() - begin_time) / CLOCKS_PER_SEC * 1000 << std::endl;
	
		//Finding random point

		x = rand() % width;
		y = rand() % height;

		//Drawing point selected
		al_draw_filled_circle(x, y, 5, al_map_rgb(0, 0, 255) );
		std::cout << "Searching in point (" << x << "," << y << ")" << std::endl;

		//Finding point in quadtree
		begin_time = clock();
		Circle* mysol=q->findByPoint(std::pair<int, int>(x, y));
		std::cout <<"Time to find circle in milisecods: " << float(clock() - begin_time) / CLOCKS_PER_SEC *1000 <<std::endl;

		//Showing solution if it exists
		if (mysol) {
			std::cout << "Circle display solution idx: " << mysol->Id << std::endl;
			al_draw_circle(mysol->Center.first, mysol->Center.second, mysol->Radius, al_map_rgb(0, 255, 0), 1);
		}
		else {
			std::cout << "No circle founded" << std::endl;
		}

		//Display allegro
		al_flip_display();
	
		//New try wiht y
		std::cout << "Press (y) to try again, else to exit: ";
		std::cin >> again;

		//Deleting quadtree and allegro
		delete q;
		al_destroy_display(display);

		//Make new allegro
		display = al_create_display(width, height);
	} while (again == 'y');
	al_destroy_display(display);
	
	
}

