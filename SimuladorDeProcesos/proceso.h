#ifndef PROCESO_H
#define PROCESO_H

#include <iostream>

#define ERROR_FINISH 0
#define SUCCESFUL_FINISH 1
#define ACTION_CODE -1

class Proceso
{
private:
    std::string operacion;
    int id;
    int tiempoEstimado;
    int tiempoTranscurrido;
    int resultadoOperacion;
    bool finalizacion;

public:
    Proceso();
    //Getters and Setters
    void setNombre(const std::string &value);
    int getId() const;
    void setId(int value);
    int getTiempoEstimado() const;
    void setTiempoEstimado(int value);
    int getTiempoTranscurrido() const;
    void setTiempoTranscurrido(int value);
    int getResultadoOperacion() const;
    void setResultadoOperacion(int value);
    std::string getOperacion() const;
    void setOperacion(const std::string &value);
    bool getFinalizacion() const;
    void setFinalizacion(bool value);

    //Operators
    Proceso &operator=(const Proceso& p);
    friend std::ostream& operator<< (std::ostream&,Proceso&);
};

#endif // PROCESO_H
