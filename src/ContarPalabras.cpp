#include <iostream>
#include <fstream>
#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"
#include <time.h>
// cant_max mode addr cant_carga
//./ContarPalabras 2 1 ../python/instancias/exp_unif 2
HashMapConcurrente h = HashMapConcurrente();
#define BILLION 1000000000L;
void preparar(vector<string> &v, string addr)
{
    string filePath = addr + "/indice.txt";
    std::fstream file;
    int cant = 0;
    std::string linea;

    // Abro el archivo.
    file.open(filePath, file.in);

    while (file >> linea)
    {
        // Completamos el vector de direcciones
        v.push_back(linea);
    }
    file.close();
}

int main(int argc, char **argv)
{
    int cant_threads_busqueda = atoi(argv[1]); //
    int mode = atoi(argv[2]);
    string addr = argv[3];
    timespec inicio_carga, fin_carga;
    timespec inicio_max, fin_max;
    double tiempo_carga;
    if (mode == 0)
    {
        clock_gettime(CLOCK_REALTIME, &inicio_carga);
        cargarArchivo(h, addr);
        clock_gettime(CLOCK_REALTIME, &fin_carga);
    }
    else
    {
        int cant_threads_carga = atoi(argv[4]);
        vector<string> archivos;
        preparar(archivos, addr);
        clock_gettime(CLOCK_REALTIME, &inicio_carga);
        cargarMultiplesArchivos(h, cant_threads_carga, archivos);
        clock_gettime(CLOCK_REALTIME, &fin_carga);
    }
    tiempo_carga = (double)(fin_carga.tv_sec - inicio_carga.tv_sec) + (double)(fin_carga.tv_nsec - inicio_carga.tv_nsec) / (double)BILLION;
    hashMapPair maximo;
    double tiempo_max;
    if (cant_threads_busqueda == 0)
    {
        clock_gettime(CLOCK_REALTIME, &inicio_max);
        maximo = h.maximo();
        clock_gettime(CLOCK_REALTIME, &fin_max);
    }

    else
    {
        clock_gettime(CLOCK_REALTIME, &inicio_max);
        maximo = h.maximoParalelo(cant_threads_busqueda);
        clock_gettime(CLOCK_REALTIME, &fin_max);
    }
    tiempo_max = (double)(fin_max.tv_sec - inicio_max.tv_sec) + (double)(fin_max.tv_nsec - inicio_max.tv_nsec) / (double)BILLION;

    // print carga -> bus

    std::ofstream fileResults;
    fileResults.open(addr+"/results.txt", std::ios::app);
    fileResults<<tiempo_carga<<"\n";
    fileResults.close();
    std::cout << tiempo_carga << " " << tiempo_max << " " << maximo.first << " " << maximo.second << std::endl;
    return 0;
}