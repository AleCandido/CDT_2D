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

#include "gaugeelement.h"
#include "randomgenerator.h"
#include "triangulation.h"
#include "edge.h"
#include <iostream>
#include <cmath>
#include <random>

GaugeElement::GaugeElement(){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(i == j)
                mat[i][j] = 1; 
            else
                mat[i][j] = 0;
        }
    }
}

GaugeElement::GaugeElement(const Label& edge)
{
    base_edge = edge;
    *this = alpha_id(1.);
}

GaugeElement::GaugeElement(const complex<double> (&matrix)[N][N])
{
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            mat[i][j] = matrix[i][j];
    }
}

Label GaugeElement::base(){ return base_edge; }

void GaugeElement::set_base(Label base){ base_edge = base; }

int GaugeElement::dimension(){ return N; }

complex<double>** GaugeElement::matrix()
{
    complex<double>** aux = 0;
    aux = new complex<double>*[N];
    
    for(int i=0; i<N; i++){
        aux[i] = new complex<double>[N];
        
        for(int j=0; j<N; j++)
            aux[i][j] = mat[i][j];
    }
    
    return aux;
}

complex<double>* GaugeElement::operator[](int i)
{
    if( i >= N )
        throw out_of_range("Index out of bounds");
    
    return mat[i];
}

double GaugeElement::partition_function()
{
    /** @todo
     * deve dipendere da N
     */ 
    
    double g_ym2 = pow(base_edge.dync_edge()->get_owner()->g_ym, 2);
    // slightly transform the Force in order to put the integrand in the form
    // exp(tr(Source.dagger * U + U.dagger * Source))
    GaugeElement Force = *this;
    GaugeElement Source = (Force / (2 * N * g_ym2)).dagger();
    double Z = 1.;
    
    if(N == 1){
        // I_0(2|z|), bib: R.Brower, P.Rossi, C.Tan "The external field problem for QCD"
        Z = cyl_bessel_i(0, 2*abs(Source.mat[0][0]));
    }
    
    return Z;
}

void GaugeElement::random_element(double a)
{
    RandomGen r;
    
    double pi = 2 * asin(1);
    
    if( N == 1 ){
        // extracted elements' distribution is lorentzian
        
        // double theta = 2 * pi * r.next();
        // mat[0][0] = exp(1i * theta);
        double c = sqrt(a/2);
        double k = atan(c*pi);
        
        double x = r.next();
        double alpha = tan(k * x) / c;
        
        this->mat[0][0] = exp(1i * alpha);
    }
    else
        throw runtime_error("random_element: Not implemented for N!=1");
}

void GaugeElement::heatbath(GaugeElement Force)
{
    RandomGen r;
    
    bool accepted = false;
    double g_ym2 = pow(Force.base()->get_owner()->g_ym, 2);
    
    double a;
    double c;
    if( N == 1)
        a = 2 * N * abs(Force.tr()) / g_ym2;
        c = sqrt(a/2);
    
    // double max_rho;
    // if(N == 1)
    //     max_rho = exp(abs(Force.tr()) / (N * g_ym2));
    // in the general case the max_R abs(tr(RZ)) = tr(S), where S is the matrix of singular values of Z
    // and is also true that re(x) <= abs(x) (so tr(S) is not the maximum for re(tr(RZ)), but is >= of the max)
    
    while(not accepted){
        this->random_element(a);
        
        if( N == 1){
            double alpha = arg(mat[0][0]);
            double x = r.next();
            double eta;
            if(a >= 0.8)
                eta = 0.99;
            else
                eta = 0.73;
            accepted = ( x < eta * (1 + pow(c,2) * pow(alpha,2)) * exp(a * (cos(alpha) - 1)) );
        }
        else
            throw runtime_error("heatbath: Not implemented for N!=1");
        
        // double rho = exp(real((*this * Force).tr()) / (N * g_ym2));
    }    
}

// ##### ALGEBRA #####

