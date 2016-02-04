#ifndef CORE_DATA_INCLDUED_3B7A1761_B49B_41D6_B7D9_7439A356614E
#define CORE_DATA_INCLDUED_3B7A1761_B49B_41D6_B7D9_7439A356614E



namespace Data {


struct Input_data
{
  
}; // struct



struct Core {

  Input_data          *input_data;

}; // struct


Input_data*
get_input_data(Data::Core *core_data);


} // ns



#endif // inc gaurd