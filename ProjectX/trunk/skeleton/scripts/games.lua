
local base 	= _G
local table 	= require("table")
local http 	= require("socket.http")
local assert	= assert
local pcall	= pcall
local loadstring	= loadstring

module("games")

local url = "http://fly.thruhere.net/status/games.json"

function get( url )
	local list = {}
	local body, code, headers = http.request(url)
	if code ~= 200 then return list end
	body = "return " .. body
	local rv,f = pcall(loadstring, body)
	if not (rv and f) then return list end
	list = f()
	return list
end

-----------------------------------------------------------------------------
-- C Helpers
----------------------------------------------------------------------------

local list = {}
local last_time = 0
local refresh_time = 1 -- seconds

function update()
	list = get( url )
end

function length()
	return table.maxn(list)
end

function name_at( index )
	return list[index].nick
end

function ip_at( index )
	return list[index].ip
end

function port_at( index )
	return list[index].port
end


