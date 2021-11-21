#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include "CargarArchivos.hpp"

int cargarArchivo(HashMapConcurrente &hashMap, std::string filePath)
{
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open())
    {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual)
    {
        // Completar (Ejercicio 4)
        hashMap.incrementar(palabraActual);
        cant++;
        
    }
    // Cierro el archivo.
    if (!file.eof())
    {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}

struct arg_struct
{
    HashMapConcurrente *hashMap;
    std::vector<std::string> *filePaths;
    atomic<int> *file_index;
} args;

void *stmf(HashMapConcurrente* h, std::vector<string> *filePaths , atomic<int>* file_index)
{ // single thread multiple files


// void store(T desired, std::memory_order order);
// T load(std::memory_order = std::memory_order::seq_cst);
// T fetch_add(T arg, std::memory_order order);
// bool compare_exchange_weak(T& expected, T desired,
// std::memory_order order);
// bool compare_exchange_strong(T& expected, T desired,
// std::memory_order order);
// // solo para std::atomic_flag
// bool test_and_set(std::memory_order order);
    int path_index;
    path_index = file_index->fetch_add(1, std::memory_order_relaxed); // actualizamos index;
    while (path_index < (*filePaths).size())
    {
        // si estamos aca es que podemos acceder al archivo dentro de filePaths en la posicion path_index
        cargarArchivo(*h, (*filePaths)[path_index]);

        // actualizamos index
        path_index = file_index->fetch_add(1, std::memory_order_relaxed);
    }
    // terminar thread
}

void cargarMultiplesArchivos(HashMapConcurrente &hashMap, unsigned int cantThreads, std::vector<std::string> filePaths)
{
    // Completar (Ejercicio 4)
    // thread ts[cantThreads];
    vector<std::thread> ts(cantThreads);
    atomic<int> count(0);

    for (long unsigned int i = 0; i < cantThreads; ++i)
    {
        ts[i] = thread(&stmf, &hashMap, &filePaths, &count);
    }

    for (unsigned int i = 0; i < cantThreads; ++i)
    { // joineamos threads
        // std::cout << "q onda" << "q onda" << endl;
        ts[i].join();
    }
}

#endif
