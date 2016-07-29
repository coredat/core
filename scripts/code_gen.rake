require_relative "lib/code_gen.rb"


desc "Generate the header and source for data containers."
task :generate_containers do |t, args|

  puts "--"
  puts "Generate data containers."

  data_path = "#{Dir.pwd}/scripts/data_desc/"

  puts "Data Path: #{data_path}"

  # Get all files in data_desc folder.
  data_descs = Dir["#{data_path}**/*.yml"]

  puts "Found #{data_descs.length} data description(s)"
  puts "--"

  # Generate all the data files.
  data_descs.each do |f|

    puts "Generating #{File.basename(f)}"
    puts "--"

    desc = YAML.load_file(f)

    parsed_data = parse_desc(desc)

    # Generate the container files.

    print "header ... "

    header_template = File.read("#{Dir.pwd}/scripts/data_desc/data_header.template")
    header = parse_template(header_template, parsed_data)
    File.write(parsed_data[:path] + parsed_data[:data_name] + ".hpp", header)

    puts "[done]"
    print "source ... "

    source_template = File.read("#{Dir.pwd}/scripts/data_desc/data_source.template")
    source = parse_template(source_template, parsed_data)
    File.write(parsed_data[:path] + parsed_data[:data_name] + ".cpp", source)

    puts "[done]"

    # Generate the debug ui files.

    if parsed_data[:debug_ui]

      print "ui header ... "

      header_template = File.read("#{Dir.pwd}/scripts/data_desc/data_ui_header.template")
      header = parse_template(header_template, parsed_data)
      File.write(parsed_data[:debug_ui_path] + "debug_ui_" + parsed_data[:data_name] + ".hpp", header)

      puts "[done]"
      print "ui source ... "

      source_template = File.read("#{Dir.pwd}/scripts/data_desc/data_ui_source.template")
      source = parse_template(source_template, parsed_data)
      File.write(parsed_data[:debug_ui_path] + "debug_ui_" + parsed_data[:data_name] + ".cpp", source)

      puts "[done]"

    end

    puts ""

  end

end
