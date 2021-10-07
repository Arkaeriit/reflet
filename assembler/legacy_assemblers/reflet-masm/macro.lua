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
    return createElem(str, 4, INST_MACRO)
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

--definite the rawbytes macro
local rawbytes = function(tabInst)
    local str = ""
    for i=2,#tabInst do
        if not math.tointeger(tabInst[i]) or math.tointeger(tabInst[i]) > 255 then
            io.stderr:write(tabInst[i], " is not a valid byte\n")
            return nil
        end
        str = str.."rawbyte "..tabInst[i].."\n"
    end
    return createElem(str, #tabInst-1, INST_MACRO)
end

--define the data macro
local data = function(str)
    local startStr = 0;
    local endStr = 0;
    for i=1,#str do
        if str:sub(i,i) == '"' then
            if startStr == 0 then
                startStr = i
            else
                if endStr == 0 then
                    endStr = i
                else
                    io.stderr:write("Error, malformed string in macro data\n")
                    return nil
                end
            end
        end
    end
    if endStr == 0 then
        io.stderr:write("Error, malformed string in macro data\n")
        return nil
    end
    local ret = ""
    for i=startStr+1,endStr-1 do
        local char = str:byte(i)
        ret = ret.."rawbyte "..tostring(char).."\n"
    end
    return createElem(ret, endStr-startStr-1, INST_MACRO)
end

--define the pushr macro
local pushr = function(tabInst)
    local reg = tabInst[2]
    local str = "read "..reg.."\npush\n"
    return createElem(str, 2, INST_MACRO)
end

--define the popr macro
local popr = function(tabInst)
    local reg = tabInst[2]
    local str = "pop\ncpy "..reg.."\n"
    return createElem(str, 2, INST_MACRO)
end


----- interface function -----

expandMacro = function(str, wordsize)
    local tabInst = str:splitLine()
    local mnemonic = tabInst[1]:lower()
    if mnemonic == "set+" then
        return setp(math.tointeger(tabInst[2]), wordsize)
    elseif mnemonic == "mov" then
        return mov(tabInst)
    elseif mnemonic == "setr" then
        return setr(tabInst, wordsize)
    elseif mnemonic == "rawbytes" then
        return rawbytes(tabInst)
    elseif mnemonic == "data" then
        return data(str)
    elseif mnemonic == "pushr" then
        return pushr(tabInst)
    elseif mnemonic == "popr" then
        return popr(tabInst)
    end
end

