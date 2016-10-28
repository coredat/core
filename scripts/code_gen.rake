require_relative "lib/code_gen/model"
require_relative "lib/code_gen/object"


desc "Generate Models"
task :generate_models do |t, args|

  template_dir = "scripts/data_desc/model/templates/"
  desc_dir     = "scripts/data_desc/model/desc/"
  out_dir      = "code/data/"

  puts "Generating Models"
  CoreDataGen::DataGen.new(desc_dir, out_dir, template_dir)

end


desc "Generate Materials"
task :generate_materials, [:data_dir, :out_dir] do |t, args|

  system("ruby", "scripts/lib/code_gen/material.rb", "#{args[:data_dir]}", "#{args[:out_dir]}")

end


desc "Generate Objects"
task :generate_game_objects, [:data_dir, :out_dir] do |t, args|

  # system("ruby scripts/lib/code_gen/object.rb #{args[:data_dir]} #{args[:out_dir]}")
  template_dir = "scripts/data_desc/object/templates/"
  puts "Generating Objects"
  CoreObjectGen::ObjectGen.new(args[:data_dir], args[:out_dir], template_dir)


end
