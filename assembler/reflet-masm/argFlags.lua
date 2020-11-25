--[[ This file contain the functions used to convert the command
--line arguments into a list of parameters usable by the main function
The table resuming the arguments will contain a boolean field 'set_prefix'
, a intger field 'start_addr', a string field 'output' containing the
path of the output file, 'input' wich is a list of all the input files,
a boolean "set_stack", "stack_value" which can be either nul or an
integer, "error", "error_file" and "help" which are two booleans.
]]

--return if the string is a flag and if so, returns its name
local readFlag = function(str)
    local ret = str:chat(1) == '-'
    if ret then
        return ret, str:sub(2, #str)
    else
        return ret, nil
    end
end

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

    --print a the content of the flag struct
    ret.debugPrintFlags = function(flags)
        for k,v in pairs(flags) do
            print(k.." : "..tostring(v))
            if(k == "input") then
                for i=1,#flags.input do
                    print("    "..tostring(i).." : "..flags.input[i])
                end
            end
        end
    end

    --run along all the arguments and updates the table acordingle
    ret.readArgs = function(flags, arg)
        local ptn = 1
        while ptn <= #arg do
            local isFlag, flag = readFlag(arg[ptn])
            if isFlag then
                if flag == "help" or flag == "h" then
                    flags.help = true
                elseif flag == "no-prefix" then
                    flags.set_prefix = false
                elseif flag == "no-stack-init" then
                    flags.set_stack = false
                elseif flag == "set-stack-to" then
                    if math.tointeger(arg[ptn+1]) then
                        flags.stack_value = math.tointeger(arg[ptn+1])
                        ptn = ptn + 1
                    else
                        flags.error = true
                    end
                elseif flag == "o" then
                    flags.output = arg[ptn+1]
                    ptn = ptn + 1
                elseif flag == "start-addr" then
                    if math.tointeger(arg[ptn+1]) then
                        flags.start_addr = math.tointeger(arg[ptn+1])
                        ptn = ptn + 1
                    else
                        flags.error = true
                    end
                else --unknow flag
                    io.stderr:write("Error, unknow flag: ",flag, "\n")
                    flags.error = true
                end
            else
                flags.input[#flags.input+1] = arg[ptn]
            end
            ptn = ptn + 1
        end
    end

    --check that there is no inconsistency il the flag givens
    ret.testFlags = function(flags)
        if not flags.help and 
        (((not flags.set_stack) and flags.stack_value ~= nil) or
        (#flags.input == 0) or 
        (flags.output == "")) then
            flags.error = true
        end
    end

    --Check that we can open all the input files
    ret.testFiles = function(flags)
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

    return ret
end

--Run the args and test for errors
runArgs = function(arg)
    local ret = defaults()
    ret:debugPrintFlags()
    print()
    ret:readArgs(arg)
    ret:testFlags()
    ret:testFiles()
    ret:debugPrintFlags()
    return ret
end

