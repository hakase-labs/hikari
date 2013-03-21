//
// Global utility belt, "underscore"!
//
_ <- { };

///
/// Fill in a given object with default properties.
///
/// @param obj the object to apply defaults to
/// @param defaults an object that supplies default keys and values
///
_.defaults <- function(obj, defaults) {
    foreach(key, val in defaults) {
        if(!(key in obj) || obj[key] == null) {
            obj[key] <- defaults[key];
        }
    }

    return obj;
};

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

//
// Logs a message to the console
//
function log(message) {
    ::hikari.internal.log(message);
}

::print("Scripting environment is ready.");