/* para compilar: 
g++ Divisor.cpp -o divisor.exe */

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

/*  clave: TipoToken
    valor: string
    cadena asociada a un tipo de token */
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
    // simbolo asociado con el nodo
    string simbolo;
    // cadena asociado con el nodo
    string cadena;
    /* almacena punteros a los nodos que son derivaciones
       de este nodo en la estructura del arbol */
    vector<nodo *> derivacion;

    /* constructor que inicializa el nodo con un símbolo
       y una cadena */
    nodo(string val, string cad){
        simbolo = val;
        cadena = borrarEspacios(cad);
    }

    /* agrega una derivación al nodo actual 
       crea un nuevo nodo con el símbolo y la
       cadena especificados */
    void agregarDerivacion(string s, string cad){
        nodo *aux = new nodo(s, cad);
        derivacion.pb(aux);
    }

    // retorna el valor del símbolo asociado con el nodo
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

set<TipoToken> operadoresAP = {ASTERISCO, DIAGONAL, COMPARADOR, EXCLAMACION, ASIGNACION, SIGNO_MAS, SIGNO_MENOS, MODULO};
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

    // imprimir la expresion
    cout << " \n\x1B[38;2;255;128;0m Expresion:\x1B[0m" << linea << endl << endl;

    /*  Automata de Pila para validar expresiones en C con una
        cinta de entrada con los estados de cada token como símbolos */
    EstadoAPila Estado = INICIO;
    stack<char> Pila;
    Pila.push('Z');
    /* itera sobre cada elemento del vector tokensProcesados*/
    for(TokenProcesado token: tokensProcesados){
        TipoToken simbolo = token.tipo;
        if(Pila.empty()){
            Estado = NO_ACEPTADO;
            break;
        }
        if(Estado == NO_ACEPTADO){
            break;
        }

        /* determinar las transiciones según el tipo de token */
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
                /* si el tipo de toquen está presente en el conjunto asignación */
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
                else if(simbolo == PUNTOYCOMA && Pila.top() == 'Z'){
                    Pila.pop();
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
        cout << " \033[36mSimbolo: \033[0m" << tipoTokenString[simbolo] << endl;
        cout << " \033[35mEstado: \033[0m" << estadoAPilaString[Estado] << endl;
        if(!Pila.empty())
            cout << " \033[33mPila: \033[0m" << Pila.top() << endl;
        cout << endl;
    }

    if(Pila.empty() && Estado != NO_ACEPTADO){
        cout << "\033[42mACEPTADO\033[0m" << endl << endl;
        arbolDerivacion(linea);
    }
    else{
        cout << "\033[41mNO ACEPTADO\033[0m" << endl;
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
                if(isalpha(c) || isdigit(c) || c == '_'){
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

    cout << "ARBOL DE DERIVACION\n" << endl;
    string aux = s;
    bool puntoComa = false;

    if(aux[aux.size() - 1] == ';'){
        aux = aux.substr(0, aux.size() - 1);
        puntoComa = true;
    }

    /* se crea un nodo raiz
       se le asigna el símbolo inicio y la cadena aux como su valor */
    nodo raiz = nodo("Inicio", aux);
    // apunta al nodo raiz
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
            /* agrega una derivación al nodo actual */
            anterior->agregarDerivacion("\x1b[35mIdentificador\x1b[0m", aux);
        }
        else{
            if(operadores.count(aux[aux.find('=')-1])){
                anterior->agregarDerivacion("\x1b[35mIdentificador\x1b[0m", aux.substr(0, aux.find('=')-1));
                anterior->agregarDerivacion("\x1B[38;2;17;245;120mAsignacion\x1B[0m", string(1, aux[aux.find('=')-1])+"=");
            }
            else{
                anterior->agregarDerivacion("\x1b[35mIdentificador\x1b[0m", aux.substr(0, aux.find('=')));
                anterior->agregarDerivacion("\x1B[38;2;17;245;120mAsignacion\x1B[0m", "=");
            }
            anterior->agregarDerivacion("\x1b[33mIgualdad\x1b[0m", aux.substr(aux.find('=') + 1, aux.size()));
            nodosPorProcesar.push(anterior->derivacion[2]);
        }
    }

    else if(anterior->simbolo == "\x1b[33mIgualdad\x1b[0m"){
        if(aux.find('=') == string::npos){
            anterior->agregarDerivacion("\x1B[36mExpresion\x1B[0m", aux);
            nodosPorProcesar.push(anterior->derivacion[0]);
        }
        else{
            if(operadores.count(aux[aux.find('=')-1])){
                anterior->agregarDerivacion("\x1b[35mIdentificador\x1b[0m", aux.substr(0, aux.find('=')-1));
                anterior->agregarDerivacion("\x1B[38;2;17;245;120mAsignacion\x1B[0m", string(1, aux[aux.find('=')-1])+"=");
            }
            else{
                anterior->agregarDerivacion("\x1b[35mIdentificador\x1b[0m", aux.substr(0, aux.find('=')));
                anterior->agregarDerivacion("\x1B[38;2;17;245;120mAsignacion\x1B[0m", "=");
            }
            anterior->agregarDerivacion("\x1b[33mIgualdad\x1b[0m", aux.substr(aux.find('=') + 1, aux.size()));
            nodosPorProcesar.push(anterior->derivacion[2]);
        }
    }
    else if(anterior->simbolo == "\x1B[36mExpresion\x1B[0m"){
        if(expresionOperador(aux, anterior, nodosPorProcesar));
        else if(terminoParentesis(aux, anterior, nodosPorProcesar));
        else{
            anterior->agregarDerivacion("\x1B[38;2;255;128;0mTermino\x1b[0m", aux);
            nodosPorProcesar.push(anterior->derivacion[0]);
        }
    }
    
    else if(anterior->simbolo == "\x1B[38;2;255;128;0mTermino\x1b[0m"){
        if(AFD(aux) == IDENTIFICADOR){
            anterior->agregarDerivacion("\x1b[35mIdentificador\x1b[0m", aux);
        }
        else if(numero.count(AFD(aux))){
            anterior->agregarDerivacion("\x1b[32mNumero\x1b[0m", aux);
        }
    }
    else if(anterior->simbolo == "\x1B[31mOperador\x1B[0m"){
        anterior->agregarDerivacion(aux, aux);
    }
}

