#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#define ubicacion "../output/resultados.txt"
/*------------------------Metodos numericos (diferenciación numérica, derivadas numericas)-----------------------*/
using namespace std;

class base{
    protected:// Para que pueda usarlo en todas las clases heredadas
        short expMax, expMax2; // expMax es para indicar el grado de un polinomio. expMax2 es para lo mismo en caso de elegir funciones homograficas
        double suma_imagen_num, suma_imagen_den_o_noArg; // para ir guardando el valor que me queda al reemplazar por cada uno de los valores obtenidos en i en f(x), se reutiliza en el pasaje
        double resultadoFinal[4];
        string fDeX = "";
        string* fragmentosFdeX = nullptr;
    public:
        base(short _expMax, short _expMax2){
            expMax = _expMax;
            expMax2 = _expMax2;
        }
        ~base(){};

        void guardarResultados(string mensaje){
            const string resultados = "../output/resultados.txt";
            ofstream archivo(resultados); //ios::app es para abrir el archivo en modo de apertura, permite agregar datos en vez de sobreescribirlos

            if(!archivo.is_open()){
                perror("Error al abrir el archivo para guardar los datos");
                return;
            }

            cout << mensaje << endl;
            archivo << mensaje;
            archivo.close();
            cout << "Resultados de derivacion guardados en: " << resultados << endl;
        };

        string extraerFuncion(double* ecuacion, short j) {
            string fragmentoFuncion = ""; // Inicializamos un fragmento de la función

            if (ecuacion[j] != 0) {
                int valorEnInt = round(ecuacion[j]);
                if (j > 0) {
                    fragmentoFuncion += "(" + to_string(valorEnInt) + "x^" + to_string(j) + ")" + "+";
                } else {
                    fragmentoFuncion += "(" + to_string(valorEnInt) + ")";
                }
            }
            return fragmentoFuncion; // Devolvemos el fragmento de la función
        };
        
        string doubleToString(double *varParaConvertir) {
            string constanteStr = to_string(*varParaConvertir);
            size_t posComa = constanteStr.find('.'); // size_t sirve para almacenar el desplazamiento de memoria
            
            if (posComa != string::npos) { //npos es para indicar una posicion no encontrada o resultado invalido
                constanteStr = constanteStr.substr(0, posComa + 3);
            }
            
            return constanteStr;
        }

