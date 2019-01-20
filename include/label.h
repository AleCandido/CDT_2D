/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2019  Alessandro Candido <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LABEL_H
#define LABEL_H

#include <memory>

#include "simplex.h"
using namespace std;

/**
 * Non so davvero perché sto usando questi Label anziché gli unique_ptr<Simplex>, i motivi sono i seguenti:
 *      - il principale è che nella tesi di Giuseppe usava i Label
 *      - posso implementare come se fossero unique_ptr, se in futuro mi venisse comodo posso aggiungere struttura e metodi a Label
 *      - ultimo, ma quasi primo: è molto più corto scrivere Label che unique_ptr<Simplex>
 */

class Vertex;
class Triangle;

/**
* @test devo testare che il costruttore con Simplex* costruisca un oggetto Label che si comporti a tutti gli effetti come uno shared_ptr<Simplex>
* 
*/
class Label : public shared_ptr<Simplex>{
public:
    Label() : shared_ptr<Simplex>(){}
    Label(Simplex*);
    
    ~Label(){}
    
    Vertex* dync_vertex();
    Triangle* dync_triangle();
    
};

#endif // LABEL_H