GaugeElement GaugeElement::operator+(const GaugeElement& V)
{
    GaugeElement U;
    
    U.base_edge = this->base_edge;
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            U.mat[i][j] = this->mat[i][j] + V.mat[i][j];
    }

    return U;
}

GaugeElement GaugeElement::operator-(const GaugeElement& V)
{
    GaugeElement U;
    
    U.base_edge = this->base_edge;
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            U.mat[i][j] = (this->mat[i][j] - V.mat[i][j]);
    }

    return U;
}

GaugeElement GaugeElement::operator*(const GaugeElement& V)
{
    GaugeElement U;
    
    U.base_edge = this->base_edge;
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            U.mat[i][j] = 0;
            for(int k=0; k<N; k++)
                U.mat[i][j] += this->mat[i][k]*V.mat[k][j];
        }
    }

    return U;
}

GaugeElement GaugeElement::operator+=(const GaugeElement& V)
{
    return *this = *this + V;
}

GaugeElement GaugeElement::operator-=(const GaugeElement& V)
{
    return *this = *this - V;
}

GaugeElement GaugeElement::operator*=(const GaugeElement& V)
{
    return *this = *this * V;
}

GaugeElement GaugeElement::dagger()
{
    GaugeElement U;
    
    U.base_edge = this->base_edge;
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            U.mat[i][j] = conj(this->mat[j][i]);
    }

    return U;    
}

complex<double> GaugeElement::trace()
{
    complex<double> trace = 0;
    
    for(int i=0; i<N; i++)
        trace += this->mat[i][i];
    
    return trace;
}
complex<double> GaugeElement::tr()
{
    return trace();
}

// scalars

GaugeElement GaugeElement::alpha_id(const complex<double>& alpha)
{
    GaugeElement alpha_id;
    
    alpha_id.base_edge = this->base_edge;
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(i == j)
                alpha_id.mat[i][j] = alpha; 
            else
                alpha_id.mat[i][j] = 0;
        }
    }
    
    return alpha_id;
}

GaugeElement GaugeElement::operator=(const complex<double>& alpha)
{
    *this = alpha_id(alpha);
    
    return *this;
}

GaugeElement GaugeElement::operator+(const complex<double>& alpha)
{
    return *this + alpha_id(alpha);
}

GaugeElement GaugeElement::operator-(const complex<double>& alpha)
{
    return *this - alpha_id(alpha);
}

GaugeElement GaugeElement::operator*(const complex<double>& alpha)
{
    return *this * alpha_id(alpha);
}

GaugeElement GaugeElement::operator/(const complex<double>& alpha)
{
    GaugeElement quotient;
    
    quotient.base_edge = this->base_edge;
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            quotient.mat[i][j] /= alpha;
        }
    }
    
    return quotient;
}

GaugeElement GaugeElement::operator+=(const complex<double>& alpha)
{
    return *this = *this + alpha;
}

GaugeElement GaugeElement::operator-=(const complex<double>& alpha)
{
    return *this = *this - alpha;
}

GaugeElement GaugeElement::operator*=(const complex<double>& alpha)
{
    return *this = *this * alpha;
}

GaugeElement GaugeElement::operator/=(const complex<double>& alpha)
{
    return *this = *this / alpha;
}

// auxiliary

void GaugeElement::unitarize()
{
    if(N == 1)
        mat[0][0] /= abs(mat[0][0]);
}

// ##### FILE I/O #####

void GaugeElement::write(std::ostream& output)
{
    int pos = base_edge->position();
    output.write((char*)&pos, sizeof(pos));
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            output.write((char*)&mat[i][j], sizeof(mat[i][j]));
    }
}

void GaugeElement::read(std::istream& input, const vector<Label>& List1)
{
    int pos = 0;
    input.read((char*)&pos, sizeof(pos));
    base_edge = List1[pos];
    
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            input.read((char*)&mat[i][j], sizeof(mat[i][j]));
    }
}