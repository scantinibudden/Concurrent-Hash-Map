#include "CargarArchivos.hpp"

HashMapConcurrente::HashMapConcurrente h;

void preparar(vector<string> &v, string addr){
    
}

int main(int argc, char** argv){
    int cant_threads_busqueda = atoi(argv[1]); // 
    int mode = atoi(argv[2]);
    string addr = argv[3];
    
    if(1 == 0)
        cargarArchivo(h, addr);
    else{
        int cant_threads_carga = atoi(argv[4]);
        vector<string> archivos;
        preparar(archivos, addr);
        cargarMultiplesArchivos(h, cant_threads_carga, archivos);
    }

    if(cant_threads_busqueda == 0);
        h.maximo();
    else
        h.maximoParalelo(cant_threads);

    // print carga -> bus

    return 0;
}