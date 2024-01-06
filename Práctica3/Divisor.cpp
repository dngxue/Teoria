#include <bits/stdc++.h>

#define endl '\n'
#define fi first
#define se second
#define MOD(n,k) ( ( ((n) % abs(k)) + abs(k) ) % abs(k))
#define forn(i,n) for (int i = 0; i < int(n); i++)
#define forr(i,a,b) for (int i = int(a); i <= int(b); i++)
#define all(v) v.begin(), v.end()
#define pb push_back
 
using namespace std;
 
typedef long long lli;
typedef long double ld;
typedef pair<int, int> p2i;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<p2i> vp2i;

const int SIZE = 1e5 + 1,INF = 1e8 + 1;
// Estados AFD 

enum TipoToken{
    INICIAL,
    DECIMAL,
    OCTAL_O_HEXADECIMAL,
    OCTAL,
    POSIBLE_HEXADECIMAL,
    HEXADECIMAL,
    SIGNO_MENOS,
    SIGNO_MAS,
    REAL_SIN_EXP,
    PUNTO,
    EXPONENTE,
    SIGNO_EXP,
    DECIMAL1EXP,
    DECIMAL2EXP,
    COMPARADOR,
    EXCLAMACION,
    ASIGNACION,
    OPERADORASIGNACION,
    ASTERISCO,
    DIAGONAL,
    MODULO,
    COMENTARIO_MULTILINEA_INICIO,
    COMENTARIO_MULTILINEA_FIN,
    COMENTARIO_UNILINEA,
    IDENTIFICADOR,
    PALABRARESERVADA,
    GUION_BAJO,
    COMILLAS,
    INCREMENTO,
    PARENTESISABRIR,
    PARENTESISCERRAR,
    PUNTOYCOMA,
    MUERTO
};

// Estados automata de pila

enum EstadoAPila{
    INICIO,
    ID_INICIAL,
    IGUALDAD,
    IDI,
    EXPRESION,
    NUMERO,
    OPERADOR,
    ID,
    NO_ACEPTADO
};

/*  clave: TipoToken
    valor: string
    cadena asociada a un tipo de token */

unordered_map<TipoToken, string> tipoTokenString = {
    {INICIAL, "INICIAL"},
    {DECIMAL, "DECIMAL"},
    {OCTAL_O_HEXADECIMAL, "OCTAL_O_HEXADECIMAL"},
    {OCTAL, "OCTAL"},
    {POSIBLE_HEXADECIMAL, "POSIBLE_HEXADECIMAL"},
    {HEXADECIMAL, "HEXADECIMAL"},
    {SIGNO_MENOS, "SIGNO_MENOS"},
    {SIGNO_MAS, "SIGNO_MAS"},
    {REAL_SIN_EXP, "REAL_SIN_EXP"},
    {PUNTO, "PUNTO"},
    {EXPONENTE, "EXPONENTE"},
    {SIGNO_EXP, "SIGNO_EXP"},
    {DECIMAL1EXP, "DECIMAL1EXP"},
    {DECIMAL2EXP, "DECIMAL2EXP"},
    {COMPARADOR, "COMPARADOR"},
    {EXCLAMACION, "EXCLAMACION"},
    {ASIGNACION, "ASIGNACION"},
    {ASTERISCO, "ASTERISCO"},
    {DIAGONAL, "DIAGONAL"},
    {MODULO, "MODULO"},
    {COMENTARIO_MULTILINEA_INICIO, "COMENTARIO_MULTILINEA_INICIO"},
    {COMENTARIO_MULTILINEA_FIN, "COMENTARIO_MULTILINEA_FIN"},
    {COMENTARIO_UNILINEA, "COMENTARIO_UNILINEA"},
    {IDENTIFICADOR, "IDENTIFICADOR"},
    {PALABRARESERVADA, "PALABRARESERVADA"},
    {GUION_BAJO, "GUION_BAJO"},
    {COMILLAS, "COMILLAS"},
    {INCREMENTO, "INCREMENTO"},
    {PARENTESISABRIR, "PARENTESISABRIR"},
    {PARENTESISCERRAR, "PARENTESISCERRAR"},
    {PUNTOYCOMA, "PUNTOYCOMA"},
    {OPERADORASIGNACION, "OPERADORASIGNACION"},
    {MUERTO, "MUERTO"}
};

