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

	vector <bool> solucion_arrhytmia(arrhythmia[1].caracteristicas.size(), true);
	vector <bool> solucion_movement(movement_libras[1].caracteristicas.size(), true);
	vector <bool> solucion_wdbc(wdbc[1].caracteristicas.size(), true);
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
		generar_submuestra_wdbc(wdbc, submuestra_wdbc1, submuestra_wdbc2, seed);
		start_timers();
		solucion_wdbc_promedio= funcion_evaluacion(submuestra_wdbc1, submuestra_wdbc2, solucion_wdbc);
		//busqueda_tabu(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc1, submuestra_wdbc2);
		//SFS(solucion_wdbc_promedio, solucion_wdbc, submuestra_wdbc1, submuestra_wdbc2);
		double time = elapsed_time();

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		cout << "El porcentaje de tasa_clas " << i*2-1 << ": " << solucion_wdbc_promedio << endl;


		start_timers();
		solucion_wdbc_promedio= funcion_evaluacion(submuestra_wdbc2, submuestra_wdbc1, solucion_wdbc);
		time = elapsed_time();
		soluciones_promedio_wdbc.push_back(solucion_wdbc_promedio);
		soluciones_wdbc.push_back(solucion_wdbc);

		cout << "Tiempo " << i*2 << ": " << time << endl;

		cout << "El porcentaje de tasa_clas " << i*2 << ": " << solucion_wdbc_promedio << endl;

		submuestra_wdbc1.clear();
		submuestra_wdbc2.clear();

	}



	cout << "\n\nMovement_libras\n\n";
	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;
		generar_submuestra_movement(movement_libras, submuestra_movement1, submuestra_movement2, seed);
		start_timers();
		solucion_movement_promedio = funcion_evaluacion(submuestra_movement1, submuestra_movement2, solucion_movement);
		double time = elapsed_time();

		cout << submuestra_movement1.size() << " " << submuestra_movement2.size() << endl;

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		cout << "El porcentaje de tasa_clas " << i*2-1 << ": " << solucion_movement_promedio << endl;


		start_timers();
		solucion_movement_promedio = funcion_evaluacion(submuestra_movement2, submuestra_movement1, solucion_movement);
		time = elapsed_time();

		cout << "Tiempo " << i*2 << ": " << time << endl;
		cout << "El porcentaje de tasa_clas " << i*2 << ": " << solucion_movement_promedio << endl;

		submuestra_movement1.clear();
		submuestra_movement2.clear();

	}


	cout << "\n\nArrhythmia\n\n";
	for(int i = 1 ; i <= 5 ; i++)
	{
		seed = i;
		generar_submuestra_arrhythmia(arrhythmia, submuestra_arrhythmia1, submuestra_arrhythmia2, seed);
		start_timers();
		solucion_arrhytmia_promedio = funcion_evaluacion(submuestra_arrhythmia1, submuestra_arrhythmia2, solucion_arrhytmia);
		double time = elapsed_time();

		cout << "Tiempo " << i*2-1 << ": " << time << endl;

		cout << "El porcentaje de tasa_clas " << i*2-i << ": " << solucion_arrhytmia_promedio << endl;


		start_timers();
		solucion_arrhytmia_promedio = funcion_evaluacion(submuestra_arrhythmia2, submuestra_arrhythmia1, solucion_arrhytmia);
		time = elapsed_time();

		cout << "Tiempo " << i*2 << ": " << time << endl;
		cout << "El porcentaje de tasa_clas " << i*2 << ": " << solucion_arrhytmia_promedio << endl;

		submuestra_arrhythmia1.clear();
		submuestra_arrhythmia2.clear();

	}


}