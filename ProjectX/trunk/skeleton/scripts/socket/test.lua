
require("Scripts/socket");
package.loaded["socket"] = socket;

require("Scripts/http");
package.loaded["socket.http"] = socket.http;

--[[

local server = "http://www.google.com"

function parse(body)
    local lines = string.gfind(body, "(.-)\r\n")
    local status = lines()
    local code, message = socket.skip(2, string.find(status, "(%d%d%d) (.*)"))
    if tonumber(code) ~= 210 then
        return nil, code, message
    end
    local data = {}
    for l in lines do
        local c = string.sub(l, 1, 1)
        if c ~= '#' and c ~= '.' then
            local key, value = socket.skip(2, string.find(l, "(.-)=(.*)"))
            value = string.gsub(value, "\\n", "\n")
            value = string.gsub(value, "\\\\", "\\")
            value = string.gsub(value, "\\t", "\t")
            data[key] = value
        end
    end
    return data, code, message 
end

local host = socket.dns.gethostname()
local query = "%s?cmd=cddb+read+%s+%s&hello=LuaSocket+%s+LuaSocket+2.0&proto=6"
local url = string.format(query, server, arg[1], arg[2], host)
local body, headers, code = http.get(url)

if code == 200 then
    local data, code, error = parse(body)
    if not data then
        print(error or code)
    else
        for i,v in pairs(data) do
            io.write(i, ': ', v, '\n')
        end
    end
else print(error) en
]]

alert("here")
