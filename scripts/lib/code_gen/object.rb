require_relative "code_gen.rb"

require 'yaml'
require 'pathname'
require 'securerandom'


module CoreObjectGen

  class Tag

    @@tag_count = {}

    attr_reader :data

    def initialize(name)

      unless @@tag_count[name.to_sym] then
        @@tag_count[name.to_sym] = 1 << @@tag_count.length
      end

      @data = {}
      @data[:value] = @@tag_count[name.to_sym]

      @data[:name] = name
    end

  end

  class Object

    attr_reader :data

    def initialize(yml, object_name)
      @data = {}

      @data[:guid]   = SecureRandom.uuid.upcase.gsub("-", "_")
      @data[:subdir] = if yml['subdir'] then "#{yml['subdir']}/" else "" end

      @data[:name]          = yml['name']
      @data[:tags]          = if yml['tags'] then yml['tags'].split(' ').collect{|t| Tag.new(t)} else Array.new() end
      @data[:combined_tags] = @data[:tags].collect{|t| t.data[:value]}.inject(0, :+)

      @data[:trans_position] = "0, 0, 0"
      @data[:trans_scale]    = "1, 1, 1"
      @data[:trans_rot]      = "0, 0, 0"

      @data[:has_rb] = false

      if yml['rigidbody'] then
        @data[:has_rb] = true
        @data[:rb_is_dynamic] = if !yml['rigidbody']['is_dynamic'].nil? then yml['rigidbody']['is_dynamic'].to_s else "true" end
        @data[:rb_mass]       = if !yml['rigidbody']['mass'].nil?       then yml['rigidbody']['mass'].to_s       else "1"     end
        @data[:rb_is_trigger] = if !yml['rigidbody']['is_trigger'].nil? then yml['rigidbody']['is_trigger'].to_s else "false" end
      end

      @data[:has_renderer] = false

      if yml['renderer'] then
        @data[:has_renderer] = true
        @data[:renderer_type]       = "Material"
        @data[:renderer_mat_shader] = "fullbright"
        @data[:renderer_mat_map_01] = "dev_squares"
      end
    end

  end

  class ObjectGen

    attr_reader :objects

    def initialize(desc_dir, out_dir, template_dir)

      @objects = []

      # Generate Objects
      descs = Dir["#{desc_dir}**/*.yml"]

      descs.each do |desc|

        yml_desc = YAML.load_file(desc)

        if(yml_desc['object']) then

          yml_desc['object']['subdir'] = File.dirname(desc.gsub(desc_dir, ""))

          objects << Object.new(yml_desc['object'], File.basename(desc, "yml").downcase)

        end

      end

      tags = {}
      tags[:tags] = []

      full_template_dir = "#{Dir.pwd}/#{template_dir}"

      # Generate Data
      @objects.each do |object|

        tags[:tags] << object.data[:tags]

        header_code = []
        src_code    = []

        # Autogen warning
        header_code << parse_template(File.read("#{full_template_dir}autogen_warning.template"),            object.data)
        header_code << parse_template(File.read("#{full_template_dir}entity_create_header.template"),       object.data)
        src_code    << parse_template(File.read("#{full_template_dir}autogen_warning.template"),            object.data)
        src_code    << parse_template(File.read("#{full_template_dir}entity_setup_header_src.template"),    object.data)
        src_code    << parse_template(File.read("#{full_template_dir}entity_setup_entity_src.template"),    object.data)
        src_code    << parse_template(File.read("#{full_template_dir}entity_setup_transform_src.template"), object.data)

        if(object.data[:has_rb]) then
          src_code << parse_template(File.read("#{full_template_dir}entity_setup_rigidbody_src.template"), object.data)
        end

        if(object.data[:has_renderer]) then
          src_code << parse_template(File.read("#{full_template_dir}entity_setup_renderer_src.template"),  object.data)
        end

        src_code    << parse_template(File.read("#{full_template_dir}entity_setup_footer_src.template"),    object.data)

        output_path = "#{out_dir}#{object.data[:subdir]}"

        Dir.mkdir(output_path) unless File.exists?(output_path)

        File.write("#{output_path}#{object.data[:name].downcase}.cpp", src_code.join(""))
        File.write("#{output_path}#{object.data[:name].downcase}.hpp", header_code.join(""))
      end

      # Tags
      tags[:guid] = SecureRandom.uuid.upcase.gsub("-", "_")
      tags[:tags] = tags[:tags].flatten

      tags_header_code = []
      tags_header_code << parse_template(File.read("#{full_template_dir}autogen_warning.template"), tags)
      tags_header_code << parse_template(File.read("#{full_template_dir}object_tags.template"), tags)

      File.write("#{out_dir}object_tags.hpp", tags_header_code.join(""));

      # Object Factory
      factory_header_code = []
      factory_source_code = []

      factory = {}
      factory[:guid] = SecureRandom.uuid.upcase.gsub("-", "_")
      factory[:objects] = @objects;

      factory_header_code << parse_template(File.read("#{full_template_dir}autogen_warning.template"), factory)
      factory_header_code << parse_template(File.read("#{full_template_dir}object_factory_header.template"), factory)
      File.write("#{out_dir}object_factory.hpp", factory_header_code.join(""))

      factory_source_code << parse_template(File.read("#{full_template_dir}autogen_warning.template"), factory)
      factory_source_code << parse_template(File.read("#{full_template_dir}object_factory_src.template"), factory)
      File.write("#{out_dir}object_factory.cpp", factory_source_code.join(""))
    end
  end

end

# Test Dirs
# DESC_DIR     = "./domain/objects/desc/"
# OUT_DIR      = "./domain/objects/output/"
# TEMPLATE_DIR = "scripts/data_desc/object/templates/"

# puts "Generating Objects"
# CoreObjectGen::ObjectGen.new(ARGV[0], ARGV[1], TEMPLATE_DIR)
