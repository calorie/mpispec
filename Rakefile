require 'rubygems'
require 'yaml'

ceedling_root = Gem.loaded_specs['ceedling'].full_gem_path
ceedling_root.slice!(File.join(File.expand_path(File.dirname(__FILE__)), ''))

data = YAML.load_file('project.yml')
data[:plugins][:load_paths] = [File.join(ceedling_root, 'plugins')]
File.open('project.yml', 'w') { |f| YAML.dump(data, f) }

Dir.glob(File.join(ceedling_root, 'lib', 'rakefile.rb')).each do |path|
  load path
end

task :default => %w[ test:all ]
