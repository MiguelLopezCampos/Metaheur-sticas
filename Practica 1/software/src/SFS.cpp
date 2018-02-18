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
			}else aux2.push_back(aux[i]);
		}

		vector_aux.push_back(atof(aux2.c_str()));
		vector_aux.pop_back();
		Caracteristicas c(vector_aux, label);
		datos.push_back(c);

	}

	datos.pop_back();
	fi.close();
}


///////////////////////////////Calcular el índice del minimo
int minimo(vector <float> & v)
{
	float minimo = 999999.0;
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


//////////////////////Función para normalizar los datos

void normalizar(vector <Caracteristicas> & datos)
{
	float maximo = -999999.0;
	float minimo = 999999.0;
	//Busco primero el máximo y el mínimo
	for(int i = 0 ; i < datos[1].caracteristicas.size() ; i++)
	{
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
float euclidean_distance(vector<float> x, vector <float> y, vector<bool> & ok)
{
	float sum = 0.0;

	for(int i = 0 ; i < x.size() ; i++)
	{
		if(ok[i] == true) sum += (x[i]-y[i])*(x[i]-y[i]);
	}

	return sqrt(sum);
}


///////////////////////////////////////////Diseño el 3-NN
int k3NN(vector <Caracteristicas> & muestra, vector <float> input, vector <bool> & ok)
{
	vector <float> distancias;

	for(int i = 0 ; i < muestra.size() ; i++)
	{
		distancias.push_back(euclidean_distance(muestra[i].caracteristicas, input, ok));
	}

	int min1 = minimo(distancias);
	distancias.erase(distancias.begin()+min1);
	int min2 = minimo(distancias);
	distancias.erase(distancias.begin()+min2);
	int min3 = minimo(distancias);
	distancias.erase(distancias.begin()+min3);

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
float funcion_objetivo(vector <Caracteristicas> & training, vector <Caracteristicas> &test, vector <bool> & ok)
{
	int tam = test.size();
	int aciertos = 0;

	for(int i = 0 ; i < tam ; i++)
	{
		int etiqueta = k3NN(training, test[i].caracteristicas, ok);
		if(etiqueta == test[i].label) aciertos++;
	}

	float promedio = (float) ((aciertos*1.0)/(tam*1.0))*100.0;

	return promedio;
}



////////////////////////////////////////SFS Algorithm
void SFS(float & mejor_solucion, vector <bool> & solucion, vector <Caracteristicas> & training,
	vector <Caracteristicas> & test)
{
	mejor_solucion = 0.0;
	vector <bool> solucion_tmp = solucion;
	bool mejora=true;
	int index_mejor = -1;
	int iters = training[1].caracteristicas.size();

	while(mejora)
	{
		mejora=false;
		solucion_tmp = solucion;

		for(int i = 0 ; i < iters ; i++)
		{
			if(solucion[i] == false)
			{			
				solucion_tmp[i] = true;

				float sol = funcion_objetivo(training, test, solucion_tmp);
				if(sol > mejor_solucion)
				{
					mejor_solucion = sol;
					mejora = true;
					//index_mejor = 
					solucion = solucion_tmp;
				}

				solucion_tmp[i] = false;
			}
		}
	}
}

////////////////////////////////Generar submuestra aleatoria
void generar_submuestra(vector <Caracteristicas> & muestra, vector <Caracteristicas> & submuestra1, vector <Caracteristicas> &submuestra2, 
	int seed)
{
	Set_random(seed);
	submuestra2 = muestra;
	vector <bool> han_salido(muestra.size(), false);

	int tam = (int) muestra.size()/2;

	for(int i = 0 ; i < tam ; i++)
	{
		int r = Randint(0, tam-1);
		//cout << "hola\n";
		if(han_salido[r]) i--;
		else
		{
			han_salido[r] = true;
			submuestra1.push_back(muestra[r]);
			submuestra2.erase(submuestra2.begin()+r);
		}
	}


}
/////////////////////////////////Generador de solución aleatoria
void generar_solucion_aleatoria(vector <bool> & sol, const int size)
{
	int random = Randint(0, size-1);
	vector <bool> repetidos(size, false);

	for(int i = 0 ; i < random ; i++)
	{
		int r = Randint(0, size-1);

		if(repetidos[i]) i--;
		else
		{
			repetidos[i] = true;
			sol[i] = true;
		}
	}
}


/////////////////////////////////Búsqueda Local
/*void busqueda_local(float & mejor_solucion, vector <bool> & solucion, vector <Caracteristicas> & training, 
	vector <Caracteristicas> & test)
{
	solucion = generar_solucion_aleatoria(solucion, solucion.size());

	bool mejora = true;

	while(mejora)
	{

	}
}*/

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


	cout << "\n\nWdbc\n\n";
	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;
		generar_submuestra(wdbc, submuestra_wdbc1, submuestra_wdbc2, seed);
		start_timers();
		SFS(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc1, submuestra_wdbc2);
		double time = elapsed_time();
		soluciones_promedio_wdbc.push_back(solucion_wdbc_promedio);
		soluciones_wdbc.push_back(solucion_wdbc);
		tiempos_wdbc.push_back(time);

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		int mejoras = 0;

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


		start_timers();
		SFS(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc2, submuestra_wdbc1);
		time = elapsed_time();
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

	}

	cout << "\n\nArrhythmia\n\n";
	for(int i = 4 ; i <= 5 ; i++)
	{
		seed = i;
		generar_submuestra(arrhythmia, submuestra_arrhythmia1, submuestra_arrhythmia2, seed);
		start_timers();
		SFS(solucion_arrhytmia_promedio, solucion_arrhytmia, submuestra_arrhythmia1, submuestra_arrhythmia2);
		double time = elapsed_time();

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


		start_timers();
		SFS(solucion_arrhytmia_promedio, solucion_arrhytmia, submuestra_arrhythmia2, submuestra_arrhythmia1);
		time = elapsed_time();

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

	}

	

	cout << "\n\nmovement libras: \n" << endl;

	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;
		generar_submuestra(movement_libras, submuestra_movement1, submuestra_movement2, seed);
		start_timers();
		SFS(solucion_movement_promedio, solucion_movement, submuestra_movement1, submuestra_movement2);
		double time = elapsed_time();

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

		cout << "El porcentaje de reducción " << i*2-1 << ": " << reduccion << endl;
		cout << "El porcentaje de tasa_clas " << i*2-1 << ": " << solucion_movement_promedio << endl;


		start_timers();
		SFS(solucion_movement_promedio, solucion_movement, submuestra_movement2, submuestra_movement1);
		time = elapsed_time();

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

	}

}