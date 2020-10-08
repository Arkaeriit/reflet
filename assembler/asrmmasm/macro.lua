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

-- define the mov macro
local mov = function(tabInst)
    local str = "push\nread "..tabInst[3].."\ncpy "..tabInst[2].."\npop\n"
    return createElem(str, 6, INST_MACRO)
end

-- define the setr macro
local setr = function(tabInst, wordsize)
    local value = math.tointeger(tabInst[3])
    local sV = setValue(value, wordsize)
    if sV == nil then
        return nil
    end
    local str = "push\n"..sV.."cpy "..tabInst[2].."\npop\n"
    return createElem(str, setsize(wordsize) + 3, INST_MACRO)
end

----- interface function -----

expandMacro = function(str, wordsize)
    local tabInst = str:splitLine()
    local mnemonic = tabInst[1]:lower()
    if mnemonic == "set+" then
        return setp(math.tointeger(tabInst[2]), wordsize)
    end
end

