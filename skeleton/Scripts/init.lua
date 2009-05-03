
config = {}

function pairsByKeys(t,f)
	local a = {}
	for n in pairs(t) do a[#a + 1] = n end
	table.sort(a, f)
	local i = 0
	return function()
		i = i + 1
		return a[i], t[a[i]]
	end
end

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
	if debug then
		config_load("debug")
	else
		config_load("main")
	end
end

