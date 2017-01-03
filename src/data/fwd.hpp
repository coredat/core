#ifndef FWD_INCLUDED_3E65F99D_FE7D_4420_BFD0_97B9E730B351
#define FWD_INCLUDED_3E65F99D_FE7D_4420_BFD0_97B9E730B351


namespace op {


struct buffer_data;
struct context_data;


namespace command {

using cmd_api_function = void(*)(context_data *context, void *data);

} // ns


} // ns


#endif // inc guard
