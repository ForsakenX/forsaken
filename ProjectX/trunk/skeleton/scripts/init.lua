
-- dofile that looks in proper folder
function dofile(filename)
	path="./scripts/"..filename
	local f = assert(loadfile(path))
	return f()
end

-- startup tasks
function init( debug )

	-- tell require() to look in ./Scripts/*
	package.path = "./scripts/?.lua;" .. package.path

	-- globals
	config = {} -- main config table

	-- load modules
	games = require("games")

	-- load library files
        dofile("extensions.lua")
        dofile("helpers.lua")
        dofile("config.lua")

	-- load config
	if debug then
		config_load("debug")
	else
		config_load("main")
	end

end

