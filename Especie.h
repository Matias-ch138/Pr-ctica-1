#ifndef ESPECIE_H
#define ESPECIE_H

#include <iostream>
using namespace std;
#include <string>



class Especie {
private:
string codigoEspecie_;
string nombreComun_;
string nombreCientifico_;
string tipoPlanta_;

public:
Especie() = default;
Especie(string codigoEspecie, string nombreComun, string nombreCientifico, string tipoPlanta){
    codigoEspecie_ = codigoEspecie;
    nombreComun_ = nombreComun;
    nombreCientifico_ = nombreCientifico;
    tipoPlanta_ = tipoPlanta;
};
//Getters and setters
string getcodigo_especie()const;
void set_codigo_especie(string codigoEspecie);
string getnombre_comun() const;
void setNombreComun(string nombreComun);
string getnombre_cientifico() const;
void setNombreCientifico(string nombreCientifico);
string gettipo_planta() const;
void setTipoPlanta(string tipoPlanta);

void mostrarInfo() const;
bool operator<(const Especie& origen) const;
bool operator==(const Especie& origen) const;
};

#endif