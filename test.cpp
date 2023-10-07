#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <fstream>
#include <string>
/*------------------------Metodos numericos (diferenciación numérica, derivadas numericas)-----------------------*/
using namespace std;
class base
{
    protected:// Para que pueda usarlo en todas las clases heredadas
        double x=0;
        short expMax, expMax2; // expMax es para indicar el grado de un polinomio. expMax2 es para lo mismo en caso de elegir funciones homograficas
        double suma1=0, suma2=0; // Suma el numerador (suma1) y denominador (suma2) para poder dividir sus resultados
        double resultadoFinal[4];
        string fDeX = "";
        string* fragmentosFdeX = nullptr;
    public:
        base(double _x, short _expMax, short _expMax2){
            x = _x;
            expMax = _expMax;
            expMax2 = _expMax2;
        }
        ~base(){};

        void guardarResultados(string mensaje){
        const string resultados = "../output/resultados.txt";
        ofstream archivo(resultados, ios::app); //ios::app es para abrir el archivo en modo de apertura, permite agregar datos en vez de sobreescribirlos

        if(!archivo.is_open()){
            perror("Error al abrir el archivo para guardar los datos");
            return;
        }

        cout << mensaje << endl;
        archivo << mensaje;
        archivo.close();
        cout << "Resultados de derivacion guardados en: " << resultados << endl;
        };

        string extraerFuncion(double ecuacion, short j){
            if(ecuacion[j] != 0){
                int valorEnInt = round(ecuacion[j]);
                if(j > 0){
                    fragmentosFdeX[j] = "(" + to_string(valorEnInt) + "x^" + to_string(j) + ")"; //ATENCION! ecuacion printea basura cuando el numero es exagerado
                    //cout << fragmentosFdeX;
                }else{
                    fragmentosFdeX[0] = "(" + to_string(valorEnInt) + ")";
                }
                if(!fDeX.empty()){
                    fDeX += "+";
                }
                fDeX += fragmentosFdeX[j];
            }
        }
};

