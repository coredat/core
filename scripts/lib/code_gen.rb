
require 'rubygems'
require 'securerandom'
require 'yaml'


# Parses the Data description yaml
def parse_desc(yml)

  data = yml['data']

  return_data = {}

  # General information"
  return_data[:data_type]  = data['type'] || "Data"
  return_data[:data_name]  = data['name'] || "data"
  return_data[:guid]       = SecureRandom.uuid.upcase.gsub("-", "_")
  return_data[:namespace]  = data['namespace'] || "Data"
  return_data[:path]       = data['path']

  # Header Includes
  return_data[:header_includes] = []
  return_data[:header_includes] << "<utilities/generic_id.hpp>"
  return_data[:header_includes] << "<utilities/memory_pool.hpp>"
  return_data[:header_includes] << "<stddef.h>"

  # Header aditional code
  return_data[:header_add_code] = if data['header'] and data['header']['additional_code'] then data['header']['additional_code'] else "" end

  # Source Includes
  return_data[:src_includes] = []
  return_data[:src_includes] << '<' + data['header_dir'] + return_data[:data_name] + '.hpp>'
  return_data[:src_includes] << "<data/global_data/memory_data.hpp>"
  return_data[:src_includes] << "<common/error_strings.hpp>"
  return_data[:src_includes] << "<utilities/logging.hpp>"
  return_data[:src_includes] << "<utilities/memory.hpp>"
  return_data[:src_includes] << "<assert.h>"

  # Key information
  data_key = data['key'] || {}

  return_data[:key_type] = "util::generic_id"
  return_data[:key_name] = data_key['name'] || "data_key"

  if data_key.has_key?('error_on_missing_key')
    return_data[:key_missing_error] = data_key['error_on_missing_key']
  else
    return_data[:key_missing_error] = true
  end

  return_data[:key_search_type] = data_key['search_type'] || "linear"


  # Properties
  properties = data['properties']
  return_data[:properties] = []

  properties.each do |prop|
    return_data[:properties] << {}
    return_data[:properties][-1][:name] = "property_#{prop['name']}"
    return_data[:properties][-1][:type] = if prop['type'] == "char_32" then "char" else prop['type'] end
    return_data[:properties][-1][:multiple] = if prop['type'] == "char_32" then 32 else 1 end

    # What type of get set we have
    return_data[:properties][-1][:get_indirection] = if prop['return'].downcase == "pointer" then "**" else "*" end
    return_data[:properties][-1][:get_indirection_value] = if prop['return'].downcase == "pointer" then "&" else "" end

    return_data[:properties][-1][:set_indirection] = if prop['return'].downcase == "pointer" then "*" else "" end
    return_data[:properties][-1][:set_indirection_value] = if prop['return'].downcase == "pointer" then "*" else "" end

    # Has a dependent include
    if(prop['include'])
      return_data[:header_includes] << prop['include']
    end
  end

  # Memory
  return_data[:allocs] = []

  return_data[:allocs] << {}
  return_data[:allocs][-1][:name] = return_data[:key_name]
  return_data[:allocs][-1][:type] = return_data[:key_type]
  return_data[:allocs][-1][:multiple] = 1

  return_data[:properties].each do |alloc|
    return_data[:allocs] << {}
    return_data[:allocs][-1][:name] = alloc[:name]
    return_data[:allocs][-1][:type] = alloc[:type]
    return_data[:allocs][-1][:multiple] = alloc[:multiple]
  end

  # Prune out duplicate includes
  return_data[:header_includes] = return_data[:header_includes].uniq

  return return_data

end


# Processes a template string.
def parse_template(template, vars = {})

  lines = template.split("\n")

  func = "Proc.new do |#{vars.keys.join(", ")}|\n"
  func << "template_str = \"\" \n"

  lines.each do |line|

    if line =~ /^\s*(%)(.*?)$/
      func << " #{line.gsub(/^\s*%(.*?)$/, '\1') }\n"
    else
      func << " template_str << \"#{line.gsub(/\{\{([^\r\n\{]*)\}\}/, '#{\1}') }\n\" \n"
    end

  end

  func << " template_str\n"
  func << "end"

  eval(func).call(vars.values)

end
