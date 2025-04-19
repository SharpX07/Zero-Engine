#pragma once

class Panel
{
public:
    //virtual ~Panel() = default;
    virtual void OnRender() {};  // Cambiado a puro virtual para forzar la implementación en las clases derivadas.
};
