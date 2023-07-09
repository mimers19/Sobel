#include <iostream>
#include <fstream>
#include <string>

using namespace std;


struct BMPFileHeader {
	int file_type{ 0 };
	int file_size{ 0 };
	int reserved1{ 0 };
	int reserved2{ 0 };
	int offset_data{ 0 };
};

struct BMPInfoHeader {
	int size{ 0 };
	int width{ 0 };
	int height{ 0 };
	int planes{ 0 };
	int bit_count{ 0 };
	int compression{ 0 };
	int size_image{ 0 };
	int x_pixels_per_meter{ 0 };
	int y_pixels_per_meter{ 0 };
	int colors_used{ 0 };
	int colors_important{ 0 };
	int smieci{ 0 };
};

struct piksel {
	int r{ 0 };
	int g{ 0 };
	int b{ 0 };
};

void wczytywanie(BMPFileHeader bfh, BMPInfoHeader bih);
void usuwanie(piksel** obr_1, BMPInfoHeader bih, piksel** obr_2);
void przetwarzanie(piksel** obr_1, BMPInfoHeader bih, BMPFileHeader bfh);
void s1(piksel** obr_1, BMPInfoHeader bih, piksel** obr_2);
void zapisz(piksel** obr_1, BMPInfoHeader bih, BMPFileHeader bfh, piksel** obr_2);
string wpisanie();

int main()
{
	BMPFileHeader bfh;                                          //Definiowanie struktury nagłówka pliku
	BMPInfoHeader bih;                                          //Definiowanie struktury nagłówka informacji

	wczytywanie(bfh, bih);

	system("PAUSE");
	return 0;
}


void wczytywanie(BMPFileHeader bfh, BMPInfoHeader bih)
{
	ifstream ifs;

	ifs.open(wpisanie(), ios::binary | ios::in);

	if (!ifs)
	{
		cout << "Plik nie zostal otwarty\n";

	}
	else
	{
		ifs.read(reinterpret_cast<char*>(&bfh.file_type), 2);
		ifs.read(reinterpret_cast<char*>(&bfh.file_size), 4);
		ifs.read(reinterpret_cast<char*>(&bfh.reserved1), 2);
		ifs.read(reinterpret_cast<char*>(&bfh.reserved2), 2);
		ifs.read(reinterpret_cast<char*>(&bfh.offset_data), 4);


		cout << "Sygnatura pliku:                    " << bfh.file_type << endl;
		cout << "Dlugosc pliku w bajtach:            " << bfh.file_size << endl;
		cout << "Pole zarezerwowane(0):              " << bfh.reserved1 << endl;
		cout << "Pole zarezerwowane2 (0):            " << bfh.reserved2 << endl;
		cout << "Pozycja danych obrazowych w pliku:  " << bfh.offset_data << endl;


		ifs.read(reinterpret_cast<char*>(&bih.size), 4);
		ifs.read(reinterpret_cast<char*>(&bih.width), 4);
		ifs.read(reinterpret_cast<char*>(&bih.height), 4);
		ifs.read(reinterpret_cast<char*>(&bih.planes), 2);
		ifs.read(reinterpret_cast<char*>(&bih.bit_count), 2);
		ifs.read(reinterpret_cast<char*>(&bih.compression), 4);
		ifs.read(reinterpret_cast<char*>(&bih.size_image), 4);
		ifs.read(reinterpret_cast<char*>(&bih.x_pixels_per_meter), 4);
		ifs.read(reinterpret_cast<char*>(&bih.y_pixels_per_meter), 4);
		ifs.read(reinterpret_cast<char*>(&bih.colors_used), 4);
		ifs.read(reinterpret_cast<char*>(&bih.colors_important), 4);


		cout << "Rozmiar naglowka:                   " << bih.size << endl;
		cout << "Szerokosc obrazu w pikselach:       " << bih.width << endl;
		cout << "Wysokosc obrazu w pikselach:        " << bih.height << endl;
		cout << "Liczba platow (1):                  " << bih.planes << endl;
		cout << "Liczba bitow na piksel :            " << bih.bit_count << endl;
		cout << "Algorytm kompresji:                 " << bih.compression << endl;
		cout << "Rozmiar rysunku:                    " << bih.size_image << endl;
		cout << "Rozdzielczosc pozioma:              " << bih.x_pixels_per_meter << endl;
		cout << "Rozdzielczosc pionowa:              " << bih.y_pixels_per_meter << endl;
		cout << "Liczba kolorow w palecie:           " << bih.colors_used << endl;
		cout << "Liczba waznych kolorow w palecie:   " << bih.colors_important << endl;
		//cout << "Smieci:   " << bih.smieci << endl;

		int odj = bfh.offset_data - 54;

		ifs.read(reinterpret_cast<char*>(&bih.smieci), odj);

		piksel** obr_1 = new piksel * [bih.height];                             //Alokowanie pamięci na tablicę wczytanego obrazu
		for (int i = 0; i < bih.height; i++)
		{
			obr_1[i] = new piksel[bih.width];

		}

		int zera = ((bfh.file_size - bfh.offset_data - 3 * bih.width * bih.height) / bih.height);

		for (int h = 0; h < bih.height; h++)                                    //Wczytywanie obrazu bez zer "zapychających"
		{
			for (int w = 0; w < bih.width; w++)
			{
				ifs.read(reinterpret_cast<char*>(&obr_1[h][w].b), 1);
				ifs.read(reinterpret_cast<char*>(&obr_1[h][w].g), 1);
				ifs.read(reinterpret_cast<char*>(&obr_1[h][w].r), 1);

			}
			if (zera != 0)
			{
				ifs.ignore(zera);

			}

		}

		przetwarzanie(obr_1, bih, bfh);


		ifs.close();

	}
}

