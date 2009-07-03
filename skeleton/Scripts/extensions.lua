
table.length = function(t)
	local i = 0
	for n in pairs(t) do i=i+1 end
	return i
end

table.keys = function(t)
	local list = {}
	for key in pairs(t) do table.insert(list,key) end
	return list
end

-- iterate table by sorted keys
table.pairsByKeys = function(t,f)
	local a = {}
	for n in pairs(t) do a[#a + 1] = n end
	table.sort(a, f)
	local i = 0
	return function()
		i = i + 1
		return a[i], t[a[i]]
	end
end

