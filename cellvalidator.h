#ifndef CELLVALIDATOR_H
#define CELLVALIDATOR_H

#include "mwind.h"

//! интерфейс проверки корректности значения для ячейки

class ICellValidator
{
public:
  //! варианты корректности значения
  enum ValueValidity {
    ValueValid,   //!< допустимое значение
    ValueBad,     //!< ошибочное значение
    ValueRecomend //!< рекомендованое значение
  };
  //! функция проверяет значение на корректность
  virtual ValueValidity check(MWind::Cell & cell,int digit) {return ValueValid;}
  //! указатель на текущий валидатор
  static ICellValidator * &cellValdator();
};

#endif // CELLVALIDATOR_H
