#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
//#include "caracteristicas.h"
#include <utility>
#include <string>
#include <cstdlib>
#include "random_ppio.h"
#include "timer.h"


using namespace std;



//Estructura de los datos
class Caracteristicas
{
public:
	vector <float> caracteristicas;
	int label;

	Caracteristicas(vector <float> car, int lab){caracteristicas=car; label=lab;}
};


///Lectura de ficheros
void load_movement(vector <Caracteristicas> & datos, const char * nombre_fichero)
{
	ifstream fi(nombre_fichero);
	string aux;
	while(!fi.eof())
	{
		getline(fi, aux);

		string aux2;
		vector <float> vector_aux;

		//cout << aux << "\n\n";
		for(int i = 0 ; i < aux.size() ; i++)
		{
			if(aux[i] == ',')
			{
				vector_aux.push_back(atof(aux2.c_str()));
				aux2.clear();
			}else
			{
				aux2.push_back(aux[i]);
			}
		}

		vector_aux.push_back(atof(aux2.c_str()));
		int label = (int) vector_aux[vector_aux.size()-1];
		vector_aux.pop_back();
		Caracteristicas c(vector_aux, label);
		datos.push_back(c);
	}

	datos.pop_back();
	fi.close();
}

//Lectura de ficheros
void load_arrhytmia(vector <Caracteristicas> & datos, const char * nombre_fichero)
{
	ifstream fi(nombre_fichero);
	string aux;
	bool datos_ = false;

	while(!fi.eof())
	{
		while(datos_ == false)
		{
			getline(fi, aux);

			if(aux=="@data")
			{
				datos_ = true;
			}			
		}


		getline(fi, aux);

		string aux2;
		vector <float> vector_aux;

		//cout << aux << "\n\n";
		for(int i = 0 ; i < aux.size() ; i++)
		{
			if(aux[i] == ',')
			{
				vector_aux.push_back(atof(aux2.c_str()));
				aux2.clear();
			}else
			{
				aux2.push_back(aux[i]);
			}
		}

		vector_aux.push_back(atof(aux2.c_str()));
		int label = (int) vector_aux[vector_aux.size()-1];
		vector_aux.pop_back();
		Caracteristicas c(vector_aux, label);
		datos.push_back(c);

	}

	datos.pop_back();
	fi.close();
}


//Lectura de ficheros
void load_wdbc(vector <Caracteristicas> & datos, const char * nombre_fichero)
{
	ifstream fi(nombre_fichero);
	string aux;
	bool datos_ = false;

	while(!fi.eof())
	{
		while(datos_ == false)
		{
			getline(fi, aux);

			if(aux=="@data")
			{
				datos_ = true;
			}			
		}


		getline(fi, aux);

		string aux2;
		vector <float> vector_aux;
		char label;
		//cout << aux << "\n\n";
		for(int i = 0 ; i < aux.size() ; i++)
		{
			

			if(aux[i] == ',')
			{
				vector_aux.push_back(atof(aux2.c_str()));
				aux2.clear();
			}else if(aux[i] == 'M' || aux[i] == 'B')
			{
				label = aux[i];
				i++;
				//cambiar esta pollada
			}else aux2.push_back(aux[i]);
		}

		//cout << atof(aux2.c_str()) << endl;
		vector_aux.push_back(atof(aux2.c_str()));
		//vector_aux.pop_back();
		Caracteristicas c(vector_aux, label);
		datos.push_back(c);

	}

	datos.pop_back();
	fi.close();
}


///////////////////////////////Calcular el índice del minimo
int minimo(vector <float> & v)
{
	float minimo = 99999999999999.0;
	int ind_min = -1;

	for(int i = 0 ; i < v.size() ; i++)
	{
		if(v[i] < minimo)
		{
			minimo = v[i];
			ind_min = i;
		}
	}

	return ind_min;
}


///////////////////////////////Calcular el índice del maximo
int maximo(vector <float> & v)
{
	float maximo = -999999.0;
	int ind_max = -1;

	for(int i = 0 ; i < v.size() ; i++)
	{
		if(v[i] > maximo)
		{
			maximo = v[i];
			ind_max = i;
		}
	}

	return ind_max;
}

int maximo(vector <int> & v)
{
	float maximo = -999999;
	int ind_max = -1;

	for(int i = 0 ; i < v.size() ; i++)
	{
		if(v[i] > maximo)
		{
			maximo = v[i];
			ind_max = i;
		}
	}

	return ind_max;
}

