require_relative "lib/code_gen.rb"


desc "Generate the header and source for data containers."
task :generate_data do |t, args|

  data_path = "#{Dir.pwd}/scripts/data_desc/"
  puts data_path

  # Get all files in data_desc folder.
  data_descs = Dir["#{data_path}**/*.yml"]
  puts data_descs.inspect

  # generate all the data files.
  data_descs.each do |f|

    puts f

  end

end
