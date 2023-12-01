/* para compilar: 
g++ practica2.cpp -o practica2.exe */
#include <bits/stdc++.h>
 
using namespace std;
typedef long long lli;
typedef long double ld;
/* vector de pares de enteros */
typedef pair<int, int> p2i;
/* vector de enteros */
typedef vector<int> vi;
/* Vector bidimensional de enteros */
typedef vector<vi> vvi;
/* Vector de pares de enteros */
typedef vector<p2i> vp2i;

/*  un token es una palabra clave, un identificador, un operador,
    un número o cualquier otra unidad atómica en el código */
// Estados
// ; no se considera porque sirve para separar las palabras al igual que los parentesis, etc 

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
    MUERTO
};

/*  Tiene como clave objetos del tipo TipoToken
    como valores objetos del tipo string */
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
    {MUERTO, "MUERTO"}
};

class TokenProcesado
{
    public:
    int linea;
    string valor;
    TipoToken tipo;
    TipoToken posibleError;

    TokenProcesado(TipoToken tipo, string valor, int linea){
        this->tipo = tipo;
        this->valor = valor;
        this->linea = linea;
        this->posibleError = INICIAL;
    }
};

// Símbolos
unordered_set<char> noAnalizados = {';','{','}','(',')','[',']',','};
unordered_set<char> comillas = {'"'};
unordered_set<char> operadores = {'*','/','<','>','=', '%'};
unordered_set<char> signos = {'+','-'};
unordered_set<char> octal = {'0', '1', '2', '3', '4', '5', '6', '7'};
unordered_set<char> hexadecimal = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
unordered_set<string> palabras_reservadas_java = {"abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const","public", "return", "short", "static", "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void", "volatile", "while", "continue", "default", "do", "double", "else", "enum", "extends", "final", "finally", "float", "for", "goto", "if", "implements", "import", "instanceof", "int", "interface", "long", "native", "new", "package", "private", "protected", "true", "false", "null", "System.out.println"};
unordered_set<TipoToken> estadosAceptacion = {DECIMAL, OCTAL, HEXADECIMAL, REAL_SIN_EXP, DECIMAL1EXP, DECIMAL2EXP, COMPARADOR, ASIGNACION, ASTERISCO, COMENTARIO_MULTILINEA_INICIO, COMENTARIO_MULTILINEA_FIN, COMENTARIO_UNILINEA, IDENTIFICADOR, PALABRARESERVADA, SIGNO_MENOS, SIGNO_MAS, COMILLAS, DIAGONAL, MODULO, INCREMENTO};
unordered_set<TipoToken> estadosComparacion = {DECIMAL, OCTAL, HEXADECIMAL, REAL_SIN_EXP, DECIMAL1EXP, DECIMAL2EXP, IDENTIFICADOR, PALABRARESERVADA, COMILLAS};
unordered_set<TipoToken> operadores_comparadores = {ASTERISCO, DIAGONAL, MODULO, COMPARADOR, SIGNO_MENOS, SIGNO_MAS};

// Variables globales
TipoToken posibleError = INICIAL;
vector<string> tokens;
vector<TokenProcesado> tokensProcesados;
vector<TokenProcesado> errores;
vi lineasError;

// Funciones
void procesarCadena(string cadena, int num_linea);
TipoToken AFD(string token);
bool esComparable(TipoToken tipoAnt, TipoToken tipoSig);

