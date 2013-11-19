# -*- encoding: utf-8 -*-
Gem::Specification.new do |gem|
  gem.authors       = ["Guy Decoux"]
  gem.email         = ["ts@moulon.inra.fr"]
  gem.description   = %q{Ruby bindings for Unix mmap(2) by Guy Decoux}
  gem.summary       = %q{Ruby bindings for Unix mmap(2) by Guy Decoux}
  gem.homepage      = "http://moulon.inra.fr/ruby/mmap.html"

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "mmap"
  gem.require_paths = ["."]
  gem.extensions    = ["extconf.rb"]
  gem.version       = "0.2.7"
end
