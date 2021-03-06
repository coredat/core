require_relative "lib/code_gen/model"
require_relative "lib/code_gen/object"
require_relative "lib/code_gen/material"
require_relative "lib/code_gen/modules"


desc "Generate Models"
task :generate_models do |t, args|

  template_dir = "#{File.dirname(__FILE__)}/data_desc/model/templates/"
  desc_dir     = "#{File.dirname(__FILE__)}/data_desc/model/desc/"
  out_dir      = "#{File.dirname(__FILE__)}/../code/data/"

  puts "Generating Models"
  puts "desc: #{desc_dir}"
  puts "output: #{out_dir}"
  CoreDataGen::DataGen.new(desc_dir, out_dir, template_dir)

end


desc "Generate Materials"
task :generate_materials, [:data_dir, :out_dir] do |t, args|

  template_dir = "#{File.dirname(__FILE__)}/data_desc/material/templates/"

  puts "Generating Materials"
  CoreMaterialGen::MaterialGen.new(args[:data_dir], args[:out_dir], template_dir)

end


desc "Generate Objects"
task :generate_game_objects, [:data_dir, :out_dir, :material_factory] do |t, args|

  template_dir = "#{File.dirname(__FILE__)}/data_desc/object/templates/"

  puts "Generating Objects"
  CoreObjectGen::ObjectGen.new(args[:data_dir], args[:out_dir], template_dir, args[:material_factory])

end


desc "Generate Core Modules"
task :generate_modules do |t, args|

  api_dir = "#{File.dirname(__FILE__)}/../code/core/"
  out_dir = "#{File.dirname(__FILE__)}/../code/core/"
  template_dir = "#{File.dirname(__FILE__)}/data_desc/modules/templates/"

  puts "Generating Modules"

  CoreModuleGen::ModuleGen.new(api_dir, out_dir, template_dir);

end