//////////////////////Función para normalizar los datos

void normalizar(vector <Caracteristicas> & datos)
{
	//float maximo = -9999999999.0;
	//float minimo = 9999999999.0;
	//Busco primero el máximo y el mínimo
	for(int i = 0 ; i < datos[1].caracteristicas.size() ; i++)
	{
		float maximo = -9999999999.0;
		float minimo = 9999999999.0;
		for(int j = 0 ; j < datos.size() ; j++)
		{
			if(datos[j].caracteristicas[i] > maximo)
			{
				maximo = datos[j].caracteristicas[i];
			}

			if(datos[j].caracteristicas[i] < minimo)
			{
				minimo = datos[j].caracteristicas[i];
			}


		}

		for(int j = 0 ; j < datos.size() ; j++)
		{
			datos[j].caracteristicas[i] = (datos[j].caracteristicas[i]-minimo)/(maximo-minimo);

			if(maximo==0.0 && minimo==0.0)
			{
				datos[j].caracteristicas[i] = 0.0;
			}
		}
	}
}


////////////////////////////////////Distancia euclídea
//Modificar esto
float euclidean_distance(vector<float> x, vector <float> y, vector<bool> & ok)
{
	float sum = 0.0;

	for(int i = 0 ; i < x.size() ; i++)
	{
		//Cambiar este if
		sum += ok[i]*((x[i]-y[i])*(x[i]-y[i]));
	}

	return sum;
}


///////////////////////////////////////////Diseño el 3-NN
int k3NN(vector <Caracteristicas> & muestra, vector <float> input, vector <bool> & ok)
{
	vector <float> distancias;
	//NUEVO
	distancias.resize(muestra.size());

	for(int i = 0 ; i < muestra.size() ; i++)
	{
		distancias.at(i) = euclidean_distance(muestra[i].caracteristicas, input, ok);
	}



	int min1 = minimo(distancias);
	//distancias.erase(distancias.begin()+min1);
	distancias[min1] = 9999999999.9;
	int min2 = minimo(distancias);
	//distancias.erase(distancias.begin()+min2);
	distancias[min2] = 9999999999.9;
	int min3 = minimo(distancias);
	//distancias.erase(distancias.begin()+min3);
	distancias[min3] = 9999999999.9;

	int label;
	if(muestra[min2].label == muestra[min3].label)
	{
		label = muestra[min2].label;
	}else
	{
		label = muestra[min1].label;
	}

	return label;
}


///////////////////////////////////////Función objetivo
float funcion_objetivo(vector <Caracteristicas> & training, vector <bool> & ok)
{
	int tam = training.size();
	float aciertos = 0.0;

	vector <Caracteristicas> aux;// = training;

	for(int i = 0 ; i < tam ; i++)
	{
		aux = training;
		aux.erase(aux.begin()+i);
		int etiqueta = k3NN(aux, training[i].caracteristicas, ok);
		if(etiqueta == training[i].label){ aciertos+=1.0;} 
	}

	float promedio = ((aciertos)/(tam))*100.0;

	return promedio;
}


////////////////////////////////////////////////Función evaluación
float funcion_evaluacion(vector <Caracteristicas> & training, vector <Caracteristicas> &test, vector <bool> & ok)
{
	int tam = test.size();
	float aciertos = 0;

	for(int i = 0 ; i < tam ; i++)
	{
		int etiqueta = k3NN(training, test[i].caracteristicas, ok);
		if(etiqueta == test[i].label) aciertos+=1.0;
	}

	float promedio = ((aciertos)/(tam))*100.0;

	return promedio;
}

////////////////////////////////Generar submuestra aleatoria
//////////////////////////////////////Generar submuestra movement_libras
///////////////////MODIFICAR ESTO
void generar_submuestra(vector <Caracteristicas> & muestra, vector <Caracteristicas> & submuestra1, vector <Caracteristicas> &submuestra2, 
	int seed)
{
	Set_random(seed);
	submuestra2 = muestra;
	//vector <bool> han_salido(muestra.size(), false);

	int tam = (int) muestra.size()/2;

	for(int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, submuestra2.size()-1);

		submuestra1.push_back(submuestra2[r]);
		submuestra2.erase(submuestra2.begin()+r);
	
	}
}


