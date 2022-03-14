#include "proceso.h"

Proceso::Proceso()
{
    this->tiempoTranscurrido = 0;
    this->finalizacion = SUCCESFUL_FINISH;
}

int Proceso::getId() const
{
    return id;
}

void Proceso::setId(int value)
{
    id = value;
}

std::string Proceso::getOperacion() const
{
    return operacion;
}

void Proceso::setOperacion(const std::string &value)
{
    operacion = value;
}

int Proceso::getTiempoEstimado() const
{
    return tiempoEstimado;
}

void Proceso::setTiempoEstimado(int value)
{
    tiempoEstimado = value;
}

int Proceso::getTiempoTranscurrido() const
{
    return tiempoTranscurrido;
}

void Proceso::setTiempoTranscurrido(int value)
{
    tiempoTranscurrido = value;
}

int Proceso::getResultadoOperacion() const
{
    return resultadoOperacion;
}

void Proceso::setResultadoOperacion(int value)
{
    resultadoOperacion = value;
}

bool Proceso::getFinalizacion() const
{
    return finalizacion;
}

void Proceso::setFinalizacion(bool value)
{
    finalizacion = value;
}

Proceso &Proceso::operator=(const Proceso &p)
{
    this->id = p.getId();
    this->operacion = p.getOperacion();
    this->tiempoEstimado = p.getTiempoEstimado();
    this->resultadoOperacion = p.getResultadoOperacion();
    this->tiempoTranscurrido = p.getTiempoTranscurrido();

    return *this;
}

std::ostream &operator<<(std::ostream &o, Proceso &p)
{
    o << p.getId() << ",";
    o << p.getOperacion() << ",";
    o << p.getResultadoOperacion() << ",";
    o << p.getTiempoEstimado() << ",";
    o << p.getTiempoTranscurrido();
    return o;
}
