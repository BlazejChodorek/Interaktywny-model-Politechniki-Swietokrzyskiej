// ConsoleApplication1.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// stałe do obsługi menu podręcznego
enum
{
	FULL_WINDOW, // aspekt obrazu - całe okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT // wyjście
};

// aspekt obrazu
int aspect = FULL_WINDOW;

// rozmiary bryły obcinania
const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble nearest = 12.0;
const GLdouble farest = 24.0;

// kąty obrotu
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

// wskaźnik naciśnięcia lewego przycisku myszki
int button_state = GLUT_UP;

// położenie kursora myszki
int button_x, button_y;

// współczynnik skalowania
GLfloat scale = 1.0;

// identyfikatory wyświetlanych obiektów
enum
{
	NONE,
	CUBE,
	RED_CUBE,
	GREEN_CUBE,
	BLUE_CUBE,
	WEAiI, //Wydział Elektrotechniki, Automatyki i Informatyki, budynek D - czwarty
	WBiA, //Wydział Budownictwa i Architektury - pierwszy
	WMiBM, //Wydział Mechaniki i Budowy Maszyn, budynek A - trzeci
	WZiMK //Wydział Zarządzania i Modelowania Komputerowego - drugi
	/*
	WISGiE//Wydział Inżynierii Środowiska, Geomatyki i Energetyki - eneris
	*/
};

// nazwa wybranego obiektu
char select_object[30] = "Trafienia: 0";

// funkcja rysująca napis w wybranym miejscu
void DrawString(GLfloat x, GLfloat y, char * string)
{
	// położenie napisu
	glRasterPos2f(x, y);

	// wyświetlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

}

// funkcja rysująca cztery główne budynki
void drawMainBulding(GLfloat spaceBetweenBuildings)
{
	GLfloat size = 0.5f;
	GLfloat height = 0.2f;

	GLfloat position1[] = { /* x */-size, /* y */height, /* z */0.0f + spaceBetweenBuildings };
	GLfloat position2[] = { size, height, 0.0f + spaceBetweenBuildings };

	GLfloat position3[] = { size, -size, 0.0f + spaceBetweenBuildings };
	GLfloat position4[] = { -size, -size, 0.0f + spaceBetweenBuildings };

	GLfloat position5[] = { size, height, 0.0f + size + spaceBetweenBuildings };
	GLfloat position6[] = { -size, height, 0.0f + size + spaceBetweenBuildings };

	GLfloat position7[] = { size, -size, 0.0f + size + spaceBetweenBuildings };
	GLfloat position8[] = { -size, -size, 0.0f + size + spaceBetweenBuildings };


	glColor3f(1.0, 1.0, 0.0);
	//pierwszy
	glBegin(GL_POLYGON);
	glVertex3fv(position1);
	glVertex3fv(position2);
	glVertex3fv(position3);
	glVertex3fv(position4);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3fv(position2);
	glVertex3fv(position5);
	glVertex3fv(position7);
	glVertex3fv(position3);
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3fv(position1);
	glVertex3fv(position6);
	glVertex3fv(position8);
	glVertex3fv(position4);
	glEnd();

	//drugi
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_POLYGON);
	glVertex3fv(position6);
	glVertex3fv(position5);
	glVertex3fv(position7);
	glVertex3fv(position8);
	glEnd();

	//góra
	glColor3f(1.0, 0.5, 0.1);
	glBegin(GL_POLYGON);
	glVertex3fv(position6);
	glVertex3fv(position5);
	glVertex3fv(position2);
	glVertex3fv(position1);
	glEnd();

	//dół
	glColor3f(0.0, 1.0, 0.4);
	glBegin(GL_POLYGON);
	glVertex3fv(position8);
	glVertex3fv(position7);
	glVertex3fv(position3);
	glVertex3fv(position4);
	glEnd();
}


// funkcja generująca scenę 3D

