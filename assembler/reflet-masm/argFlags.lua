--[[ This file contain the functions used to convert the command
--line arguments into a list of parameters usable by the main function
The table resuming the arguments will contain a boolean field 'set_prefix'
, a intger field 'start_addr', a string field 'output' containing the
path of the output file, 'input' wich is a list of all the input files,
a boolean "set_stack", "stack_value" which can be either nul or an
integer, "error", "error_file" and "help" which are two booleans.
]]

--generate a list of the flags in its default configuration
local defaults = function()
    local ret = {
        set_prefix = true,
        start_addr = 0,
        output = "",
        input = {},
        set_stack = true,
        stack_value = nil,
        error = false,
        error_file = false,
        help = false
    }
    return ret
end

--print a the content of the flag struct
local debugPrintFlags = function(flags)
    for k,v in pairs(flags) do
        print(k.." : "..tostring(v))
        if(k == "input") then
            for i=1,#flags.input do
                print("    "..tostring(i).." : "..flags.input[i])
            end
        end
    end
end

--return if the string is a flag and if so, returns its name
local readFlag = function(str)
    local ret = str:chat(1) == '-'
    if ret then
        return ret, str:sub(2, #str)
    else
        return ret, nil
    end
end

--run along all the arguments and updates the table acordingle
local readArgs = function(arg, tab)
    local ptn = 1
    while ptn <= #arg do
        local isFlag, flag = readFlag(arg[ptn])
        if isFlag then
            if flag == "help" or flag == "h" then
                tab.help = true
            elseif flag == "no-prefix" then
                tab.set_prefix = false
            elseif flag == "no-stack-init" then
                tab.set_stack = false
            elseif flag == "set-stack-to" then
                if math.tointeger(arg[ptn+1]) then
                    tab.stack_value = math.tointeger(arg[ptn+1])
                    ptn = ptn + 1
                else
                    tab.error = true
                end
            elseif flag == "o" then
                tab.output = arg[ptn+1]
                ptn = ptn + 1
            elseif flag == "start-addr" then
                if math.tointeger(arg[ptn+1]) then
                    tab.start_addr = math.tointeger(arg[ptn+1])
                    ptn = ptn + 1
                else
                    tab.error = true
                end
            else --unknow flag
                io.stderr:write("Error, unknow flag: ",flag, "\n")
                tab.error = true
            end
        else
            
            tab.input[#tab.input+1] = arg[ptn]
        end
        ptn = ptn + 1
    end
end

--check that there is no inconsistency il the flag givens
local testFlags = function(flags)
    if not flags.help and 
    (((not flags.set_stack) and flags.stack_value ~= nil) or
    (#flags.input == 0) or 
    (flags.output == "")) then
        flags.error = true
    end
end

--Check that we can open all the input files
local testFiles = function(flags)
    for i=1,#flags.input do
        local filename = flags.input[i]
        local f = io.open(filename, "r")
        if f then
            f:close()
        else
            io.stderr:write("Error, unable to open the file: ", filename, "\n")
            flags.error_file = true
        end
    end
end

--Run the args and test for errors
runArgs = function(arg)
    local ret = defaults()
    debugPrintFlags(ret)
    print()
    readArgs(arg, ret)
    testFlags(ret)
    testFiles(ret)
    debugPrintFlags(ret)
    return ret
end

