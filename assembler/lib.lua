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
    return wordsize * 6 + 3
end

