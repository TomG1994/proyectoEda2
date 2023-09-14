#include<stdlib.h>
#include<iostream>
#include<cmath>
/*------------------------Metodos_numericos_para_Ingenieros_7ma_Ed-----------------------*/
using namespace std;
class derivadas{
    private:
        float x;
        float h; // Los saltos que va a dar segun x
        // Los resultados al reemplazar x+i
        float resu_xiMenos2H;
        float resu_xiMenos1H;
        float resu_x;
        float resu_xiMas1H;
        float resu_xiMas2H;
    public:
        ~derivadas(){}
        derivadas(float _x, float _h){ // Constructor para los valores ingresados por teclado
            x = _x;
            h = _h;
        }
        void setResultados(float _xiMenos2H,float _xiMenos1H,float _resu_x,float _xiMas1H,float _xiMas2H){// Constructor para los resultados finales
            resu_xiMenos2H = _xiMenos2H;
            resu_xiMenos1H = _xiMenos1H;
            resu_x = _resu_x;
            resu_xiMas1H = _xiMas1H;
            resu_xiMas2H = _xiMas2H;
        }
        // Funciones para derivar
        float der1(){
            float formula=0;
            formula = (-resu_xiMas2H+8*resu_xiMas1H-8*resu_xiMenos1H+resu_xiMenos2H)/(12*h);
    
            return formula;
        }
        float der2(){
            float formula=0;
            formula = (resu_xiMas1H-2*resu_x+resu_xiMenos1H)/(pow(h,2));

            return formula;
        }
        float der3(){
            float formula=0;
            formula = (resu_xiMas2H-2*resu_xiMas1H+2*resu_xiMenos1H-resu_xiMenos2H)/(2*pow(h,3));

            return formula;
        }
        float der4(){
            float formula=0;
            formula = (resu_xiMas2H-4*resu_xiMas1H+6*resu_x-4*resu_xiMenos1H+resu_xiMenos2H)/(pow(h,4));

            return formula;
        }
};
void resuReemplazandoX(float, float *, short *, float* ,short, short,float, char); // Segun si suma o resta, se muestra con cout
int main(){
    float x2H_Atras=0, x1H_Atras=0, x=0,x1H_Adelante=0,x2H_Adelante=0;
    float h;
    short cant; // Cantidad de numeros con x
    short cantConst; // Cantidad de constantes

    float *ecuacion=NULL;
    short *exponente=NULL;
    float *constante=NULL;
    // Resultados parciales al reemplazar la x 
    float *resultadoParcial_x_i_menos2h=NULL;
    float *resultadoParcial_x_i_menos1h=NULL;
    float *resultadoParcial_x=NULL;
    float *resultadoParcial_x_i_mas1h=NULL;
    float *resultadoParcial_x_i_mas2h=NULL;

    float resultadoFinal_x_i_menos2h=0;// Resultado final con 2 saltos atras
    float resultadoFinal_x_i_menos1h=0;// Resultado final con 1 salto atras
    float resultadoFinal_x=0; // Resultado final con el valor ingresado por teclado
    float resultadoFinal_x_i_mas1h=0;// Resultado final con 1 salto adelante
    float resultadoFinal_x_i_mas2h=0;// Resultado final con 2 saltos adelante
    float formula;
    char operador;
    short opcion;

    do
    {
        cout << "[1] Derivacion numerica (diferenciacion centrada)\n[2] Integracion numerica (metodo trapecio)\nOpcion: ";
        cin >> opcion;
        if (opcion==1)
        {
            cout << "Operacion a realizar[+][-]: ";
            cin >> operador;

            cout << "Cuanto vale el punto x: ";
            cin >> x;

            cout << "Cuanto vale h: ";
            cin >> h;

            derivadas derivar(x,h); // Tomo el valor por teclado

            x2H_Atras = x-(2*h); // Xi - 2h
            x1H_Atras = x-h;// Xi - 1h
            x1H_Adelante = x+h;// Xi + 1h
            x2H_Adelante = x+(2*h);// Xi + 2h
            
            cout << "Cantidad de terminos con x: ";
            cin >> cant;

            cout << "Cuantos terminos independientes: ";
            cin >> cantConst;
            
            ecuacion = new float[cant];
            exponente = new short[cant];
            constante = new float[cantConst];
            resultadoParcial_x_i_menos2h = new float[cant];// Ecuación 2 pasos atrás
            resultadoParcial_x_i_menos1h = new float[cant]; // Ecuación 1 paso atrás
            resultadoParcial_x = new float[cant];// Ecuación con el valor de x original
            resultadoParcial_x_i_mas1h = new float[cant];// Ecuación 1 paso adelante
            resultadoParcial_x_i_mas2h = new float[cant];// Ecuación 2 pasos adelante

            for (short i = 0; i < cant; i++)
            {
                resultadoParcial_x_i_menos2h[i] = 0;
                resultadoParcial_x_i_menos1h[i] = 0;
                resultadoParcial_x[i] = 0;
                resultadoParcial_x_i_mas1h[i] = 0;
                resultadoParcial_x_i_mas2h[i] = 0;
            }

            for (short i = 0; i < cant; i++)
            {
                cout << "Numero "<<i+1<<": ";
                cin>>ecuacion[i];

                cout << "Exponente de '"<<ecuacion[i]<<"': ";
                cin >> exponente[i];
            }
            for (short i = 0; i < cantConst; i++)
            {
                cout << "Termino independiente "<<i+1<<": ";
                cin >> constante[i];   
            }
            if (operador=='+')
            {
                for (short i = 0; i < cant; i++)
                {
                    // Hago las operaciones entre todos los términos que ingresó el usuario             0-2*(0.5) = -1
                    resultadoParcial_x_i_menos2h[i] += (ecuacion[i])*(pow(x2H_Atras,exponente[i])); // 2*-1^3+2*-1^3+2*-1^3
                    resultadoParcial_x_i_menos1h[i] +=(ecuacion[i])*(pow(x1H_Atras,exponente[i]));
                    resultadoParcial_x[i] += (ecuacion[i])*(pow(x,exponente[i]));
                    resultadoParcial_x_i_mas1h[i] += (ecuacion[i])*(pow(x1H_Adelante,exponente[i]));
                    resultadoParcial_x_i_mas2h[i] += (ecuacion[i])*(pow(x2H_Adelante,exponente[i]));
                    // Paso todos los resultados a variables normales para que se vayan sumando de a 1 término segun la cantidad ingresada
                    resultadoFinal_x_i_menos2h += resultadoParcial_x_i_menos2h[i];
                    resultadoFinal_x_i_menos1h += resultadoParcial_x_i_menos1h[i];
                    resultadoFinal_x += resultadoParcial_x[i];
                    resultadoFinal_x_i_mas1h += resultadoParcial_x_i_mas1h[i];
                    resultadoFinal_x_i_mas2h += resultadoParcial_x_i_mas2h[i];
                }

                for (short i = 0; i < cantConst; i++)
                {
                    resultadoFinal_x_i_menos2h+=constante[i];
                    resultadoFinal_x_i_menos1h+=constante[i];
                    resultadoFinal_x+=constante[i];
                    resultadoFinal_x_i_mas1h+=constante[i];
                    resultadoFinal_x_i_mas2h+=constante[i];
                }
                cout << "\n\n\n\n";
                resuReemplazandoX(x2H_Atras, ecuacion, exponente, constante ,cant, cantConst, resultadoFinal_x_i_menos2h, operador);
                resuReemplazandoX(x1H_Atras, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x_i_menos1h, operador);
                resuReemplazandoX(x, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x, operador);
                resuReemplazandoX(x1H_Adelante, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x_i_mas1h, operador);
                resuReemplazandoX(x2H_Adelante, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x_i_mas2h, operador);

                derivar.setResultados(resultadoFinal_x_i_menos2h,resultadoFinal_x_i_menos1h,resultadoFinal_x,resultadoFinal_x_i_mas1h,resultadoFinal_x_i_mas2h);
                // Seteo todos los resultados finales en el segundo constructor y los guardo en la clase derivadas para que después me los derive.
            }
            else if(operador == '-'){
                for (short i = 0; i < cant; i++)
                {
                    // Hago las operaciones entre todos los términos que ingresó el usuario
                    resultadoParcial_x_i_menos2h[i] -= (ecuacion[i])*(pow(x2H_Atras,exponente[i]));
                    resultadoParcial_x_i_menos1h[i] -=(ecuacion[i])*(pow(x1H_Atras,exponente[i]));
                    resultadoParcial_x[i] -= (ecuacion[i])*(pow(x,exponente[i]));
                    resultadoParcial_x_i_mas1h[i] -= (ecuacion[i])*(pow(x1H_Adelante,exponente[i]));
                    resultadoParcial_x_i_mas2h[i] -= (ecuacion[i])*(pow(x2H_Adelante,exponente[i]));
                    // Paso todos los resultados a variables normales para que se vayan sumando de a 1 término segun la cantidad ingresada
                    resultadoFinal_x_i_menos2h -= resultadoParcial_x_i_menos2h[i];
                    resultadoFinal_x_i_menos1h -= resultadoParcial_x_i_menos1h[i];
                    resultadoFinal_x -= resultadoParcial_x[i];
                    resultadoFinal_x_i_mas1h -= resultadoParcial_x_i_mas1h[i];
                    resultadoFinal_x_i_mas2h -= resultadoParcial_x_i_mas2h[i];
                }

                for (short i = 0; i < cantConst; i++)
                {
                    resultadoFinal_x_i_menos2h-=constante[i];
                    resultadoFinal_x_i_menos1h-=constante[i];
                    resultadoFinal_x-=constante[i];
                    resultadoFinal_x_i_mas1h-=constante[i];
                    resultadoFinal_x_i_mas2h-=constante[i];
                }
                resuReemplazandoX(x2H_Atras, ecuacion, exponente, constante ,cant, cantConst, resultadoFinal_x_i_menos2h, operador);
                resuReemplazandoX(x1H_Atras, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x_i_menos1h, operador);
                resuReemplazandoX(x, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x, operador);
                resuReemplazandoX(x1H_Adelante, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x_i_mas1h, operador);
                resuReemplazandoX(x2H_Adelante, ecuacion, exponente, constante ,cant,cantConst,resultadoFinal_x_i_mas2h, operador);

                derivar.setResultados(resultadoFinal_x_i_menos2h,resultadoFinal_x_i_menos1h,resultadoFinal_x,resultadoFinal_x_i_mas1h,resultadoFinal_x_i_mas2h);
                // Seteo todos los resultados finales en el segundo constructor y los guardo en la clase derivadas para que después me los derive.
            }
            cout << "\t\t\t\tDerivadas\n\n\n";
            formula = derivar.der1();
            cout << "Resultado f'(x): "<< formula <<"\n\n\n";

            formula = derivar.der2();
            cout << "Resultado f''(x): "<< formula <<"\n\n\n";

            formula = derivar.der3();
            cout << "Resultado f'''(x): "<< formula <<"\n\n\n";

            formula = derivar.der4();
            cout << "Resultado f''''(x): "<< formula <<"\n\n\n";

            delete[] resultadoParcial_x_i_menos2h;
            delete[] resultadoParcial_x_i_menos1h;
            delete[] resultadoParcial_x;
            delete[] resultadoParcial_x_i_mas1h;
            delete[] resultadoParcial_x_i_mas2h;
            delete[] ecuacion;
            delete[] constante;
            delete[] exponente;
            resultadoFinal_x_i_menos2h=0;
            resultadoFinal_x_i_menos1h=0;
            resultadoFinal_x=0;
            resultadoFinal_x_i_mas1h=0;
            resultadoFinal_x_i_mas2h=0;
        }else if(opcion == 2){
            cout << "Integracion numerica..."<<endl;
        }
    }while (opcion!=0);

    return 0;
}

void resuReemplazandoX(float x, float *ecuacion, short *exponente, float *constante,short cant, short cantConst, float resu, char operador){

    if (operador == '+')
    {
        cout << "Reemplazamos x ("<<x<<")"<<endl;
        for (short i = 0; i < cant; i++)
        {
            cout <<"("<<ecuacion[i]<<"*"<<x<<"^"<<exponente[i]<<")";
            
            if (i < cant-1)
            {
                cout << "+";
            }
        }
        for (short i = 0; i < cantConst; i++)
        {
            cout << "+(";
            cout << constante[i]<<")";
        }
        cout << "\n";
        cout << "Resultado: "<< resu << endl; 
        cout << "\n\n";
    }else if(operador == '-'){
        cout << "Reemplazamos x ("<<x<<")"<<endl;
        for (short i = 0; i < cant; i++)
        {
            cout <<"("<<ecuacion[i]<<"*"<<x<<"^"<<exponente[i]<<")";
            
            if (i < cant-1)
            {
                cout << "-";
            }
        }
        for (short i = 0; i < cantConst; i++)
        {
            cout << "-";
            cout << constante[i];
        }
        cout << "\n";
        cout << "Resultado: "<< resu << endl; 
        cout << "\n\n";
    }
}