int main(){

    // Leer archivo
    string nombreArchivo = "texto.java";
    ifstream archivo(nombreArchivo.c_str());
    
    string linea;
    string cadena;
    int num_linea = 1;

    // Por facilidad de implementación
    tokensProcesados.push_back(TokenProcesado(INICIAL, "", num_linea));

    /*  mientras sea posible leer una línea completa 
        del archivo y almacenarla en la variable línea */
    while(getline(archivo, linea)){
        /*  trata a una cadena de texto como una secuencia
            de entrada */
        istringstream ss(linea);

        /*  mientras sea posible extraer palabras de la
            cadena ss
            extrae una palabra del flujo de entrada ss 
            en cada iteración se extráe una nueva palabra y
            se almacena en la variable cadena */



        while(ss >> cadena){
            /*  se manda la palabra actual y el numero
                de línea actual */
            procesarCadena(cadena, num_linea);
        }
        /*  despues de procesar todas las palabras de la
            línea acutal, sigue a la siguiente línea */
        num_linea++;
    }

    // Por facilidad de implementación
    tokensProcesados.push_back(TokenProcesado(INICIAL, "", num_linea));

    bool comentMultAct = false;
    bool comillasAct = false;
    bool ant = false;
    bool sig = false;
    int lineaComentUni = 0;
    // Se hacen aparte para 0 y tokensProcesados.size()-1 por el tema de los indices
    for(int i = 1; i < tokensProcesados.size()-1; i++){
        
        /*  se declara una referencia t a un elemento del vector
            tokensProcesado en la posicion i
            asi facilitamos el acceso al elemento actual 
            sin necesidad de utilizar la notación del puntero */ 
        TokenProcesado &t = tokensProcesados[i];
        TokenProcesado &t_ant = tokensProcesados[i-1];
        TokenProcesado &t_sig = tokensProcesados[i+1];
        
        //cout << "Palabra: " << t.valor << ", Tipo: " << tipoTokenString[t.tipo] <<  ", Linea: " << t.linea << endl;
        
        /*  si la linea del token actual es igual a lineaComentUni
            continúa, lo ignora */
        if(t.linea == lineaComentUni){
            continue;
        }
        /*  se declara una variable tipo y se le asigna el valor del
            miembro tipo del token actual t */
        TipoToken tipo = t.tipo;
        if(tipo == COMENTARIO_UNILINEA){
            lineaComentUni = t.linea;
        }
        else if(tipo == COMENTARIO_MULTILINEA_INICIO){
            comentMultAct = true;
        }
        else if(tipo == COMENTARIO_MULTILINEA_FIN && comentMultAct){
            /*  indica que se ha encontrado al final de un
                comentario multilinea */
            comentMultAct = false;
        }
        /*  si el tipo de token es un comentario multilinea final
            pero comentMultAct es falso, es decir que nunca se 
            dio inicio al comentario multilínea, se considera un error
            y el token se agrega a un vector de errores y el número de
            línea se agrega al vector de líneas error */
        else if(tipo == COMENTARIO_MULTILINEA_FIN && !comentMultAct){
            errores.push_back(t);
            lineasError.push_back(t.linea);
        }
        else if(comentMultAct){
            continue;
        }
        /* inicio de un token con comillas */
        else if(tipo == COMILLAS && !comillasAct){
            comillasAct = true;
        }
        else if(tipo == COMILLAS && comillasAct){
            comillasAct = false;
        }
        else if(comillasAct){
            continue;
        }
        /* si no llego a ningún estado de aceptación */
        else if(tipo == MUERTO){
            errores.push_back(t);
            lineasError.push_back(t.linea);
        }
        else if(estadosAceptacion.count(tipo) == 0){
            t.posibleError = t.tipo;
            errores.push_back(t);
            lineasError.push_back(t.linea);
        }
        /* si el tipo de token es un operador comparador */
        else if(operadores_comparadores.count(tipo) > 0){
            if(esComparable(t_ant.tipo, t_sig.tipo)){
                continue;
            }
            else{
                t.posibleError = t.tipo;
                errores.push_back(t);
                lineasError.push_back(t.linea);
            }
        }
        else if(tipo == ASIGNACION){
            if(t_ant.tipo == IDENTIFICADOR && (estadosComparacion.count(t_sig.tipo) > 0)){
                continue;
            }
            else{
                t.posibleError = t.tipo;
                errores.push_back(t);
                lineasError.push_back(t.linea);
            }
        }
        else if(tipo == INCREMENTO){
            if(t_ant.tipo == IDENTIFICADOR){
                continue;
            }
            else{
                t.posibleError = t.tipo;
                errores.push_back(t);
                lineasError.push_back(t.linea);
            }
        }
    }
    
    if(errores.empty()){
        cout << "\033[36mNo hay errores de analisis lexico en las constantes del archivo \033[0m" << nombreArchivo <<endl;
    }else
    {
        for(TokenProcesado e: errores){
        cout << "\n\033[31mError en la linea \033[0m \033[36m" << e.linea << "\033[0m: " << e.valor << endl;
        cout << "\033[33mPosible error: \033[0m" << tipoTokenString[e.posibleError] << endl;
        }
    }
    
    

    

    return 0;
}


// Funciones

/*
    Divide la cadena en tokens de acuerdo con
    los caracteres especiales
*/