//////////////////////////////////////////////////////////////////////////////////////
void generar_submuestra_wdbc(vector <Caracteristicas> & muestra, vector <Caracteristicas> & submuestra1, vector <Caracteristicas> &submuestra2, 
	int seed)
{
	Set_random(seed);
	submuestra2 = muestra;
	vector <Caracteristicas> M;
	vector <Caracteristicas> B;

	for(int i = 0 ; i < muestra.size() ; i++)
	{
		if(muestra[i].label == 'M')
		{
			M.push_back(muestra[i]);
		}else
		{
			B.push_back(muestra[i]);
		}
	}

	vector <int> ind_M;
	vector <int> ind_B;
	vector <Caracteristicas> M_aux = M;
	vector <Caracteristicas> B_aux = B;

	for(int i = 0 ; i < M.size() ; i++)
	{
		ind_M.push_back(i);
	}


	for(int i = 0 ; i < B.size() ; i++)
	{
		ind_B.push_back(i);
	}


	//submuestra2 = M;
	for(int i = 0 ; i < M.size()/2 ; i++)
	{
		int r = Randint(0, M_aux.size()-1);
		submuestra1.push_back(M_aux[r]);
		M_aux.erase(M_aux.begin()+r);
		//ind_M.erase(ind_M.begin()+r);
	}

	for(int i = 0 ; i < B.size()/2 ; i++)
	{
		int r = Randint(0, B_aux.size()-1);
		submuestra1.push_back(B_aux[r]);
		B_aux.erase(B_aux.begin()+r);
		//ind_B.erase(ind_B.begin()+r);
	}

	M_aux.insert(M_aux.end(), B_aux.begin(), B_aux.end());
	submuestra2 = M_aux;

	/*


	int tam = (int) muestra.size()/2;

	for(int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, submuestra2.size()-1);

		submuestra1.push_back(submuestra2[r]);
		submuestra2.erase(submuestra2.begin()+r);
	
	}*/
}

/////////////////////////////////Submuestra para movement_libras
void generar_submuestra_movement(vector <Caracteristicas> & muestra, vector <Caracteristicas> & submuestra1, vector <Caracteristicas> &submuestra2, 
	int seed)
{
	Set_random(seed);
	for(int i = 0 ; i < 15*24 ; i+=24)
	{
		/*vector <int> validos;
		validos.resize(24);*/
		/*for(int j = 0 ; j < 24 ; j++)
		{
			validos.at(j) = i+j;
		}*/
		vector <Caracteristicas> aux;
		aux.insert(aux.end(), muestra.begin()+i, muestra.begin()+i+24);
		//cout << aux.size() << endl;

		for(int j = 0 ; j < 12 ; j++)
		{
			int r = Randint(0, aux.size()-1);
			submuestra1.push_back(aux[r]);
			aux.erase(aux.begin()+r);
		}

		submuestra2.insert(submuestra2.end(), aux.begin(), aux.end());
	}
}


/////////////////////////////////Submuestra para movement_libras
void generar_submuestra_arrhythmia(vector <Caracteristicas> & muestra, vector <Caracteristicas> & submuestra1, vector <Caracteristicas> &submuestra2, 
	int seed)
{
	Set_random(seed);
	vector <Caracteristicas> C1;
	vector <Caracteristicas> C2;
	vector <Caracteristicas> C3;
	vector <Caracteristicas> C4;
	vector <Caracteristicas> C5;

	for(int i = 0 ; i < muestra.size() ; i++)
	{
		int label = muestra[i].label;
		if(label == 1)
		{
			C1.push_back(muestra[i]);
		}else if(label == 2)
		{
			C2.push_back(muestra[i]);
		}else if(label == 6)
		{
			C3.push_back(muestra[i]);
		}else if(label == 10)
		{
			C4.push_back(muestra[i]);
		}else if(label == 16)
		{
			C5.push_back(muestra[i]);
		}
	}

	//cout << C1.size() << " " << C2.size() << " " << C3.size() << " " << C4.size() << " " << C5.size() << endl;

	int tam = C1.size()/2;

	for( int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, C1.size()-1);
		submuestra1.push_back(C1[r]);
		C1.erase(C1.begin()+r);
	}

	submuestra2.insert(submuestra2.end(), C1.begin(), C1.end());

	//cout << submuestra1.size() << " " << submuestra2.size() << endl;

	tam = C2.size()/2;
	for( int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, C2.size()-1);
		submuestra1.push_back(C2[r]);
		C2.erase(C2.begin()+r);
	}

	submuestra2.insert(submuestra2.end(), C2.begin(), C2.end());

	tam = C3.size()/2;
	for( int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, C3.size()-1);
		submuestra2.push_back(C3[r]);
		C3.erase(C3.begin()+r);
	}
	submuestra1.insert(submuestra1.end(), C3.begin(), C3.end());

	tam = C4.size()/2;

	for( int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, C4.size()-1);
		submuestra1.push_back(C4[r]);
		C4.erase(C4.begin()+r);
	}
	submuestra2.insert(submuestra2.end(), C4.begin(), C4.end());

	tam = C5.size()/2;

	for( int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, C5.size()-1);
		submuestra1.push_back(C5[r]);
		C5.erase(C5.begin()+r);
	}
	submuestra2.insert(submuestra2.end(), C5.begin(), C5.end());

}

