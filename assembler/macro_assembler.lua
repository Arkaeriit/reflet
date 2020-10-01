--[[
This file contain the code for the macro_assembler. The assembly is in
three steps.

1. basic assembly
The input file is read and each instructions is analysed. Basic
instructions are keps as-is, macro instructions are processed and jump-related
instructions are noted to be processed.
To do so, a list is generated, each ellement correspond to an instruction.
Elements of the list have 3 fields: one telling what kind of instruction they
are, one contain the equivalent micro-instruction code and the last one
containes their compiled size in bytes.

2. linking
The micro-assembly code of linking instructions are completed. We now
have a comple micro-insttuction code.

3. compiling
The micro-assembly code is written in a tmp file and the micro-assembler
is used to create a binairy file
]]

----- main steps -----

local basicASM = function(filename, wordsize)
    local f = io.open(filename, "r")
    local line = 1
    local ret = {}
    local str = f:read()
    while str do
        --print(line, str)
        local type = analyzeLine(str)
        --print(type)
        if type == INST_ERR then
            io.stderr:write("Error line ",line,"\n")
            return
        elseif type == INST_BAS then
            ret[#ret+1] = createElem(str..'\n', 1, INST_BAS)
        elseif type == INST_MACRO then
            ret[#ret+1] = expandMacro(str, wordsize)
        elseif type ~= NO_INST then
            print("Un truc cloche")
        end
        str = f:read()
        line = line + 1
    end
    f:close()
    return ret
end

local linking = function(tab)
    --todo
end

local compiling = function(tab)
    --todo
end

----- function usable outside of the fine -----

--create an ellement that will be put in the code list
createElem = function(str, size, type)
    local ret = {content = str, size = size, type = type}
    return ret
end

macro_assembler = function()
    local help = function()
        print("asrmmasm : the asrm macro-assembler.")
        print("Usage : asrmmasm <assembly file> <output file>")
    end
    if #arg == 1 and (arg[1] == "help" or arg[1] == "--help" or arg[1] == "-h") then --asking for help
        help()
        return 0
    elseif #arg == 2 then --normal usage
        local f = io.open(arg[1])
        if not f then --checking for OK file
            io.stderr:write("Error : unable to open ",arg[1], "\n")
            return 3
        end
        local frstLine = f:read():splitLine() --checking for word size
        local wordsize = 2
        if #frstLine == 2 and math.tointeger(frstLine[2]) and tonumber(frstLine[2]) > 0 and frstLine[1] == "wordsize" then
            wordsize = math.tointeger(frstLine[2])
            if wordsize ~= 1 or wordsize ~= 2 or wordsize ~= 4 or wordsize ~= 8 then
                io.stderr:write("Warning: non standart word size.\n")
            end
        else
            io.stderr:write("Warning: no word size specified, used 2 bytes as default.\n")
        end
        f:close()
        --assembling
        local tab = basicASM(arg[1], wordsize)
        linking(tab)
        compiling(tab)
    else --error
        io.stderr:write("Error: invalid arguments.\n\n")
        help()
        return 1
    end
end

