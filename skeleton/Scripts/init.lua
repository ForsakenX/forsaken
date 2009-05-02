
config = {}

function init( debug_on )
        dofile("Scripts/config.lua")
	load_raw('Configs/defaults.txt')
	if debug_on == 1 then
		load_config('debug')
	else
		load_config('main')
	end
end