unordered_map<EstadoAPila, string> estadoAPilaString = {
    {INICIO, "INICIO"},
    {ID_INICIAL, "ID_INICIAL"},
    {IGUALDAD, "IGUALDAD"},
    {IDI, "IDI"},
    {EXPRESION, "EXPRESION"},
    {NUMERO, "NUMERO"},
    {OPERADOR, "OPERADOR"},
    {ID, "ID"},
    {NO_ACEPTADO, "NO_ACEPTADO"}
};

class TokenProcesado
{
    public:
    string valor;
    TipoToken tipo;

    TokenProcesado(TipoToken tipo, string valor){
        this->tipo = tipo;
        this->valor = valor;
    }
};

string borrarEspacios(string s);
class nodo{

    public:
    string simbolo;
    string cadena;
    vector<nodo *> derivacion;

    nodo(string val, string cad){
        simbolo = val;
        cadena = borrarEspacios(cad);
    }

    void agregarDerivacion(string s, string cad){
        nodo *aux = new nodo(s, cad);
        derivacion.pb(aux);
    }

    string getValor(){
        return simbolo;
    }

};

// Símbolos
set<char> noAnalizados = {'{','}','[',']',','};
set<char> comillas = {'"'};
set<char> operadores = {'*','/','<','>','=', '%', '+', '-'};
set<char> signos = {'+','-'};
set<char> octal = {'0', '1', '2', '3', '4', '5', '6', '7'};
set<char> hexadecimal = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
set<string> palabras_reservadas_java = {"abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const","public", "return", "short", "static", "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void", "volatile", "while", "continue", "default", "do", "double", "else", "enum", "extends", "final", "finally", "float", "for", "goto", "if", "implements", "import", "instanceof", "int", "interface", "long", "native", "new", "package", "private", "protected", "true", "false", "null", "System.out.println"};

set<TipoToken> operadoresAP = {ASTERISCO, DIAGONAL, COMPARADOR, EXCLAMACION, ASIGNACION, OPERADORASIGNACION, SIGNO_MAS, SIGNO_MENOS, MODULO};
set<TipoToken> asignacion = {ASIGNACION, OPERADORASIGNACION};
set<TipoToken> numero = {DECIMAL, OCTAL, HEXADECIMAL, REAL_SIN_EXP, DECIMAL1EXP, DECIMAL2EXP};

// Variables globales
vector<string> tokens;
vector<TokenProcesado> tokensProcesados;
bool operadorant = false;
bool Eant = false;

// Funciones
void procesarCadena(string cadena);
TipoToken AFD(string token);

// Funciones arbolderivacion
void Procesarsimbolo(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar);
bool terminoParentesis(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar);
bool expresionOperador(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar);
void preOrden(nodo *raiz);
void imprimirNodo(nodo *n);
void arbolDerivacion(string s);