class derivadas : public base{
    private:
        double h; // Los saltos que va a dar segun x
        double resultados[4]; // Me va a guardar los resultados de f(x) (resu_ximenos2,resu_ximenos1, resu_ximas1, etc)
    public:
        derivadas(double, short, short, double);
        ~derivadas(){}// Destructor
        void operaciones(double*, char, double*);
        void der1(double[]);
        void der2(double[]);
        void der3(double[]);
        void der4(double[]);
        void crearMensaje(double[], string);
};
derivadas::derivadas(double _x, short _expMax,short _expMax2,double _h) : base(_x,_expMax,_expMax2){// Constructor
    h = _h;
}
void derivadas::operaciones(double *ecuacion, char opcion, double *denominador){ //*ecuacion lo uso como numerador para las homograficas
    double resultado_x; // Me sirve para mostrar todos los resultados de la operacion al reemplazar x
    short contador=0;
    fDeX = "";
    fragmentosFdeX = new string [expMax+1];

    if (opcion=='1')
    {
        for (double i = x-(2*h); i <= x+(2*h); i+=h){//me toma todos los valores de x sin necesidad de crear variables aparte
            resultado_x=0; // para qsue no se acumulen los resultados tras cada ciclo
            fDeX = "";
            for (short j = expMax; j >= 0; j--){
                if(ecuacion[j] != 0){
                    int valorEnInt = round(ecuacion[j]);
                    if(j > 0){
                        fragmentosFdeX[j] = "(" + to_string(valorEnInt) + "x^" + to_string(j) + ")"; //ATENCION! ecuacion printea basura cuando el numero es exagerado
                        //cout << fragmentosFdeX;
                    }else{
                        fragmentosFdeX[0] = "(" + to_string(valorEnInt) + ")";
                    }
                    if(!fDeX.empty()){
                        fDeX += "+";
                    }
                    fDeX += fragmentosFdeX[j];
                }
                resultado_x += (ecuacion[j])*(pow(i,j));
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
    }
    else if(opcion=='2'){
        // Suma numerador
        cout << endl;
        for (double i = x-(2*h); i <= x+(2*h); i+=h){
            suma1=0;// Reseteo los valores para que no se acumulen tras cada ciclo
            suma2=0;
            for (short j = expMax; j >= 0; j--){
                if (j==0){
                    suma1 += ecuacion[j];
                    cout << "("<<ecuacion[j]<<")";
                }
                else{
                    suma1 += (ecuacion[j])*(pow(i,j));
                    cout << "f(x) = ("<<ecuacion[j]<<"*"<<i<<"^"<<j<<")+";
                }
            }
            // Suma denominador
            cout << "/";
            for (short j = expMax2; j >= 0; j--){
                if (j==0){
                    suma2+=denominador[j];
                    cout << "("<<denominador[j]<<")";
                }else{
                    suma2+=(denominador[j])*(pow(i,j));
                    cout << "("<<denominador[j]<<"*"<<i<<"^"<<j<<")+";
                }
            }
            // Primero hago la operacion y despues la guardo en cada variable dependiendo de el valor que tenga
            resultado_x = suma1/suma2;
            if (i==x-(2*h)){
                resultados[contador] = resultado_x;
                cout << " = "<<resultado_x<<"\n\n";
            }
            else if(i==x-h){
                resultados[contador] = resultado_x;
                cout << " = "<<resultado_x<<"\n\n";
            }
            else if (i==x){
                resultados[contador] = resultado_x;
                cout << " = "<<resultado_x<<"\n\n";
            }
            else if (i==x+h){
                resultados[contador] = resultado_x;
                cout << " = "<<resultado_x<<"\n\n";
            }
            else{
                resultados[contador] = resultado_x;
                cout << " = "<<resultado_x<<"\n\n";
            }
            contador++; // Me sirve para poder darle un valor al arreglo "resultados[4]"
        }
    }
    delete[] fragmentosFdeX;
    cout << fDeX << "\n";
    der1(resultadoFinal);
    der2(resultadoFinal);
    der3(resultadoFinal);
    der4(resultadoFinal);
    crearMensaje(resultadoFinal, fDeX);
}
void derivadas::der1(double resultadoFinal[]){
    resultadoFinal[0] = (-resultados[4]+8*resultados[3]-8*resultados[1]+resultados[0])/(12*h);
    cout <<"f'(x): " <<resultadoFinal[0]<<endl;
}
void derivadas::der2(double resultadoFinal[]){
    resultadoFinal[1] = ((resultados[3])-(2*resultados[2])+(resultados[1]))/(pow(h,2));
    cout <<"f''(x): " <<resultadoFinal[1]<<endl;
}
void derivadas::der3(double resultadoFinal[]){
    resultadoFinal[2] = ((resultados[4])-(2*resultados[3])+(2*resultados[1])-(resultados[0]))/(2*pow(h,3));
    cout <<"f'''(x): " <<resultadoFinal[2]<<endl;
}
void derivadas::der4(double resultadoFinal[]){
    resultadoFinal[3] = ((resultados[4])-(4*resultados[3])+(6*resultados[2])-(4*resultados[1])+(resultados[0]))/(pow(h,4));
    cout <<"f''''(x): " <<resultadoFinal[3]<<endl;
}
void derivadas::crearMensaje(double resultadoFinal[], string fDeX){
    string mensaje = "\nLas derivadas de la función: '" + fDeX + "' son:\n\n" +

                        "\tf'(x): " + to_string(resultadoFinal[0]) + "\n"
                        "\tf''(x): " + to_string(resultadoFinal[1]) + "\n"
                        "\tf'''(x): " + to_string(resultadoFinal[2]) + "\n"
                        "\tf''''(x): " + to_string(resultadoFinal[3]) + "\n"; //Si el numero es una locura como 5.539252e69 entonces el string se convierte en 0.0000000
    cout << mensaje;
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
        integral(double, short, short, double, double, int);
        ~integral(){}
        void calcular_xi(double *, char, double*);
};

integral::integral(double _x, short _expMax, short _expMax2, double _a, double _b, int _n) : base(_x,_expMax, _expMax2){ // para pasar limite a(inferior) y b(superior)
    a = _a;
    b = _b;
    n = _n;
}

void integral::calcular_xi(double *termino, char opcion, double *denominador){
    deltaX = (b-a)/n;   // calculo el valor de deltaX

    double suma_xExtremos=0;
    double suma_imagen_num, suma_imagen_den; // para ir guardando el valor que me queda al reemplazar por cada uno de los valores obtenidos en i en f(x), se reutiliza en el pasaje
    double suma_imagenMedio=0;
    short salir=0;

    for(short i=0; i<=n && salir==0; i++){ // este bucle me mueve por cada indice de i en la tabla{
        double resultado_x_i = a+(i*deltaX); // aca calculo el valor de x sub i

        if (opcion=='1'){
            if(i==0 || i==n){ // Tomo el valor de los extremos
                suma_imagen_num=0;
                for(short exp=expMax; exp>=0; exp--){
                    if(exp==0){ // termino independiente
                        suma_imagen_num += termino[exp];
                        suma_xExtremos += suma_imagen_num;
                    } else {
                        suma_imagen_num += termino[exp] * pow(resultado_x_i, exp);
                    }
                }
            }else{
                suma_imagen_num=0;
                for(short exp=expMax; exp>=0; exp--){
                    if(exp==0){ // termino independiente
                        suma_imagen_num += termino[exp];
                    } else {
                        suma_imagen_num += termino[exp] * pow(resultado_x_i, exp);
                    }
                }
                suma_imagenMedio += suma_imagen_num;
            }
        }
        else if(opcion=='2'){ // función racional
            if(i==0 || i==n){ // extremo
                suma_imagen_num=0;
                suma_imagen_den=0;

                for(short exp=expMax; exp>=0; exp--){
                    if(exp==0){
                        suma_imagen_num += termino[exp];
                    }else{
                        suma_imagen_num += termino[exp] * pow(resultado_x_i, exp);
                    }
                }
                for(short exp=expMax2; exp>=0; exp--){
                    if(exp==0){
                        suma_imagen_den += denominador[exp];
                    }else{
                        suma_imagen_den += denominador[exp] * pow(resultado_x_i, exp);
                    }
                }

                if(suma_imagen_num==0 && suma_imagen_den==0){
                    cout << "La función en f(" << resultado_x_i << ") es = 0/0 (indeterminación). Intente calcular con un valor distinto para 'n'\n";
                    salir=1;
                }else if(suma_imagen_num!=0 && suma_imagen_den==0){
                    cout << "La función en f(" << resultado_x_i << ") es = m/0 (siendo m un numero real != 0), es decir que el área va a ser = infinito\n";
                    salir=1;
                }else{
                    suma_xExtremos += (suma_imagen_num/suma_imagen_den);
                }
            }else{
                suma_imagen_num=0;
                suma_imagen_den=0;

                for(short exp=expMax; exp>=0; exp--){
                    if(exp==0){
                        suma_imagen_num += termino[exp];
                    }else{
                        suma_imagen_num += termino[exp] * pow(resultado_x_i, exp);
                    }
                }
                for(short exp=expMax2; exp>=0; exp--){
                    if(exp==0){
                        suma_imagen_den += denominador[exp];
                    }else{
                        suma_imagen_den += denominador[exp] * pow(resultado_x_i, exp);
                    }
                }

                if(suma_imagen_num==0 && suma_imagen_den==0){
                    cout << "\nLa función en f(" << resultado_x_i << ") es = 0/0 (indeterminación). Intente calcular con un valor distinto para 'n'\n";
                    salir=1;
                }else if(suma_imagen_num!=0 && suma_imagen_den==0){
                    cout << "\nLa función en f(" << resultado_x_i << ") es = m/0 (siendo m un numero real != 0), es decir que el área va a ser = infinito.\nIntente calcular con un valor distinto para 'n'\n\n";
                    salir=1;
                }else{
                    suma_imagenMedio += (suma_imagen_num/suma_imagen_den);
                }
            }
        }
    }
    // cout <<"Delta x: " <<deltaX << endl;
    // cout <<"Suma de todas las imagenes: " <<suma_imagen<<endl;
    // cout <<"Suma de las imagenes del medio: " <<suma_imagenMedio<<endl;
    // cout <<"Suma de los extremos: " <<suma_xExtremos<<endl;
    if(salir==0){
        area = (deltaX/2)*(2*suma_imagenMedio + suma_xExtremos);
        cout<<"\nEl area total es: "<<area<<"u^2"<<endl;
    }
}

short ingresoEcuacion(double *, char, double *, short, short);

int main(void){
    setlocale(LC_ALL, "en-US" );
    SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

    double x,h;
    short mayExp=0, n, mayExp2=0;
    double a,b;
    double *ecuacion=nullptr;
    double *denominador=nullptr;
    char opcion;
    base inicializar (x,mayExp, mayExp2); // Constructor general

    do{
        cout << "\t-----MENU PRINCIPAL-----\n\n";
        cout << "[0] Salir\n[1] Derivacion numerica (diferenciacion centrada)\n[2] Integracion numerica (metodo trapecio)\nOpcion: ";
        cin >> opcion; cout << endl;
        
        if (opcion=='1'){
            system("cls");
            cout << "\t-----Derivación numérica-----\n";
            
            char opcion1;
            cout << "\n\tTipo de función:\n";
            cout << "[1] Polinómica\n";
            cout << "[2] Homográfica\n";
            cout << "[3] Trigonométrica\n";
            cout << "Opción: "; cin >> opcion1;

            cout << "\nValor de x: "; cin >> x;
            do{
                cout << "Valor de h: "; cin >> h;
                if (h<=0){
                    cout << "Valor no valido. Intente con otro\n";
                }
            } while (h<=0);

            if (opcion1=='1'){
                do{
                    cout<<"\nGrado del polinomio: "; cin>>mayExp;
                    if (mayExp<=0){
                        cout << "Valor no valido"<<endl;
                        break;
                    }
                }while(mayExp<=0);

                derivadas derivar(x,mayExp,mayExp2,h);

                ecuacion = new double[mayExp+1]; // +1 para el T.I

                ingresoEcuacion(ecuacion, opcion1, nullptr, mayExp, mayExp2);  // nullptr y 0 porque ahi estarÃ­an los terminos para las funciones homograficas
                cout<<endl;
                derivar.operaciones(ecuacion,opcion1,0);// 0 porque ahi estarÃ­a el denominador (funciones racionales)
                delete[] ecuacion;
            }else if(opcion1=='2'){
                cout << "Grado del polinomio (numerador): "; cin >> mayExp;
                cout << "Grado del polinomio (denominador): "; cin >> mayExp2;

                derivadas derivar(x,mayExp,mayExp2,h);

                ecuacion = new double[mayExp+1];// Reutilizo la variable ecuacion para usarla como numerador
                denominador = new double[mayExp2+1];

                if(ingresoEcuacion(ecuacion, opcion1, denominador, mayExp, mayExp2)==1){
                    cout << "\aEl denominador no puede ser igual a 0. Ingrese los datos nuevamente.\n";
                }else{
                    derivar.operaciones(ecuacion,opcion1,denominador);
                }
        
                delete[] ecuacion; // Numerador
                delete[] denominador;
            }
            
            system("pause"); system("cls");
        }
        else if(opcion == '2'){
            system("cls");
            cout << "\t-----Integración numérica-----\n\n";
            cout << "Límite inferior (a): "; cin >> a;
            cout << "Límite superior (b): "; cin >> b;

            if (a==b){
                cout << "Area = 0. Límites iguales" << endl;
            }else{
                cout << "Cantidad de subintervalos (n. Máximo de 100): "; cin >> n;
                if (n<=0 || n>100){ // limito los subintervalos
                    cout << "La cantidad de subintervalos esta fuera del rango permitido\n\n";
                    cin.clear();
                }else{
                    char opcion2;
                    cout << "\n\tTipo de función:\n";
                    cout << "[1] Polinómica\n";
                    cout << "[2] Homográfica\n";
                    cout << "[3] Trigonométrica\n";
                    cout << "Opción: "; cin >> opcion2;
                    cout << endl;

                    if (opcion2=='1'){
                        do{
                            cout << "Grado del polinomio: "; cin >> mayExp;
                        } while (mayExp <= 0);

                        if (mayExp>=0){
                            integral funcion(x, mayExp, mayExp2, a, b, n);
                            ecuacion = new double[mayExp+1];

                            ingresoEcuacion(ecuacion, opcion2, nullptr, mayExp, mayExp2);
                            funcion.calcular_xi(ecuacion, opcion2, nullptr);
                            delete[] ecuacion;
                        }else{
                            cout << "Opcion inválida\n";
                        }
                    }
                    else if(opcion2=='2'){
                        cout << "Grado del polinomio (numerador): "; cin >> mayExp;
                        cout << "Grado del polinomio (denominador): "; cin >> mayExp2;

                        integral funcion(x, mayExp, mayExp2, a, b, n);

                        ecuacion = new double[mayExp+1];// Reutilizo la variable ecuacion para usarla como numerador
                        denominador = new double[mayExp2+1];

                        if(ingresoEcuacion(ecuacion, opcion, denominador, mayExp, mayExp2)==1){
                            cout << "\aEl denominador no puede ser igual a 0. Ingrese los datos nuevamente.\n";
                        }else{
                            funcion.calcular_xi(ecuacion, opcion2, denominador);
                        }
                        
                        delete[] ecuacion;
                        delete[] denominador;
                    }
                }        
            }
            system("pause"); system("cls");
        }
        else if(opcion == '0'){
            cout << "Finalizando programa...";
        }else{
            cout<<"Error. Opcion inválida\n";
            system("pause"); system("cls");
        }
    }while (opcion!='0');

    return 0;
}

short ingresoEcuacion(double *ecuacion, char opcion, double *denominador, short mayExp, short mayExp2){

    cout << endl;
    if (opcion=='1'){
        for (short i = mayExp; i >= 0; i--){
            if (i==0){
                cout << "T.I: "; cin >> ecuacion[i];
            }else{
                cout << "x^"<<i<<": "; cin >> ecuacion[i];
            }
        }
    } else if (opcion=='2'){
        cout << "  Numerador\n";
        for (short i = mayExp; i >= 0; i--){
            if (i==0){
                cout << "T.I: "; cin >> ecuacion[i];
            }
            else{
                cout << "x^"<<i<<": "; cin >> ecuacion[i];
            }
        }

        short bandera=0; // si no cambia a 1 es porque no se ingreso ningun valor para el denominador o se ingreso un 0
        cout << "\n  Denominador\n";
        for (short i = mayExp2; i >= 0; i--){
            if (i==0){
                cout << "T.I: "; cin >> denominador[i];
                if(bandera==0 && denominador[i]==0){ // no se ingreso un coeficiente válido y el denominador es = 0, error matemático
                    return 1;
                }
            }
            else{
                cout << "x^"<<i<<": "; cin >> denominador[i];
                if(denominador[i]!=0){
                    bandera=1; // hay al menos un término != de 0
                } // sino no hacer nada, si se ingresa 1*x^2 y dps 0*x no importa, porque la bandera ya esta en 1
            }
        }
    }
    return 0;
}