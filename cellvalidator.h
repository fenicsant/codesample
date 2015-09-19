#ifndef CELLVALIDATOR_H
#define CELLVALIDATOR_H

#include "mwind.h"


class ICellValidator
{
public:
  enum ValueValidity {
    ValueValid,
    ValueBad,
    ValueRecomend
  };
  virtual ValueValidity check(MWind::Cell & cell,int value);
};

#endif // CELLVALIDATOR_H
