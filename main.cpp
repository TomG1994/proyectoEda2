#include <iostream>
#include <cmath>
#include "funciones.cpp"

using namespace std;

//Hay que sobrecargar mas funciones, EJ: ingresoEcuaciones.

int main(void){
    double x,h;
    short mayExp=0, n, mayExp2=0;
    double constante=0; // para Func. exponenciales
    float amplitud=0; // para Fun. trigonometricas
    double a,b;
    double *ecuacion=nullptr;
    double *denom_o_noArgumento=nullptr; // se usa para el denominador en funciones racionales y para el no argumento en las trigonometricas
    char opcion; // derivacion o integracion
    char opcion1; // tipo de funcion (polinomica, homografica, trigonometrica)
    char opcion2;
    double constanteSumar = 0; // Me sirve para sumarle un valor constante al final de las funciones exponenciales
    base inicializar (mayExp, mayExp2); // Constructor general

    do{
        cout << "\t-----MENU PRINCIPAL-----\n\n";
        cout << "[0] Salir\n[1] Derivacion numerica (diferenciacion centrada)\n[2] Integracion numerica (metodo trapecio)\nOpcion: ";
        cin >> opcion; cout << endl;
       
        if (opcion=='1'){
            system("cls");
            cout << "\t-----Derivacion numerica-----\n";
            cout << "\n\tTipo de funcion:\n";
            cout << "[1] Polinomica\n";
            cout << "[2] Racional\n";
            cout << "[3] Trigonometrica\n";
            cout << "[4] Exponencial\n";
            cout << "Opcion: "; cin >> opcion1;

            cout << "\nValor de x: "; cin >> x;
            do{
                cout << "Valor de h: "; cin >> h;

                if (h<=0){
                    cout << "Valor no valido. Intente con otro\n";
                    cin.clear();
                }
            } while (h<=0);

            if (opcion1=='1'){ // derivacion de funcion polinomica

                ingresoExponentes(&mayExp, &mayExp2, opcion1);
                derivadas derivar(x,mayExp,mayExp2,h);


                ecuacion = new double[mayExp+1]; // +1 para el T.I

                derivar.ingresoEcuacion(ecuacion, opcion1, nullptr, mayExp, mayExp2, &constanteSumar, nullptr, opcion2);  // nullptr porque ahi va el denominador para las funciones homograficas
                cout<<endl;
                derivar.operaciones(ecuacion,opcion1,nullptr,constante, amplitud, '0'); // '0' es la opcion 2 para trigonometricas
                delete[] ecuacion;
            }else if(opcion1=='2'){ // derivacion de funcion racional

                ingresoExponentes(&mayExp,&mayExp2, opcion1);
                derivadas derivar(x,mayExp,mayExp2,h);


                ecuacion = new double[mayExp+1]; // Reutilizo la variable ecuacion para usarla como numerador
                denom_o_noArgumento = new double[mayExp2+1];

                if(derivar.ingresoEcuacion(ecuacion, opcion1, denom_o_noArgumento, mayExp, mayExp2, &constanteSumar, nullptr, opcion2)==1){
                    cout << "\aEl denominador no puede ser igual a 0. Ingrese los datos nuevamente.\n";
                }else{
                    derivar.operaciones(ecuacion,opcion1,denom_o_noArgumento,constante, amplitud, '0');
                }

                delete[] ecuacion; // Numerador
                delete[] denom_o_noArgumento;
            }else if(opcion1=='3'){
                char opcion2;
                cout << "\n\tTipo de funcion trigonometrica:\n";
                cout << "[1] Seno\n";
                cout << "[2] Coseno\n";
                cout << "[3] Tangente\n";
                cout << "Opcion: "; cin >> opcion2;
                cout << endl;

                if(opcion2>='1' && opcion2<='3'){

                    ingresoExponentes(&mayExp,&mayExp2, opcion1); // opcion1 porque paso el tipo de funcion, no de funcion trigonometrica
                    derivadas derivar(x, mayExp, mayExp2, h);

                    cout << "Amplitud (A): "; cin >> amplitud;

                    ecuacion = new double[mayExp+1];
                    denom_o_noArgumento = new double[mayExp2+1];
                    derivar.ingresoEcuacion(ecuacion, opcion1, denom_o_noArgumento, mayExp, mayExp2, &constanteSumar, nullptr, opcion2);

                    derivar.operaciones(ecuacion, opcion1, denom_o_noArgumento, constante, amplitud, opcion2);

                    delete[] ecuacion;
                    delete[] denom_o_noArgumento;
                }else{
                    cout << "Opcion invalida.\n";
                }
            }else if(opcion1=='4'){ // derivacion de funcion exponencial
                cout << "Base: "; cin >> constante;
                if (constante==0 || constante == 1){
                    cout << "Valor no permitido una funcion exponencial. Vuelva a intentarlo.\n"<<endl;
                }else{

                    ingresoExponentes(&mayExp,&mayExp2, opcion1);
                    derivadas derivar(x,mayExp,mayExp2,h);


                    ecuacion = new double[mayExp+1];

                    derivar.ingresoEcuacion(ecuacion, opcion1, denom_o_noArgumento, mayExp, mayExp2, &constanteSumar, &constante, opcion2);

                    derivar.operaciones(ecuacion, opcion1, denom_o_noArgumento, constante, amplitud, '0');    
                }
                
                delete[] ecuacion;
            }
            else{
                cout << "Opcion invalida.\n";
            }
            
            system("pause"); system("cls");
        }
        else if(opcion == '2'){
            system("cls");
            cout << "\t-----Integracion numerica-----\n\n";
            cout << "Limite inferior (a): "; cin >> a;
            cout << "Limite superior (b): "; cin >> b;

            if (a==b){
                cout << "Area = 0. Limites iguales" << endl;
            }else{
                cout << "Cantidad de subintervalos (n. Maximo de 1000): "; cin >> n;
                if (n<=0 || n>=1001){ // Limito los subintervalos
                    cout << "La cantidad de subintervalos esta fuera del rango permitido.\n\n";
                    cin.clear(); // Limpio el buffer
                }else{
                    cout << "\n\tTipo de funcion:\n";
                    cout << "[1] Polinomica\n";
                    cout << "[2] Racional\n";
                    cout << "[3] Trigonometrica\n";
                    cout << "Opcion: "; cin >> opcion1;
                    cout << endl;

                    if (opcion1=='1'){ // integracion de funcion polinomica
                        ingresoExponentes(&mayExp,&mayExp2, opcion1);
                        integral funcion(mayExp, mayExp2, a, b, n);
                        ecuacion = new double[mayExp+1];

                        funcion.ingresoEcuacion(ecuacion, opcion1, nullptr, mayExp, mayExp2, &constanteSumar, nullptr, opcion2);
                        funcion.calcular_xi(ecuacion, opcion1, nullptr, 0, '0', constante, constanteSumar); // el primer nullptr es porque no tiene denominador, y el 0 es la amplitud para las trigonometricas
                        
                        delete[] ecuacion;
                    }
                    else if(opcion1=='2'){ // integracion de funcion racional

                        ingresoExponentes(&mayExp,&mayExp2, opcion1);
                        integral funcion(mayExp, mayExp2, a, b, n);    
                        ecuacion = new double[mayExp+1];
                        denom_o_noArgumento = new double[mayExp2+1];

                        if(funcion.ingresoEcuacion(ecuacion, opcion1, denom_o_noArgumento, mayExp, mayExp2, &constanteSumar, nullptr, opcion2)==1){
                            cout << "\aEl denominador no puede ser igual a 0. Ingrese los datos nuevamente.\n";
                        }else{
                            funcion.calcular_xi(ecuacion, opcion1, denom_o_noArgumento, 0, '0',constante, constanteSumar); // 0 para la amplitud en trigonometricas
                        }
                        
                        delete[] ecuacion;
                        delete[] denom_o_noArgumento;
                    }
                    else if(opcion1=='3'){ // integracion de funcion trigonometrica
                        cout << "\tTipo de funcion trigonometrica:\n";
                        cout << "[1] Seno\n";
                        cout << "[2] Coseno\n";
                        cout << "[3] Tangente\n";
                        cout << "Opcion: "; cin >> opcion2;
                        cout << endl;

                        if(opcion2>='1' && opcion2<='3'){
                            ingresoExponentes(&mayExp,&mayExp2, opcion1); // opcion1 porque paso el tipo de funcion, no de funcion trigonometrica
                            integral funcion(mayExp, mayExp2, a, b, n);
                        
                            float amplitud;
                            cout << "Amplitud (A): "; cin >> amplitud;

                            ecuacion = new double[mayExp+1];
                            denom_o_noArgumento = new double[mayExp2+1];
                            funcion.ingresoEcuacion(ecuacion, opcion1, denom_o_noArgumento, mayExp, mayExp2, &constanteSumar, nullptr, opcion2);

                            funcion.calcular_xi(ecuacion, opcion1, denom_o_noArgumento, amplitud, opcion2, constante, constanteSumar);
                        
                            delete[] ecuacion;
                            delete[] denom_o_noArgumento;
                        }else{
                            cout << "Opcion invalida.\n";
                        }
                    }
                    else if(opcion1=='4'){
                        cout << "Base: "; cin >> constante;
                        if (constante==0 || constante == 1){
                            cout << "Valor no permitido una funcion exponencial. Vuelva a intentarlo.\n"<<endl;
                        }else{
                            ingresoExponentes(&mayExp,&mayExp2, opcion1);
                            integral funcion(mayExp, mayExp2, a, b, n);

                            ecuacion = new double[mayExp+1];
                            funcion.ingresoEcuacion(ecuacion, opcion1, denom_o_noArgumento, mayExp, mayExp2, &constanteSumar, &constante, opcion2);

                            funcion.calcular_xi(ecuacion, opcion1, denom_o_noArgumento, amplitud, opcion1, constante, constanteSumar);
                        }
                    }
                    else{
                        cout << "Opcion invalida.\n";
                    }
                }
            }
            system("pause"); system("cls");
        }
        else if(opcion == '0'){
            cout << "Finalizando programa...";
        }else{
            cout<<"Error. Opcion invÃ¡lida\n";
            system("pause"); system("cls");
        }
    }while (opcion!='0');

    return 0;
}