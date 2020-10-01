----- macro creators -----

-- define the set+ macro
-- If the code can not be made, return nil
local setp = function(value, wordsize)
    local str = setValue(value, wordsize)
    if str == nil then
        return nil 
    end
    return createElem(str, setsize(wordsize), INST_MACRO)
end

----- interface function -----

expandMacro = function(str, wordsize)
    local tabInst = str:splitLine()
    local mnemonic = tabInst[1]:lower()
    if mnemonic == "set+" then
        return setp(math.tointeger(tabInst[2]), wordsize)
    end
end

