#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>
#include "HashMapConcurrente.hpp"

using namespace std;

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

ListaAtomica<hashMapPair>::iterator HashMapConcurrente::itSearch(string clave){
    unsigned int index = hashIndex(clave);
    ListaAtomica<hashMapPair> *tabla = (this->tabla)[index];

    ListaAtomica<hashMapPair>::iterator it = (*tabla).begin();
    while(it != (*tabla).end() && (*it).first != clave)
        it++;

    return it;

}

std::mutex ms[26];
void HashMapConcurrente::incrementar(std::string clave) {
    unsigned int index = hashIndex(clave);
    ListaAtomica<hashMapPair> *lista = (this->tabla)[index];


    ms[index].lock();//---------

    ListaAtomica<hashMapPair>::iterator it = itSearch(clave);
    if(it == (*lista).end()){
        //this->c.push_back(clave); 
        lista->insertar(make_pair(clave, 0));
        it = (*lista).begin();
    }
    (*it).second++;

    ms[index].unlock();//-------
}
// void store(T desired, std::memory_order order);
// T load(std::memory_order = std::memory_order::seq_cst);
// T fetch_add(T arg, std::memory_order order);
// bool compare_exchange_weak(T& expected, T desired,
// std::memory_order order);
// bool compare_exchange_strong(T& expected, T desired,
// std::memory_order order);
// // solo para std::atomic_flag
// bool test_and_set(std::memory_order order);
std::vector<std::string> HashMapConcurrente::claves() {
    vector<std::string> claves;
    // bool encontro;
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        ms[index].lock();
        for (auto &p : *tabla[index]) {
            // encontro = false;
            // for(auto word : claves)
            //     if(p.first == word)
            //         encontro = true;
            // if (!encontro)
                claves.push_back(p.first);
        }
        ms[index].unlock();
    }

    return claves;

    
}
///tp 3 1 ../python/instancias/exp_unif 2 
unsigned int HashMapConcurrente::valor(std::string clave) {
    ListaAtomica<hashMapPair> *lista = (this->tabla)[hashIndex(clave)];

    ListaAtomica<hashMapPair>::iterator it = itSearch(clave);

    if(it == (*lista).end())
        return 0;
    
    return(*it).second;
}


// std::mutex ms[26];
hashMapPair HashMapConcurrente::maximo() {
    hashMapPair *max = new hashMapPair();
    max->second = 0;

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        ms[index].lock();
        for (auto &p : *tabla[index]) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
        ms[index].unlock();
    }

    return *max;
}

// void store(T desired, std::memory_order order);
// T load(std::memory_order = std::memory_order::seq_cst);
// T fetch_add(T arg, std::memory_order order);
// bool compare_exchange_weak(T& expected, T desired,
// std::memory_order order);
// bool compare_exchange_strong(T& expected, T desired,
// std::memory_order order);
// // solo para std::atomic_flag
// bool test_and_set(std::memory_order order);


atomic<int> hash_index;
vector<hashMapPair> max_value;

void HashMapConcurrente::maxList(){
    int numList = hash_index.fetch_add(1); // actualizamos index
    while(numList < 26){
        // si estamos aca es que podemos buscar linealmente en la lista en index

        //buscamos

        ListaAtomica<hashMapPair> *l = this->tabla[numList];
        hashMapPair maximoLocal = make_pair("texto", 0);  
 
        for(ListaAtomica<hashMapPair>::iterator it = (*l).begin(); it != (*l).end(); ++it){
            hashMapPair actual = *it;
            if(actual.second > maximoLocal.second)
                maximoLocal = actual; 
        }

        max_value[numList] = maximoLocal;

        //actualizamos index
        numList = hash_index.fetch_add(1);
    }
    // terminate(); // terminar thread
}

hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    hash_index = 0;
    max_value = vector<hashMapPair>(26);
    vector<std::thread> ts(cant_threads);
    
    for(unsigned int i = 0; i < cant_threads; ++i){ // lanzamos threads
        ts[i] = thread(&HashMapConcurrente::maxList, this);
        
    }

    for(unsigned int i = 0; i < cant_threads ; ++i) // joineamos threads
        ts[i].join();

    return *max_element(max_value.begin(), max_value.end(), [](const hashMapPair &a, const hashMapPair &b) {return a.second < b.second;});
}

#endif