/////////////////////////////////Generador de solución aleatoria
void generar_solucion_aleatoria(vector <bool> & sol, const int size)
{
	int aleatorio = Randint(0, size-1);

	vector <int> validos;
	validos.resize(size);
	for(int i = 0 ; i < size ; i++)
	{
		validos.at(i)=i;
	}

	for(int i = 0 ; i < aleatorio ; i++)
	{
		int r = Randint(0, validos.size()-1);
		sol[validos[r]] = true;
		validos.erase(validos.begin()+r);
	}
}


/////////////////////////////////Función heurística
void heuristica(vector<Caracteristicas> & conjunto_datos, vector <float> & v_heuristico, vector <int> & labels)
{
	//vector< vector <float> > matriz;
	//matriz.resize(labels.size());
	v_heuristico.resize(conjunto_datos[1].caracteristicas.size(),0.0);
	//Contamos las etiquetas de cada clase
	vector <int> n_class(labels.size(), 0);
	n_class.resize(labels.size());


	//Calculo el número de cada una de las etiquetas
	for(int i = 0 ; i < conjunto_datos.size() ; i++)
	{
		bool found = false;
		for(int j = 0; j < labels.size() && !found ;j++)
		{
			if(conjunto_datos[i].label == labels[j])
			{
				n_class[j]++;
				found = true;
			}
		}
	}

	for(int i = 0 ; i < conjunto_datos[1].caracteristicas.size() ; i++)
	{
		//Primero calculo cuanto 'mide' el intervalo cuando discretizo
		vector <float> c;

		for(int j = 0 ; j < conjunto_datos.size() ; j++)
		{
			c.push_back(conjunto_datos[j].caracteristicas[i]);
		}

		int max = maximo(c);
		int min = minimo(c);

		float factor = c[max]-c[min];
		//Para discretizar, dividimos en 10 intervalos
		factor = factor/10.0;

		vector< vector <int> > matriz;
		matriz.resize(labels.size());



		for(int j = 0 ; j < labels.size() ; j++)
		{
			matriz.at(j).resize(10, 0);

		
			for(int k = 0 ; k < conjunto_datos.size() ; k++)
			{

				bool found = false;
				for(int h = 1 ; h <= 10 && !found; h++)
				{
					if((conjunto_datos[k].caracteristicas[i] <= c[min]+factor*h) && labels[j]==conjunto_datos[k].label)
					{
						matriz.at(j).at(h-1)++;
						found = true;
					}
				}

			}
		}

		///Aquí por fin calculo el valor heurístico (tanto pa na)
		for(int j = 0 ; j < labels.size() ;j++)
		{
			for(int k = 0 ; k < 10 ; k++)
			{
				double p_i_j=0.0;
				int total=0;
				for(int h = 0 ; h < matriz.size() ; h++)
				{
					total += matriz.at(h).at(k);
					p_i_j += (double) matriz.at(h).at(k);
				}


				//cout << total << endl;
				double p_c_f;
				if(total == 0)
				{p_c_f = 0.0;}
				else
				{ p_c_f = matriz.at(j).at(k)*1.0/total*1.0;}

				p_i_j = p_i_j/conjunto_datos.size();

				double p_c = (double) n_class[j]/conjunto_datos.size();

				if(p_c_f!=0) v_heuristico.at(i)+= p_c_f*log2(p_c_f/((p_c)*p_i_j));
				else v_heuristico.at(i) += 0;
			}
		}
	}

}

