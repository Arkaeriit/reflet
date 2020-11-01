--[[
This file contain the code for the macro_assembler. The assembly is in
three steps.

1. reading
The input file is read and each line is put in a list. Furthermore,
a little runtime is put at the end and at the start of the list.

2. basic assembly
The input file list is read and each instructions is analysed. Basic
instructions are keps as-is, macro instructions are processed and jump-related
instructions are noted to be processed.
To do so, a list is generated, each ellement correspond to an instruction.
Elements of the list have 3 fields: one telling what kind of instruction they
are, one contain the equivalent micro-instruction code and the last one
containes their compiled size in bytes.

3. linking
The micro-assembly code of linking instructions are completed. We now
have a comple micro-insttuction code. In a first time, the linker get the
addresses of each label, the it write the addresses of label where they
need to be.

4. compiling
The micro-assembly code is written in a tmp file and the micro-assembler
is used to create a binairy file

Unlike in most other file in this project, here wordsize is in bytes.
]]

----- main steps -----

local reading = function(filename, wordsize)
    local f = io.open(filename, "r")
    local ret = {}
    --Setting the stack pointer and jumping to start
    ret[1] = "setlab ##@@stack@@##"
    ret[2] = "cpy SP"
    ret[3] = "setlab start"
    ret[4] = "jmp" 
    --The content of the file
    local str = f:read()
    while str do 
        ret[#ret+1] = str
        str = f:read()
    end
    f:close()
    --The stack is iitialised at the end of the code
    local firstFree = 4 + #ret --A 4 word majic word starts the binary programm
    while firstFree%wordsize ~= 0 do
        ret[#ret+1] = "slp"
        firstFree = 4 + #ret
    end
    ret[#ret+1] = "label ##@@stack@@##"
    return ret
end

local basicASM = function(filetab, wordsize)
    local ret = {}
    for line=1,#filetab do
        str = filetab[line]
        --print(line, str)
        local type = analyzeLine(str)
        --print(type, str)
        if type == INST_ERR then
            io.stderr:write("Syntax error line ",line - 4,"\n") -- -4 is not to bither the user with the 4 line runtime
            return nil,true
        elseif type == INST_BAS then
            ret[#ret+1] = createElem(str..'\n', 1, INST_BAS)
        elseif type == INST_MACRO then
            local exM = expandMacro(str, wordsize)
            if exM then
                ret[#ret+1] = exM
            else
                io.stderr:write("Error line ",line - 4,", macro can not be expanded.\n")
                return nil,true
            end
        elseif type == INST_LABEL then
            ret[#ret+1] = createElem(str, labelSize(str, wordsize), INST_LABEL)
        elseif type == INST_LINK then
            ret[#ret+1] = createElem(str, linkSize(str, wordsize), INST_LINK)
        elseif type ~= NO_INST then
            print("Un truc cloche")
        end
    end
    return ret,false
end

local linking = function(tab, wordsize)
    local labelMap = {}
    local currentPointer = 4 --the first instruction is at addr 4
    --getting label positions
    for i=1,#tab do
        if tab[i].type == INST_LABEL then
            local label = tab[i].content:splitLine()[2]
            if labelMap[label] then
                io.stderr:write("Error: label ",label," is set twice.\n")
                return true
            else
                labelMap[label] = currentPointer
                --print(label, currentPointer)
            end
            tab[i].content = ";"..tab[i].content..";\n" --usefull for debug but that's it
        end
        currentPointer = currentPointer + tab[i].size
    end
    --checking binary size
    if currentPointer >= 2^(wordsize*8) then
        io.stderr:write("Error: binary output too large (",currentPointer,"/",2^(wordsize*8),"). Consider extending wordsize.\n")
        return true
    end
    --placing label calls
    for i=1,#tab do
        if tab[i].type == INST_LINK then
            local label = tab[i].content:splitLine()[2]
            local mnemonic = tab[i].content:splitLine()[1]
            if labelMap[label] then
                local addr = labelMap[label]
                tab[i].content = setLabel(mnemonic, addr, wordsize)
            else
                io.stderr:write("Error: label ",label," is called but not set.\n")
                return true
            end
        end
    end
    return false
end

local compiling = function(tab, outFile)
    math.randomseed(os.time())
    local tmpFile = "/tmp/asrmasm"..tostring(math.random(10000))
    local f = io.open(tmpFile,"w")
    for i=1,#tab do
        f:write(tab[i].content)
    end
    f:close()
    assembleFile(tmpFile, outFile)
end

----- function usable outside of the fine -----

--create an ellement that will be put in the code list
createElem = function(str, size, type)
    local ret = {content = str, size = size, type = type}
    return ret
end

macro_assembler = function(arg)
    local help = function()
        print("asrmmasm : the asrm macro-assembler.")
        print("Usage : asrmmasm <assembly file> <output file>")
    end
    if #arg == 1 and (arg[1] == "help" or arg[1] == "--help" or arg[1] == "-h") then --asking for help
        help()
        return RET_OK
    elseif #arg == 2 then --normal usage
        local f = io.open(arg[1])
        if not f then --checking for OK file
            io.stderr:write("Error: unable to open ",arg[1], "\n")
            return RET_UNOPEN_FILE
        end
        local frstLine = f:read():splitLine() --checking for word size
        local wordsize = 2
        if #frstLine == 2 and math.tointeger(frstLine[2]) and tonumber(frstLine[2]) > 0 and frstLine[1] == "wordsize" and math.tointeger(math.tointeger(frstLine[2]/8)) then
            wordsize = math.tointeger(frstLine[2])/8 --wordsize in cunted in byte in all the files here but enterd in bits in the source file
            if wordsize ~= 1 and wordsize ~= 2 and wordsize ~= 4 and wordsize ~= 8 then
                io.stderr:write("Warning: non standart word size.\n")
            end
        else
            io.stderr:write("Warning: no word size specified, used 2 bytes as default.\n")
        end
        f:close()
        --assembling
        local filetab = reading(arg[1], wordsize)
        local tab,err = basicASM(filetab, wordsize)
        if err then
            return RET_ERROR_COMPILATION
        end
        err = linking(tab, wordsize)
        if err then
            return RET_ERROR_LINK
        end
        compiling(tab, arg[2])
        return RET_OK
    else --error
        io.stderr:write("Error: invalid arguments.\n\n")
        help()
        return RET_INVALID_ARGS
    end
end

