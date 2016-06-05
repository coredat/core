require_relative "lib/code_gen.rb"


desc "Generate the header and source for data containers."
task :generate_containers do |t, args|

  data_path = "#{Dir.pwd}/scripts/data_desc/"

  # Get all files in data_desc folder.
  data_descs = Dir["#{data_path}**/*.yml"]

  # generate all the data files.
  data_descs.each do |f|

    desc = YAML.load_file(f)
   
    parsed_data = parse_desc(desc)
    
    header_template = File.read("#{Dir.pwd}/scripts/data_desc/data_header.template")
    header = parse_template(header_template, parsed_data)
    File.write(parsed_data[:path] + parsed_data[:data_name] + ".hpp", header)

    source_template = File.read("#{Dir.pwd}/scripts/data_desc/data_source.template")
    source = parse_template(source_template, parsed_data)
    File.write(parsed_data[:path] + parsed_data[:data_name] + ".cpp", source)

  end

end
