/*

Para compilar:
gcc practica1.cpp -o p1.exe -lstdc++



*/

#include <iostream>
#include <stdlib.h>
#include <regex>
#include <string>

using namespace std;

string leerAlfabeto();
void leerCadenas(string &w1, string &w2, string alfa);
void compararCadenas(string &w1, string &w2);
void expresionRegular();

int main()
{
    //alfabeto
    string alfa;
    //cadenas
    string w1;
    string w2;

    alfa = leerAlfabeto();
    regex alfabeto(alfa);
    cout<<"\nEl alfabeto ingresado es: "<<alfa<<endl;
    leerCadenas(w1,w2,alfa);
    expresionRegular();


	return 0;
}

string leerAlfabeto()
{
	int op;
	string lee;
	string alfa;

	do
	{
		system("cls");
		op = 0;
		lee = "";
		alfa = "";
		
		cout<<"\t1) DEFINIR UN ALFABETO"<<endl;
		cout<<"\t1. El alfabeto debe tener al menos tres simbolos"<<endl;
		cout<<"\t2. Los simbolos pueden ser ingresados:"<<endl;
		cout<<"\t\t a. De manera individual\t Ejemplo: a, b, c, e"<<endl;
		cout<<"\t\t b. Por rango\t Ejemplo: m-x"<<endl;
		
		while(true)
	    {
	    	// lee caracter por caracter
	    	cin>>lee;

	    	// Ingresados de manera individual
	    	if(lee.length()==1)
	    	{
	    		if(lee == "[" || lee == "]")
	    		{
	    			lee = "\\" + lee;
	    		}

	    		alfa += lee;

	    	// Ingresados por rango
	    	}else if(lee.length()==3 && lee[1]=='-')
	    	{
	    		alfa += lee;
	    		break;
	    	}else
	    	{
	    		break;
	    	}
	    }

	    // Verifica que el alfabeto tenga al menos 3 símbolos
	    if(alfa.length() < 3)
	    {
	    	cout<<"El alfabeto tiene menos de tres simbolos!"<<endl;
	    	system("pause");
	    	op = 1;
	    }

	}while(op != 0);
	
	alfa = "[" + alfa + "]";

	return alfa; 	
}

void leerCadenas(string &w1, string &w2, string alfa)
{
	// Cero o más ocurrencias del elemento anterior
	regex alfabeto(alfa + "*");
	cout<<"\n\t2) INGRESAR DOS CADENAS SOBRE EL ALFABETO DEFINIDO"<<endl;
	cout<<"\t1. Ingrese dos cadenas w1 y w2"<<endl;
	cout<<"\t2. Ambos deben ser elementos del alfabeto definido"<<endl;

	do{
		w1 = "";
		cout<<"\na. Ingrese la cadena w1:"<<endl;
		cin>>w1;

		if(regex_match(w1,alfabeto))
		{
			cout<<"Candena valida\n";
			break;
		}else
		{
			cout<<"Cadena invalida\n";
		}
	}while(true);

	do{
		w2 = "";
		cout<<"\nb. Ingrese la cadena w2:"<<endl;
		cin>>w2;

		if(regex_match(w2,alfabeto))
		{
			cout<<"Candena valida\n";
			break;
		}else
		{
			cout<<"Cadena invalida\n";
		}
	}while(true);

	compararCadenas(w1,w2);
}


void compararCadenas(string &w1, string &w2)
{
	string cadena = "";

	cout<<"\n\n\t3) COMPARAR AMBAS CADENAS"<<endl;
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
	// [b-hj-np-tv-z]e[b-hj-np-tv-z]+[b-df-np-tv-z]i[b-df-np-tv-z]+[b-df-hj-tv-z]o[b-df-hj-tv-z]+[b-df-hj-np-z]u[b-df-hj-np-z]
	regex expresionr("^[a-df-hj-np-tv-z]*a[a-df-hj-np-tv-z]*e[b-hj-np-tv-z]*i[b-df-np-tv-z]*o[b-df-hj-tv-z]*u[b-df-hj-np-z]*$");

	cout<<"\n\n\t7) IMPLEMENTACION DE UNA EXPRESION REGULAR"<<endl;
	cout<<"\t1. Todas las cadenas de letras en minusculas (a-z) que contengan las 5 vocales en orden."<<endl;
	cout<<"\t2. Las vocales pueden estar repetidas siempre que mantengan el orden."<<endl;
	cout<<"\t3. La secuencia completa de vocales tambien puede repetirse."<<endl;
	cout<<"\t\ta. Ejemplo de cadena valida: rtaeioujutf"<<endl;
	cout<<"\t\ta. Ejemplo de cadena invalida: kayteemnoyug"<<endl;

	while(true)
	{
		cin>>a;
		if(regex_match(a,expresionr))
		{
			cout<<"Cadena valida"<<endl;
			break;
		}else
		{
			cout<<"Cadena invalida"<<endl;
		}
	}

}
