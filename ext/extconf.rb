require "mkmf"

dir_config "c_unix", "aup"
create_makefile "c_unix"

Dir.chdir("aup") do
	system "ruby extconf.rb && make"
end
