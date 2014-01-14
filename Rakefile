require 'rubygems'
require 'yaml'

data = YAML.load_file('project.yml')
ceedling_root = Gem.loaded_specs['ceedling'].full_gem_path
unless File.exists?(data[:plugins][:load_paths].first)
  ceedling_relative_root = ceedling_root.slice(File.join(File.expand_path(File.dirname(__FILE__)), ''))
  data[:plugins][:load_paths] = [File.join(ceedling_relative_root, 'plugins')]
  File.open('project.yml', 'w') { |f| YAML.dump(data, f) }
end

Dir.glob(File.join(ceedling_root, 'lib', 'rakefile.rb')).each do |path|
  load path
end

task :default => %w[ test:all ]
