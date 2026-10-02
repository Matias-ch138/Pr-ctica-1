/**
 * @author Matías Camacho Hoyo mch00039@red.ujaen.es
 * @author Jose Rivas Ceacero jrc00091@red.ujaen.es
*/

#include <fstream>
#include <iostream>
#include <sstream>

#include "VDinamico.h"
#include "Especie.h"
#include "LectorCSV.h"
#include <string>
#include <iostream>
#include <string>
using namespace std;

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

string obtenerDirectorioActual() {
    char buffer[1024];
    if (GetCurrentDir(buffer, sizeof(buffer)) != nullptr) {
        return string(buffer);
    }
    return "";
}

/**
 * @brief Función para encontrar las especies con nombre común no nulo
 * @param vEspecies VDinámico en el que hacer la búsqueda
 * @return VDinámico de punteros a especies en el VDinámico completo
 *          que no tienen nombre común nulo
 */
VDinamico<Especie*> getEspNComun(VDinamico<Especie>& vEspecies)
{
    VDinamico<Especie*> vectorEspNComun;
    for (int i = 0; i < vEspecies.gettLogico(); i++)
    {
        if (!vEspecies[i].getnombre_comun().empty())
        {
            vectorEspNComun.insertar(&vEspecies[i]);
        }
    }
    return vectorEspNComun;
}
/**
 * @brief bubble_sort para ordenar el vector
 * @param vectorCompleto el vector de datos que queremos ordenar
 */
void bubble_sort(VDinamico<Especie> &vectorCompleto)
{
    int n = vectorCompleto.gettLogico();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (vectorCompleto[j].getcodigo_especie() > vectorCompleto[j + 1].getcodigo_especie())
            {
                // Intercambiar elementos
                Especie temp = vectorCompleto[j];
                vectorCompleto[j] = vectorCompleto[j + 1];
                vectorCompleto[j + 1] = temp;
            }
        }
    }
}
/**
 * @brief buscar por la primera palabra del nombre cientifico de la especie
 * @param vEspecies el vector de especie
 * @param palabra la palabra que queremos buscar
 * @return el vector con los datos filtrados
 */
VDinamico<Especie *> buscarPorPrimeraPalabraCientifica(VDinamico<Especie> &vEspecies, const string &palabra)
{
    // Vector que guarda punteros a Especie
    VDinamico<Especie *> resultado;

    for (unsigned int i = 0; i < vEspecies.gettLogico(); ++i){
        string nomCientifico = vEspecies[i].getnombre_cientifico();

        string primeraPalabra;
        int posEspacio = nomCientifico.find(' ');

        if (posEspacio == -1){
            // Si vale -1, es que no encontró ningún espacio
            primeraPalabra = nomCientifico;
        }
        else{
            // Si encontró espacio, recortamos hasta esa posición
            primeraPalabra = nomCientifico.substr(0, posEspacio);
        }

        if (primeraPalabra == palabra){
            // Guardamos la dirección de memoria usando '&'
            resultado.insertar(&vEspecies[i]);
        }
    }
    return resultado;
}
//CONSTANTES
const string RUTA_FICHERO_ESPECIES = "data/arbolado-especies.csv";

