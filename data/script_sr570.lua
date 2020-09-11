package.cpath = package.cpath .. ";modules/lib/lib\\dyn/?.so"
package.cpath = package.cpath .. ";modules/lib/lib\\dyn/?.dll"

sr570 = require("sr570")

sr570:help()

sr_object = sr570:connectNewDevice("/dev/ttyUSB0");

--sr_object:reset()

sr_object:setSens(1e-3) --OK
sr_object:setSenCal(0) --OK
sr_object:setSigInvertSense(1) --OK
sr_object:setUncalSensVernier(0) --???

sr_object:setCalOffsetCurrentLVL(2e-12) --OK
sr_object:setUncalInOffsetVernier(0) --???
sr_object:setInOffsetCalMode(0) --OK
sr_object:turnInOffsetCurr(0) -- OK
sr_object:setInOffsetCurrSign(1) --OK

sr_object:setTypeFilter(2) --OK
sr_object:setHighFilter(0.3) --OK
sr_object:setLowFilter(10*1000) --OK

sr_object:turnBiasVolt(0) --OK
sr_object:setBiasVoltLVL(0) -- OK
sr_object:setGainMode(0) --OK