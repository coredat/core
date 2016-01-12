#ifndef LOGIC_INCLUDED_5DE311B8_ED7B_4ADB_BCF5_2608833A4315
#define LOGIC_INCLUDED_5DE311B8_ED7B_4ADB_BCF5_2608833A4315


#include "logic_fwd.hpp"


namespace Logic {


/*!
  Loops through the array calling on_start.
  \param logic_components Array of logic components.
  \param size_of_logic_array Size of the inputted array.
*/
void
callback_on_start(Base *logic_components[], const std::size_t size_of_logic_array);


/*!
  Loops through the array call on_update.
  \param logic_components Array of logic components.
  \param size_of_logic_array Size of the inputted array.
  \param dt The delta time from when the array was last called.
*/
void
callback_update(Base *logic_components[], const std::size_t size_of_logic_array, const float dt);


} // ns


#endif // inc guard