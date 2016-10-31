require_relative "code_gen.rb"

require 'yaml'
require 'pathname'
require 'securerandom'


module CoreMaterialGen

  class Material

    attr_reader :data

    def initialize(yml, shd_path, tex_path)
      @data = {}

      @data[:guid] = SecureRandom.uuid.upcase.gsub("-", "_")

      @data[:name] = if yml['name'].nil? then name else yml['name'].downcase.gsub(" ", "_") end
      @data[:shader] = yml['shader']
      @data[:map_01] = yml['map_01']
      @data[:color] = yml['color']

      @data[:shader_path] = shd_path
      @data[:texture_path] = tex_path

    end

  end

  class MaterialGen

    def initialize(desc_dir, out_dir, template_dir)

      @materials = []

      descs = Dir["#{desc_dir}**/*.yml"]

      descs.each do |desc|
        yml_desc = YAML.load_file(desc)

        yml_desc['material']['name'] = File.basename(desc, ".yml").downcase
        yml_desc['material']['subdir'] = File.dirname(desc.gsub(desc_dir, ""))

        @materials << Material.new(yml_desc['material'], "", "")

      end

      @materials.each do |material|

        header_code = []
        src_code = []

        # Autogen warning
        header_code << parse_template(File.read("#{template_dir}/autogen_warning.template"))
        src_code    << parse_template(File.read("#{template_dir}/autogen_warning.template"))

        header_code << parse_template(File.read("#{template_dir}/material_header.template"), material.data)
        src_code    << parse_template(File.read("#{template_dir}/material_src.template"), material.data)

        # Write out the files
        output_path = "#{out_dir}#{material.data[:subdir]}"
        Dir.mkdir(output_path) unless File.exists?(output_path)

        File.write("#{output_path}#{material.data[:name].downcase}.cpp", src_code.join(""))
        File.write("#{output_path}#{material.data[:name].downcase}.hpp", header_code.join(""))
      end

      material_data             = {}
      material_data[:guid]      = SecureRandom.uuid.upcase.gsub("-", "_")
      material_data[:materials] = @materials

      material_factory_header_code = []
      material_factory_src_code    = []

      material_factory_header_code << parse_template(File.read("#{template_dir}/autogen_warning.template"))
      material_factory_src_code    << parse_template(File.read("#{template_dir}/autogen_warning.template"))

      material_factory_header_code << parse_template(File.read("#{template_dir}/material_factory_header.template"), material_data)
      material_factory_src_code    << parse_template(File.read("#{template_dir}/material_factory_src.template"), material_data)

      File.write("#{out_dir}material_factory.hpp", material_factory_header_code.join(""))
      File.write("#{out_dir}material_factory.cpp", material_factory_src_code.join(""))
    end

  end

end