////////////////////////////////////Flip
int flip(vector <bool> & sol, vector <int> & valores_validos)
{
	//bool valido = false;
	int tam = sol.size();
	int r;
	r=Randint(0,valores_validos.size()-1);

	//El valor que cambiaremos aleatorio es el índice que nos diga valores_validos
	int index = valores_validos[r];
	if(sol[index] == true){sol[index]=false;}
	else{sol[index]=true;}

	valores_validos.erase(valores_validos.begin()+r);


	return index;
}

///////////////////////////////Algoritmo de hormigas
void SCHBL(float & mejor_solucion, vector <bool> & solucion, vector <Caracteristicas> & training, vector <float> & heuristic)
{
	//Inicializo los vectores de feromona
	//La feromona para seleccionar la característica se inicializará a 10^-6
	//La feromona para seleccionar el número de características se inicializa a 1/N
	vector <double> feromona_car(solucion.size(),0.000006);
	vector <double> feromona_n_car(solucion.size(), (double) 1/solucion.size());
	//double feromona_car_total = 0.000006*solucion.size();
	double feromona_n_car_total = 1.0;

	mejor_solucion=0;
	//Creo una matriz 10xtamañosolucion que contendrá los 'grafos' de cada hormiga


	//UN vector de candidatos (índices) para cada hormiga


	//Inicializo el mejor número de características
	//Para posteriormente hacer la actualización de su feromona
	int mejor_n_car;

	int evaluaciones = 0;

	while(evaluaciones < 15000)
	{

		//Primero inicializamos la probabilidad de coger cada característica
		//También creamos la lista de candidatos. Habrá 10 listas de candidatos
		//Una para cada hormiga
		//Primero creo una lista de candidatos que contendrá los enteros
		//que harán como punteros a la posición del array solucion
		vector <int> candidatos_aux;
		candidatos_aux.resize(solucion.size());

		for(int j = 0 ; j < solucion.size() ; j++)
		{
			candidatos_aux.at(j) = j;
		}

		vector <vector <int> > candidatos(10);

		for(int i = 0 ; i < 10 ; i++)
		{
			candidatos.at(i) = candidatos_aux;
		}

		//Inicializo cada uno de los grafos (los recorridos de las hormigas)
		vector <vector <bool> > grafos(10);
		for(int i = 0 ; i < 10 ; i++)
		{
			grafos.at(i).resize(solucion.size(), false);
		}


		//Saco el número de características que cogerá cada hormiga
		//según una ruleta
		//Llevaré un contador para cada hormiga de los pasos que ha dado
		//para asegurarme de que no coge más características de las ya elegidas a priori
		vector <int> n_car(10);
		vector <int> n_car_seleccionadas(10,0);

		for(int i = 0 ; i < 10 ; i++)
		{
			//Hacemos la ruleta
			double aleatorio = Randfloat(0,feromona_n_car_total);
			bool fin = false;

			//cout << aleatorio << endl;

			for(int j = 0 ; j < feromona_n_car.size() && !fin ; j++)
			{
				aleatorio -= feromona_n_car[j];

				//cout << aleatorio << endl;

				//cout << feromona_n_car[j] << endl;
				if(aleatorio <= 0)
				{
					n_car.at(i) = j+1; //Cojo j+1, de esta manera cogerá mínimo una
					fin=true;
				}
			}



		}


		////////////////////////////////Hasta aquí la selección del número de características
		//Ahora vamos a seleccionar las características
		//Hago un bucle que irá desde 0 hasta el máximo número de características elegido anteriormente

		int n_car_max = maximo(n_car);
		n_car_max = n_car[n_car_max];


		for(int i = 0 ;  i < n_car_max ; i++)
		{
			for(int j = 0 ; j < 10 ; j++)
			{

				if(n_car_seleccionadas[j] < n_car[j])
				{

					//Actualizo las probabilidades
					vector <double> probs(solucion.size(), 0.0);

					double suma_probabilidades = 0.0;

					for(int k = 0 ; k < candidatos[j].size() ; k++)
					{
						suma_probabilidades+=heuristic[candidatos[j][k]]*heuristic[candidatos[j][k]]*feromona_car[candidatos[j][k]];
						probs.at(candidatos[j][k]) = heuristic[candidatos[j][k]]*heuristic[candidatos[j][k]]*feromona_car[candidatos[j][k]];
					}


					//Hago la regla de transición de SCH
					//Calculo un Random uniforme y si es menor o igual que 0.8
					//me quedo la característica con más probabilidad
					//si no, hago una ruleta (como en SH)
					if(Rand() <= 0.8)
					{
						double max = -99999999999.99;
						int i_mayor_prob = -1;

						for(int k = 0 ; k < candidatos[j].size() ; k++)
						{
							if(probs[candidatos[j][k]] >= max)
							{
								max=probs[candidatos[j][k]];
								i_mayor_prob = k;
							}
						}

						grafos.at(j).at(candidatos[j][i_mayor_prob]) = true;
						feromona_car[candidatos[j][i_mayor_prob]] = 0.8*feromona_car[candidatos[j][i_mayor_prob]]+0.2*0.000006;
						candidatos.at(j).erase(candidatos.at(j).begin()+i_mayor_prob);
					}else
					{

						float aleatorio = Randfloat(0, suma_probabilidades);
						bool fin=false;
						int index;

						for(int k = 0 ; k < candidatos.at(j).size() && !fin; k++)
						{
							aleatorio -= probs[candidatos[j][k]];

							//cout << aleatorio << endl;

							if(aleatorio <= 0)
							{
								index = k;
								fin=true;
							}
						}

						grafos.at(j).at(candidatos[j][index]) = true;
						feromona_car[candidatos[j][index]] = 0.8*feromona_car[candidatos[j][index]]+0.2*0.000006;
						candidatos.at(j).erase(candidatos.at(j).begin() + index);

					}

					n_car_seleccionadas[j]++;
				}

			}
		}





		//A continuación aplico búsqueda local a cada una de las hormigas
		//y hago la actualización global de feromona
		for(int i = 0 ; i < grafos.size() ; i++)
		{
			evaluaciones++;
			float s_act_valor = funcion_objetivo(training, grafos.at(i));
			bool mejora=false;
			vector <int> repetido;
			bool completo = false;
			repetido.resize(solucion.size());
			//Inicialización de repetido
			//Repetido se inicializa con todos los índices posibles
			//del vector solución.
			for(int j = 0 ; j < solucion.size() ; j++)
			{
				repetido.at(j) = j;
			}

			while(!completo && !mejora)
			{
				vector <bool> s = grafos.at(i);

				flip(s, repetido);

				evaluaciones++;
				float s_promedio = funcion_objetivo(training, s);

				if(s_act_valor < s_promedio)
				{
					grafos.at(i) = s;
					s_act_valor=s_promedio;
					mejora = true;
				}


				if(repetido.empty()) completo=true;
			}

			//Ahora actualizamos la solución (en el caso de que sea mejor)
			if(s_act_valor >= mejor_solucion)
			{
				mejor_solucion = s_act_valor;
				solucion = grafos.at(i);
				mejor_n_car = n_car[i];
			}
		}
		
		//Ahora actualizamos la feromona
		for(int i = 0 ; i < feromona_car.size() ; i++)
		{
			feromona_car[i] = 0.8*feromona_car[i]+0.2*solucion[i]*mejor_solucion;
		}


		feromona_n_car_total = 0.0;

		mejor_n_car = 0;

		for(int i = 0 ; i < solucion.size() ; i++)
		{
			mejor_n_car+=1*solucion[i];
		}

		for(int i = 0 ; i < feromona_n_car.size() ; i++)
		{
			feromona_n_car[i] = 0.8*feromona_n_car[i]+0.2*(mejor_n_car==i+1)*mejor_solucion;
			feromona_n_car_total+=feromona_n_car[i];
		}

	}

}