string wpisanie()
{
	string nazwa;
	cout << "Wpisz nazwe pliku: " << endl;
	cin >> nazwa;
	nazwa = nazwa + ".bmp";
	return nazwa;
}

void przetwarzanie(piksel** obr_1, BMPInfoHeader bih, BMPFileHeader bfh)
{

	piksel** obr_2 = new piksel * [bih.height];                     //Alokowanie pamięci na tablicę nowego obrazu
	for (int i = 0; i < bih.height; i++)
	{
		obr_2[i] = new piksel[bih.width];

	}

	for (int h = 0; h < bih.height; h++)
	{
		for (int w = 0; w < bih.width; w++)
		{
			obr_2[h][w].r = 0;
			obr_2[h][w].g = 0;
			obr_2[h][w].b = 0;

		}

	}

	s1(obr_1, bih, obr_2);                                                          //Operacje masek

	cout << "Wybierz wersje obrazu wynikowego:" << endl << "1. Zwykla (prawidlowa)" << endl << "2. Ostrzejsze kolory" << endl;
	int dzielnik;
	int zmienna;
	cin >> zmienna;
	switch (zmienna)
	{
	case 1:
		dzielnik = 8;
		break;
	case 2:
		dzielnik = 4;
		break;
	default:
		cout << "Podano nieprawidlowe dane!" << endl;
		dzielnik = 8;

	}

	for (int h = 0; h < bih.height; h++)                    //Obliczanie średniej
	{
		for (int w = 0; w < bih.width; w++)
		{
			obr_2[h][w].r = obr_2[h][w].r / dzielnik;
			obr_2[h][w].g = obr_2[h][w].g / dzielnik;
			obr_2[h][w].b = obr_2[h][w].b / dzielnik;


			if (obr_2[h][w].g < 0)                                                  //Zabezpieczenie (NP)
			{
				obr_2[h][w].g = 0;

			}
			else if (obr_2[h][w].g > 255)
			{
				obr_2[h][w].g = 255;

			}

			if (obr_2[h][w].b < 0)
			{
				obr_2[h][w].b = 0;

			}
			else if (obr_2[h][w].b > 255)
			{
				obr_2[h][w].b = 255;

			}

			if (obr_2[h][w].r < 0)
			{
				obr_2[h][w].r = 0;

			}
			else if (obr_2[h][w].r > 255)
			{
				obr_2[h][w].r = 255;

			}


			if (obr_2[h][w].r > 255 || obr_2[h][w].g > 255 || obr_2[h][w].b > 255 || obr_2[h][w].r < 0 || obr_2[h][w].g < 0 || obr_2[h][w].b < 0)
			{
				cout << h << "x" << w << " - " << obr_2[h][w].r << "\t";                                        //"Kod błędu"
				cout << h << "x" << w << " - " << obr_2[h][w].g << "\t";
				cout << h << "x" << w << " - " << obr_2[h][w].b << "\t";

			}

		}

	}



	zapisz(obr_1, bih, bfh, obr_2);
	usuwanie(obr_1, bih, obr_2);
}

