require_relative "code_gen.rb"

require 'yaml'
require 'pathname'
require 'securerandom'


module CoreMaterialGen

  class Material

    attr_reader :data

    def initialize(yml, name)
      @data = {}

      @data[:guid] = SecureRandom.uuid.upcase.gsub("-", "_")

      @data[:name] = if yml['name'].nil? then name else yml['name'].downcase.gsub(" ", "_") end
      @data[:shader] = yml['shader']
      @data[:map_01] = yml['map_01']
      @data[:color] = yml['color']

    end

  end

  class MaterialGen

    def initialize(desc_dir, out_dir, template_dir)



    end

  end

end

#Test Dirs
DESC_DIR     = "./domain/materials/desc/"
OUT_DIR      = "./domain/materials/output/"
TEMPLATE_DIR = "scripts/data_desc/materials/templates/"

puts "Generating Materials"
CoreMaterialGen::MaterialGen.new(DESC_DIR, OUT_DIR, TEMPLAT_DIR)