        short ingresoEcuacion(double *ecuacion, char opcion, double *denom_o_noArgumento, short mayExp, short mayExp2, double *constanteSuma, double *constante, char opcion2){
            string fDeX_fragmento = "";
            cout << endl;
            if(opcion=='2'){
                cout << "  Numerador\n";
            }else if(opcion=='3'){
                cout << "  Argumento\n";
            }else if(opcion=='4'){
                cout << "  Potencia\n";
            }
            for (short i = mayExp; i >= 0; i--){
                if (i==0){
                    cout << "T.I: "; cin >> ecuacion[i];
                }else{
                    cout << "x^"<<i<<": "; cin >> ecuacion[i];
                }
                fDeX_fragmento = extraerFuncion(ecuacion, i);
                fDeX += fDeX_fragmento;
            }

            if(opcion=='2'){
                string fDeX_numerador = "";
                string fDeX_denominador = "";
                fDeX_numerador = fDeX;
                fDeX = "";
                short bandera=0; // si no cambia a 1 es porque no se ingreso ningun valor valido para el denominador
                cout << "\n  Denominador\n";
                for (short i = mayExp2; i >= 0; i--){
                    if (i==0){
                        cout << "T.I: "; cin >> denom_o_noArgumento[i];
                        if(bandera==0 && denom_o_noArgumento[i]==0){ // no se ingreso un coeficiente valido y el denominador es = 0, error matematico
                            return 1;
                        }
                    }else{
                        cout << "x^"<<i<<": "; cin >> denom_o_noArgumento[i];
                        if(denom_o_noArgumento[i]!=0){
                            bandera=1; // hay al menos un termino !=0, da igual si el T.I es ==0
                        } // si la bandera no cambia, SI O SI el denominador tiene que ser un numero natural!=0 para que no sea un error matematico
                    }
                    fDeX_fragmento = extraerFuncion(denom_o_noArgumento, i);
                    fDeX_denominador += fDeX_fragmento;
                }
                fDeX = fDeX_numerador + "/" + fDeX_denominador;
            }else if(opcion=='3'){
                string fDeX_noArgumento = "";
                cout << "\n  No argumento (si no hay ingresar '0')\n";
                for (short i=mayExp2; i>=0; i--){
                    if (i==0){
                        cout << "T.I: "; cin >> denom_o_noArgumento[i];
                    }else{
                        cout << "x^"<<i<<": "; cin >> denom_o_noArgumento[i];
                    }
                    fDeX_fragmento = extraerFuncion(denom_o_noArgumento, i);
                    fDeX_noArgumento += fDeX_fragmento;
                }
                switch (opcion2) {
                    case '1':
                        if(mayExp2 != 0 || fDeX_noArgumento != "")
                            fDeX = "Sin(" + fDeX + ")" + " + " + fDeX_noArgumento ;
                        else
                            fDeX = "Sin(" + fDeX + ")";
                        break;
                    case '2':
                        if(mayExp2 != 0 || fDeX_noArgumento != "")
                            fDeX = "Cos(" + fDeX + ")" + " + " + fDeX_noArgumento ;
                        else
                            fDeX = "Cos(" + fDeX + ")";
                        break;
                    case '3':
                        if(mayExp2 != 0 || fDeX_noArgumento != "")
                            fDeX = "Tan(" + fDeX + ")" + " + (" + fDeX_noArgumento + ")";
                        else
                            fDeX = "Tan(" + fDeX + ")";
                        break;
                }   
            }else if (opcion=='4'){
                cout << "Constante a sumar: ";
                cin >> *constanteSuma;
                fDeX = doubleToString(constante) + "^(" + fDeX + ")" + " + " + doubleToString(constanteSuma);
                cout << fDeX;
            }
            return 0;
        };
};

class derivadas : public base{
    private:
        double x=0;
        double h; // Los saltos que va a dar segun x
        double resultados[4]; // Me va a guardar los resultados de f(x) (resu_ximenos2,resu_ximenos1, resu_ximas1, etc)
    public:
        derivadas(double, short, short, double);
        ~derivadas(){}// Destructor
        void operaciones(double*, char, double*, double, float, char);
        void der1(double[]);
        void der2(double[]);
        void der3(double[]);
        void der4(double[]);
        string crearMensaje(double[], string);
};
derivadas::derivadas(double _x, short _expMax,short _expMax2,double _h) : base(_expMax,_expMax2){// Constructor
    x = _x;
    h = _h;
}

string derivadas::crearMensaje(double resultadoFinal[], string fDeX){
    string mensaje = "\nLas derivadas de la funcion: '" + fDeX + "' son:\n\n" +
    /*Se imprimen los resultados de las derivadas. "to_string" convierte el resultadoFinal[] en string*/
                        "\tf'(x): " + to_string(resultadoFinal[0]) + "\n"
                        "\tf''(x): " + to_string(resultadoFinal[1]) + "\n"
                        "\tf'''(x): " + to_string(resultadoFinal[2]) + "\n"
                        "\tf''''(x): " + to_string(resultadoFinal[3]) + "\n"; //Si el numero es una locura como 5.539252e69 entonces el string se convierte en 0.0000000
    cout << mensaje;
    return mensaje;
}



