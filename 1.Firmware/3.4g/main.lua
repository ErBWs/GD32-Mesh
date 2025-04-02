PROJECT = "mqtt"
VERSION = "1.0.0"

_G.sys = require("sys")
_G.sysplus = require("sysplus")

-- Disable wake up key for module using EC618 firmware.
if rtos.bsp() == "EC618" and pm and pm.PWK_MODE then
    pm.power(pm.PWK_MODE, false)
end

require "single_mqtt"

-- There should be no code after this line.
sys.run()