void Display()
{
	// background
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// czyszczenie bufora koloru i bufora głębokości
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesunięcie układu współrzędnych obiektu do środka bryły odcinania
	glTranslatef(0, 0, -(nearest + farest) / 2);

	// obroty obiektu
	glRotatef(rotatex, 1.0, 0.0, 0.0);
	glRotatef(rotatey, 0.0, 1.0, 0.0);

	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);

	// włączenie testu bufora głębokości
	glEnable(GL_DEPTH_TEST);

	// włączenie oświetlenia
	glEnable(GL_LIGHTING);

	// włączenie światła GL_LIGHT0 z parametrami domyślnymi
	glEnable(GL_LIGHT0);

	// włączenie automatycznej normalizacji wektorów normalnych
	glEnable(GL_NORMALIZE);

	// włączenie obsługi właściwości materiałów
	glEnable(GL_COLOR_MATERIAL);

	// inicjalizacja stosu nazw obiektów
	glInitNames();

	// umieszczenie nazwy na stosie nazw, aby nie był on pusty
	glPushName(NONE);

	// obiekt RED_CUBE z grupy CUBE
	glLoadName(CUBE);
	glPushName(RED_CUBE);
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 1.0, 0.0);
	glutSolidCube(0.5);
	glPopName();

	// obiekt GREEN_CUBE z grupy CUBE
	glPushName(GREEN_CUBE);
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(1.0, -1.0, 0.0);
	glutSolidCube(0.5);
	glPopName();

	// obiekt BLUE_CUBE z grupy CUBE
	glPushName(BLUE_CUBE);
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(-1.0, -1.0, 0.0);
	glutSolidCube(0.5);
	glPopName();


	/***** Cztery główne budynki BEGIN ****/

	GLfloat spaceBetweenBuildings = 0.0f;
	glTranslatef(-1.0, 1.0, -1.0);

	// obiekt WEAiI, Wydział Elektrotechniki, Automatyki i Informatyki
	glLoadName(WEAiI);
	drawMainBulding(spaceBetweenBuildings);

	// obiekt WZiMK, Wydział Zarządzania i Modelowania Komputerowego
	glLoadName(WZiMK);
	drawMainBulding(spaceBetweenBuildings += 1.5f);

	// obiekt WMiBM, Wydział Mechaniki i Budowy Maszyn
	glLoadName(WMiBM);
	drawMainBulding(spaceBetweenBuildings += 1.5f);

	// obiekt WBiA, Wydział Budownictwa i Architektury
	glLoadName(WBiA);
	drawMainBulding(spaceBetweenBuildings += 1.5f);

	/***** Cztery główne budynki END ****/


	// wyłączenie oświetlenia
	glDisable(GL_LIGHTING);

	// wyłączenie testu bufora głębokości
	glDisable(GL_DEPTH_TEST);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// trzeba odpowiednio przekształcić układ współrzędnych
	// aby napis znajdował się na samej "górze" bryły obcinania
	glLoadIdentity();
	glTranslatef(0, 0, -nearest);

	// narysowanie napisu o wybranym obiekcie
	glColor3f(1.0, 0.0, 0.0);
	DrawString(left + 0.02, bottom + 0.03, select_object);

	// skierowanie poleceń do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}


void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom * height / width, top * height / width, nearest, farest);
		else

			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left * width / height, right * width / height, bottom, top, nearest, farest);

	}
	else
		glFrustum(left, right, bottom, top, nearest, farest);

	// generowanie sceny 3D
	Display();
}

