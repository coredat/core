
require 'rubygems'
require 'securerandom'
require 'yaml'
require 'date'


# Parses the Data description yaml
def parse_desc(yml)

  data = yml['data']

  return_data = {}

  return_data[:file_warning]     = "
/*
  WARNING
  -------
  This file is auto generated any changes here may be overwritten.
  See code_gen.rake in scripts folder.

  This file was last generated on: #{Date.today.strftime('%a %d %b %Y')}
*/
"

  # General information"
  return_data[:data_type]         = data['name'].capitalize + "_data"
  return_data[:data_name]         = data['name'] + "_data"
  return_data[:data_display_name] = data['name']
  return_data[:guid]              = SecureRandom.uuid.upcase.gsub("-", "_")
  return_data[:namespace]         = data['namespace'] || "Data"
  return_data[:path]              = data['path']

  # Debug UI
  return_data[:debug_ui]          = false
  return_data[:debug_ui_path]        = "./"

  if data['debug_ui']
    if data['debug_ui']['generate']
      return_data[:debug_ui] = data['debug_ui']['generate']
    end

    if data['debug_ui']['path']
      return_data[:debug_ui_path] = data['debug_ui']['path']
    end
  end

  # Header Includes
  return_data[:header_includes] = []
  return_data[:header_includes] << "<utilities/generic_id.hpp>"
  return_data[:header_includes] << "<utilities/memory_pool.hpp>"
  return_data[:header_includes] << "<stddef.h>"

  # Aditional headers
  if data['additional_includes']
    data['additional_includes'].each do |inc|
      if inc['file'] == 'header'
        return_data[:header_includes] << inc['include']
      end
    end
  end

  # Header aditional code
  if data['header'] && data['header']['additional_headers']
    data['header']['additional_headers'].each do |inc|
      return_data[:header_includes] << inc['include']
    end
  end

  return_data[:header_add_code] = if data['header'] and data['header']['additional_code'] then data['header']['additional_code'] else "" end

  # Source Includes
  return_data[:src_includes] = []
  return_data[:src_includes] << '<' + data['header_dir'] + return_data[:data_name] + '.hpp>'
  return_data[:src_includes] << "<data/global_data/memory_data.hpp>"
  return_data[:src_includes] << "<common/error_strings.hpp>"
  return_data[:src_includes] << "<utilities/logging.hpp>"
  return_data[:src_includes] << "<utilities/memory.hpp>"
  return_data[:src_includes] << "<assert.h>"

  # Aditional headers
  if data['additional_includes']
    data['additional_includes'].each do |inc|
      if inc['file'] == 'source'
        return_data[:src_includes] << inc['include']
      end
    end
  end


  # Key information
  data_key = data['key'] || {}

  return_data[:key_type] = "util::generic_id"
  return_data[:key_name] = data['name'] + "_id"
  return_data[:key_auto_inc] = data_key['auto_increment'] || false

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

    return_data[:properties][-1][:return_type] = if prop['return'] then prop['return'].downcase else "value".downcase end

    return_data[:properties][-1][:name] = "property_#{prop['name']}"
    return_data[:properties][-1][:type] = if prop['type'] == "char_32" then "char" else prop['type'] end
    return_data[:properties][-1][:multiple] = if prop['type'] == "char_32" then 32 else 1 end
    return_data[:properties][-1][:searchable] = prop['searchable'] || false

    # What type of get set we have
    return_data[:properties][-1][:get_indirection]       = if return_data[:properties][-1][:return_type] == "pointer" then "**" else "*" end
    return_data[:properties][-1][:get_indirection_value] = if return_data[:properties][-1][:return_type] == "pointer" then "&" else "" end

    return_data[:properties][-1][:set_indirection]       = if return_data[:properties][-1][:return_type] == "pointer" then "*" else "" end
    return_data[:properties][-1][:set_indirection_value] = if return_data[:properties][-1][:return_type] == "pointer" then "*" else "" end

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
# Mostly derived from this.
# http://bits.citrusbyte.com/how-to-write-a-template-library/
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
