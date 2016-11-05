require_relative "code_gen.rb"

require 'yaml'
require 'pathname'
require 'securerandom'


module CoreDataGen


  class Field

    attr_reader :data

    def initialize(yml, model_name, model_type, model_var, key_name, key_type, key_invalid)
      @data = {}

      @data[:name]         = if yml['name'] then yml['name'] else "ERROR_NO_FIELD_NAME" end
      @data[:type]         = if yml['type'] then yml['type'] else "ERROR_NO_FIELD_TYPE" end
      @data[:is_sentinal]  = false
      @data[:sentinal_val] = 0
      @data[:include]      = if yml['include'] then yml['include'] else "" end

      if @data[:type].end_with? "[]"
        @data[:is_sentinal]  = true
        @data[:type]         = @data[:type].gsub("[]", "")
        @data[:var]          = "field_sentinal_#{@data[:name].downcase}"
        @data[:sentinal_val] = if yml['sentinal'] then yml['sentinal'] else 0 end
      else
        @data[:var] = "field_#{@data[:name].downcase}"
      end

      @data[:data_type] = if yml['data_type'] then yml['data_type'] else @data[:type] end
      @data[:include]   = if yml['include'] then yml['include'] else "" end
      @data[:count]     = if yml['count'] then yml['count'] else 1 end

      # Sinked in arguments
      @data[:model_name]  = model_name
      @data[:model_type]  = model_type
      @data[:model_var]   = model_var
      @data[:key_name]    = key_name
      @data[:key_type]    = key_type
      @data[:key_invalid] = key_invalid
    end

  end


  class Model

    attr_reader :fields
    attr_reader :data

    def initialize(yml)

      @data = {}

      header_includes = []

      # Add some defaults.
      header_includes << "<stdint.h>"
      header_includes << "<stddef.h>"

      src_includes = []
      src_includes << "<assert.h>"
      src_includes << "<cstring>"

      # General settings
      @data[:name] = if yml['name'] then yml['name'] else "ERROR_NO_MODEL_NAME" end
      @data[:type] = "#{@data[:name].capitalize}_data"
      @data[:var]  = @data[:name].downcase
      @data[:guid] = SecureRandom.uuid.upcase.gsub("-", "_")

      @data[:subdir]   = if yml['subdir'] then "#{yml['subdir']}/" else "" end
      @data[:group]    = if yml['group'] then yml['group'] else "Data" end
      @data[:ns_start] = "namespace #{@data[:group]} {"
      @data[:ns_end]   = "} // #{@data[:group]} ns"

      # Keys
      @data[:key_type]    = "uint32_t"
      @data[:key_var]     = "keys"
      @data[:key_invalid] = "0"
      @data[:key_autogen] = true

      if(yml['key']) then
        @data[:key_type]    = if yml['key']['type']                    then yml['key']['type']              else @data[:key_type]    end
        @data[:key_var]     = if yml['key']['var']                     then yml['key']['var']               else @data[:key_var]     end
        @data[:key_invalid] = if yml['key']['invalid']                 then yml['key']['invalid']           else @data[:key_invalid] end
        @data[:key_autogen] = if !yml['key']['auto_generated_id'].nil? then yml['key']['auto_generated_id'] else @data[:key_autogen] end

        if yml['key']['include'] then header_includes << yml['key']['include'] end
      end

      # Generate the field data
      @fields = []

      if(yml['fields']) then
        yml['fields'].each do |f|
          @fields << Field.new(f, @data[:name], @data[:type], @data[:var], @data[:key_name], @data[:key_type], @data[:key_invalid])

          if @fields[-1].data[:include] != ""
            header_includes << @fields[-1].data[:include];
          end
        end
      end

      # Need to generate a partial data for the model
      # so it can use that data to create the model structure
      @data[:fields] = []

      @fields.each do |f|
        field = {}

        @data[:fields] << f.data
      end

      # Includes
      @data[:header_includes] = header_includes.collect{|i| "#include #{i}"}.uniq.join("\n")
      @data[:src_includes]    = src_includes.collect{|i| "#include #{i}"}.uniq.join("\n")

    end

  end


  class DataGen

    attr_reader :models

    def initialize(desc_dir, out_dir, template_dir)

      @models = []

      # Generate Models and Model fields.
      descs = Dir["#{desc_dir}**/*.yml"]

      descs.each do |desc|
        yml_desc = YAML.load_file(desc)

        if(yml_desc['data']) then

          yml_desc['data']['name'] = File.basename(desc, ".yml").downcase
          yml_desc['data']['subdir'] = File.dirname(desc.gsub(desc_dir, ""))

          # puts out_dir

          @models << Model.new(yml_desc['data'])
        end
      end

      # Generate data.
      @models.each do |model|

        header_code = []
        src_code    = []

        # Autogen warning
        header_code << parse_template(File.read("#{template_dir}/autogen_warning.template"))
        src_code    << parse_template(File.read("#{template_dir}/autogen_warning.template"))

        # Template headers
        header_code << parse_template(File.read("#{template_dir}/model_header_header.template"), model.data)
        src_code    << parse_template(File.read("#{template_dir}/model_header_src.template"), model.data)

        # Template common functions
        header_code << parse_template(File.read("#{template_dir}/seperation_common.template"), model.data)
        src_code    << parse_template(File.read("#{template_dir}/seperation_common.template"), model.data)

        header_code << parse_template(File.read("#{template_dir}/model_common_header.template"), model.data)
        src_code    << parse_template(File.read("#{template_dir}/model_common_src.template"), model.data)

        # Generate data getters
        header_code << parse_template(File.read("#{template_dir}/seperation_data.template"), model.data)
        src_code    << parse_template(File.read("#{template_dir}/seperation_data.template"), model.data)

        model.fields.each do |f|
          header_code << parse_template(File.read("#{template_dir}/field_data_header.template"), f.data)
          src_code    << parse_template(File.read("#{template_dir}/field_data_src.template"), f.data)
        end

        # Generate field getter and setter methods
        header_code << parse_template(File.read("#{template_dir}/seperation_fields.template"), model.data)
        src_code    << parse_template(File.read("#{template_dir}/seperation_fields.template"), model.data)

        model.fields.each do |f|
          header_code << parse_template(File.read("#{template_dir}/field_value_header.template"), f.data)
          src_code    << parse_template(File.read("#{template_dir}/field_value_src.template"), f.data)
        end

        # Template footers
        header_code << parse_template(File.read("#{template_dir}/model_footer_header.template"), model.data)
        src_code    << parse_template(File.read("#{template_dir}/model_footer_src.template"), model.data)

        # Write out the files
        output_path = "#{out_dir}#{model.data[:subdir]}"
        Dir.mkdir(output_path) unless File.exists?(output_path)

        File.write("#{output_path}#{model.data[:name].downcase}_data.cpp", src_code.join(""))
        File.write("#{output_path}#{model.data[:name].downcase}_data.hpp", header_code.join(""))
      end

      # Generate the fwd dec file
      fwd_dec_data = {}
      fwd_dec_data[:guid] = SecureRandom.uuid.upcase.gsub("-", "_")
      fwd_dec_data[:groups] = {}

      @models.collect{|x| x.data[:group]}.uniq.each{|x| fwd_dec_data[:groups][x] = []}
      @models.each{|x| fwd_dec_data[:groups][x.data[:group]] << x.data[:type]}

      fwd_code = []
      fwd_code << parse_template(File.read("#{template_dir}/autogen_warning.template"))
      fwd_code << parse_template(File.read("#{template_dir}/forward_decleration.template"), fwd_dec_data)

      File.write("#{out_dir}fwd_dec_data.hpp", fwd_code.join(""))
    end # initialize

  end

end

# Test directories
# --
# From the outter mose Rake this coould be improved.
DESC_DIR     = "scripts/data_desc/model/desc/"
OUT_DIR      = "code/data/"
TEMPLATE_DIR = "scripts/data_desc/model/templates/"

# Generate Models
# puts "Generating Models"
# CoreDataGen::DataGen.new(DESC_DIR, OUT_DIR, TEMPLATE_DIR)
