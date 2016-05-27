#ifndef ERROR_STRINGS_INCLUDED_3EB9B8AB_CF95_442A_B5FA_D9D2E523B369
#define ERROR_STRINGS_INCLUDED_3EB9B8AB_CF95_442A_B5FA_D9D2E523B369


namespace Error_string {


constexpr const char *
entity_is_invalid() { return "Core::Entity is invalid."; }


constexpr const char *
entity_not_found() { return "Core::Entity not found."; }


constexpr const char *
object_has_no_valid_world() { return "Object has no valid Core::World."; }


constexpr const char *
invalid_arguments() { return "Invalid arguments."; }


constexpr const char *
data_not_found() { return "Data not found."; }


constexpr const char *
unknown_type() { return "Unknown type."; }


constexpr const char *
resource_not_found() { return "Resource not found."; }


constexpr const char *
no_free_space() { return "Cannot find free space."; }


constexpr const char *
failed_to_create_resource() { return "Failed to create resource."; }


constexpr const char *
failed_to_find_resource() { return "Failed to find resource."; }


constexpr const char *
generic_sdl_fail() { return "SDL error."; }


constexpr const char *
generic_ogl_fail() { return "OpenGL error."; }


}


#endif // inc guard