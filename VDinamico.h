#ifndef VDINAMICO_H
#define VDINAMICO_H
#include <iostream>
using namespace std;
template <typename T>
class VDinamico{
    private:
    unasigned int tamlog_;
    unasigned int tamfis_;
    T* datos_;

    void comprobarCapacidad(unsigned int capacidadNecesaria){
        capacidadNecesaria++;
        if(capacidadNecesaria<=tamfis_){
            return;
        }
        tamfis_=2*tamfis_;
        T* newdatos=new T[tamfis_];
        for(int i=0;i<tamlog_;i++){
            newdatos[i]=datos_[i];
        }
        delete[] datos_;
        datos_=newdatos;
    }

    public:
    VDinamico() : datos_(new T[1]), tamfis_(1),tamlog_(0) {}
    VDinamico(const VDinamico<T>& origen, unsigned int posicioInicial,unsigned int numElementos){
        T* nuevo=new T[1]
        int auxTam=0;
        for(int i=posicionInicial;i<numElementos;i++){
            nuevo[auxTam]->insertar(origen[i]);
            auxTam++;
        }
    }
    void insertar(const T& dato,unsigned int pos=dato.length()-1){
        comprobarCapacidad(tamlog_)
        for(unsigned int i =tamlog_; i > pos; i--){
            datos_[i] = datos_[i-1];
        }
        datos_[pos]=dato;
        tamlog_++;
    }
};



#endif 
