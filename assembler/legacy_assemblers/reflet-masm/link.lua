----- size calculator ----

linkSize = function(str, wordsize)
    local mnemonic = str:splitLine()[1]:lower()
    if mnemonic == "setlab" then
        return setsize(wordsize)
    elseif mnemonic == "callf" then
        return setsize(wordsize) + 1
    elseif mnemonic == "goto" then
        return setsize(wordsize) + 1
    else
        print("Un truc cloche")
    end
end

labelSize = function(str, wordsize)
    local mnemonic = str:splitLine()[1]:lower()
    if mnemonic == "label" then
        return 0
    else
        print("Un truc cloche")
    end
end

----- label set -----

setLabel = function(mnemonic, addr, wordsize)
    if mnemonic == "setlab" then
        return setValue(addr, wordsize)
    elseif mnemonic == "callf" then
        return setValue(addr, wordsize).."call\n"
    elseif mnemonic == "goto" then
        return setValue(addr, wordsize).."jmp\n"
    else
        print("Un truc cloche")
    end
end
    
