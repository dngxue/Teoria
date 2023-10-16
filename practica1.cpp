/*

Para compilar:
gcc practica1.cpp -o p1.exe -lstdc++



*/

#include <iostream>
#include <stdlib.h>
#include <regex>
#include <string>

using namespace std;

void leerAlfabeto(string &alfa, string &repAlfa);
void leerCadenas(string &w1, string &w2, string alfa);
void compararCadenas(string &w1, string &w2);
void expresionRegular();

int main()
{
    //alfabeto
    string alfa;
	string repAlfa;
    //cadenas
    string w1;
    string w2;

    leerAlfabeto(alfa, repAlfa);
    regex alfabeto(alfa);
    cout<<"\nEl alfabeto ingresado es: "<<repAlfa<<endl;
    leerCadenas(w1,w2,alfa);
    expresionRegular();


	return 0;
}

void leerAlfabeto(string &alfa, string &repAlfa)
{
	char op;
	string input = "";

	system("cls");
	alfa = "";
	repAlfa = "";
	
	cout<<"\t\033[96m1) DEFINIR UN ALFABETO\033[0m"<<endl;
	cout<<"\t1. El alfabeto debe tener al menos tres simbolos"<<endl;
	cout<<"\t2. Los simbolos pueden ser ingresados:"<<endl;
	cout<<"\t\t a. De manera individual\t \033[33mEjemplo: a, b, c, e\033[0m"<<endl;
	cout<<"\t\t b. Por rango\t \033[33mEjemplo: m-x\033[0m"<<endl;
	
	do{

		cout<<"\tIngrese la opcion en \033[31ma\033[0m o \033[31mb\033[0m"<<endl;
		cin >> op;

		if(op == 'a'){

			cout<<"\nIngrese el alfabeto:"<<endl;

			bool tresSimbolos = false;

			while(!tresSimbolos)
			{
				alfa = "";
				repAlfa = "";
				int cont = 0;
				tresSimbolos = false;
				cin >> input;

				for(int i = 0; i < input.length(); i++)
				{
					if(input[i] == ',') continue;
					else
					{
						int contRepeated = 0;
						bool repeated = false;
						// Revisar si se repiten
						for(int j = 0; j < alfa.length(); j++)
						{
							if(alfa[j] == input[i])	contRepeated++;
						}
						if(contRepeated > 0)	repeated = true;
						if(!repeated)
						{
							alfa += input[i];
							repAlfa += input[i];
							if(i != input.length() - 1)
								repAlfa += ",";
							cont++;
						}	
					}	
				}

				if(cont >= 3)	tresSimbolos = true;
				else
				{
					cout << "\nIngrese de nuevo el alfabeto con al menos 3 símbolos:" << endl;
				}	
			}
			
			alfa = '[' + alfa + ']';
			if(repAlfa[repAlfa.length()-1] == ',')
			{
				repAlfa[repAlfa.length()-1] = '}';
				repAlfa = '{' + repAlfa;
			}
			else
				repAlfa = '{' + repAlfa + '}';
		}
		else if(op == 'b')
		{
			cout << "\nIngrese el alfabeto por rango:" << endl;
			bool guion = false;

			while(!guion)
			{
				alfa = "";
				repAlfa = "";
				cin >> alfa;

				if(alfa.length() == 3 && alfa[1] == '-' && alfa[0] <= alfa[2] && alfa[2] - alfa[0] >= 3)
				{
					repAlfa = '{' ;
					repAlfa += alfa[0];
					repAlfa += ", ..., ";
					repAlfa += alfa[2];
					repAlfa += '}';
					alfa = '[' + alfa + ']';
					guion = true;
				}
				else if (alfa.length() == 3 && alfa[1] == '-' && alfa[2] - alfa[0] < 2 )
				{
					cout<<"\nIngrese de nuevo el alfabeto con al menos 3 símbolos y en orden correcto:"<<endl;
				}
				else
				{
					cout<<"Ingrese el alfabeto con el formato correcto:"<<endl;
				}
			}
		}
		else{
			cout<<"\n\033[31mOpcion invalida\033[0m"<<endl;
		}

	}while(op != 'a' && op != 'b');	
}

