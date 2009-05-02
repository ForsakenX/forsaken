
config = {}

function load_file(name)
	local f,m = loadfile(name)
	if f then
		return f
	else
		error(m)
	end
end

function do_file(name)
	dofile("Scripts/"..name)
end

function init( debug )
        do_file("config.lua")
	config_load_raw('defaults')
	if debug then
		config_load("debug")
	else
		config_load("main")
	end
end

