#ifndef VDINAMICO_H
#define VDINAMICO_H

#include <algorithm>
#include <climits>
#include <limits>
#include <stdexcept>
#include <utility>

template <typename T>
class VDinamico {
public:
    VDinamico() : datos_(new T[1]), tamFisico_(1), tamLogico_(0) {}

    VDinamico(unsigned int tamlog, const T& dato)
        : datos_(nullptr), tamFisico_(capacidadPara(tamlog)), tamLogico_(tamlog) {
        datos_ = new T[tamFisico_];
        for (unsigned int i = 0; i < tamLogico_; ++i) datos_[i] = dato;
    }

    VDinamico(const VDinamico& origen)
        : datos_(new T[origen.tamFisico_]), tamFisico_(origen.tamFisico_), tamLogico_(origen.tamLogico_) {
        for (unsigned int i = 0; i < tamLogico_; ++i) datos_[i] = origen.datos_[i];
    }

    VDinamico(const VDinamico& origen, unsigned int posicionInicial, unsigned int numElementos)
        : datos_(nullptr), tamFisico_(0), tamLogico_(0) {
        if (posicionInicial > origen.tamLogico_ || numElementos > origen.tamLogico_ - posicionInicial)
            throw std::out_of_range("El rango de copia no pertenece al vector origen");
        tamFisico_ = capacidadPara(numElementos);
        tamLogico_ = numElementos;
        datos_ = new T[tamFisico_];
        for (unsigned int i = 0; i < tamLogico_; ++i) datos_[i] = origen.datos_[posicionInicial + i];
    }

    ~VDinamico() { delete[] datos_; }

    VDinamico& operator=(VDinamico otro) {
        intercambiar(otro);
        return *this;
    }

    T& operator[](unsigned int pos) { comprobarPosicion(pos); return datos_[pos]; }
    const T& operator[](unsigned int pos) const { comprobarPosicion(pos); return datos_[pos]; }

    void insertar(const T& dato, unsigned int pos = UINT_MAX) {
        if (pos == UINT_MAX) pos = tamLogico_;
        if (pos > tamLogico_) throw std::out_of_range("Posicion de insercion no valida");
        if (tamLogico_ == std::numeric_limits<unsigned int>::max())
            throw std::length_error("El vector dinamico es demasiado grande");
        asegurarCapacidad(tamLogico_ + 1);
        for (unsigned int i = tamLogico_; i > pos; --i) datos_[i] = datos_[i - 1];
        datos_[pos] = dato;
        ++tamLogico_;
    }

    T borrar(unsigned int pos = UINT_MAX) {
        if (tamLogico_ == 0) throw std::out_of_range("No se puede borrar de un vector vacio");
        if (pos == UINT_MAX) pos = tamLogico_ - 1;
        comprobarPosicion(pos);
        T eliminado = datos_[pos];
        for (unsigned int i = pos; i + 1 < tamLogico_; ++i) datos_[i] = datos_[i + 1];
        --tamLogico_;
        return eliminado;
    }

    void ordenar() { std::sort(datos_, datos_ + tamLogico_); }

    int busquedaDicotomica(const T& dato) const {
        unsigned int inicio = 0, fin = tamLogico_;
        while (inicio < fin) {
            const unsigned int mitad = inicio + (fin - inicio) / 2;
            if (datos_[mitad] == dato) return static_cast<int>(mitad);
            if (datos_[mitad] < dato) inicio = mitad + 1;
            else fin = mitad;
        }
        return -1;
    }

    unsigned int tamlog() const { return tamLogico_; }
    unsigned int gettLogico() const { return tamLogico_; }

private:
    T* datos_;
    unsigned int tamFisico_;
    unsigned int tamLogico_;

    static unsigned int capacidadPara(unsigned int cantidad) {
        unsigned int capacidad = 1;
        while (capacidad < cantidad) {
            if (capacidad > std::numeric_limits<unsigned int>::max() / 2)
                throw std::length_error("El vector dinamico es demasiado grande");
            capacidad *= 2;
        }
        return capacidad;
    }

    void asegurarCapacidad(unsigned int necesaria) {
        if (necesaria <= tamFisico_) return;
        const unsigned int nuevaCapacidad = capacidadPara(necesaria);
        T* nuevosDatos = new T[nuevaCapacidad];
        for (unsigned int i = 0; i < tamLogico_; ++i) nuevosDatos[i] = datos_[i];
        delete[] datos_;
        datos_ = nuevosDatos;
        tamFisico_ = nuevaCapacidad;
    }

    void comprobarPosicion(unsigned int pos) const {
        if (pos >= tamLogico_) throw std::out_of_range("Posicion fuera de los limites del vector");
    }

    void intercambiar(VDinamico& otro) noexcept {
        using std::swap;
        swap(datos_, otro.datos_);
        swap(tamFisico_, otro.tamFisico_);
        swap(tamLogico_, otro.tamLogico_);
    }
};

#endif // VDINAMICO_H
