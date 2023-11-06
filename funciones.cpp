#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

/*------------------------Metodos numericos (diferenciación numérica, derivadas numericas)-----------------------*/
using namespace std;

class base{
    protected:// Para que pueda usarlo en todas las clases heredadas
        short expMax, expMax2; // expMax es para indicar el grado de un polinomio. expMax2 es para lo mismo en caso de elegir funciones homograficas y funciones trigonometricas.
        double suma_imagen_num, suma_imagen_den_o_noArg; // para ir guardando el valor que me queda al reemplazar por cada uno de los valores obtenidos en i en f(x), se reutiliza en el pasaje
        double resultadoFinal[4]; // Almacena los resultados finales de las derivadas e integrales.
        string fDeX = "";
    public:
        base(short _expMax, short _expMax2){
            expMax = _expMax;
            expMax2 = _expMax2;
        }
        ~base(){};

        void guardarResultados(string mensaje){
            const string resultados = "../resultados.txt";
            ofstream archivo(resultados); //ios::app es para abrir el archivo en modo de apertura, permite agregar datos en vez de sobreescribirlos

            if(!archivo.is_open()){
                perror("Error al abrir el archivo para guardar los datos");
                return;
            }

            archivo << mensaje;
            archivo.close();
            cout << "Resultados de derivacion guardados en: " << resultados << endl;
        };

        string extraerFuncion(double* ecuacion, short mayExp){ // exporta los coeficientes para f(x).
            string fragmentoFuncion = "";
            for (short i = mayExp; i >=0; i--){
                if (ecuacion[i] != 0){
                    string valorEnStr = strDeDouble(&ecuacion[i]);
                    if (i > 0){
                        fragmentoFuncion += "(" + valorEnStr + "x^" + to_string(i) + ")";
                        if (ecuacion[i-1]!=0){
                            fragmentoFuncion += "+";
                        }
                    }else {
                        fragmentoFuncion += "(" + valorEnStr + ")";
                    }
                }
            }
            return fragmentoFuncion; // devolvemos el fragmento de la funcion
        };

        string strDeDouble(double *varParaConvertir) {       // redondeo los valores con ptrdouble
            string dStr = to_string(*varParaConvertir); 
            size_t posComa = dStr.find('.');
            if (posComa != string::npos) {
                while (dStr.back() == '0' && dStr.front() != 0) {  // si no hay suficientes dígitos después del punto decimal eliminar la parte decimal
                    dStr.pop_back();
                }
                if (dStr.back() == '.') {
                    dStr.pop_back(); // elimina el punto decimal si es el último carácter
                }
            }
            return dStr;
        }
        
        short ingresoEcuacion(double *ecuacion, char opcion, double *denom_o_noArgumento, short mayExp, short mayExp2, double *constanteSuma, double *constante, char opcion2, float amplitud, double *constanteMultiplicar){
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
            }
            // Extraego lo que se ingreso como funcion para guardarlo en un string
            fDeX = extraerFuncion(ecuacion, mayExp);

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
                }
                fDeX_denominador = extraerFuncion(denom_o_noArgumento, mayExp2);
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
                }
                fDeX_noArgumento = extraerFuncion(denom_o_noArgumento, mayExp2);
                double amplDouble = static_cast<double>(amplitud);
                string strAmpl = strDeDouble(&amplDouble); // Guarda la amplitud ingresada para la funcion trigonometrica
                switch (opcion2) {
                    case '1':
                        if(mayExp2 != 0 || fDeX_noArgumento != "")
                            fDeX = strAmpl + "Sin(" + fDeX + ")" + " + " + fDeX_noArgumento ;
                        else
                            fDeX = strAmpl + "Sin(" + fDeX + ")";
                        break;
                    case '2':
                        if(mayExp2 != 0 || fDeX_noArgumento != "")
                            fDeX = strAmpl + "Cos(" + fDeX + ")" + " + " + fDeX_noArgumento ;
                        else
                            fDeX = strAmpl + "Cos(" + fDeX + ")";
                        break;
                    case '3':
                        if(mayExp2 != 0 || fDeX_noArgumento != "")
                            fDeX = strAmpl + "Tan(" + fDeX + ")" + " + (" + fDeX_noArgumento + ")";
                        else
                            fDeX = strAmpl + "Tan(" + fDeX + ")";
                        break;
                }   
            }
            // Me sirve para tomar la constante y guardarla en un string e imprimirla despues
            else if (opcion=='4'){
                do{
                    cout << "Constante para multiplicar la funcion exponencial (si no desea multiplicarla, coloque un '1'): ";
                    cin >> *constanteMultiplicar;
                } while (*constanteMultiplicar < 0);
                
                cout << "Constante a sumar: ";
                cin >> *constanteSuma;
                if(*constanteSuma != 0){
                    fDeX = strDeDouble(constante) + "^(" + fDeX + ")" + " + " + "(" + strDeDouble(constanteSuma) + ")"; // Redondeo
                }
                if(*constanteMultiplicar>0){
                    fDeX = strDeDouble(constanteMultiplicar) + "*" + "(" + fDeX + ")";
                }
            }
            return 0;
        };
};

