-- vim:set sw=4 ts=4:
--
-- Copyright (C) 2009  Pim Goossens
--
-- Modified by Daniel Aquino (aka methods) for ProjectX
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
-- 
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in the
--    documentation and/or other materials provided with the distribution.
-- 3. The name of the author may not be used to endorse or promote
--    products derived from this software without specific prior written
--    permission.
-- 
-- THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
-- IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
-- ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
-- INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
-- SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
-- HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
-- STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
-- IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
-- POSSIBILITY OF SUCH DAMAGE.

function load_file(name)
	f,m = loadfile(name)
	if f then
		return f
	else
		error(m)
	end
end

function load_raw(name)
	local rawcfg = {}
	rawset(config, '_config', rawcfg)
	setfenv(load_file(name), rawcfg)()
end

function config_path(name)
	return "Configs/"..name..".txt"
end

function load_config(name)
	local cfg = {}
	setfenv(load_file(config_path(name)), cfg)()
	for key, value in pairs(cfg) do
		config[key] = value
	end
end

function write_config(name)
	local f = file.open(name, 'wb')
	for key, value in pairs(config._config) do
		f:write(key.." = "..tostring(value).."\n")
	end
	f:close()
end

setmetatable(config, {
	__index = function(t, key)
		local value = rawget(t, '_config')[key]
		if value ~= nil then
			return value
		end
	end,
	__newindex = function(t, key, value)
		if rawget(t, '_config')[key] ~= nil then
			rawset(rawget(t, '_config'), key, value)
		end
	end
})

