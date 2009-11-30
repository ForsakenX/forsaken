
-- calls error() instead of returning nil
function load_file(name)
	local f,m = loadfile(name)
	if f then
		return f
	else
		error(m)
	end
end