class derivadas : public base{
    private:
        double x=0;
        double h; // Los saltos que va a dar segun x
        double resultados[5]; // Me va a guardar los resultados de f(x) (resu_ximenos2h,resu_ximenos1h, resu_ximas1h, etc)
    public:
        derivadas(double, short, short, double);
        ~derivadas(){}// Destructor
        void operaciones(double*, char, double*, double, float, char, double);
        void der1(double[], double);
        void der2(double[], double);
        void der3(double[], double);
        void der4(double[], double);
        string crearMensaje(double[], string);
};
derivadas::derivadas(double _x, short _expMax,short _expMax2,double _h) : base(_expMax,_expMax2){// Constructor
    x = _x;
    h = _h;
}

string derivadas::crearMensaje(double resultadoFinal[], string fDeX){
    string mensaje = "\nLas derivadas de f(x)" + fDeX + " son:\n\n" +
    /*Se imprimen los resultados de las derivadas. "to_string" convierte el resultadoFinal[] en string*/
                        "\tf'(x): " + strDeDouble(&resultadoFinal[0]) + "\n"
                        "\tf''(x): " + strDeDouble(&resultadoFinal[1]) + "\n"
                        "\tf'''(x): " + strDeDouble(&resultadoFinal[2]) + "\n"
                        "\tf''''(x): " + strDeDouble(&resultadoFinal[3]) + "\n";
    cout << mensaje;
    return mensaje;
}

