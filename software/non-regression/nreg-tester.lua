#!/usr/bin/env lua

-------------------------------------- Utils -----------------------------------

-- Return each lines of a file as a line in a table.
function file_to_table(filename)
    local f=io.open(filename, "r")
    if not f then
        return {}
    end
    local ret = {}
    repeat
        local line = f:read("l")
        ret[#ret+1] = line
    until line == nil
    f:close()
    return ret
end

-- Handy extensions to the string library
function string.char_at(str, index)
    return str:sub(index, index)
end
function string.is_whitespace(char)
    return char == ' ' or char == '\n' or char == '\r' or char == '\t'
end

----------------------------------- Formatting ---------------------------------

-- Pre-format a command in a table of the form {command, argument}.
function preformat_command(cmd)
    local first_space = 0
    for i=1,#cmd do
        if cmd:char_at(i):is_whitespace() then
            first_space = i
            break
        end
    end
    if first_space == 0 then
        return {cmd}
    end
    local command = cmd:sub(1,first_space-1)
    local second_non_space = 0
    for i=first_space,#cmd do
        if not cmd:char_at(i):is_whitespace() then
            second_non_space = i
            break
        end
    end
    if second_non_space == 0 then
        return {command}
    end
    local argument = cmd:sub(second_non_space, #cmd)
    return {command, argument}
end

-- Remove null bytes from a string
function no_null(line)
    local ret = ""
    for i=1,#line do
        if line:char_at(i) ~= "\0" then
            ret = ret..line:char_at(i)
        end
    end
    return ret
end

-- Reads a line that should be a debug instruction. If it is, return the value
-- of the working register. If it is not, return nil.
function read_debug(_line)
    local line = no_null(_line)
    local dbg_match = line:match("Debug instruction reached at address %x+. [a-zA-Z0-9 ]+ 0x%x+")
    if dbg_match == line then
        return line:match("0x%x+")
    else
        return nil
    end
end

-- Return true if the line is a misaligned message and false otherwise.
function read_mis(line)
    local match = line:match("Missaligned access at addr 0x%x+. Triying to acces %d+ bytes from address 0x%x+.")
    return match == line
end

--------------------------------- Executing tests ------------------------------

-- Reads a table of line of a non regression test. Ignore the optional shebang.
-- Then, executes the command in the first line.
-- Return a handle to that command and a list of all instructions in the test.
function prepare_run(tab)
    local index = 1
    if tab[1]:sub(1,2) == "#!" then
        index = 2
    end
    local handle = io.popen(tab[index])
    index = index+1
    local ret = {}
    for i=index,#tab do
        if tab[i] ~= "" then
            if tab[i]:char_at(1) ~= '#' then
                ret[#ret+1] = tab[i]
            end
        end
    end
    return handle, ret
end

-- Compares a command to an output of the simulator. Return true if they match
-- and false if they don't.
function run_test_line(_command, sim_line)
    local command = preformat_command(_command)
    if command[1] == "raw" then
        if #command ~= 2 then
            print("Error, command `raw` expects an argument!")
            return false
        end
        if command[2] == sim_line then
            return true
        else
            print("Error, mismatch for in `raw` command!")
            return false
        end
    elseif command[1] == "mis" then
        if read_mis(sim_line) then
            return true
        else
            print("Error, expected a misaligned access message!")
            return false
        end
    elseif command[1] == "dbg" then
        if #command ~= 2 then
            print("Error, command `dbg` expects an argument!")
            return false
        end
        local wr = read_debug(sim_line)
        if wr then
            if wr == command[2] then
                return true
            else
                print(string.format("Error, command `dbg` expected %s as working register value but got %s!", command[2], wr))
                return false
            end
        else
            print(string.format("Error, expected debug instruction!"))
            return false
        end
    elseif command[1] == "dbg*" then
        return read_debug(sim_line) ~= nil
    elseif command[1] == "#" then
        return true
    else
        print(string.format("Error, `%s` is not a valid line of command!", _command))
        return false
    end
end

-- Runs a complete test. Returns true on success and false on failure.
function run_test(filename)
    local tab = file_to_table(filename)
    if #tab < 2 then
        print("Error, test file too short!")
        return false
    end
    local handle, test_lines = prepare_run(tab)
    local i = 1
    while true do
        local test_line = test_lines[i]
        local sim_line = handle:read("l")
        if test_line == nil and sim_line == nil then
            return true
        elseif test_line == nil then
            print(string.format("Error, simulator outputed line '%s' while test file ended!", sim_line))
            return false
        elseif sim_line == nil then
            print(string.format("Error, test file contains line '%s' at command index %i while simulator ended!", test_line, i))
            return false
        else
            if run_test_line(test_line, sim_line) then
                i = i+1
            else
                print(string.format("Error at line '%s' at command %i of the test file. Simulator outputed '%s'.", test_line, i, sim_line))
                return false
            end
        end
    end
end

---------------------------------- Unit testing --------------------------------

function test_preformat()
    function test(input, output)
        local sample = preformat_command(input)
        if #sample ~= #output then
            print(string.format("Error pre-formatting %s. Expected %i elements but got %i.", input, #output, #sample))
            return false
        end
        for i=1,#sample do
            if sample[i] ~= output[i] then
                print(string.format("Error element %i of input %s does not match. Expected %s but got %s.", i, input, output[i], sample[i]))
                return false
            end
        end
        return true
    end

    if not test("aa", {"aa"}) then return false end
    if not test("aa bb", {"aa", "bb"}) then return false end
    if not test("aa bb ", {"aa", "bb "}) then return false end
    if not test("aa  bb ", {"aa", "bb "}) then return false end
    return true
end

-- Runs all unit test and exit depending on success or failure
function run_unit_tests()
    if not test_preformat() then
        print("Failed test_preformat")
        os.exit(1)
    else
        print("Succeed test_preformat")
    end
end

-------------------------------------- main ------------------------------------

if arg[1] then
    if run_test(arg[1]) then
        os.exit(0)
    else
        os.exit(1)
    end
else
    print("No arguments, running unit tests.")
    run_unit_tests()
end

