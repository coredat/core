require_relative "lib/code_gen/model"


desc "Generate Models"
task :generate_models do |t, args|

  `ruby scripts/lib/code_gen/model.rb`

end
