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


	ALLEGRO_DISPLAY *display = NULL;
	al_init_primitives_addon();
	al_init_primitives_addon();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(1000, 1000);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const int width = 1000;
	const int height = 1000;
	const int numCircuferences = 1000;
	int x, y, zbuffer;
	char again;


	do{
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "-------------------Quadtree-------------------"<<std::endl;
		std::cout << "----------------------------------------------" << std::endl;
		Quadtree *q = new Quadtree(std::pair<int, int>(0, 0), std::pair<int, int>(width, height),0);
		srand(time(NULL));
		for (int i = 0; i < numCircuferences; i++) {
			x = rand() % width;
			y = rand() % height;
			zbuffer = rand() % 100;
			Circle c(i, zbuffer, std::pair<int, int>(x, y));
			q->insert(&c);
			al_draw_circle(x,y, c.Radius, al_map_rgb(255, 0, 0),1);
		}

	
		x = rand() % width;
		y = rand() % height;
		al_draw_filled_circle(x, y, 5, al_map_rgb(255, 255, 255) );
		std::cout << "Searching in point (" << x << "," << y << ")" << std::endl;
		const clock_t begin_time = clock();
		Circle* mysol=q->findByPoint(std::pair<int, int>(x, y));
		std::cout <<"Time in milisecods: " << float(clock() - begin_time) / CLOCKS_PER_SEC *1000 <<std::endl;

		if (mysol) {
			std::cout << "Circle display solution idx: " << mysol->Id << std::endl;
			al_draw_circle(mysol->Center.first, mysol->Center.second, mysol->Radius, al_map_rgb(0, 255, 0), 1);
		}
		else {
			std::cout << "No circle founded" << std::endl;
		}


		al_flip_display();
	

		std::cout << "Press (y) to try again, else to exit: ";
		std::cin >> again;
		delete q;
		al_destroy_display(display);
		display = al_create_display(1000, 1000);
	} while (again == 'y');
	al_destroy_display(display);
	
	
}

