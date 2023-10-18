/*
Práctica No. 1 Alfabetos, lenguajes y expresiones regulares

Autores:	Olvera Martínez Aarón
			Reyes Francisco Paola

Para compilar:
gcc practica1.cpp -o p1.exe -lstdc++

*/

#include <iostream>
#include <stdlib.h>
#include <regex>
#include <string>
#include <random>
#include <queue>

using namespace std;

void leerAlfabeto(string &alfa, string &repAlfa, vector<char> &letrasAlfa);
void leerCadenas(string &w1, string &w2, string alfa);
void compararCadenas(string &w1, string &w2);
bool palabraRepetida(vector<string> &L, string s);
void crearLenguajes(vector<char> &letrasAlfa);
void diferenciaLenguajes(vector<string> &L1, vector<string> &L2);
void alfabetoPotencia(vector<char> &letrasAlfa);
void expresionRegular();

int main()
{
    //alfabeto
    string alfa;
	string repAlfa;
	vector<char> letrasAlfa;
    //cadenas
    string w1;
    string w2;
    int potencia;
    
    leerAlfabeto(alfa, repAlfa, letrasAlfa);
    regex alfabeto(alfa);

    cout<<"\nEl alfabeto ingresado es: "<<repAlfa<<endl;

    leerCadenas(w1,w2,alfa);
	crearLenguajes(letrasAlfa);
	alfabetoPotencia(letrasAlfa);

	expresionRegular();

	return 0;
}

void leerAlfabeto(string &alfa, string &repAlfa, vector<char> &letrasAlfa)
{
	char op;
	string input = "";

	system("cls");
	alfa = "";
	repAlfa = "";
	
	cout<<"\t\033[96m1) DEFINIR UN ALFABETO\033[0m"<<endl;
	cout<<"\t1. El alfabeto debe tener al menos tres simbolos"<<endl;
	cout<<"\t2. Los simbolos pueden ser ingresados:"<<endl;
	cout<<"\t\t a. De manera individual \t \033[33mEjemplo: a,b,c,e (separado por comas y sin espacios)\033[0m"<<endl;
	cout<<"\t\t b. Por rango\t\t\t \033[33mEjemplo: m-x\033[0m"<<endl;
	
	do{

		cout<<"\tIngrese la opcion en \033[31ma\033[0m o \033[31mb\033[0m"<<endl;
		cin >> op;

		if(op == 'a'){

			cout<<"\n\033[95m Ingrese el alfabeto:\033[0m"<<endl;

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
					cout << "\n\033[95m Ingrese de nuevo el alfabeto con al menos 3 simbolos:\033[0m" << endl;
				}	
			}
			for(char c: alfa){
				letrasAlfa.push_back(c);
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
			cout << "\n\033[95m Ingrese el alfabeto por rango:\033[0m" << endl;
			bool guion = false;

			while(!guion)
			{
				alfa = "";
				repAlfa = "";
				cin >> alfa;

				if(alfa.length() == 3 && alfa[1] == '-' && alfa[0] <= alfa[2] && alfa[2] - alfa[0] >= 2)
				{
					repAlfa = '{' ;
					repAlfa += alfa[0];
					repAlfa += ", ..., ";
					repAlfa += alfa[2];
					repAlfa += '}';
					for(char i = alfa[0]; i <= alfa[2]; i++){
						letrasAlfa.push_back(i);
					}
					alfa = '[' + alfa + ']';
					guion = true;
				}
				else if (alfa.length() == 3 && alfa[1] == '-' && alfa[2] - alfa[0] < 2 )
				{
					cout<<"\n\033[95m Ingrese de nuevo el alfabeto con al menos 3 simbolos y en orden correcto:\033[0m"<<endl;
				}
				else
				{
					cout<<"\033[95m\n Ingrese el alfabeto con el formato correcto:\033[0m"<<endl;
				}
			}
		}
		else{
			cout<<"\n\033[31m Opcion invalida\033[0m"<<endl;
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
		cout<<"\n\033[95ma. Ingrese la cadena w1:\033[0m ";
		cin>>w1;

		if(regex_match(w1,alfabeto))
		{
			cout<<"\033[92mCadena valida\033[0m\n";
			break;
		}else
		{
			cout<<"\033[31mCadena invalida\033[0m, ingrese otra opcion\n";
		}
	}while(true);

	do{
		w2 = "";
		cout<<"\n\033[95mb. Ingrese la cadena w2:\033[0m ";
		cin>>w2;

		if(regex_match(w2,alfabeto))
		{
			cout<<"\033[92mCadena valida\033[0m\n";
			break;
		}else
		{
			cout<<"\033[31mCadena invalida\033[0m, ingrese otra opcion\n";
		}
	}while(true);

	compararCadenas(w1,w2);
}


void compararCadenas(string &w1, string &w2)
{
	string cadena = "";

	cout<<"\n\t\033[96m3) COMPARAR AMBAS CADENAS\033[0m"<<endl;
	cout<<"\tComparacion entre la cadena w1 y w2\n\n";
	cout<<"\t\033[93mw1\033[0m respecto a \033[93mw2\033[0m:"<<endl;
	
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
		cout<<"\t\033[31mNO\033[0m es PREFIJO"<<endl;	
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
		cout<<"\t\033[31mNO\033[0m es SUFIJO"<<endl;	
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
			cout<<"\tes SUBCADENA \033[31mNO\033[0m PROPIA"<<endl;
		}
	}else
	{
		cout<<"\t\033[31mNO\033[0m es SUBCADENA"<<endl;	
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
		cout<<"\t\033[31mNO\033[0m es SUBSECUENCIA"<<endl;
	}

	printf("\n\n");
	system("pause");
}


