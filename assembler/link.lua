----- size calculator ----

linkSize = function(str, wordsize)
    local mnemonic = str:splitLine()[1]:lower()
    if mnemonic == "setlab" then
        return setsize(wordsize)
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
    else
        print("Un truc cloche")
    end
end
    
