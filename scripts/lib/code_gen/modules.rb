require_relative "code_gen.rb"

require 'yaml'
require 'pathname'
require 'securerandom'


module CoreModuleGen

  # Loops through all the directories and
  # creates a moodule file.
  class ModuleGen

    def initialize(core_dir, out_dir, template_dir)

      dirs = Dir.entries(core_dir).select {|entry| File.directory? File.join(core_dir,entry) and !(entry =='.' || entry == '..') }

      dirs.each do |d|
        File.write("#{out_dir}lib/#{d.to_s}", "")
      end

    end

  end

end
