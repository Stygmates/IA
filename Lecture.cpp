#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>


typedef struct {
	int X;
	int Y;
}Position;

typedef struct{
	Position positionCourante;
	Position positionSortie;
}Ecart;

typedef struct{
	Position positionCourante;
	Position positionSortie;
	int round;
}Ecart2;

typedef struct {
	Position joueur;
	int largeur;
	int hauteur;
	int **laby;
	Position sortie;
}Labyrinthe;

Labyrinthe *charger(char *fichier)
{
	Labyrinthe *labyrinthe = (Labyrinthe*)malloc(sizeof(Labyrinthe));
	    std::fstream myfile(fichier, std::ios_base::in);
	    myfile >> labyrinthe->joueur.X >> labyrinthe->joueur.Y >> labyrinthe->largeur >> labyrinthe->hauteur;
	    labyrinthe->laby = (int**)malloc(labyrinthe->hauteur*sizeof(int*));
	    for(int i = 0; i < labyrinthe->hauteur; i++)
	    {
	    	labyrinthe->laby[i] = (int*)malloc(labyrinthe->largeur*sizeof(int));
	    }
	    for(int j = 0; j < labyrinthe->hauteur; j++)
	    {
	    	for(int k = 0; k < labyrinthe->largeur; k++)
	    	{
	    		myfile >> labyrinthe->laby[j][k];
	    		if(labyrinthe->laby[j][k] == -1)
	    		{
	    			labyrinthe->sortie.Y = j;
	    			labyrinthe->sortie.X = k;
	    		}
	    	}
	    }
	    return labyrinthe;
}

void affichage(Labyrinthe *labyrinthe)
{
	printf("Largeur %d hauteur %d et la sortie est en %d %d\n", labyrinthe->largeur, labyrinthe->hauteur, labyrinthe->sortie.X, labyrinthe->sortie.Y);
	for(int i = 0; i < labyrinthe->hauteur; i++)
	{
		for(int j = 0; j < labyrinthe->largeur; j++)
		{
			printf("%d ", labyrinthe->laby[i][j]);
		}
		printf("\n");
	}
}

void suppression(Labyrinthe *labyrinthe)
{
	for(int i = 0; i < labyrinthe->hauteur; i++)
	{
		free(labyrinthe->laby[i]);
	}
	free(labyrinthe->laby);
	free(labyrinthe);
}

/*int eloignementsortie(Labyrinthe *labyrinthe, Position joueur)
{
	int valeur = 0;
	if(joueur.X > labyrinthe->sortie.X)
	{
		valeur = joueur.X - labyrinthe->sortie.X;
	}
	else
	{
		valeur = labyrinthe->sortie.X - joueur.X;
	}

	if(joueur.Y > labyrinthe->sortie.Y)
	{
		valeur = valeur + joueur.Y - labyrinthe->sortie.Y;
	}
	else
	{
		valeur = valeur + labyrinthe->sortie.Y - joueur.Y;
	}
	return valeur;
}*/

int eloignementsortie(Ecart position)
{
	int valeur = 0;
	//printf("Comparaison de %d %d %d %d\n",position.positionCourante.X, position.positionCourante.Y, position.positionSortie.X, position.positionSortie.Y);
	if(position.positionCourante.X > position.positionSortie.X)
	{
		valeur = position.positionCourante.X - position.positionSortie.X;
	}
	else
	{
		valeur = position.positionSortie.X - position.positionCourante.X;
	}

	if(position.positionCourante.Y > position.positionSortie.Y)
	{
		valeur = valeur + position.positionCourante.Y - position.positionSortie.Y;
	}
	else
	{
		valeur = valeur + position.positionSortie.Y - position.positionCourante.Y;
	}
	return valeur;
}

int eloignementsortie(Ecart2 position)
{
	int valeur = 0;
	//printf("Comparaison de %d %d %d %d\n",position.positionCourante.X, position.positionCourante.Y, position.positionSortie.X, position.positionSortie.Y);
	if(position.positionCourante.X > position.positionSortie.X)
	{
		valeur = position.positionCourante.X - position.positionSortie.X;
	}
	else
	{
		valeur = position.positionSortie.X - position.positionCourante.X;
	}

	if(position.positionCourante.Y > position.positionSortie.Y)
	{
		valeur = valeur + position.positionCourante.Y - position.positionSortie.Y;
	}
	else
	{
		valeur = valeur + position.positionSortie.Y - position.positionCourante.Y;
	}
	return valeur;
}

struct less_than_key
{
    inline bool operator() (const Ecart &position1, const Ecart &position2)
    {
        return (eloignementsortie(position1) < eloignementsortie(position2));
    }
};

struct less_than_key2
{
    inline bool operator() (const Ecart2 &position1, const Ecart2 &position2)
    {
        return (eloignementsortie(position1)+position1.round < eloignementsortie(position2)+position2.round);
    }
};