void s1(piksel** obr_1, BMPInfoHeader bih, piksel** obr_2)
{
	int tab[8][3][3] = { {{-1, 0, 1},
						  {-2, 0, 2},
						  {-1, 0, 1}},

						  {{0, 1, 2},
						  {-1, 0, 1},
						  {-2, -1, 0}},

						 {{1, 2, 1},
						  {0, 0, 0},
						  {-1, -2, -1}},

						 {{2, 1, 0},
						 {1, 0, -1},
						 {0, -1, -2}},

						{{1, 0, -1},
						 {2, 0, -2},
						 {1, 0, -1}},

						 {{0, -1, -2},
						 {1, 0, -1},
						 {2, 1, 0}},

						{{-1, -2, -1},
						 {0, 0, 0},
						 {1, 2, 1}},

						{{-2, -1, 0},
						 {-1, 0, 1},
						 {0, 1, 2}}
	};
	int bfr_r;
	int bfr_g;
	int bfr_b;

	for (int i = 0; i < 8; i++)
	{
		for (int h = 0; h < bih.height - 2; h++)
		{
			for (int w = 0; w < bih.width - 2; w++)
			{
				bfr_r = tab[i][0][0] * obr_1[h][w].r + tab[i][0][1] * obr_1[h][w + 1].r + tab[i][0][2] * obr_1[h][w + 2].r +
					tab[i][1][0] * obr_1[h + 1][w].r + tab[i][1][1] * obr_1[h + 1][w + 1].r + tab[i][1][2] * obr_1[h + 1][w + 2].r +
					tab[i][2][0] * obr_1[h + 2][w].r + tab[i][2][1] * obr_1[h + 2][w + 1].r + tab[i][2][2] * obr_1[h + 2][w + 2].r;

				bfr_g = tab[i][0][0] * obr_1[h][w].g + tab[i][0][1] * obr_1[h][w + 1].g + tab[i][0][2] * obr_1[h][w + 2].g +
					tab[i][1][0] * obr_1[h + 1][w].g + tab[i][1][1] * obr_1[h + 1][w + 1].g + tab[i][1][2] * obr_1[h + 1][w + 2].g +
					tab[i][2][0] * obr_1[h + 2][w].g + tab[i][2][1] * obr_1[h + 2][w + 1].g + tab[i][2][2] * obr_1[h + 2][w + 2].g;

				bfr_b = tab[i][0][0] * obr_1[h][w].b + tab[i][0][1] * obr_1[h][w + 1].b + tab[i][0][2] * obr_1[h][w + 2].b +
					tab[i][1][0] * obr_1[h + 1][w].b + tab[i][1][1] * obr_1[h + 1][w + 1].b + tab[i][1][2] * obr_1[h + 1][w + 2].b +
					tab[i][2][0] * obr_1[h + 2][w].b + tab[i][2][1] * obr_1[h + 2][w + 1].b + tab[i][2][2] * obr_1[h + 2][w + 2].b;

				if (bfr_r < 0)
				{
					bfr_r = 0;

				}
				else if (bfr_r > 255)
				{
					bfr_r = 255;

				}
				obr_2[h + 1][w + 1].r += bfr_r;

				if (bfr_g < 0)
				{
					bfr_g = 0;

				}
				else if (bfr_g > 255)
				{
					bfr_g = 255;

				}
				obr_2[h + 1][w + 1].g += bfr_g;

				if (bfr_b < 0)
				{
					bfr_b = 0;

				}
				else if (bfr_b > 255)
				{
					bfr_b = 255;

				}
				obr_2[h + 1][w + 1].b += bfr_b;
			}
		}
	}
}

void zapisz(piksel** obr_1, BMPInfoHeader bih, BMPFileHeader bfh, piksel** obr_2)
{
	const int  brak = 0;
	fstream ifs2;
	ifs2.open(wpisanie(), ios::binary | ios::out | ios::trunc);

	int odj = bfh.offset_data - 54;

	ifs2.write((char*)&bfh.file_type, 2);
	ifs2.write((char*)&bfh.file_size, 4);
	ifs2.write((char*)&bfh.reserved1, 2);
	ifs2.write((char*)&bfh.reserved2, 2);
	ifs2.write((char*)&bfh.offset_data, 4);

	ifs2.write((char*)&bih.size, 4);
	ifs2.write((char*)&bih.width, 4);
	ifs2.write((char*)&bih.height, 4);
	ifs2.write((char*)&bih.planes, 2);
	ifs2.write((char*)&bih.bit_count, 2);
	ifs2.write((char*)&bih.compression, 4);
	ifs2.write((char*)&bih.size_image, 4);
	ifs2.write((char*)&bih.x_pixels_per_meter, 4);
	ifs2.write((char*)&bih.y_pixels_per_meter, 4);
	ifs2.write((char*)&bih.colors_used, 4);
	ifs2.write((char*)&bih.colors_important, 4);
	ifs2.write((char*)&bih.smieci, odj);

	int zera = ((bfh.file_size - bfh.offset_data - 3 * bih.width * bih.height) / bih.height);
	int zero = 0;

	for (int h = 0; h < bih.height; h++)                                    //Wczytywanie obrazu bez zer "zapychających"
	{
		for (int w = 0; w < bih.width; w++)
		{
			ifs2.write(reinterpret_cast<char*>(&obr_2[h][w].b), 1);
			ifs2.write(reinterpret_cast<char*>(&obr_2[h][w].g), 1);
			ifs2.write(reinterpret_cast<char*>(&obr_2[h][w].r), 1);

		}
		if (zera != 0)
		{
			ifs2.write(reinterpret_cast<char*>(&zero), zera);

		}

	}

	ifs2.close();
}

void usuwanie(piksel** obr_1, BMPInfoHeader bih, piksel** obr_2)
{
	for (int i = 0; i < bih.height; i++)                         //Usuwanie tablicy dynamicznej
	{
		delete[] obr_1[i];

	}
	delete[] obr_1;

	for (int i = 0; i < bih.height; i++)                         //Usuwanie tablicy dynamicznej
	{
		delete[] obr_2[i];

	}
	delete[] obr_2;
}