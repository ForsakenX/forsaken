
local http = require("socket.http")

function games( url )
	local list = {}
	local url = "http://fly.thruhere.net/status/games.json"
	local body, code, headers = http.request(url)
	if code ~= 200 then return list end
	body = "return " .. body
	local code = assert(loadstring(body))
	if not code then return list end
	list = code()
	return list
end

