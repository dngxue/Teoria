#include <bits/stdc++.h>
 
using namespace std;
typedef long long lli;
typedef long double ld;
typedef pair<int, int> p2i;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<p2i> vp2i;


// Estados
// ; no se considera porque sirve para separar las palabras al igual que los parentesis, etc 

enum TipoToken{
    INICIAL,
    DECIMAL,
    OCTAL_O_HEXADECIMAL,
    OCTAL,
    POSIBLE_HEXADECIMAL,
    HEXADECIMAL,
    SIGNO,
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
    MUERTO
};

unordered_map<TipoToken, string> tipoTokenString = {
    {INICIAL, "INICIAL"},
    {DECIMAL, "DECIMAL"},
    {OCTAL_O_HEXADECIMAL, "OCTAL_O_HEXADECIMAL"},
    {OCTAL, "OCTAL"},
    {POSIBLE_HEXADECIMAL, "POSIBLE_HEXADECIMAL"},
    {HEXADECIMAL, "HEXADECIMAL"},
    {SIGNO, "SIGNO"},
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
unordered_set<char> hexadecimal = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                         'A', 'B', 'C', 'D', 'E', 'F'};
unordered_set<string> palabras_reservadas_java = {"abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const","public", "return", "short", "static", "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void", "volatile", "while", "continue", "default", "do", "double", "else", "enum", "extends", "final", "finally", "float", "for", "goto", "if", "implements", "import", "instanceof", "int", "interface", "long", "native", "new", "package", "private", "protected", "true", "false", "null", };
unordered_set<TipoToken> estadosAceptacion = {DECIMAL, OCTAL, HEXADECIMAL, REAL_SIN_EXP, DECIMAL1EXP, DECIMAL2EXP, COMPARADOR, ASIGNACION, ASTERISCO, COMENTARIO_MULTILINEA_INICIO, COMENTARIO_MULTILINEA_FIN, COMENTARIO_UNILINEA, IDENTIFICADOR, PALABRARESERVADA, SIGNO, COMILLAS, DIAGONAL, MODULO};
unordered_set<TipoToken> estadosComparacion = {DECIMAL, OCTAL, HEXADECIMAL, REAL_SIN_EXP, DECIMAL1EXP, DECIMAL2EXP, IDENTIFICADOR};
unordered_set<TipoToken> operadores_comparadores = {ASTERISCO, DIAGONAL, MODULO, COMPARADOR, SIGNO};

// Variables globales
TipoToken posibleError = INICIAL;
vector<string> tokens;
vector<TokenProcesado> tokensProcesados;
vector<TokenProcesado> errores;
vi lineasError;

// Funciones
void procesarCadena(string cadena, int num_linea);
TipoToken ADF(string token);
bool esComparable(TipoToken tipoAnt, TipoToken tipoSig);