void procesarCadena(string cadena, int num_linea){
    if(cadena.size() == 0) return;
    /* se inicializa el auxiliar vacío */
    string aux = "";

    // Comentario Unilinea
    if(cadena.size() >= 2){
        if(cadena[0] == '/' && cadena[1] == '/'){
            /*  se agrega al final del vector tokens
                la cadena completa */
            tokens.push_back(cadena);
            /*  se crea un objeto TokenProcesado utilizando el constructor
                de la clase
                ADN(cadena) --> tipo
                cadena --> valor
                num_linea --> linea
                este objeto se agrega al final de otro vector 
                llamado tokensProcesados
            */
            tokensProcesados.push_back(TokenProcesado(AFD(cadena), cadena, num_linea));
            /*  devuelve un iterador al último elemento del vector
                NO ENTENDER *************************************

            */
            tokensProcesados.rbegin()->posibleError = posibleError;
            /*  retorna después de procesar un comentario de una
                sola línea*/
            return;
        }
    }

    // Comentario Multilinea
    if(cadena.size() >= 2){
        if(cadena[0] == '/' && cadena[1] == '*'){
            tokens.push_back(cadena);
            tokensProcesados.push_back(TokenProcesado(AFD(cadena), cadena, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            return;
        }
    }

    if(cadena.size() >= 2){
        if(cadena[0] == '*' && cadena[1] == '/'){
            tokens.push_back("*/");
            tokensProcesados.push_back(TokenProcesado(AFD("*/"), "*/", num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            return;
        }
    }

    // Comparadores de dos caracteres
    if(cadena.size() >= 2){
        /* operador de igualdad o desigualdad */
        if((cadena[0] == '=' || cadena[0] == '!' || cadena[0] == '+' || cadena[0] == '-' || cadena[0] == '>' || cadena[0] == '<') && cadena[1] == '='){
            /* se concatenan en la cadena aux */
            aux += cadena[0];
            aux += cadena[1];
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            aux = "";
            /*  se eliminan los dos primeros caracteres de 
                la cadena, una vez que fueron procesados
                y almacenados */
            cadena = cadena.substr(2);
        }
        else if((cadena[0] == '+' && cadena[1] == '+') || (cadena[0] == '-' && cadena[1] == '-')){
            /* se concatenan en la cadena aux */
            aux += cadena[0];
            aux += cadena[1];
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            aux = "";
            /*  se eliminan los dos primeros caracteres de 
                la cadena, una vez que fueron procesados
                y almacenados */
            cadena = cadena.substr(2);
        }
    }



    // Por cada caracter especial, divide la cadena y agrega el resultado a tokens
    /* itera a través de cada caracter c de la cadena */
    for(char c: cadena){
        // si está presente en el connjunto noAnalizados
        if(noAnalizados.count(c)){
            // se ha completado una palabra/token previo
            if(!aux.empty()){
                /*  la cadena aux que contiene el token completo se
                    agrega al final del vector tokens */
                tokens.push_back(aux);
                /*  se crea un nuevo objeto que se agrega al
                    final del vector tokens procesados */
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
                tokensProcesados.rbegin()->posibleError = posibleError;
            }
            aux = "";
        }
        else if(comillas.count(c)){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
                tokensProcesados.rbegin()->posibleError = posibleError;
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            aux = "";
        }
        // Como ya se separó del atenterior, solo se da el caso donde forma parte de la palabra
        else if(signos.count(c)){
            aux += c;
        }
        else if(operadores.count(c)){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
                tokensProcesados.rbegin()->posibleError = posibleError;
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            aux = "";
        }
        else{
            aux += c;
        }

    }

    if(!aux.empty()){
        tokens.push_back(aux);
        tokensProcesados.push_back(TokenProcesado(AFD(aux), aux, num_linea));
        tokensProcesados.rbegin()->posibleError = posibleError;
    }
}

/* devuelve el tipo de token */
TipoToken AFD(string token){
    TipoToken Estado = INICIAL;
    /* itera cada caracter c en la cadena token */
    for(char c: token){
        
        if(Estado == MUERTO) return MUERTO;

        /* si está presente en las palabras reservadas */
        if(palabras_reservadas_java.count(token)){
            return PALABRARESERVADA;
        }

        if(Estado != MUERTO){
            /*  se actualiza la variable global con
                el valor de Estado */
            posibleError = Estado;
        }

        switch(Estado){

            case INICIAL:
                /* si c es un dígito y es diferente de 0 */
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
                /* si c es una letra (alfabético) */
                else if(isalpha(c)){
                    Estado = IDENTIFICADOR;
                }
                else if(c == '"'){
                    Estado = COMILLAS;
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
                    Estado = ASIGNACION;
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
                    Estado = ASIGNACION;
                }
                else if(c == '+'){
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

bool esComparable(TipoToken tipoAnt, TipoToken tipoSig){
    /*  si el tipo anterior o el tipo siguiente no está dentro de los 
        estados de comparación, retorna false
    */
    if(estadosComparacion.count(tipoAnt) == 0 || estadosComparacion.count(tipoSig) == 0){
        return false;
    }
    else if(tipoAnt != IDENTIFICADOR && tipoSig != IDENTIFICADOR){
        if(tipoAnt != tipoSig){
            return false;
        }
    }
    
    return true;
}