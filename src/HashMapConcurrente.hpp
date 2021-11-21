#ifndef HMC_HPP
#define HMC_HPP

#include <mutex>
#include <array>
#include <atomic>
#include <string>
#include <vector>
#include <algorithm>
#include "ListaAtomica.hpp"

typedef std::pair<std::string, unsigned int> hashMapPair;

class HashMapConcurrente {
 public:
    static constexpr int cantLetras = 26;

    HashMapConcurrente();

    void incrementar(std::string clave);
    std::vector<std::string> claves();
    unsigned int valor(std::string clave);

    hashMapPair maximo();
    hashMapPair maximoParalelo(unsigned int cantThreads);

 private:
    void maxList();
    ListaAtomica<hashMapPair>::iterator itSearch (std::string clave);
    ListaAtomica<hashMapPair> *tabla[HashMapConcurrente::cantLetras];
   //  std::vector<std::string> c;


    static unsigned int hashIndex(std::string clave);
};

#endif  /* HMC_HPP */
