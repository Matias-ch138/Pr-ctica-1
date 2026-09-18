#ifndef ESPECIE_H
#define ESPECIE_H

#include <iostream>
#include <string>

class Especie {
public:
    Especie() = default;

    Especie(const std::string& codigoEspecie, const std::string& nombreComun,
            const std::string& nombreCientifico, const std::string& tipoPlanta)
        : codigoEspecie_(codigoEspecie), nombreComun_(nombreComun),
          nombreCientifico_(nombreCientifico), tipoPlanta_(tipoPlanta) {}

    const std::string& getcodigoEspecie() const { return codigoEspecie_; }
    const std::string& getnombre_comun() const { return nombreComun_; }
    const std::string& getnombre_cientifico() const { return nombreCientifico_; }
    const std::string& gettipo_planta() const { return tipoPlanta_; }

    void set_codigo_especie(const std::string& codigoEspecie) { codigoEspecie_ = codigoEspecie; }
    void set_nombre_comun(const std::string& nombreComun) { nombreComun_ = nombreComun; }
    void set_nombre_cientifico(const std::string& nombreCientifico) { nombreCientifico_ = nombreCientifico; }
    void set_tipo_planta(const std::string& tipoPlanta) { tipoPlanta_ = tipoPlanta; }

    bool operator<(const Especie& otra) const { return codigoEspecie_ < otra.codigoEspecie_; }
    bool operator==(const Especie& otra) const { return codigoEspecie_ == otra.codigoEspecie_; }

    void mostrarInfo() const {
        std::cout << " - [" << codigoEspecie_ << "] " << nombreComun_ << " ("
                  << nombreCientifico_ << ") - " << tipoPlanta_ << '\n';
    }

private:
    std::string codigoEspecie_;
    std::string nombreComun_;
    std::string nombreCientifico_;
    std::string tipoPlanta_;
};

#endif // ESPECIE_H
