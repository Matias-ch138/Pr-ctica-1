#include <iostream>
using namespace std;
#include "Especie.h"

string Especie::getcodigo_especie() const{
    return codigoEspecie_;
}
void Especie::set_codigo_especie(string codigoEspecie){
    codigoEspecie_ = codigoEspecie;
}
string Especie::getnombre_comun() const{
    return nombreComun_;
}
void Especie::setNombreComun(string nombreComun){
    nombreComun_ = nombreComun;
}
string Especie::getnombre_cientifico() const{
    return nombreCientifico_;
}
void Especie::setNombreCientifico(string nombreCientifico){
    nombreCientifico_ = nombreCientifico;
}
string Especie::gettipo_planta() const{
    return tipoPlanta_;
}
void Especie::setTipoPlanta(string tipoPlanta){
    tipoPlanta_ = tipoPlanta;
}

void Especie::mostrarInfo() const{
    cout<<"Codigo de especie: " + codigoEspecie_ + "\nNombre comun: " + nombreComun_ + "\nNombre cientifico: " + nombreCientifico_ + "\nTipo de planta: " + tipoPlanta_+"\n";
}
bool Especie::operator<(const Especie& origen) const{
    return codigoEspecie_ < origen.codigoEspecie_;
}
bool Especie::operator==(const Especie& origen) const{
    return codigoEspecie_ == origen.codigoEspecie_;
}