/////////////////////////////////Main


int main()
{
	//Hago una lectura de los ficheros de datos
	vector <Caracteristicas> arrhythmia;
	load_arrhytmia(arrhythmia, "arrhythmia.arff");
	vector <Caracteristicas> movement_libras;
	load_movement(movement_libras, "movement_libras.data.txt");
	vector <Caracteristicas> wdbc;
	load_wdbc(wdbc, "wdbc.arff");


	//A continuación procedo a normalizar los datos
	normalizar(arrhythmia);
	normalizar(movement_libras);
	normalizar(wdbc);

	vector <bool> solucion_arrhytmia(arrhythmia[1].caracteristicas.size(), false);
	vector <bool> solucion_movement(movement_libras[1].caracteristicas.size(), false);
	vector <bool> solucion_wdbc(wdbc[1].caracteristicas.size(), false);
	float solucion_arrhytmia_promedio;
	float solucion_wdbc_promedio;
	float solucion_movement_promedio;


	vector <Caracteristicas> submuestra_arrhythmia1;
	vector <Caracteristicas> submuestra_arrhythmia2;
	vector <Caracteristicas> submuestra_movement1;
	vector <Caracteristicas> submuestra_movement2;
	vector <Caracteristicas> submuestra_wdbc1;
	vector <Caracteristicas> submuestra_wdbc2;
	int seed = 1;


	vector <float> soluciones_promedio_wdbc;
	vector <vector<bool> > soluciones_wdbc;
	vector <double> tiempos_wdbc;

	vector <int> labels_wdbc;
	labels_wdbc.push_back('B');
	labels_wdbc.push_back('M');

	vector <int> labels_movement;
	for(int i = 1 ; i <= 15 ; i++)
	{
		labels_movement.push_back(i);
	}

	vector <int> labels_arrhythmia;

	labels_arrhythmia.push_back(1);
	labels_arrhythmia.push_back(2);
	labels_arrhythmia.push_back(6);
	labels_arrhythmia.push_back(10);
	labels_arrhythmia.push_back(16);


	vector <float> heuristic;



/*	cout << "\n\nWDBC\n\n";
	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;
		generar_submuestra_wdbc(wdbc, submuestra_wdbc1, submuestra_wdbc2, seed);
		heuristica(submuestra_wdbc1, heuristic, labels_wdbc);



		start_timers();

		//AGE(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc1);
		SCHBL(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc1, heuristic);
		double time = elapsed_time();
		solucion_wdbc_promedio = funcion_evaluacion(submuestra_wdbc1, submuestra_wdbc2, solucion_wdbc);
		soluciones_promedio_wdbc.push_back(solucion_wdbc_promedio);
		soluciones_wdbc.push_back(solucion_wdbc);
		tiempos_wdbc.push_back(time);

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		int mejoras = 0;

		cout << endl;

		for(int j = 0 ; j < solucion_wdbc.size() ; j++)
		{
			if(solucion_wdbc[j] == true)
			{
				mejoras++;
			}

			solucion_wdbc[j] = false;
		}

		float reduccion= (float) (solucion_wdbc.size()*1.0-mejoras*1.0)/solucion_wdbc.size()*100.0;

		cout << "El porcentaje de reducción " << i*2-1 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2-i << ": " << solucion_wdbc_promedio << endl;
	

		heuristic.clear();
		heuristica(submuestra_wdbc2, heuristic, labels_wdbc);

		start_timers();
		SCHBL(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc2, heuristic);
		time = elapsed_time();
		solucion_wdbc_promedio = funcion_evaluacion(submuestra_wdbc2, submuestra_wdbc1, solucion_wdbc);
		soluciones_promedio_wdbc.push_back(solucion_wdbc_promedio);
		soluciones_wdbc.push_back(solucion_wdbc);
		tiempos_wdbc.push_back(time);


		cout << "Tiempo " << i*2 << ": " << time << endl;

		mejoras = 0;

		for(int j = 0 ; j < solucion_wdbc.size() ; j++)
		{
			if(solucion_wdbc[j] == true)
			{
				mejoras++;
			}

			solucion_wdbc[j] = false;
		}

		reduccion= (float) (solucion_wdbc.size()-mejoras*1.0)/solucion_wdbc.size()*100.0;

		cout << "El porcentaje de reducción " << i*2 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2 << ": " << solucion_wdbc_promedio << endl;

		soluciones_promedio_wdbc.push_back(solucion_wdbc_promedio);
		soluciones_wdbc.push_back(solucion_wdbc);
		tiempos_wdbc.push_back(time);

		submuestra_wdbc1.clear();
		submuestra_wdbc2.clear();
		heuristic.clear();

	}

	cout << "\n\nmovement libras: \n" << endl;

	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;

		submuestra_movement1.clear();
		submuestra_movement2.clear();
		generar_submuestra_movement(movement_libras, submuestra_movement1, submuestra_movement2, seed);
		heuristica(submuestra_movement1, heuristic, labels_movement);

		start_timers();
		SCHBL(solucion_movement_promedio, solucion_movement, submuestra_movement1, heuristic);
		double time = elapsed_time();
		solucion_movement_promedio = funcion_evaluacion(submuestra_movement1, submuestra_movement2, solucion_movement);

		cout << submuestra_movement1.size() << " " << submuestra_movement2.size() << endl;

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		int mejoras = 0;

		for(int j = 0 ; j < solucion_movement.size() ; j++)
		{
			if(solucion_movement[j] == true)
			{
				mejoras++;
			}

			solucion_movement[j] = false;
		}

		float reduccion= (float) (solucion_movement.size()*1.0-mejoras*1.0)/solucion_movement.size()*100.0;

		heuristic.clear();
		cout << "El porcentaje de reducción " << i*2-1 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2-1 << ": " << solucion_movement_promedio << endl;

		heuristica(submuestra_movement2, heuristic, labels_movement);
		start_timers();
		SCHBL(solucion_movement_promedio, solucion_movement, submuestra_movement2, heuristic);
		time = elapsed_time();
		solucion_movement_promedio = funcion_evaluacion(submuestra_movement2, submuestra_movement1, solucion_movement);


		cout << "Tiempo " << i*2 << ": " << time << endl;

		mejoras = 0;

		for(int j = 0 ; j < solucion_movement.size() ; j++)
		{
			if(solucion_movement[j] == true)
			{
				mejoras++;
			}

			solucion_movement[j] = false;
		}

		reduccion= (float) (solucion_movement.size()*1.0-mejoras*1.0)/solucion_movement.size()*100.0;

		cout << "El porcentaje de reducción " << i*2 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2 << ": " << solucion_movement_promedio << endl;

		submuestra_movement1.clear();
		submuestra_movement2.clear();
		heuristic.clear();
	
		//submuestra_movement2.clear();
		//submuestra_movement1.clear();
	}
*/

	cout << "\n\nArrhythmia\n\n";

	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;

		generar_submuestra_arrhythmia(arrhythmia, submuestra_arrhythmia1, submuestra_arrhythmia2, seed);
		heuristica(submuestra_arrhythmia1, heuristic, labels_arrhythmia);
		cout << "ei" << endl;


		start_timers();
		SCHBL(solucion_arrhytmia_promedio, solucion_arrhytmia, submuestra_arrhythmia1, heuristic);
		double time = elapsed_time();
		solucion_arrhytmia_promedio = funcion_evaluacion(submuestra_arrhythmia1, submuestra_arrhythmia2, solucion_arrhytmia);

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		int mejoras = 0;

		for(int j = 0 ; j < solucion_arrhytmia.size() ; j++)
		{
			if(solucion_arrhytmia[j] == true)
			{
				mejoras++;
			}

			solucion_arrhytmia[j] = false;
		}

		float reduccion= (float) (solucion_arrhytmia.size()*1.0-mejoras*1.0)/solucion_arrhytmia.size()*100.0;

		cout << "El porcentaje de reducción " << i*2-1 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2-i << ": " << solucion_arrhytmia_promedio << endl;

		heuristic.clear();

		heuristica(submuestra_arrhythmia2, heuristic, labels_arrhythmia);

		start_timers();
		SCHBL(solucion_arrhytmia_promedio, solucion_arrhytmia, submuestra_arrhythmia2, heuristic);
		time = elapsed_time();
		cout << "hola " << endl;
		solucion_arrhytmia_promedio = funcion_evaluacion(submuestra_arrhythmia2, submuestra_arrhythmia1, solucion_arrhytmia);

		cout << "Tiempo " << i*2 << ": " << time << endl;

		mejoras = 0;

		for(int j = 0 ; j < solucion_arrhytmia.size() ; j++)
		{
			if(solucion_arrhytmia[j] == true)
			{
				mejoras++;
			}

			solucion_arrhytmia[j] = false;
		}

		reduccion= (float) (solucion_arrhytmia.size()-mejoras*1.0)/solucion_arrhytmia.size()*100.0;

		cout << "El porcentaje de reducción " << i*2 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2 << ": " << solucion_arrhytmia_promedio << endl;

		submuestra_arrhythmia1.clear();
		submuestra_arrhythmia2.clear();

		heuristic.clear();

	}


}