// Detectar termino de la forma (<Expresion>)
bool terminoParentesis(string aux, nodo *anterior, queue<nodo *> &nodosPorProcesar){
    if(aux[0] == '(' && aux[aux.size() - 1] == ')'){
        anterior->agregarDerivacion("\x1B[38;2;255;128;0mTermino\x1b[0m", aux);
        anterior = anterior->derivacion[0];
        anterior->agregarDerivacion("(", "(");
        anterior->agregarDerivacion("\x1B[36mExpresion\x1B[0m", aux.substr(1, aux.size() - 2));
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
        // si el primer caracter
        if(i == 0 && signos.find(aux[i]) != signos.end()){
            Eant = false;
            continue;
        }

        else if(aux[i] == 'E'){
            // + / - actuan como signo de un num exponencial
            Eant = true;
            continue;
        }

        else if(aux[i] == '('){
            contpA++;
            Eant = false;
        }

        else if(aux[i] == ')'){
            contpB++;
            Eant = false;
        }
        else if(contpA > contpB){
            Eant = false;
            continue;
        }
        // cuando Eant no es parte de un num exponencial
        else if(operadores.find(aux[i]) != operadores.end() && !Eant){
            anterior->agregarDerivacion("\x1B[36mExpresion\x1B[0m", aux.substr(0, i));
            nodosPorProcesar.push(anterior->derivacion[0]);
            anterior->agregarDerivacion("\x1B[31mOperador\x1B[0m", string(1, aux[i]));
            nodosPorProcesar.push(anterior->derivacion[1]);
            anterior->agregarDerivacion("\x1B[36mExpresion\x1B[0m", aux.substr(i + 1, aux.size()));
            nodosPorProcesar.push(anterior->derivacion[2]);
            Eant = false;
            return true;
        }else{
            Eant = false;
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
