----- macro creators -----

-- define the set+ macro
-- todo: check for the correct size of number
local setp = function(value, wordsize)
    local str = "set 4\ncpy R12\nset 0\ncpy R11\n"
    for i=1,wordsize*2 do
        local currentNibble = (value >> (4 * (wordsize*2 -i))) --the currrent nibble, starting at the ed
        currentNibble = currentNibble & 15
        str = str.."set "..tostring(currentNibble).."\nor R11\n"
        if i ~= wordsize*2 then 
            str = str.."lsl R12\n"
        end
    end
    return createElem(str, wordsize * 6 + 3, INST_MACRO)
end

----- interface function -----

expandMacro = function(str, wordsize)
    local tabInst = str:splitLine()
    local mnemonic = tabInst[1]:lower()
    if mnemonic == "set+" then
        return setp(math.tointeger(tabInst[2]), wordsize)
    end
end

