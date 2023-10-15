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
		cout<<"b. Ingrese la cadena w2:"<<endl;
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
	cout<<"\n\n\t3) COMPARAR AMBAS CADENAS"<<endl;
	cout<<"\tComparacion entre la cadena w1 y w2\n\n";
	/* Para indicar si es prefijo (propio o no propio) */

	//la ocurrencia debe ocurrir al comienzo de la cadena
	regex prefijo("^" + w1);

	if(regex_search(w2, prefijo))
	{
		cout<<"w1 es PREFIJO de w2"<<endl;

	}else
	{
		cout<<"w1 NO es PREFIJO de w2"<<endl;	
	}

	/* Para indicar si es sufijo (propio o no propio) */

	//la ocurrencia debe ocurrir al final de la cadena
	regex sufijo(w1 + "$");

	if(regex_search(w2, sufijo))
	{
		cout<<"w1 es SUFIJO de w2"<<endl;
	}else
	{
		cout<<"w1 NO es SUFIJO de w2"<<endl;	
	}
}