int main(){

    // Leer archivo
    string nombreArchivo = "texto.txt";
    ifstream archivo(nombreArchivo.c_str());
    
    string linea;
    string cadena;

    while(getline(archivo, linea)){
        /*  trata a una cadena de texto como una secuencia de entrada */
        istringstream ss(linea);

        while(ss >> cadena){
            procesarCadena(cadena);
        }
    }

    cout << "Expresion: " << linea << endl << endl;

    // Automata de Pila para validad expresiones en C con una
    // cinta de entrada con los estados de cada token como símbolos
    EstadoAPila Estado = INICIO;
    stack<char> Pila;
    Pila.push('Z');
    for(TokenProcesado token: tokensProcesados){
        TipoToken simbolo = token.tipo;
        if(Pila.empty()){
            Estado = NO_ACEPTADO;
            break;
        }
        if(Estado == NO_ACEPTADO){
            break;
        }

        switch(Estado){
            case INICIO:
                if(simbolo == IDENTIFICADOR && Pila.top() == 'Z'){
                    Estado = ID_INICIAL;
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case ID_INICIAL:
                if(asignacion.count(simbolo) && Pila.top() == 'Z'){
                    Estado = IGUALDAD;
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case IGUALDAD:
                if(simbolo == IDENTIFICADOR && Pila.top() == 'Z'){
                    Estado = IDI;
                }
                else if(simbolo == PARENTESISABRIR && Pila.top() == 'Z'){
                    Estado = EXPRESION;
                    Pila.push('(');
                }
                else if(numero.count(simbolo) && Pila.top() == 'Z'){
                    Estado = NUMERO;
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case IDI:
                if(asignacion.count(simbolo) && Pila.top() == 'Z'){
                    Estado = IGUALDAD;
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == 'Z'){
                    Estado = OPERADOR;
                    Pila.push('O');
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case EXPRESION:
                if(simbolo == PARENTESISABRIR && Pila.top() == 'Z'){
                    Pila.push('(');
                }
                else if(simbolo == PARENTESISABRIR && Pila.top() == '('){
                    Pila.push('(');
                }
                else if(simbolo == PARENTESISABRIR && Pila.top() == 'O'){
                    Pila.push('(');
                }
                else if(numero.count(simbolo) && Pila.top() == 'Z'){
                    Estado = NUMERO;
                }
                else if(numero.count(simbolo) && Pila.top() == '('){
                    Estado = NUMERO;
                }
                else if(numero.count(simbolo) && Pila.top() == 'O'){
                    Estado = NUMERO;
                }
                else if(simbolo == IDENTIFICADOR && Pila.top() == 'Z'){
                    Estado = ID;
                }
                else if(simbolo == IDENTIFICADOR && Pila.top() == '('){
                    Estado = ID;
                }
                else if(simbolo == IDENTIFICADOR && Pila.top() == 'O'){
                    Estado = ID;
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case NUMERO:
                if(simbolo == PARENTESISCERRAR && Pila.top() == '('){
                    Pila.pop();
                }
                else if(simbolo == PARENTESISCERRAR && Pila.top() == 'O'){
                    Pila.pop();
                    Pila.pop();
                }
                else if(simbolo == PUNTOYCOMA && Pila.top() == 'Z'){
                    Pila.pop();
                }
                else if(simbolo == PUNTOYCOMA && Pila.top() == 'O'){
                    Pila.pop();
                    Pila.pop();
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == 'Z'){
                    Estado = OPERADOR;
                    Pila.push('O');
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == 'O'){
                    Estado = OPERADOR;
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == '('){
                    Estado = OPERADOR;
                    Pila.push('O');
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case OPERADOR:
                if(simbolo == IDENTIFICADOR && Pila.top() == 'O'){
                    Estado = ID;
                }
                else if(simbolo == PARENTESISABRIR && Pila.top() == 'O'){
                    Estado = EXPRESION;
                    Pila.pop();
                    Pila.push('(');
                }
                else if(numero.count(simbolo) && Pila.top() == 'O'){
                    Estado = NUMERO;
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
            case ID:
                if(simbolo == PARENTESISCERRAR && Pila.top() == '('){
                    Pila.pop();
                }
                else if(simbolo == PARENTESISCERRAR && Pila.top() == 'O'){
                    Pila.pop();
                    Pila.pop();
                }
                else if(simbolo == PUNTOYCOMA && Pila.top() == 'Z'){
                    Pila.pop();
                }
                else if(simbolo == PUNTOYCOMA && Pila.top() == 'O'){
                    Pila.pop();
                    Pila.pop();
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == 'Z'){
                    Estado = OPERADOR;
                    Pila.push('O');
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == 'O'){
                    Estado = OPERADOR;
                }
                else if(operadoresAP.count(simbolo) && Pila.top() == '('){
                    Estado = OPERADOR;
                    Pila.push('O');
                }
                else{
                    Estado = NO_ACEPTADO;
                }
                break;
        }
        cout << "Simbolo: " << tipoTokenString[simbolo] << endl;
        cout << "Estado: " << estadoAPilaString[Estado] << endl;
        if(!Pila.empty())
            cout << "Pila: " << Pila.top() << endl;
        cout << endl;
    }

    if(Pila.empty() && Estado != NO_ACEPTADO){
        cout << "La expresion es valida" << endl << endl;
        arbolDerivacion(linea);
    }
    else{
        cout << "La expresion no es valida" << endl;
    }
    

    return 0;
}


// Funciones

/*
    Divide la cadena en tokens de acuerdo con
    los caracteres especiales
*/

void procesarCadena(string cadena){
    if(cadena.size() == 0) return;
    string aux = "";

    // Comentario Unilinea
    if(cadena.size() >= 2){
        if(cadena[0] == '/' && cadena[1] == '/'){
            tokens.push_back(cadena);
            /*  se crea un objeto TokenProcesado
                ADN(cadena) --> tipo
                cadena --> valor
                num_linea --> linea
            */
            tokensProcesados.push_back(TokenProcesado(AFD(cadena), cadena));
            return;
        }
    }

    // Comentario Multilinea
    if(cadena.size() >= 2){
        if(cadena[0] == '/' && cadena[1] == '*'){
            tokens.push_back(cadena);
            tokensProcesados.push_back(TokenProcesado(AFD(cadena), cadena));
            return;
        }
    }

    if(cadena.size() >= 2){
        if(cadena[0] == '*' && cadena[1] == '/'){
            tokens.push_back("*/");
            tokensProcesados.push_back(TokenProcesado(AFD("*/"), "*/"));
            return;
        }
    }

    // Comparadores de dos caracteres
    if(cadena.size() >= 2){
        if((cadena[0] == '=' || cadena[0] == '!' || cadena[0] == '+' || cadena[0] == '-' || cadena[0] == '>' || cadena[0] == '<') && cadena[1] == '='){
            aux += cadena[0];
            aux += cadena[1];
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            aux = "";
            cadena = cadena.substr(2);
        }
    }

    // Por cada caracter especial, divide la cadena y agrega el resultado a tokens
    for(char c: cadena){
        if(noAnalizados.count(c)){
            // se ha completado una palabra/token previo
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            }
            aux = "";
            operadorant = false;
            Eant = false;
        }
        else if(c == '(' || c == ')'){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            aux = "";
            operadorant = false;
            Eant = false;
        }

        else if(c == ';'){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            }
            tokens.push_back(";");
            tokensProcesados.push_back(TokenProcesado(AFD(";"), ";"));
            aux = "";
            operadorant = false;
            Eant = false;
        }

        else if(comillas.count(c)){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            aux = "";
            operadorant = false;
            Eant = false;
        }
        else if(operadores.count(c) && !operadorant && !Eant){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
            aux = "";
            operadorant = true;
            Eant = false;
        }
        // Como ya se separó del anterior, solo se da el caso donde forma parte del token
        else if(signos.count(c) && (operadorant || Eant)){
            aux += c;
            Eant = false;
        }
        else{
            if(c == 'E'){
                Eant = true;
            }
            else{
                Eant = false;
            }
            aux += c;
            operadorant = false;
        }

    }

    if(!aux.empty()){
        tokens.push_back(aux);
        tokensProcesados.push_back(TokenProcesado(AFD(aux), aux));
    }
}


TipoToken AFD(string token){
    TipoToken Estado = INICIAL;
    /* itera cada caracter c en la cadena token */
    for(char c: token){
        
        if(Estado == MUERTO) return MUERTO;

        if(palabras_reservadas_java.count(token)){
            return PALABRARESERVADA;
        }

        switch(Estado){

            case INICIAL:
                if(isdigit(c) && c != '0'){
                    Estado = DECIMAL;
                }
                else if(c == '0'){
                    Estado = OCTAL_O_HEXADECIMAL;
                }
                else if(c == '+'){
                    Estado = SIGNO_MAS;
                }
                else if(c == '-'){
                    Estado = SIGNO_MENOS;
                }
                else if(c == '<' || c == '>'){
                    Estado = COMPARADOR;
                }
                else if(c == '!'){
                    Estado = EXCLAMACION;
                }
                else if(c == '='){
                    Estado = ASIGNACION;
                }
                else if(c == '*'){
                    Estado = ASTERISCO;
                }
                else if(c == '/'){
                    Estado = DIAGONAL;
                }
                else if(c == '%'){
                    Estado = MODULO;
                }
                else if(c == '_'){
                    Estado = GUION_BAJO;
                }
                else if(c == '$'){
                    Estado = IDENTIFICADOR;
                }
                else if(isalpha(c)){
                    Estado = IDENTIFICADOR;
                }
                else if(c == '"'){
                    Estado = COMILLAS;
                }
                else if(c == '('){
                    Estado = PARENTESISABRIR;
                }
                else if(c == ')'){
                    Estado = PARENTESISCERRAR;
                }
                else if(c == ';'){
                    Estado = PUNTOYCOMA;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case DECIMAL:
                if(isdigit(c)){
                    Estado = DECIMAL;
                }
                else if(c == '.'){
                    Estado = PUNTO;
                }
                // EN LA TABLA DEL PROFE SOLO LLEVA EXPONENTE EL REAL
                // else if(c == 'e' || c == 'E'){
                //     Estado = EXPONENTE;
                // }
                else{
                    Estado = MUERTO;
                }
                break;
            case PUNTO:
                if(isdigit(c)){
                    Estado = REAL_SIN_EXP;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case REAL_SIN_EXP:
                if(c == 'E'){
                    Estado = EXPONENTE;
                }
                else if(isdigit(c)){
                    Estado = REAL_SIN_EXP;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case EXPONENTE:
                if(signos.count(c)){
                    Estado = SIGNO_EXP;
                }
                // REVISAR SI DEBE LLEVAR SIGNO NECESARIAMENTE, EN LA TABLA ES OBLIGATORIO
                // else if(isdigit(c)){
                //     Estado = DECIMAL1EXP;
                // }
                else{
                    Estado = MUERTO;
                }
                break;
            case SIGNO_EXP:
                if(isdigit(c)){
                    Estado = DECIMAL1EXP;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case DECIMAL1EXP:
                if(isdigit(c)){
                    Estado = DECIMAL2EXP;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case DECIMAL2EXP:
                Estado = MUERTO;
                break;

            case SIGNO_MAS:
                if(isdigit(c) && c != '0'){
                    Estado = DECIMAL;
                }
                else if(c == '0'){
                    Estado = OCTAL_O_HEXADECIMAL;
                }
                else if(c == '='){
                    Estado = OPERADORASIGNACION;
                }
                else if(c == '+'){
                    Estado = INCREMENTO;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case SIGNO_MENOS:
                if(isdigit(c) && c != '0'){
                    Estado = DECIMAL;
                }
                else if(c == '0'){
                    Estado = OCTAL_O_HEXADECIMAL;
                }
                else if(c == '='){
                    Estado = OPERADORASIGNACION;
                }
                else if(c == '-'){
                    Estado = INCREMENTO;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case OCTAL_O_HEXADECIMAL:
                if(c == 'x' || c == 'X'){
                    Estado = POSIBLE_HEXADECIMAL;
                }
                else if(octal.count(c)){
                    Estado = OCTAL;
                }
                else if(c == '.'){
                    Estado = PUNTO;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case OCTAL:
                if(octal.count(c)){
                    Estado = OCTAL;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case POSIBLE_HEXADECIMAL:
                if(hexadecimal.count(c)){
                    Estado = HEXADECIMAL;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case HEXADECIMAL:
                if(hexadecimal.count(c)){
                    Estado = HEXADECIMAL;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case EXCLAMACION:
                if(c == '='){
                    Estado = COMPARADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case COMPARADOR:
                if(c == '='){
                    Estado = COMPARADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case ASIGNACION:
                if(c == '='){
                    Estado = COMPARADOR;
                } 
                else{
                    Estado = MUERTO;
                }
                break;
            case ASTERISCO:
                if(c == '/'){
                    Estado = COMENTARIO_MULTILINEA_FIN;
                }
                else{
                    Estado = MUERTO;
                    
                }
                break;
            case DIAGONAL:
                if(c == '/'){
                    Estado = COMENTARIO_UNILINEA;
                }
                else if(c == '*'){
                    Estado = COMENTARIO_MULTILINEA_INICIO;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case GUION_BAJO:
                if(isalpha(c) || isdigit(c) || c == '$' || c == '_'){
                    Estado = IDENTIFICADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
            case IDENTIFICADOR:
                if(isalpha(c) || isdigit(c) || c == '$' || c == '_'){
                    Estado = IDENTIFICADOR;
                }
                else{
                    Estado = MUERTO;
                }
                break;
        }

    }

    return Estado;

}


// Funciones arbolderivacion
void arbolDerivacion(string s){

    cout << "Arbol de derivacion:" << endl;
    string aux = s;
    bool puntoComa = false;
    if(aux[aux.size() - 1] == ';'){
        aux = aux.substr(0, aux.size() - 1);
        puntoComa = true;
    }
    nodo raiz = nodo("Inicio", aux);
    nodo *anterior = &raiz;

    queue<nodo *> nodosPorProcesar;
    nodosPorProcesar.push(anterior);

    while(!nodosPorProcesar.empty()){
        anterior = nodosPorProcesar.front();
        nodosPorProcesar.pop();
        aux = anterior->cadena;

        Procesarsimbolo(aux, anterior, nodosPorProcesar);
        // cout << aux << endl;
        // cout << anterior->simbolo << endl;
        // cout << nodosPorProcesar.size() << endl;
    }

    if(puntoComa){
        raiz.agregarDerivacion(";", ";");
    }

    preOrden(&raiz);

}

void Procesarsimbolo(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar){
    if(aux.size() == 0){
        return;
    }

    if(anterior->simbolo == "Inicio"){
        if(aux.find('=') == string::npos){
            anterior->agregarDerivacion("Identificador", aux);
        }
        else{
            if(operadores.count(aux[aux.find('=')-1])){
                anterior->agregarDerivacion("Identificador", aux.substr(0, aux.find('=')-1));
                anterior->agregarDerivacion("=", string(1, aux[aux.find('=')-1])+"=");
            }
            else{
                anterior->agregarDerivacion("Identificador", aux.substr(0, aux.find('=')));
                anterior->agregarDerivacion("=", "=");
            }
            anterior->agregarDerivacion("Igualdad", aux.substr(aux.find('=') + 1, aux.size()));
            nodosPorProcesar.push(anterior->derivacion[2]);
        }
    }
    else if(anterior->simbolo == "Igualdad"){
        if(aux.find('=') == string::npos){
            anterior->agregarDerivacion("Expresion", aux);
            nodosPorProcesar.push(anterior->derivacion[0]);
        }
        else{
            if(operadores.count(aux[aux.find('=')-1])){
                anterior->agregarDerivacion("Identificador", aux.substr(0, aux.find('=')-1));
                anterior->agregarDerivacion("=", string(1, aux[aux.find('=')-1])+"=");
            }
            else{
                anterior->agregarDerivacion("Identificador", aux.substr(0, aux.find('=')));
                anterior->agregarDerivacion("=", "=");
            }
            anterior->agregarDerivacion("Igualdad", aux.substr(aux.find('=') + 1, aux.size()));
            nodosPorProcesar.push(anterior->derivacion[2]);
        }
    }
    else if(anterior->simbolo == "Expresion"){
        if(expresionOperador(aux, anterior, nodosPorProcesar));
        else if(terminoParentesis(aux, anterior, nodosPorProcesar));
        else{
            anterior->agregarDerivacion("Termino", aux);
            nodosPorProcesar.push(anterior->derivacion[0]);
        }
    }
    
    else if(anterior->simbolo == "Termino"){
        if(AFD(aux) == IDENTIFICADOR){
            anterior->agregarDerivacion("Identificador", aux);
        }
        else if(numero.count(AFD(aux))){
            anterior->agregarDerivacion("Numero", aux);
        }
    }
    else if(anterior->simbolo == "Operador"){
        anterior->agregarDerivacion(aux, aux);
    }
}

// Detectar termino de la forma (<Expresion>)
bool terminoParentesis(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar){
    if(aux[0] == '(' && aux[aux.size() - 1] == ')'){
        anterior->agregarDerivacion("Termino", aux);
        anterior = anterior->derivacion[0];
        anterior->agregarDerivacion("(", "(");
        anterior->agregarDerivacion("Expresion", aux.substr(1, aux.size() - 2));
        nodosPorProcesar.push(anterior->derivacion[1]);
        anterior->agregarDerivacion(")", ")");
        return true;
    }
    return false;
}

// Detectar expresión de la forma <Expresion> <Operador> <Expresion>
bool expresionOperador(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar){
    
    // Cuando el + o - sea el signo del numero ()
    bool Eant = false;

    int contpA = 0;
    int contpB = 0;
    for(int i = 0; i < aux.size(); i++){
        if(i == 0 && signos.find(aux[i]) != signos.end()){
            continue;
        }

        else if(aux[i] == 'E'){
            Eant = true;
            continue;
        }

        else if(aux[i] == '('){
            contpA++;
        }

        else if(aux[i] == ')'){
            contpB++;
        }
        else if(contpA > contpB){
            continue;
        }
        else if(operadores.find(aux[i]) != operadores.end()){
            anterior->agregarDerivacion("Expresion", aux.substr(0, i));
            nodosPorProcesar.push(anterior->derivacion[0]);
            anterior->agregarDerivacion("Operador", string(1, aux[i]));
            nodosPorProcesar.push(anterior->derivacion[1]);
            anterior->agregarDerivacion("Expresion", aux.substr(i + 1, aux.size()));
            nodosPorProcesar.push(anterior->derivacion[2]);
            return true;
        }
    }
    return false;
}

void preOrden(nodo *raiz){
    if(raiz->derivacion.size() == 0){
        return;
    }

    imprimirNodo(raiz);
    for(int i = 0; i < raiz->derivacion.size(); i++){
        preOrden(raiz->derivacion[i]);
    }

}
void imprimirNodo(nodo *n){
    cout << n->simbolo << "[" << n->cadena << "]: ";
    for(int i = 0; i < n->derivacion.size(); i++){
        cout << n->derivacion[i]->simbolo << "[" << n->derivacion[i]->cadena << "]";
        if(i != n->derivacion.size() - 1){
            cout << ", ";
        }
    }
    cout << endl;
}

string borrarEspacios(string s){
    while(s[0] == ' '){
        s = s.substr(1, s.size());
    }
    while(s[s.size() - 1] == ' '){
        s = s.substr(0, s.size() - 1);
    }
    return s;
}
