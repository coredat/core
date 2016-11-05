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

    def initialize(yml, object_name, material_factory)
      @data = {}

      @data[:guid]   = SecureRandom.uuid.upcase.gsub("-", "_")
      @data[:subdir] = if yml['subdir'] then "#{yml['subdir']}/" else "" end

      @data[:name]             = yml['name']
      @data[:tags]             = if yml['tags'] then yml['tags'].split(' ').collect{|t| Tag.new(t)} else Array.new() end
      @data[:combined_tags]    = @data[:tags].collect{|t| t.data[:value]}.inject(0, :+)
      @data[:material_factory] = material_factory;

      @data[:has_transform] = true # because we just default to it

      # because its
      if yml['transform'].nil? then yml['transform'] = "" end

      if yml['transform'] then
        @data[:trans_position] = if !yml['transform']['position'].nil? then yml['transform']['position'].to_s else "0, 0, 0" end
        @data[:trans_scale]    = if !yml['transform']['scale'].nil?    then yml['transform']['scale'].to_s    else "1, 1, 1" end
        @data[:trans_rot]      = if !yml['transform']['rotation'].nil? then yml['transform']['rotation'].to_s else ""        end
      end

      @data[:has_rb] = false

      if yml['rigidbody'] then
        @data[:has_rb] = true
        @data[:rb_is_kinematic] = if !yml['rigidbody']['is_kinematic'].nil?    then yml['rigidbody']['is_kinematic'].to_s else "false" end
        @data[:rb_mass]         = if !yml['rigidbody']['mass'].nil?            then yml['rigidbody']['mass'].to_s            else "1"     end
        @data[:rb_is_trigger]   = if !yml['rigidbody']['is_trigger'].nil?      then yml['rigidbody']['is_trigger'].to_s      else "false" end
      end

      @data[:has_collider] = false
      @data[:rb_collider_args] = "0.5, 0.5, 0.5"

      if yml['collider'] then
        @data[:has_collider] = true
        @data[:rb_collider_args] = if !yml['collider']['box'].nil? then yml['collider']['box'] else "0.5, 0.5, 0.5" end
      end

      @data[:has_renderer] = false

      if yml['renderer'] then
        @data[:has_renderer]      = true
        @data[:renderer_type]     = "Material"
        @data[:renderer_mat_name] = if !yml['renderer']['material'].nil? then yml['renderer']['material'] else "" end
      end

      @data[:has_camera] = false

      if yml['camera'] then
        @data[:has_camera] = true
      end

      @data[:mesh_file] = if !yml['mesh'].nil? then yml['mesh'] else "unit_cube.obj" end
    end

  end

  class ObjectGen

    attr_reader :objects

    def initialize(desc_dir, out_dir, full_template_dir, material_factory)

      @objects = []

      # Generate Objects
      descs = Dir["#{desc_dir}**/*.yml"]

      descs.each do |desc|

        yml_desc = YAML.load_file(desc)

        if(yml_desc['object']) then
          puts material_factory

          yml_desc['object']['subdir'] = File.dirname(desc.gsub(desc_dir, ""))

          objects << Object.new(yml_desc['object'], File.basename(desc, "yml").downcase, material_factory)

        end

      end

      tags = {}
      tags[:tags] = []

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

        if(object.data[:has_camera]) then
          puts "Camera"
          src_code << parse_template(File.read("#{full_template_dir}entity_setup_camera_src.template"),  object.data)
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

      factory           = {}
      factory[:guid]    = SecureRandom.uuid.upcase.gsub("-", "_")
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