void derivadas::operaciones(double *ecuacion, char opcion, double *denom_o_noArgumento, double constante, float amplitud, char opcion2, double constanteMultiplicar){ //*ecuacion lo uso como numerador para las homograficas
    double resultado_x; // Me sirve para mostrar todos los resultados de la operacion al reemplazar x
    short contador=0;
    double tolerance = 1e-6;  // Una pequeña tolerancia para valores cercanos a x+(2*h)

    for (double i = x-(2*h); i <= x+(2*h) + tolerance; i += h){
        if (opcion=='1'){
            resultado_x=0; // para que no se acumulen los resultados tras cada ciclo
            for (short j = expMax; j >= 0; j--){
                resultado_x += (ecuacion[j])*(pow(i,j));
                // cout <<ecuacion[j]<<" * " <<i<<"^"<<j<<" = "<<resultado_x<<endl;
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
                    break;
                case '2':   resultado_x = amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                    break;
                case '3':   resultado_x = amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
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
        }
        else if(opcion=='4'){
            resultado_x=0; // para que no se acumulen los resultados tras cada ciclo
            for (short j = expMax; j >= 0; j--){
                resultado_x += (ecuacion[j])*(pow(i,j));
                // cout <<ecuacion[j]<<" * " <<i<<"^"<<j<<" = "<<resultado_x<<endl;
            }
            // Una vez corregido, poner los if adentro del for
            if (i==x-(2*h)){
                resultados[contador] = pow(constante, resultado_x);
                // cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                // cout << "\n\nX = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else if(i==x-h){
                resultados[contador] = pow(constante, resultado_x);
                // cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                // cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else if (i==x){
                resultados[contador] = pow(constante, resultado_x);
                // cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                // cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else if (i==x+h){
                resultados[contador] = pow(constante, resultado_x);
                // cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                // cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
            else{
                resultados[contador] = pow(constante, resultado_x);
                // cout <<constante<<"^"<<resultado_x<<" = "<<resultados[contador]<<endl;
                // cout << "X = "<<i<<". Resultados["<<contador<<"] = "<<resultados[contador]<<endl<<endl;
            }
        }
        contador++;
    }

    der1(resultadoFinal, constanteMultiplicar);
    der2(resultadoFinal, constanteMultiplicar);
    der3(resultadoFinal, constanteMultiplicar);
    der4(resultadoFinal, constanteMultiplicar);
    guardarResultados(crearMensaje(resultadoFinal, fDeX));
}
void derivadas::der1(double resultadoFinal[], double constanteMultiplicar){
    resultadoFinal[0] = constanteMultiplicar*(-resultados[4]+8*resultados[3]-8*resultados[1]+resultados[0])/(12*h);
}
void derivadas::der2(double resultadoFinal[], double constanteMultiplicar){
    resultadoFinal[1] = constanteMultiplicar*((resultados[3])-(2*resultados[2])+(resultados[1]))/(pow(h,2));
}
void derivadas::der3(double resultadoFinal[], double constanteMultiplicar){
    resultadoFinal[2] = constanteMultiplicar*((resultados[4])-(2*resultados[3])+(2*resultados[1])-(resultados[0]))/(2*pow(h,3));
}
void derivadas::der4(double resultadoFinal[], double constanteMultiplicar){
    resultadoFinal[3] = constanteMultiplicar*((resultados[4])-(4*resultados[3])+(6*resultados[2])-(4*resultados[1])+(resultados[0]))/(pow(h,4));
}
/*-----------------------Metodo del trapecio (integracion numerica)-----------------------*/
class integral : public base{
    private:
        double a; // limite inferior
        double b; // limite superior
        int n; // subintervalos
        double deltaX; // es el ancho de cada trapecio
        double area=0;
    public:
        integral(short, short, double, double, int);
        ~integral(){}
        void calcular_xi(double *, char, double*, float, char, double, double, short);
        string crearMensaje(double[], double[], string, double, double, short);
};
integral::integral(short _expMax, short _expMax2, double _a, double _b, int _n) : base(_expMax, _expMax2){ // Heredo 2 variables de la clase base
    a = _a;
    b = _b;
    n = _n;
}

string integral::crearMensaje(double resultadoFinal[], double xExtremos[], string fDeX, double a, double b, short salir){
    string mensaje = "\nLos resultados de la intregracion del intervalo entre: "+ strDeDouble(&a) + " y " + strDeDouble(&b) + " de f(x) " + fDeX + " son:\n\n" +
                            "\t~Delta X: " + strDeDouble(&resultadoFinal[0]) + "\n"
                            "\t~Suma de los subintervalos de los extremos: " + strDeDouble(&resultadoFinal[1]) + "\n"
                            "\t\tExtremo inferior: " + strDeDouble(&xExtremos[0]) + "\n"
                            "\t\tExtremo superior: " + strDeDouble(&xExtremos[1]) + "\n\n"
                            "\t~Suma de los subintervalos del medio: " + strDeDouble(&resultadoFinal[2]) + "\n";
    switch(salir){
        case 0:
                 mensaje += "\t~Area total es: " + strDeDouble(&resultadoFinal[3]) + "u^2" + "\n";
            break;
        case 1:
                 mensaje += "\t~Area total es: Indeterminado\n";
            break;
        case 2:
                 mensaje += "\t~Area total es: Infinito\n";
            break;
    }
    cout << mensaje;
    return mensaje;
};

void integral::calcular_xi(double *ecuacion, char opcion, double *denom_o_noArgumento, float amplitud, char opcion2, double constante, double constanteSumar, short constanteMultiplicar){
    deltaX = (b-a)/n;   // calculo el valor de deltaX
    float difLimites=b-a; // La diferencia entre los limites. Me sirve para calcular el area cuando la funcion exponencial tiene una constante al final. 2^(x) = 8 2^(x)+2 = 8 2 * difLimites
    double xExtremos[2]; // 0 = a xExtremo del limite inferior 'a' y 1 = a xExtremo del limite superior 'b' 
    double suma_imagenMedio=0;
    short salir=0; // Bandera

    for(short i=0; i<=n && salir==0; i++){ // este bucle me mueve por cada indice de i en la tabla
        double resultado_x_i = a+(i*deltaX); // aca calculo el valor de x sub i

        suma_imagen_num=0;
        suma_imagen_den_o_noArg=0;

        // Hago que todas las funciones (excepto las racionales y trigonometricas, con el denominador y el "no argumento") pasen por el mismo for, para optimizar el codigo.

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
                if(i==0){ // Extracción de extremos
                    xExtremos[0] = suma_imagen_num;
                    // cout << "\n" << " I == 0: " << xExtremos[0] << endl; //Debuggin
                }else if(i==n){
                    xExtremos[1] = suma_imagen_num;
                    // cout << "\n" << " I == 0: " << xExtremos[1] << endl; //Debuggin
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
                salir=2;
            }else{
                if(i==0){ // Extracción de extremos
                    xExtremos[0] = (suma_imagen_num/suma_imagen_den_o_noArg);
                }else if(i==n){
                    xExtremos[1] = (suma_imagen_num/suma_imagen_den_o_noArg);
                }else{
                    suma_imagenMedio += (suma_imagen_num/suma_imagen_den_o_noArg);
                }
            }
        }else if(opcion=='3'){ // funcion trigonometrica
            switch(opcion2){
                case '1': 
                            if (i == 0){ // Extracción de xExtremos
                                xExtremos[0] = amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg); 
                                // cout << xExtremos[0] << endl;//debug
                            } else if(i==n){
                                xExtremos[1] = amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                // cout << xExtremos[1] << endl;//debug
                            }else{
                                suma_imagenMedio += amplitud*sin(suma_imagen_num)+(suma_imagen_den_o_noArg);
                            }
                    break;
                case '2':   
                            if (i == 0){ // Extracción de xExtremos
                                xExtremos[0] = amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                // cout << xExtremos[0] << endl;//debug
                            } else if(i==n){
                                xExtremos[1] = amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                // cout << xExtremos[1] << endl;//debug
                            }else{
                                suma_imagenMedio += amplitud*cos(suma_imagen_num)+(suma_imagen_den_o_noArg);
                            }

                    break;
                case '3':
                            if (i == 0){ // Extracción de xExtremos
                                xExtremos[0] = amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg); 
                                // cout << xExtremos[0] << endl; //debug
                            } else if(i==n){
                                xExtremos[1] = amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
                                // cout << xExtremos[1] << endl;//debug
                            }else{
                                suma_imagenMedio += amplitud*tan(suma_imagen_num)+(suma_imagen_den_o_noArg);
                            }
                    break;
            }
        }else if(opcion=='4'){
            double resultadoFinalExponenciales = 0; // me sirve para elevar el resultado de la ecuacion a la constante
            suma_imagen_num=0;
            for(short exp=expMax; exp>=0; exp--){
                suma_imagen_num += ecuacion[exp] * pow(resultado_x_i,exp);
            }
            resultadoFinalExponenciales += pow(constante,suma_imagen_num);

            if (i == 0){
                xExtremos[0] = resultadoFinalExponenciales; 
            } else if(i==n){
                xExtremos[1] = resultadoFinalExponenciales;
            }else{
                suma_imagenMedio += resultadoFinalExponenciales;
            }
        }
    }

    // Esto es para crear el mensaje para el archivo
    resultadoFinal[0] = deltaX;
    resultadoFinal[1] = xExtremos[0]+xExtremos[1];
    resultadoFinal[2] = suma_imagenMedio;
    
    if(salir==0){
        area = (deltaX/2)*(2*suma_imagenMedio + (xExtremos[0]+xExtremos[1])) + (difLimites*constanteSumar);
        
        if (area>=0){
            resultadoFinal[3] = constanteMultiplicar * area;
        }else{  
            resultadoFinal[3] = constanteMultiplicar * abs(area);
        }

    }
    guardarResultados(crearMensaje(resultadoFinal, xExtremos, fDeX, a, b, salir));
}
void ingresoExponentes(short *mayExp, short *mayExp2, char opcion){
    if (opcion=='1'){
        do{
            cout << "Grado del polinomio: "; cin >> *mayExp;
        } while (*mayExp<0);   
    }
    else if(opcion=='2'){
        do{
            cout << "Grado de la funcion (numerador): "; cin >> *mayExp;
            cout << "Grado de la funcion (denominador): "; cin >> *mayExp2;

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