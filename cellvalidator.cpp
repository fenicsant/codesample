#include "cellvalidator.h"

class BaseValidator :public ICellValidator
{
public:
  ICellValidator::ValueValidity check(MWind::Cell &cell, int digit);
};

ICellValidator * & ICellValidator::cellValdator()
{
  static ICellValidator *res = new BaseValidator;
  return res;
}

ICellValidator::ValueValidity BaseValidator::check(MWind::Cell &cell, int digit)
{
  foreach(MWind::Cell *c,cell.relatedCells()) if (c->isDeterminans()==digit) return ValueBad;
  return ValueValid;
}

