print("TOP OF ENVIRONMENT");
//
// Global utility belt, "underscore"!
//
_ <- { };

print("CREATED _");
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

print("_.defaults");
print(hikari);

if(::hikari.internal) {
    print("global hikari.internal exists");
}
//
// Table which keeps track of scripts loaded via require(). The keys in this
// table are the file names, values are the compiled script.
//
::hikari.internal._loaded <- { };
print("::hikari.internal._loaded <- { };");
function require(fileName, reload = false) {
    local _loaded = ::hikari.internal._loaded;
    local _read = ::hikari.internal.readFileAsString;
    local compiledScript = null;

    if(!reload && fileName in _loaded) {
        compiledScript = _loaded[fileName];
    } else {
        compiledScript = ::compilestring(_read(fileName), fileName);

        _loaded[fileName] <- compiledScript;

        return compiledScript();
    }
}
print("require");
//
// Logs a message to the console
//
function log(message) {
    ::hikari.internal.log(message);
}
print("log");

log("Scripting environment is ready.");