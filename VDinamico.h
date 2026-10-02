#ifndef VDINAMICO_H
#define VDINAMICO_H

#include <iostream>
#include <algorithm>
#include <climits>
#include <stdexcept>

using namespace std;

template <typename T>
class VDinamico {
private:
    unsigned int tamlog_;
    unsigned int tamfis_;
    T* datos_;

    /**
     * @brief Método auxiliar para redimensionar el vector garantizado potencia de 2
     * @param capacidadNecesaria es la capacidad nueva que va a necesitar el vector
     */
    void comprobarCapacidad(unsigned int capacidadNecesaria) {
        if (capacidadNecesaria <= tamfis_) {
            return;
        }
        while (tamfis_ < capacidadNecesaria) {
            tamfis_ *= 2;
        }
        T* newdatos = new T[tamfis_];
        for (unsigned int i = 0; i < tamlog_; ++i) {
            newdatos[i] = datos_[i];
        }
        delete[] datos_;
        datos_ = newdatos;
    }


    /**
     * @brief Comprobación de límites para los operadores de acceso y borrado
     * @param pos la posición que queremos comprobar si esta dentro de los límites del vector
     * @throw out_of_range si @param tamlog_ es igual a 0 o si @param pos es mayor o igual que @param tamlog_
     */
    void comprobarPosicion(unsigned int pos) const {
        if (tamlog_ == 0) {
            throw out_of_range("[VDinamico::comprobarPosicion]: El vector esta vacio.");
        }
        if (pos >= tamlog_) {
            throw out_of_range("[VDinamico::comprobarPosicion]: Posicion fuera de los limites del vector.");
        }
    }

public:
    // 1. Constructor por defecto
    /**
     * @brief Constructor por defecto
     */
    VDinamico() : tamlog_(0), tamfis_(1), datos_(new T[1]) {}

    /**
     * @brief constructor con tamaño y valor inicial
     * @param tamlog tamaño lógico del vector
     * @param dato el dato con el que queremos inicializar el vector
     */
    VDinamico(unsigned int tamlog, const T& dato) : tamlog_(tamlog), tamfis_(1) {
        while (tamfis_ < tamlog_) {
            tamfis_ *= 2;
        }
        datos_ = new T[tamfis_];
        for (unsigned int i = 0; i < tamlog_; ++i) {
            datos_[i] = dato;
        }
    }


    /**
     * @brief constructor de copia completa
     * @param origen el objeto con el que queremos inicializar el nuevo objeto creado
     */
    VDinamico(const VDinamico<T>& origen) 
        : tamlog_(origen.tamlog_), tamfis_(origen.tamfis_), datos_(new T[origen.tamfis_]) {
        for (unsigned int i = 0; i < origen.tamlog_; ++i) {
            datos_[i] = origen.datos_[i];
        }
    }


    /**
     * @brief constructor de copia con rango o copia parcial
     * @param origen el objeto con el que queremos inicializar el nuevo objeto desde una posición inicial 
     * @param posicionInicial la posición desde donde empezamos a copiar en el nuevo objeto
     * @param numElementos el numero de elementos que queremos que haya en el nuevo objeto
     * @trhow out_of_range el rango que se solicita excede el tamaño del vector origen
     */
    VDinamico(const VDinamico<T>& origen, unsigned int posicionInicial, unsigned int numElementos) 
        : tamlog_(numElementos), tamfis_(1) {
        if (posicionInicial + numElementos > origen.tamlog_) {
            throw out_of_range("[VDinamico::VDinamico copia parcial]: El rango solicitado excede el tamano del vector origen.");
        }
        while (tamfis_ < tamlog_) {
            tamfis_ *= 2;
        }
        datos_ = new T[tamfis_];
        for (unsigned int i = 0; i < numElementos; ++i) {
            datos_[i] = origen.datos_[posicionInicial + i];
        }
    }


    /**
     * @brief Destructor del objeto
     */
    ~VDinamico() {
        delete[] datos_;
    }