void bestFirst(Labyrinthe *labyrinthe)
{
	int i = 0;
	std::vector<Ecart> open;
	Ecart ecart;
	ecart.positionSortie = labyrinthe->sortie;
	ecart.positionCourante = labyrinthe->joueur;
	open.push_back(ecart);
	Ecart next;
	next.positionSortie = labyrinthe->sortie;
	while(!open.empty())
	{
		sleep(1);
		i++;
		//printf("%d\n", i);
		std::sort(open.begin(),open.end(),less_than_key());
		Ecart ecartCourant = open.at(0);
		open.erase(open.begin());
		labyrinthe->laby[ecartCourant.positionCourante.Y][ecartCourant.positionCourante.X] = 2;
		//std::cout << ecartCourant.positionCourante.X << " " << ecartCourant.positionCourante.Y << "\n"; 
		affichage(labyrinthe);
		if(eloignementsortie(ecartCourant) == 0)
		{
			printf("Sortie trouvée!\n");
			return;
		}
		else
		{
			next.positionCourante.X = ecartCourant.positionCourante.X-1;
			next.positionCourante.Y = ecartCourant.positionCourante.Y;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X+1;
			next.positionCourante.Y = ecartCourant.positionCourante.Y;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X;
			next.positionCourante.Y = ecartCourant.positionCourante.Y-1;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X;
			next.positionCourante.Y = ecartCourant.positionCourante.Y+1;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
		}
		//sleep(50);
	}
}

void aStar(Labyrinthe *labyrinthe)
{
	int round = 0;
	std::vector<Ecart2> open;
	Ecart2 ecart;
	ecart.positionSortie = labyrinthe->sortie;
	ecart.positionCourante = labyrinthe->joueur;
	ecart.round = 0;
	open.push_back(ecart);
	Ecart2 next;
	next.positionSortie = labyrinthe->sortie;
	while(!open.empty())
	{
		sleep(1);
		round++;
		//printf("%d\n", i);
		std::sort(open.begin(),open.end(),less_than_key2());
		Ecart2 ecartCourant = open.at(0);
		open.erase(open.begin());
		labyrinthe->laby[ecartCourant.positionCourante.Y][ecartCourant.positionCourante.X] = 2;
		//std::cout << ecartCourant.positionCourante.X << " " << ecartCourant.positionCourante.Y << "\n"; 
		affichage(labyrinthe);
		if(eloignementsortie(ecartCourant) == 0)
		{
			printf("Sortie trouvée!\n");
			return;
		}
		else
		{
			next.positionCourante.X = ecartCourant.positionCourante.X-1;
			next.positionCourante.Y = ecartCourant.positionCourante.Y;
			next.round = round;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X+1;
			next.positionCourante.Y = ecartCourant.positionCourante.Y;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X;
			next.positionCourante.Y = ecartCourant.positionCourante.Y-1;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X;
			next.positionCourante.Y = ecartCourant.positionCourante.Y+1;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
		}
		//sleep(50);
	}
}

void coutUniforme(Labyrinthe *labyrinthe)
{
	int i = 0;
	std::vector<Ecart> open;
	Ecart ecart;
	ecart.positionSortie = labyrinthe->sortie;
	ecart.positionCourante = labyrinthe->joueur;
	open.push_back(ecart);
	Ecart next;
	next.positionSortie = labyrinthe->sortie;
	while(!open.empty())
	{
		usleep(500);
		i++;
		//printf("%d\n", i);
		//std::sort(open.begin(),open.end(),less_than_key());
		Ecart ecartCourant = open.at(0);
		open.erase(open.begin());
		labyrinthe->laby[ecartCourant.positionCourante.Y][ecartCourant.positionCourante.X] = 2;
		//std::cout << ecartCourant.positionCourante.X << " " << ecartCourant.positionCourante.Y << "\n"; 
		affichage(labyrinthe);
		if(eloignementsortie(ecartCourant) == 0)
		{
			printf("Sortie trouvée!\n");
			return;
		}
		else
		{
			next.positionCourante.X = ecartCourant.positionCourante.X-1;
			next.positionCourante.Y = ecartCourant.positionCourante.Y;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X+1;
			next.positionCourante.Y = ecartCourant.positionCourante.Y;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X;
			next.positionCourante.Y = ecartCourant.positionCourante.Y-1;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
			next.positionCourante.X = ecartCourant.positionCourante.X;
			next.positionCourante.Y = ecartCourant.positionCourante.Y+1;
			if(labyrinthe->laby[next.positionCourante.Y][next.positionCourante.X] <= 0)
			{
				open.push_back(next);
			}
		}
		//sleep(50);
	}
}

void draw()
{
	
}
int main(int argc, char *argv[])
{
	if(argc == 2)
	{
		Labyrinthe *labyrinthe = charger(argv[1]);
		
		//affichage(labyrinthe);
		coutUniforme(labyrinthe);
		suppression(labyrinthe);
	}
	else
	{
		printf("Nombre d'argument incorrect\n");
		glutInit(&argc, argv);
		//Simple buffer
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
		glutInitWindowPosition(50,25);
		glutInitWindowSize(500,250);
		glutCreateWindow("Green window");
		//Call to the drawing function
		glutDisplayFunc(draw);
		glutMainLoop();
		return 0;
	}


	return 1;
}

/*
		std::vector<Ecart> vecteur;
		Position position;
		position.X = 5;
		position.Y = 7;	
		Position position2;
		position2.X = 6;
		position2.Y = 8;
		Position position3;
		position3.X = 5;
		position3.Y = 7;
		Ecart ecart;
		ecart.positionSortie = position;
		ecart.positionCourante = position2;
		Ecart ecart2;
		ecart2.positionSortie = position;
		ecart2.positionCourante = position3;

		vecteur.push_back(ecart);
		vecteur.push_back(ecart2);

		std::vector<Ecart>::iterator it;  // declare an iterator to a vector of strings
		for(it=vecteur.begin() ; it < vecteur.end(); it++) {
			std::cout << it->positionCourante.X << "\n";
		}		
		std::sort(vecteur.begin(),vecteur.end(),less_than_key());
		for(it=vecteur.begin() ; it < vecteur.end(); it++) {
			std::cout << it->positionCourante.X << "\n";
		}
*/