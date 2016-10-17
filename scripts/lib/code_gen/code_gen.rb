

# Processes a template string.
# Mostly derived from this.
# http://bits.citrusbyte.com/how-to-write-a-template-library/
def parse_template(template, vars = {})

  lines = template.split("\n")

  func = "Proc.new do |#{vars.keys.join(", ")}|\n"
  func << "template_str = \"\" \n"

  lines.each do |line|

    if line =~ /^\s*(%)(.*?)$/
      func << " #{line.gsub(/^\s*%(.*?)$/, '\1') }\n"
    else
      func << " template_str << \"#{line.gsub(/\{\{([^\r\n\{]*)\}\}/, '#{\1}') }\n\" \n"
    end

  end

  func << " template_str\n"
  func << "end"

  eval(func).call(vars.values)

end
