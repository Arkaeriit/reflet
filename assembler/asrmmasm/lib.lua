----- constants -----

NO_INST = 0
INST_ERR = 1
INST_UKN = 2
INST_BAS = 3
INST_LINK = 4
INST_LABEL = 5
INST_MACRO = 6

RET_OK = 0
RET_INVALID_ARGS = 1
RET_UNOPEN_FILE = 3
RET_ERROR_COMPILATION = 5
RET_ERROR_LINK = 6

----- auxyliary functions -----

--idexes a single char from a string
string.chat = function(str, index)
    return str:sub(index, index)
end

--log2
math.log2 = function(num)
    return math.log(num)/math.log(2)
end

--Takes a string and put each word before a ; in a separate place in a table
string.splitLine = function(str)
    local ret = {}
    local strpnt = 1
    local inword = false
    while strpnt <= #str and  str:chat(strpnt) ~= ';' and str:chat(strpnt) ~= '\n' do
        local ch = str:chat(strpnt)
        if inword then
            if ch == ' ' or  ch == '	' then
                inword = false
            else
                ret[#ret] = ret[#ret]..ch
            end
        else
            if ch ~= ' ' and ch ~= '	' then
                inword = true
                ret[#ret+1] = ch
            end
        end
        strpnt = strpnt + 1
    end
    return ret
end

setsize = function(wordsize)
    return wordsize * 8 + 2
end

--Return the code to set a value in the vorking register
--also return a boolean telling if we try to set too big of a number
setValue = function(value, wordsize)
    if value > 2^(wordsize*8) then
        return nil
    end
    local sizeInNimbles = math.ceil(math.ceil(math.log2(value))/4)
    local str = "set 4\ncpy R12\nset 0\ncpy R11\n"
    for i=1,sizeInNimbles do
        local currentNibble = (value >> (4 * (sizeInNimbles -i))) --the currrent nibble, starting at the end
        currentNibble = currentNibble & 15
        str = str.."set "..tostring(currentNibble).."\nor R11\n"
        if i ~= sizeInNimbles then 
            str = str.."lsl R12\ncpy R11\n"
        end
    end
    return str
end

