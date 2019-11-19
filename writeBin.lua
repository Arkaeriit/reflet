#!/usr/local/bin/lua

--utility to manualy write bytes on a file
--manualy enter each hex number one afterthe other

local inp = io.stdin:read()
local boolSmall = false --indique si on travaille sur les premier nibble d'un octes
local ret = 0
while inp ~= "" do
    local char = inp:sub(1,1):byte()
    local boolEffet = false
    if 48 <= char and char <= 57 then
        char = char-48
        boolEffet = true
    elseif 65 <= char and char <= 70 then
        char = char - 55
        boolEffet = true
    end
    if boolEffet then
        if boolSmall then
            ret = ret + char
            io.stdout:write(string.char(ret))
            boolSmall = false
        else
            ret = char * math.pow(2,4)
            boolSmall = true
        end
    end
    inp = io.stdin:read()
end