int main()
{
    try
    {
        string ruta = obtenerDirectorioActual();
        if (!ruta.empty()) {
            cout << "Directorio actual: " << ruta << endl;
        } else {
            cerr << "Error al obtener el directorio." << endl;
        }


        //INSTANCIAR Y MOSTRAR EL VECTOR DE Especies
        LectorCSV lector_csv;
        VDinamico<Especie> vectorCompleto;
        if (!lector_csv.cargar(vectorCompleto, RUTA_FICHERO_ESPECIES))
        {
            cerr << "Error: no se pudo abrir el fichero '" << RUTA_FICHERO_ESPECIES << "'.\n";
            cerr << "Comprueba que ejecutas el programa desde la raiz del proyecto "
                         "(o revisa el Working Directory en la configuracion de ejecucion).\n";
            return 1;
        }
        cout << "Vector leido de fichero"<<endl;;
        
        //ORDENACIÓN DEL VECTOR POR CÓDIGO DE ESPECIE MEDIANTE BUBBLE SORT
        bubble_sort(vectorCompleto);
        // PRUEBA DE BÚSQUEDA POR PRIMERA PALABRA
        cout << "===================================================" << endl;
        string palabraBuscada = "Jasminum";
        
        // Llamada a la función
        VDinamico<Especie*> especiesFiltradas = buscarPorPrimeraPalabraCientifica(vectorCompleto, palabraBuscada);

        // A. Imprimimos el total devuelto (debe salir 6)
        cout << "Numero de especies cuyo nombre cientifico empieza por '" 
             << palabraBuscada << "': " << especiesFiltradas.gettLogico() << endl;

        // B. Recorremos el vector de punteros para mostrarlas por pantalla
        cout << "\nListado de especies encontradas:" << endl;
        for (unsigned int i = 0; i < especiesFiltradas.gettLogico(); ++i) {
            especiesFiltradas[i]->mostrarInfo(); 
        }

        
        //Mostrar los 50 ultimos identificadores
        cout<<"==================================================="<<endl;
        cout << "Identificador de las ultimas 50 especies" << endl;
        for (int i = vectorCompleto.gettLogico()-51; i < vectorCompleto.gettLogico() ; ++i)
        {
            cout<<to_string(i)<<": "<<vectorCompleto[i].getcodigo_especie()<<endl;
        }
         //Mostrar los 50 primeros identificadores
        cout<<"==================================================="<<endl;
        cout << "Identificador de las primeras 50 especies" << endl;
        for (int i = 0; i < 50; ++i)
        {
            cout<<to_string(i)<<": "<<vectorCompleto[i].getcodigo_especie()<<endl;
        } 

        //Ordenación del vector
        cout<<"==================================================="<<endl;
        vectorCompleto.ordenar();
        cout << "Vector ordenado. Mostrar las primeras 50 especies" << endl;
        for (int i = 0; i < 50; i++)
        {
            cout<<to_string(i);
            vectorCompleto[i].mostrarInfo();
        }

        //Busqueda de posición de codigos de especies
        cout<<"==================================================="<<endl;
        Especie esp;
        cout << "Posición de códigos de especies: " << endl;
        int pos;
        esp.set_codigo_especie("CTA");
        pos = vectorCompleto.busquedaDicotomica(Especie("CTA", "","",""));
        cout << "   Posicion de CTA: " << pos << endl;
        pos = vectorCompleto.busquedaDicotomica(Especie("DMD", "","",""));
        cout << "   Posicion de DMD: " << pos << endl;
        pos = vectorCompleto.busquedaDicotomica(Especie("HCN", "","",""));
        cout << "   Posicion de HCN: " << pos << endl;
        pos = vectorCompleto. busquedaDicotomica(Especie("NDOF", "","",""));
        cout << "   Posicion de NDOF : " << pos << endl;
        pos = vectorCompleto.busquedaDicotomica(Especie("JAX", "","",""));
        cout << "   Posicion de JAX: " << pos << endl;


        //Búsqueda de Nombre común no nulo
        cout<<"==================================================="<<endl;
        cout << "Nombre comun no nulo" << endl;
        VDinamico<Especie*> vectorEspNComun = getEspNComun(vectorCompleto);
        cout<<"Numero de elemntos no nulos: "<<vectorEspNComun.gettLogico()<<endl;
        for (int i = 0; i < vectorEspNComun.gettLogico(); i++)
        {
            vectorEspNComun[i]->mostrarInfo();
        } 
    } 
    catch (const out_of_range& e) {
    cerr << "Error de rango: " << e.what() << endl;
    }
    catch (const exception& e)
    {
        cerr << endl << "main.cpp -> " << e.what() << endl;
        return 1;
    }
    return 0;
}