void leerCadenas(string &w1, string &w2, string alfa)
{
	// Cero o más ocurrencias del elemento anterior
	regex alfabeto(alfa + "*");
	cout<<"\n\t\033[96m2) INGRESAR DOS CADENAS SOBRE EL ALFABETO DEFINIDO\033[0m"<<endl;
	cout<<"\t1. Ingrese dos cadenas w1 y w2"<<endl;
	cout<<"\t2. Ambos deben ser elementos del alfabeto definido"<<endl;

	do{
		w1 = "";
		cout<<"\n\033[95ma. Ingrese la cadena w1:\033[0m"<<endl;
		cin>>w1;

		if(regex_match(w1,alfabeto))
		{
			cout<<"\033[32mCadena valida\033[0m\n";
			break;
		}else
		{
			cout<<"\033[31mCadena invalida\033[0m\n";
		}
	}while(true);

	do{
		w2 = "";
		cout<<"\n\033[95mb. Ingrese la cadena w2:\033[0m"<<endl;
		cin>>w2;

		if(regex_match(w2,alfabeto))
		{
			cout<<"\033[32mCadena valida\033[0m\n";
			break;
		}else
		{
			cout<<"\033[31mCadena invalida\033[0m\n";
		}
	}while(true);

	compararCadenas(w1,w2);
}


void compararCadenas(string &w1, string &w2)
{
	string cadena = "";

	cout<<"\n\t\033[96m3) COMPARAR AMBAS CADENAS\033[0m"<<endl;
	cout<<"\tComparacion entre la cadena w1 y w2\n\n";
	cout<<"\tw1 respecto a w2:"<<endl;
	
	/* PREFIJO (PROPIO O NO PROPIO) */

	//la ocurrencia debe ocurrir al comienzo de la cadena
	regex prefijo("^" + w1);

	if(regex_search(w2, prefijo))
	{
		if(w1!=w2)
		{
			cout<<"\tes PREFIJO PROPIO"<<endl;
		}else
		{
			cout<<"\tes PREFIJO NO PROPIO"<<endl;
		}
	}else
	{
		cout<<"\tNO es PREFIJO"<<endl;	
	}

	/* SUFIJO (PROPIO O NO PROPIO) */

	//la ocurrencia debe ocurrir al final de la cadena
	regex sufijo(w1 + "$");

	if(regex_search(w2, sufijo))
	{
		if(w1!=w2)
		{
			cout<<"\tes SUFIJO PROPIO"<<endl;
		}else
		{
			cout<<"\tes SUFIJO NO PROPIO"<<endl;
		}
	}else
	{
		cout<<"\tNO es SUFIJO"<<endl;	
	}

	/* SUBCADENA (PROPIO O NO PROPIO) */
	regex subcadena(w1);

	if(regex_search(w2, subcadena))
	{
		if(w1!=w2)
		{
			cout<<"\tes SUBCADENA PROPIA"<<endl;
		}else
		{
			cout<<"\tes SUBCADENA NO PROPIA"<<endl;
		}
	}else
	{
		cout<<"\tNO es SUBCADENA"<<endl;	
	}

	/* SUBSECUENCIA */
	for(char c: w1)
	{
		cadena += ".*" + string(1,c);
	}

	cadena += ".*";
	regex subsecuencia(cadena);

	if(regex_search(w2, subsecuencia))
	{
		cout<<"\tes SUBSECUENCIA"<<endl;
	}else
	{
		cout<<"\tNO es SUBSECUENCIA"<<endl;
	}

	system("pause");
}

void expresionRegular()
{
	int op;
	string a = "";

	system("cls");
	regex expresionr("^[a-df-hj-np-tv-z]*a[a-df-hj-np-tv-z]*e[b-hj-np-tv-z]*i[b-df-np-tv-z]*o[b-df-hj-tv-z]*u[b-df-hj-np-z]*$");

	cout<<"\t\033[96m7) IMPLEMENTACION DE UNA EXPRESION REGULAR\033[0m"<<endl;
	cout<<"\t1. Todas las cadenas de letras en minusculas (a-z) que contengan las 5 vocales en orden"<<endl;
	cout<<"\t2. Las vocales pueden estar repetidas siempre que mantengan el orden"<<endl;
	cout<<"\t3. La secuencia completa de vocales tambien puede repetirse"<<endl;
	cout<<"\t\ta. \033[33mEjemplo de cadena valida: rtaeioujutf\033[0m"<<endl;
	cout<<"\t\tb. \033[33mEjemplo de cadena invalida: kayteemnoyug\033[0m"<<endl;

	while(true)
	{
		cin>>a;
		if(regex_match(a,expresionr))
		{
			cout<<"\033[32mCadena valida\033[0m\n";
			break;
		}else
		{
			cout<<"\033[31mCadena invalida\033[0m\n";
		}
	}

}
