task :ext do
	Dir.chdir("ext") do
		system "ruby extconf.rb && make clean && make"
	end
end

task :default => :ext
