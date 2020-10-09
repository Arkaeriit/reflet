----- iternaly used funtions -----

--check if a number is an integer between 0 and 15
local validNum = function(num)
    return math.tointeger(num) and tonumber(num) >= 0 and tonumber(num) <= 15
end

--check if a string is decribing a register
local isreg = function(reg)
    if reg == "WR" or reg == "SR" or reg == "PC3" or reg == "SP" then
        return true
    else
        return (reg:chat(1) == 'R' or reg:chat(1) == 'r') and validNum(reg:sub(2,#reg))
    end
end

-- check if the instruction in a tabline is a basic instruction or a macro one
-- return INST_ERR in case of an error, INST_BAS in case it is a basic instruction and INST_UKN otherwise
local isBasic = function(tabInst)
    mnemonic = tabInst[1]:lower()
    if mnemonic == "slp" or mnemonic == "jif" or mnemonic == "jmp" or mnemonic == "pop" or mnemonic == "push" or mnemonic == "ret" or mnemonic == "call" or mnemonic == "quit" then
        if #tabInst == 1 then
            return INST_BAS
        else
            return INST_ERR
        end
    end
    if mnemonic == "set" then
        if #tabInst == 2 and validNum(tabInst[2]) then
            return INST_BAS
        else
            return INST_ERR
        end
    end
    if mnemonic == "read" or mnemonic == "cpy" or mnemonic == "add" or mnemonic == "sub" or mnemonic == "and" or mnemonic == "or" or mnemonic == "xor" or mnemonic == "not" or mnemonic == "lsl" or mnemonic == "lsr" or mnemonic == "eq" or mnemonic == "les" or mnemonic == "str" or mnemonic == "load" then
        if #tabInst == 2 and isreg(tabInst[2]) then
            return INST_BAS
        else
            return INST_ERR
        end
    end
    return INST_UKN
end

-- check if the instruction in a tabline is a basic instruction or a macro one
-- return INST_ERR in case of an error, INST_MACRO in case it is a macro instruction and INST_LINK if it is a label. Return INST_UKN otherwize
local isMacro = function(tabInst)
    mnemonic = tabInst[1]:lower()
    if mnemonic == "set+" then
        if #tabInst == 2 and math.tointeger(tabInst[2]) and tonumber(tabInst[2]) >= 0 then
            return INST_MACRO
        else
            return INST_ERR
        end
    elseif mnemonic == "mov" then
        if #tabInst == 3 and isreg(tabInst[2]) and isreg(tabInst[3]) then
            return INST_MACRO
        else
            return INST_ERR
        end
    elseif mnemonic == "setr" then
        if #tabInst == 3 and isreg(tabInst[2]) and math.tointeger(tabInst[3]) and tonumber(tabInst[3]) >= 0 then
            return INST_MACRO
        else
            return INST_ERR
        end
    elseif mnemonic == "label" then
        if #tabInst == 2 then
            return INST_LABEL
        else
            return INST_ERR
        end
    elseif mnemonic == "setlab" or mnemonic == "callf" or mnemonic == "goto" then
        if #tabInst == 2 then
            return INST_LINK
        else
            return INST_ERR
        end
    elseif mnemonic == "wordsize" then --not a real macro but will be treated as such by analyzeLine
        return NO_INST
    end
    return INST_UKN
end

----- interface function -----

-- take a line and tell what kind of instruction it is
analyzeLine = function(line)
    local tabInst = line:splitLine()
    --print(tabInst, #tabInst, tabInst[1] and #tabInst[1], "!"..tostring(tabInst[1]).."!",validNum)
    if #tabInst == 0 then
        return NO_INST
    end
    if isBasic(tabInst) == INST_ERR then
        return INST_ERR
    elseif isBasic(tabInst) == INST_BAS then
        return INST_BAS
    end --Only INST_UKN left
    if isMacro(tabInst) == INST_ERR or isMacro(tabInst) == INST_UKN then
        return INST_ERR
    else
        return isMacro(tabInst)
    end
end