// obsługa selekcji obietków
void Selection(int x, int y)
{
	// wielkość bufora selekcji
	const int BUFFER_LENGTH = 64;

	// bufor selekcji
	GLuint select_buffer[BUFFER_LENGTH];

	// przygotowanie bufora selekcji
	glSelectBuffer(BUFFER_LENGTH, select_buffer);

	// pobranie obszaru rozmiaru renderingu
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// szerokość i wysokość obszaru renderingu
	int width = viewport[2];
	int height = viewport[3];

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// odłożenie macierzy rzutowania na stos
	glPushMatrix();

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania - jednostkowa kostka dookoła punktu wskaźnika
	// myszy (x,y) rozciągającej się na dwa piksele w poziomie i w pionie
	gluPickMatrix(x, height - y, 2, 2, viewport);

	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom * height / width, top * height / width, nearest, farest);
		else

			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left * width / height, right * width / height, bottom, top, nearest, farest);

	}
	else
		glFrustum(left, right, bottom, top, nearest, farest);

	// włączenie trybu selekcji
	glRenderMode(GL_SELECT);

	// generowanie sceny 3D
	Display();

	// zliczenie ilości rekordów trafień, powrót do domyślnego trybu renderingu
	GLint hits = glRenderMode(GL_RENDER);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// zdjęcie macierzy rzutowania ze stosu
	glPopMatrix();

	// w wyniku slekcji nie wybrano żadnego obiektu
	if (hits == 0)
		strcpy(select_object, "Trafienia: 0");

	// w wyniku selekcji wybrano jeden obiekt
	if (hits == 1)
	{
		// pojedyncza nazwa na stosie - kula
		// sprawdzenie drugiego warunku nie jest konieczne
		if (select_buffer[0] == 1 && select_buffer[3] == WEAiI)
		{
			strcpy(select_object, "Trafienia: 1, obiekt: WEAiI");
		}
		else if (select_buffer[0] == 1 && select_buffer[3] == WBiA)
		{
			strcpy(select_object, "Trafienia: 1, obiekt: WBiA");
		}
		else if (select_buffer[0] == 1 && select_buffer[3] == WMiBM)
		{
			strcpy(select_object, "Trafienia: 1, obiekt: WMiBM");
		}
		else if (select_buffer[0] == 1 && select_buffer[3] == WZiMK)
		{
			strcpy(select_object, "Trafienia: 1, obiekt: WZiMK");
		}

		/*	
		WEAiI, //Wydział Elektrotechniki, Automatyki i Informatyki, budynek D - czwarty
		WBiA, //Wydział Budownictwa i Architektury - pierwszy
		WMiBM, //Wydział Mechaniki i Budowy Maszyn, budynek A - trzeci
		WZiMK //Wydział Zarządzania i Modelowania Komputerowego - drugi
	*/
		else
			// dwie nazwy na stosie - jeden z trzech sześcianów
			// sprawdzenie drugiego warunku nie jest konieczne
			if (select_buffer[0] == 2 && select_buffer[3] == CUBE)

				// sprawdzenie wierzchołka stosu nazw
				switch (select_buffer[4])
				{
					// czerwony sześcan
				case RED_CUBE:
					strcpy(select_object, "Trafienia: 1, obiekt: RED_CUBE");
					break;

					// zielony sześcan
				case GREEN_CUBE:
					strcpy(select_object, "Trafienia: 1, obiekt: GREEN_CUBE");
					break;

					// niebieski sześcan
				case BLUE_CUBE:
					strcpy(select_object, "Trafienia: 1, obiekt: BLUE_CUBE");
					break;
				}
	}

	// w wyniku selekcji wybrano dwa obiekty
	if (hits == 2)
	{
		// pierwszy obiekt - sześcian, drugi obiekt kula
		if (select_buffer[0] == 2 && select_buffer[8] == WEAiI)
		{
			// sprawdzenie, który z obiektów jest bliżej obserwatora
			if (select_buffer[2] > select_buffer[7])
				strcpy(select_object, "Trafienia: 2, pierwszy obiekt: WEAiI");
			else
				// sprawdzenie, który sześcian jest bliżej obserwatora
				switch (select_buffer[4])
				{
					// czerwony sześcan
				case RED_CUBE:
					strcpy(select_object, "Trafienia: 2, pierwszy obiekt: RED_CUBE");
					break;

					// zielony sześcan
				case GREEN_CUBE:
					strcpy(select_object, "Trafienia: 2, pierwszy obiekt: GREEN_CUBE");
					break;

					// niebieski sześcan
				case BLUE_CUBE:
					strcpy(select_object, "Trafienia: 2, pierwszy obiekt: BLUE_CUBE");
					break;
				}
		}
		else
			// pierwszy obiekt - sześcian, drugi obiekt także sześcian
			// z uwagi na kolejność definiowania obiektów przypadek,
			// gdy pierwszym obiektem jest kula a drugim sześcian nie wystąpi
			if (select_buffer[0] == 2 && select_buffer[8] == CUBE)
			{
				// sprawdzenie, który z sześcianów jest bliżej obserwatora
				if (select_buffer[2] > select_buffer[7])

					// drugi sześcian jest bliżej
					switch (select_buffer[9])
					{
						// czerwony sześcan
					case RED_CUBE:
						strcpy(select_object, "Trafienia: 2, pierwszy obiekt: RED_CUBE");
						break;

						// zielony sześcan
					case GREEN_CUBE:
						strcpy(select_object, "Trafienia: 2, pierwszy obiekt: GREEN_CUBE");
						break;

						// niebieski sześcan
					case BLUE_CUBE:
						strcpy(select_object, "Trafienia: 2, pierwszy obiekt: BLUE_CUBE");
						break;
					}
				else

					// pierwszy sześcian jest bliżej
					switch (select_buffer[4])
					{
						// czerwony sześcan
					case RED_CUBE:
						strcpy(select_object, "Trafienia: 2, pierwszy obiekt: RED_CUBE");
						break;

						// zielony sześcan
					case GREEN_CUBE:
						strcpy(select_object, "Trafienia: 2, pierwszy obiekt: GREEN_CUBE");
						break;

						// niebieski sześcan
					case BLUE_CUBE:
						strcpy(select_object, "Trafienia: 2, pierwszy obiekt: BLUE_CUBE");
						break;
					}
			}
	}
}

// obsługa klawiatury

void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.05;
	else

		// klawisz -
		if (key == '-' && scale > 0.05)
			scale -= 0.05;

	// narysowanie sceny
	Display();
}

// obsługa klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;

		// kursor w górę
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;

		// kursor w dół
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obsługa przycisków myszki

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// obsługa selekcji obiektów
		Selection(x, y);
		glutPostRedisplay();

		// zapamiętanie stanu lewego przycisku myszki
		button_state = state;

		// zapamiętanie położenia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

// obsługa ruchu kursora myszki

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatey += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) *(x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) *(button_y - y);
		button_y = y;
		glutPostRedisplay();

		// obsługa selekcji obiektów
		Selection(x, y);
	}
}

// obsługa menu podręcznego
void Menu(int value)
{
	switch (value)
	{
		// obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// wyjście
	case EXIT:
		exit(0);
	}
}

int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary głównego okna programu
	glutInitWindowSize(600, 600);

	// utworzenie głównego okna programu
#ifdef WIN32

	glutCreateWindow("Selekcja obiektow");
#else

	glutCreateWindow("Selekcja obiektow");
#endif

	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(Display);

	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Keyboard);

	// dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	// obsługa przycisków myszki
	glutMouseFunc(MouseButton);

	// obsługa ruchu kursora myszki
	glutMotionFunc(MouseMotion);

	// utworzenie menu podręcznego
	glutCreateMenu(Menu);

	// utworzenie podmenu - aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// menu główne
	glutCreateMenu(Menu);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
#ifdef WIN32

	glutAddMenuEntry("Wyjście", EXIT);
#else

	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// określenie przycisku myszki obsługującej menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}
