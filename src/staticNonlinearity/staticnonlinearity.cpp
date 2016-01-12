#include "staticnonlinearity.h"

StaticNonlinearity::StaticNonlinearity()
{

}

StaticNonlinearity::~StaticNonlinearity()
{

}

void StaticNonlinearity::applyStaticNonlinearity(cube * const linearFilter)
{
    for(int k = 0; k < int(linearFilter->n_slices); k++){
        for(int i = 0; i < int(linearFilter->n_rows); i++){
            for(int j = 0; j < int(linearFilter->n_cols); j++){
                linearFilter->at(i,j,k)  = advance(linearFilter->at(i,j,k));
            }
        }
    }
}