int main(){

    // Leer archivo
    string nombreArchivo = "texto.java";
    ifstream archivo(nombreArchivo.c_str());
    
    string linea;
    string cadena;
    int num_linea = 1;
    while(getline(archivo, linea)){
        istringstream ss(linea);
        while(ss >> cadena){
            procesarCadena(cadena, num_linea);
        }
        num_linea++;
    }

    // cout << "\n\nResultado: " << endl;
    // for(string s: tokens){
    //     cout << s << endl;
    // }

    // Guardar los errores en un arreglo

    bool comentMultAct = false;
    bool ant = false;
    bool sig = false;
    int lineaComentUni = 0;
    // Se hacen aparte para 0 y tokensProcesados.size()-1 por el tema de los indices
    for(int i = 1; i < tokensProcesados.size()-1; i++){
        
        TokenProcesado &t = tokensProcesados[i];
        TokenProcesado &t_ant = tokensProcesados[i-1];
        TokenProcesado &t_sig = tokensProcesados[i+1];
        
        //cout << "Palabra: " << s.valor << ", Tipo: " << tipoTokenString[s.tipo] <<  ", Linea: " << s.linea << endl;
        
        if(t.linea == lineaComentUni){
            continue;
        }

        TipoToken tipo = t.tipo;
        if(tipo == COMENTARIO_UNILINEA){
            lineaComentUni = t.linea;
        }
        else if(tipo == COMENTARIO_MULTILINEA_INICIO){
            comentMultAct = true;
        }
        else if(tipo == COMENTARIO_MULTILINEA_FIN && comentMultAct){
            comentMultAct = false;
        }
        else if(tipo == COMENTARIO_MULTILINEA_FIN && !comentMultAct){
            errores.push_back(t);
            lineasError.push_back(t.linea);
        }
        else if(comentMultAct){
            continue;
        }
        else if(tipo == MUERTO){
            errores.push_back(t);
            lineasError.push_back(t.linea);
        }
        else if(estadosAceptacion.count(tipo) == 0){
            t.posibleError = t.tipo;
            errores.push_back(t);
            lineasError.push_back(t.linea);
        }
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
    }

    // for(TokenProcesado &t: tokensProcesados){
    //     //cout << "Palabra: " << s.valor << ", Tipo: " << tipoTokenString[s.tipo] <<  ", Linea: " << s.linea << endl;
        
    //     if(t.linea == lineaComentUni){
    //         continue;
    //     }

    //     TipoToken tipo = t.tipo;
    //     if(tipo == COMENTARIO_UNILINEA){
    //         lineaComentUni = t.linea;
    //     }
    //     else if(tipo == COMENTARIO_MULTILINEA_INICIO){
    //         comentMultAct = true;
    //     }
    //     else if(tipo == COMENTARIO_MULTILINEA_FIN && comentMultAct){
    //         comentMultAct = false;
    //     }
    //     else if(tipo == COMENTARIO_MULTILINEA_FIN && !comentMultAct){
    //         errores.push_back(t);
    //         lineasError.push_back(t.linea);
    //     }
    //     else if(comentMultAct){
    //         continue;
    //     }
    //     else if(tipo == MUERTO){
    //         errores.push_back(t);
    //         lineasError.push_back(t.linea);
    //     }
    //     else if(estadosAceptacion.count(tipo) == 0){
    //         t.posibleError = t.tipo;
    //         errores.push_back(t);
    //         lineasError.push_back(t.linea);
    //     }
    // }
    
    for(TokenProcesado e: errores){
        cout << "Error en la linea " << e.linea << ": " << e.valor << endl;
        cout << "Posible de error: " << tipoTokenString[e.posibleError] << endl;
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
    string aux = "";

    // Comentario Unilinea
    if(cadena.size() >= 2){
        if(cadena[0] == '/' && cadena[1] == '/'){
            tokens.push_back(cadena);
            tokensProcesados.push_back(TokenProcesado(ADF(cadena), cadena, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            return;
        }
    }

    // Comentario Multilinea
    if(cadena.size() >= 2){
        if(cadena[0] == '/' && cadena[1] == '*'){
            tokens.push_back(cadena);
            tokensProcesados.push_back(TokenProcesado(ADF(cadena), cadena, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            return;
        }
    }

    if(cadena.size() >= 2){
        if(cadena[0] == '*' && cadena[1] == '/'){
            tokens.push_back("*/");
            tokensProcesados.push_back(TokenProcesado(ADF("*/"), "*/", num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            return;
        }
    }

    // Comparadores de dos caracteres
    if(cadena.size() >= 2){
        if((cadena[0] == '=' || cadena[1] == '!') && cadena[1] == '='){
            aux += cadena[0];
            aux += cadena[1];
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            aux = "";
        }
    }

    // Por cada caracter especial, divide la cadena y agrega el resultado a tokens
    for(char c: cadena){

        if(noAnalizados.count(c)){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
                tokensProcesados.rbegin()->posibleError = posibleError;
            }
            aux = "";
        }
        else if(comillas.count(c)){
            if(!aux.empty()){
                tokens.push_back(aux);
                tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
                tokensProcesados.rbegin()->posibleError = posibleError;
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
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
                tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
                tokensProcesados.rbegin()->posibleError = posibleError;
            }
            aux = "";
            aux += c;
            tokens.push_back(aux);
            tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
            tokensProcesados.rbegin()->posibleError = posibleError;
            aux = "";
        }
        else{
            aux += c;
        }

    }

    if(!aux.empty()){
        tokens.push_back(aux);
        tokensProcesados.push_back(TokenProcesado(ADF(aux), aux, num_linea));
        tokensProcesados.rbegin()->posibleError = posibleError;
    }
}

TipoToken ADF(string token){
    TipoToken Estado = INICIAL;

    for(char c: token){
        
        if(Estado == MUERTO) return MUERTO;

        if(palabras_reservadas_java.count(token)){
            return PALABRARESERVADA;
        }

        if(Estado != MUERTO){
            posibleError = Estado;
        }

        switch(Estado){

            case INICIAL:
                if(isdigit(c) && c != '0'){
                    Estado = DECIMAL;
                }
                else if(c == '0'){
                    Estado = OCTAL_O_HEXADECIMAL;
                }
                else if(signos.count(c)){
                    Estado = SIGNO;
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

            case SIGNO:
                if(isdigit(c) && c != '0'){
                    Estado = DECIMAL;
                }
                else if(c == '0'){
                    Estado = OCTAL_O_HEXADECIMAL;
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