void derivadas::operaciones(double *ecuacion, char opcion, double *denom_o_noArgumento, double constante, float amplitud, char opcion2){ //*ecuacion lo uso como numerador para las homograficas
    double resultado_x; // Me sirve para mostrar todos los resultados de la operacion al reemplazar x
    short contador=0;
    string fDeX_fragmento;
    double tolerance = 1e-6;  // Una pequeña tolerancia para valores cercanos a x+(2*h)

    for (double i = x-(2*h); i <= x+(2*h)+tolerance; i += h){
        if (opcion=='1'){
            resultado_x=0; // para que no se acumulen los resultados tras cada ciclo
            for (short j = expMax; j >= 0; j--){
                resultado_x += (ecuacion[j])*(pow(i,j));
                cout <<ecuacion[j]<<" * " <<i<<"^"<<j<<" = "<<resultado_x<<endl;
                if (i==x-(2*h)){
                    resultados[contador] = resultado_x;
                }
                else if(i==x-h){
                    resultados[contador] = resultado_x;
                }
                else if (i==x){
                    resultados[contador] = resultado_x;
                }
                else if (i==x+h){
                    resultados[contador] = resultado_x;
                }
                else{
                    resultados[contador] = resultado_x;
                }
            }
            contador++;
        }
        else if(opcion=='2'){
            // Suma numerador
            cout << endl;
            suma_imagen_num=0;// Reseteo los valores para que no se acumulen tras cada ciclo
            suma_imagen_den_o_noArg=0;
            for (short j = expMax; j >= 0; j--){
                if (j==0){
                    suma_imagen_num += ecuacion[j];
                }else{
                    suma_imagen_num += (ecuacion[j])*(pow(i,j));
                }
            }
            // Suma denominador
            for (short j = expMax2; j >= 0; j--){
                if (j==0){
                    suma_imagen_den_o_noArg+=denom_o_noArgumento[j];
                }else{
                    suma_imagen_den_o_noArg+=(denom_o_noArgumento[j])*(pow(i,j));
                }
            }
            // Primero hago la operacion y despues la guardo en cada variable dependiendo de el valor que tenga
            resultado_x = suma_imagen_num/suma_imagen_den_o_noArg;
            if (i==x-(2*h)){
                resultados[contador] = resultado_x;
            }
            else if(i==x-h){
                resultados[contador] = resultado_x;
            }
            else if (i==x){
                resultados[contador] = resultado_x;
            }
            else if (i==x+h){
                resultados[contador] = resultado_x;
            }
            else{
                resultados[contador] = resultado_x;
            }
            contador++; // Me sirve para poder darle un valor al arreglo "resultados[4]"
        }
        else if(opcion=='3'){
            suma_imagen_num=0;
            suma_imagen_den_o_noArg=0;
                
            for(short j=expMax; j>=0; j--){
                if(j==0){
                    suma_imagen_num += ecuacion[j];
                }else{
                    suma_imagen_num += (ecuacion[j])*(pow(i,j));
                }
            }
            for(short j=expMax2; j>=0; j--){
                if(j==0){
                    suma_imagen_den_o_noArg += denom_o_noArgumento[j];
                }else{
                    suma_imagen_den_o_noArg += (denom_o_noArgumento[j])*(pow(i,j));
                }
            }
            switch(opcion2){
                case '1':   resultado_x = amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg);
                    fDeX_fragmento = "Seno(" + fDeX + ")";
                    break;
                case '2':   resultado_x = amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                    fDeX_fragmento = "Seno(" + fDeX + ")";
                    break;
                case '3':   resultado_x = amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
                    fDeX_fragmento = "Seno(" + fDeX + ")";
                    break;
            }
                
            if (i==x-(2*h)){
                resultados[contador] = resultado_x;
            }
            else if(i==x-h){
                resultados[contador] = resultado_x;
            }
            else if (i==x){
                resultados[contador] = resultado_x;
            }
            else if (i==x+h){
                resultados[contador] = resultado_x;
            }
            else{
                resultados[contador] = resultado_x;
            }
            contador++;
        }
        else if(opcion=='4'){
            // 2^(x^2+2x)
            resultado_x=0; // para que no se acumulen los resultados tras cada ciclo
            for (short j = expMax; j >= 0; j--){
                resultado_x += (ecuacion[j])*(pow(i,j));
                cout <<ecuacion[j]<<" * " <<i<<"^"<<j<<" = "<<resultado_x<<endl;
            }
            // Una vez corregido, poner los if adentro del for
            if (i==x-(2*h)){
                resultados[contador] = pow(constante, resultado_x);
                cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                cout << "\n\nX = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else if(i==x-h){
                resultados[contador] = pow(constante, resultado_x);
                cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else if (i==x){
                resultados[contador] = pow(constante, resultado_x);
                cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else if (i==x+h){
                resultados[contador] = pow(constante, resultado_x);
                cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else{
                resultados[contador] = pow(constante, resultado_x);
                cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            contador++;
        }
    }
    if(opcion==3)
        fDeX = fDeX_fragmento;
    // system("pause");
    // cout << resultados[0]<<endl;
    // cout << resultados[1]<<endl;
    // cout << resultados[2]<<endl;
    // cout << resultados[3]<<endl;
    // cout << resultados[4]<<endl;
    // system("pause");
    cout << "f(x): "<<fDeX << "\n";
    der1(resultadoFinal);
    der2(resultadoFinal);
    der3(resultadoFinal);
    der4(resultadoFinal);
    guardarResultados(crearMensaje(resultadoFinal, fDeX));
}
void derivadas::der1(double resultadoFinal[]){
    resultadoFinal[0] = (-resultados[4]+8*resultados[3]-8*resultados[1]+resultados[0])/(12*h);
    //cout << resultadoFinal[0];
}
void derivadas::der2(double resultadoFinal[]){
    resultadoFinal[1] = ((resultados[3])-(2*resultados[2])+(resultados[1]))/(pow(h,2));
    //cout << resultadoFinal[1];
}
void derivadas::der3(double resultadoFinal[]){
    resultadoFinal[2] = ((resultados[4])-(2*resultados[3])+(2*resultados[1])-(resultados[0]))/(2*pow(h,3));
    //cout << resultadoFinal[2];
}
void derivadas::der4(double resultadoFinal[]){
    resultadoFinal[3] = ((resultados[4])-(4*resultados[3])+(6*resultados[2])-(4*resultados[1])+(resultados[0]))/(pow(h,4));
    //cout << resultadoFinal[3];
}
/*-----------------------Mï¿½todo del trapecio (integraciï¿½n numï¿½rica)-----------------------*/
class integral : public base{
    private:
        double a; // limite inferior
        double b; // limite superior
        int n; // subintervalos
        double deltaX;
        double area=0;
    public:
        integral(short, short, double, double, int);
        ~integral(){}
        void calcular_xi(double *, char, double*, float, char, double, double);
        string crearMensaje(double[], double[], string, double, double);
};
integral::integral(short _expMax, short _expMax2, double _a, double _b, int _n) : base(_expMax, _expMax2){ // Heredo 3 variables de la clase base
    a = _a;
    b = _b;
    n = _n;
}

string integral::crearMensaje(double resultadoFinal[], double xExtremos[],string fDeX, double a, double b){
    string mensaje = "\nLos resultados de la intregracion del intervalo entre: "+ to_string(a) + " y " + to_string(b) + " de la F(x) " + fDeX + " son:\n\n" +
                        "\t~Delta X: " + to_string(resultadoFinal[0]) + "\n"
                        "\t~Suma de los subintervalos de los extremos: " + to_string(resultadoFinal[1]) + "\n"
                        "\t\t·Extremo inferior: " + to_string(xExtremos[0]) + "\n"
                        "\t\t·Extremo superior: " + to_string(xExtremos[1]) + "\n\n"
                        "\t~Suma de los subintervalos del medio: " + to_string(resultadoFinal[2]) + "\n"
                        "\t~Area total es: " + to_string(resultadoFinal[3]) + "u^2" + "\n";
    cout << mensaje;
    return mensaje;
};

void integral::calcular_xi(double *ecuacion, char opcion, double *denom_o_noArgumento, float amplitud, char opcion2, double constante, double constanteSumar){
    deltaX = (b-a)/n;   // calculo el valor de deltaX
    float difLimites=b-a; // La diferencia entre los limites. Me sirve para calcular el area cuando la funcion exponencial tiene una constante al final
    double xExtremos[2]; // 0 = a xExtremo del limite inferior 'a' y 1 = a xExtremo del limite superior 'b'
    double suma_xExtremos=0;// 2^(x)+2
    double suma_imagenMedio=0;
    short salir=0;

    for(short i=0; i<=n && salir==0; i++){ // este bucle me mueve por cada indice de i en la tabla
        double resultado_x_i = a+(i*deltaX); // aca calculo el valor de x sub i
    
        suma_imagen_num=0;
        suma_imagen_den_o_noArg=0;

        for(short exp=expMax; exp>=0; exp--){
            if(exp==0){ // termino independiente
                suma_imagen_num += ecuacion[exp];
            }else{
                suma_imagen_num += ecuacion[exp] * pow(resultado_x_i, exp);
            }
        }
        if(opcion=='2' || opcion=='3'){
            for(short exp=expMax2; exp>=0; exp--){
                if(exp==0){
                    suma_imagen_den_o_noArg += denom_o_noArgumento[exp];
                }else{
                    suma_imagen_den_o_noArg += denom_o_noArgumento[exp] * pow(resultado_x_i, exp);
                }
            }
        }

        if(opcion=='1'){ // funcion polinomica
            if(i==0 || i==n){
                suma_xExtremos += suma_imagen_num;
                    if(i==0) // Extracción de extremos
                        xExtremos[0] = suma_imagen_num;
                    else if(i==n)
                        xExtremos[1] = suma_imagen_num;
            }else{
                suma_imagenMedio += suma_imagen_num;
            }
        }else if(opcion=='2'){ // funcion racional
            if(suma_imagen_num==0 && suma_imagen_den_o_noArg==0){
                cout << "La funcion en f(" << resultado_x_i << ") es = 0/0 (indeterminacion). Intente calcular con un valor distinto para 'n'\n";
                salir=1;
            }else if(suma_imagen_num!=0 && suma_imagen_den_o_noArg==0){
                cout << "La funcion en f(" << resultado_x_i << ") es = m/0 (siendo m un numero real != 0), es decir que el area va a ser = infinito.\n";
                cout << "Intente calcular con un valor distinto para 'n'\n\n";
                salir=1;
            }else{
                if(i==0 || i==n){
                    suma_xExtremos += (suma_imagen_num/suma_imagen_den_o_noArg);
                    if(i==0){ // Extracción de extremos
                        xExtremos[0] = (suma_imagen_num/suma_imagen_den_o_noArg);
                    }else{
                        xExtremos[1] = (suma_imagen_num/suma_imagen_den_o_noArg);
                    }
                }else{
                    suma_imagenMedio += (suma_imagen_num/suma_imagen_den_o_noArg);
                }
            }
        }else if(opcion=='3'){ // funcion trigonometrica
            switch(opcion2){
                case '1':   if(i==0 || i==n){
                                suma_xExtremos += amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                if (i == 0){ // Extracción de xExtremos
                                    xExtremos[0] = amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg); 
                                    cout << xExtremos[0] << endl;//debug
                                } else{
                                    xExtremos[1] = amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                    cout << xExtremos[1] << endl;//debug
                                }
                            }else{
                                suma_imagenMedio += amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg);
                            }
                    break;
                case '2':   if(i==0 || i==n){
                                suma_xExtremos += amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                if (i == 0){ // Extracción de xExtremos
                                    xExtremos[0] = amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                    cout << xExtremos[0] << endl;//debug
                                } else{
                                    xExtremos[1] = amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                    cout << xExtremos[1] << endl;//debug
                                }
                            }else{
                                suma_imagenMedio += amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                            }
                    break;
                case '3':   if(i==0 || i==n){
                                suma_xExtremos += amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                if (i == 0){ // Extracción de xExtremos
                                    xExtremos[0] = amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg); 
                                    cout << xExtremos[0] << endl; //debug
                                } else{
                                    xExtremos[1] = amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                    cout << xExtremos[1] << endl;//debug
                                }
                            }else{
                                suma_imagenMedio += amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
                            }
                    break;
            }
        }else if(opcion=='4'){
            double resultadoFinal = 0; // me sirve para elevar el resultado de la ecuacion a la constante
            // cout << "Resultado x_i["<<i<<"]: "<<resultado_x_i<<endl;
            if(i==0 || i==n){ 
                suma_imagen_num=0;
                for(short exp=expMax; exp>=0; exp--){
                    suma_imagen_num += ecuacion[exp] * pow(resultado_x_i,exp);
                }
                resultadoFinal += pow(constante,suma_imagen_num);
                suma_xExtremos+=resultadoFinal;
                if (i == 0){
                            xExtremos[0] = resultadoFinal; 
                        } else {
                            xExtremos[1] = resultadoFinal;
                        }
                cout <<constante<<"^"<<suma_imagen_num<<" = "<<resultadoFinal<<endl;
                suma_xExtremos = xExtremos[0] + xExtremos[1];
                cout << "extremos: "<<suma_xExtremos<<endl;//debug
                system("pause");
            }else{
                suma_imagen_num=0;
                for(short exp=expMax; exp>=0; exp--){
                    suma_imagen_num += ecuacion[exp] * pow(resultado_x_i,exp);
                }
                resultadoFinal += pow(constante,suma_imagen_num);
                cout <<constante<<"^"<<suma_imagen_num<<" = "<<resultadoFinal<<endl;//debug
                system("pause");
                suma_imagenMedio += resultadoFinal;
                cout << "Imagen medio con x = "<<resultado_x_i<<" : "<<suma_imagenMedio<<endl;//debug
            }
        }
    }
    suma_xExtremos = (xExtremos[0] + xExtremos[1]);

    cout <<"Delta x: " <<deltaX << endl;
    cout <<"Suma de las imagenes del medio: " <<suma_imagenMedio<<endl;
    cout <<"Suma de los extremos: " <<suma_xExtremos<<endl;
    cout << "a-b="<<difLimites<<endl;
    resultadoFinal[0] = deltaX;
    resultadoFinal[1] = suma_xExtremos;
    resultadoFinal[2] = suma_imagenMedio;
    

    if(salir==0){
        area = (deltaX/2)*(2*suma_imagenMedio + suma_xExtremos) + (difLimites*constanteSumar);
        
        if (area>=0){
            resultadoFinal[3] = area;
        }else{  
            resultadoFinal[3] = abs(area);
        }
    guardarResultados(crearMensaje(resultadoFinal, xExtremos, fDeX, a, b));
    }
}
void ingresoExponentes(short *mayExp, short *mayExp2, char opcion){
    if (opcion=='1'){
        do{
            cout << "Grado del polinomio: "; cin >> *mayExp;
        } while (*mayExp<0);   
    }
    else if(opcion=='2'){
        do{
            cout << "Grado del polinomio (numerador): "; cin >> *mayExp;
            cout << "Grado del polinomio (denominador): "; cin >> *mayExp2;

            if(*mayExp<0 || *mayExp2<0){
                cout << "Los exponentes deben ser numeros naturales o '0'. Ingreselos nuevamente.\n\n";
            }
        } while (*mayExp<0 || *mayExp2<0);  
    }
    else if(opcion=='3'){
        do{
            cout << "Grado del argumento: "; cin >> *mayExp;
            cout << "Grado del no argumento (si no hay ingresar '0'): "; cin >> *mayExp2;
            if(*mayExp<0 || *mayExp2<0){
                cout << "Los exponentes deben ser numeros naturales o '0'. Ingreselos nuevamente.\n\n";
            }
        }while (*mayExp<0 || *mayExp2<0);
    }
    else if(opcion=='4'){
        do{
            cout << "Grado del polinomio (potencia): "; cin >> *mayExp;
        } while (*mayExp<=0);
    }
}