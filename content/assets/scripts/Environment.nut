::print("Scripting environment is ready.");

//
// Table which keeps track of scripts loaded via require(). The keys in this 
// table are the file names, values are the compiled script.
//
::hikari.internal._loaded <- { };

function require(fileName, reload = false) {
    local _loaded = ::hikari.internal._loaded;
    local _read = ::hikari.internal.readFileAsString;
    local compiledScript = null;

    if(!reload && fileName in _loaded) {
        compiledScript = _loaded[fileName];
    } else {
        compiledScript = ::compilestring(_read(fileName), fileName);

        _loaded[fileName] <- compiledScript;

        compiledScript();
    }
}

require("assets/scripts/TestRequire.nut");
require("assets/scripts/TestRequire.nut");
require("assets/scripts/TestRequire.nut");