    /**
     * @brief Operador de asignación
     * @param origen el objeto el cuál queremos asignar
     * @return el objeto que asignamos
     */
    VDinamico<T>& operator=(const VDinamico<T>& origen) {
        if (this != &origen) {
            delete[] datos_;
            tamlog_ = origen.tamlog_;
            tamfis_ = origen.tamfis_;
            datos_ = new T[tamfis_];
            for (unsigned int i = 0; i < tamlog_; ++i) {
                datos_[i] = origen.datos_[i];
            }
        }
        return *this;
    }


    /**
     * @brief operador de acceso por índice escritura con verificación de límites
     * @param pos posición del vector al que queremos acceder
     * @return el dato que esta en es @param pos del vector
     */
    T& operator[](unsigned int pos) {
        comprobarPosicion(pos);
        return datos_[pos];
    }
    /**
     * @brief operador de acceso por índice lectura con verificación de límites
     * @param pos posición del vector al que queremos acceder
     * @return el dato que esta en es @param pos del vector
     */
    const T& operator[](unsigned int pos) const {
        comprobarPosicion(pos);
        return datos_[pos];
    }

    /**
     * @brief Insertar un elemento en una posición concreta
     * @param dato el dato que queremos insertar
     * @param pos la posición en la que queremos insertar el @param dato
     * @throw out_of_range la posición de insercion esta fuera de rango
     */
    void insertar(const T& dato, unsigned int pos = UINT_MAX) {
        if (pos != UINT_MAX && pos > tamlog_) {
            throw out_of_range("[VDinamico::insertar]: Posicion de insercion fuera de rango.");
        }

        if (pos == UINT_MAX) {
            pos = tamlog_;
        }

        comprobarCapacidad(tamlog_ + 1);

        for (unsigned int i = tamlog_; i > pos; --i) {
            datos_[i] = datos_[i - 1];
        }
        datos_[pos] = dato;
        tamlog_++;
    }

    /**
     * @brief Eliminar un elemento
     * @param pos posición en la que queremos eliminar el dato
     * @return devuelve el dato borrado
     * @throw out_of_range si esta vacio o la posición esta fuera de rango
     */
    T borrar(unsigned int pos = UINT_MAX) {
        if (tamlog_ == 0) {
            throw out_of_range("[VDinamico::borrar]: No se puede borrar en un vector vacio.");
        }

        if (pos == UINT_MAX) {
            pos = tamlog_ - 1;
        } else if (pos >= tamlog_) {
            throw out_of_range("[VDinamico::borrar]: Posicion a borrar fuera de rango.");
        }

        T elementoBorrado = datos_[pos];

        for (unsigned int i = pos; i < tamlog_ - 1; ++i) {
            datos_[i] = datos_[i + 1];
        }
        tamlog_--;

        // Redimensionar a la mitad si la ocupación baja de un tercio (tamlog_ * 3 < tamfis_)
        if (tamlog_ > 0 && tamlog_ * 3 < tamfis_) {
            tamfis_ /= 2;
            T* newdatos = new T[tamfis_];
            for (unsigned int i = 0; i < tamlog_; ++i) {
                newdatos[i] = datos_[i];
            }
            delete[] datos_;
            datos_ = newdatos;
        }

        return elementoBorrado;
    }
    /**
     * @brief ordena el vector mediante el sort()
     */
    void ordenar() {
        sort(datos_, datos_ + tamlog_);
    }

    /**
     * @brief Búsqueda dicotómica O(log n)
     * @param dato el dato que queremos buscar en el vector
     * @return -1 si @param tamlog_ es igual a 0 es decir esta vacio
     */
    int busquedaDicotomica(const T& dato) const {
        if (tamlog_ == 0) return -1;

        int inicio = 0;
        int fin = static_cast<int>(tamlog_) - 1;

        while (inicio <= fin) {
            int medio = inicio + (fin - inicio) / 2;
            if (datos_[medio] == dato) {
                return medio;
            } else if (datos_[medio] < dato) {
                inicio = medio + 1;
            } else {
                fin = medio - 1;
            }
        }
        return 0;
    }

    /**
     * @brief obtención del tamaño lógico del vector
     * @return @param tamlog_
     */
    unsigned int tamlog() const { return tamlog_; }
     /**
      * @brief obtención del tamaño lógico del vector
      * @return @param tamlog_
      */
    unsigned int gettLogico() const { return tamlog_; }
};

#endif