bool palabraRepetida(vector<string> &L, string s){
	for(string u: L){
		if(s == u)	return true;
	}
	return false;
}

void imprimirLenguaje(vector<string> &L){
	for(int i = 0; i < L.size() - 1; i++){
		cout << L[i] << ", ";
	}
	cout << L[L.size()-1] << "}\n";
}

void crearLenguajes(vector<char> &letrasAlfa){

	vector<string> L1;
	vector<string> L2;
	string aux;
	int np, l;
	random_device rd;
	mt19937 gen(rd());
	int min = 0;
	int max = letrasAlfa.size() - 1;
	uniform_int_distribution<int> dist(min, max);

	cout<<"\n\t\033[96m4) GENERAR LOS LENGUAJES L1 Y L2\033[0m"<<endl;
	cout<<"\t1. Con base en el alfabeto definido, generar los lenguajes L1 y L2 de forma aleatoria"<<endl;
	cout<<"\t2. Debera ingresar el numero de palabras (\033[31mnp\033[0m) asi como su longitud (\033[31ml\033[0m)"<<endl;
	cout<<"\t\033[33mEjemplo: 4 6\033[0m"<<endl;

	cout<<"\n\033[95m a. Para el lenguaje L1, ingrese el numero de palabras seguido de su longitud:\033[0m"<<endl;
	
	bool opInvalida = false;
	do{
		if(opInvalida == true) cout << "\n\033[31mOpcion invalida\033[0m" << endl;
		cin >> np >> l;
		opInvalida = true;
	}while(np > pow(letrasAlfa.size(), l));

	int i = 0;
	while(i < np){
		aux = "";
		for(int j = 0; j < l; j++){
			aux += letrasAlfa[dist(gen)];
		}
		if(!palabraRepetida(L1, aux)){
			L1.push_back(aux);
			i++;
		}
	}

	cout<<"\n\033[95m b. Para el lenguaje L2, ingrese el numero de palabras seguido de su longitud:\033[0m"<<endl;
	cin >> np >> l;
	printf("\n");

	i = 0;
	while(i < np){
		aux = "";
		for(int j = 0; j < l; j++){
			aux += letrasAlfa[dist(gen)];
		}
		if(!palabraRepetida(L2, aux)){
			L2.push_back(aux);
			i++;
		}
	}

	cout << "\033[93m L1\033[0m = {";
	imprimirLenguaje(L1);
	cout << "\033[93m L2\033[0m = {";
	imprimirLenguaje(L2);
	printf("\n\n");
	diferenciaLenguajes(L1, L2);

}

void diferenciaLenguajes(vector<string> &L1, vector<string> &L2)
{
	vector<string> LD;

	cout<<"\t\033[96m5) GENERAR LD = L1 - L2\033[0m"<<endl;
	cout<<"\t1. Generar el lenguaje LD resultado de la diferencia de los lenguajes"<<endl;
	cout<<""<<endl;

	for(string s: L1){
		if(!palabraRepetida(L2, s))
			LD.push_back(s);
	}
	if(!LD.empty()){
		cout << " \033[93mLD\033[0m = {";
		imprimirLenguaje(LD);
	}
	else{
		cout << " \033[93mLD\033[0m = Lenguaje vacio \n";
	}
}

void alfabetoPotencia(vector<char> &letrasAlfa){
	queue<string> q;
	string aux;
	int i, j, potencia;
	vector<string> alfaPotencia;
	q.push("");
	int np_parcial;

	cout<<"\n\t\033[96m6) OBTENER LA POTENCIA DEL ALFABETO DEFINIDO\033[0m"<<endl;
	cout<<"\t1. El valor de la potencia es un numero entero (rango: -5 a 5)"<<endl;
	cout<<"\t2. Ingrese la potencia\t \033[33mEjemplo: -3\033[0m"<<endl;
	cin>>potencia;

	if(potencia == 0){
		cout << "\n \033[91mAlfaPotencia\033[0m = {lambda}\n";
		return;
	}

	if(potencia < 0)	potencia = -potencia;

	i = 0;
	while(i < potencia){
		np_parcial = pow(letrasAlfa.size(), i);
		j = 0;
		while(j < np_parcial){
			for(char c: letrasAlfa){
				aux = "" + q.front() + string(1,c);
				q.push(aux);
			}
			q.pop();
			j++;
		}
		i++;
	}

	while(!q.empty()){
		alfaPotencia.push_back(q.front());
		q.pop();
	}

	cout << "AlfaPotencia = {";
	imprimirLenguaje(alfaPotencia);
	printf("\n\n");
	system("pause");

}

void expresionRegular()
{
	int op;
	string a = "";

	regex expresionr("^[a-df-hj-np-tv-z]*a[a-df-hj-np-tv-z]*e[b-hj-np-tv-z]*i[b-df-np-tv-z]*o[b-df-hj-tv-z]*u[b-df-hj-np-z]*$");

	cout<<"\n\t\033[96m7) IMPLEMENTACION DE UNA EXPRESION REGULAR\033[0m"<<endl;
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
			cout<<"\033[31mCadena invalida, vuelva a ingresar la cadena\033[0m\n";
		}
	}

}