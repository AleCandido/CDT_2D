/** @file */
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
