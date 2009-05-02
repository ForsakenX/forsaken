
config = {}

function init( debug_on )
        dofile("lua_files/config.lua")
	load_raw('configs/defaults.txt')
	if debug_on == 1 then
		load_config('configs/debug.txt')
	else
		load_config('configs/main.txt')
	end
end

--[[
2
3
4
--]]
