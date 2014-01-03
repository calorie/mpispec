Dir.glob('vendor/bundle/ruby/**/gems/ceedling*/lib/rakefile.rb').each do |path|
  load path
end

task :default => %w[ test:all ]
