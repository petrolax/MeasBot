package.cpath = package.cpath .. ";modules/lib/lib\\dyn/?.so"
package.cpath = package.cpath .. ";modules/lib/lib\\dyn/?.dll"

function tblprint(tbl, pref, postf)
  if pref == nil then pref="[" end
  if postf== nil then postf="]" end
  local mt = getmetatable(tbl)
  if mt ~= nil then
    tblprint(mt, pref..pref, postf..postf)
  end
  mt = tbl
  print("(", tbl, ")")
  for idx,val in pairs(mt) do
    print(pref, idx, postf, " = ", val)
  end
  if pref == "" or pref == nil then
    print("-------------------------------")
  end
end

com_settings = {}
pwr_supl = {}
vmeter = {}
ameter = {}

function com_settings:VisualTab(tab)
  self.ui = {}
  self.ui.tab = tab
  
  self.ui.pnum = tab:NewSpinEdit(100, 10, 1, 100000, 0)
  self.ui.pnum.step = 1
  self.ui.pnum.value = 10
  self.ui.pnum.label = tab:NewLabel(10, 10, "Points num")
end
function com_settings:connect()
  self.pnum = self.ui.pnum.value
  return ""
end

function pwr_supl:VisualTab(tab)
  self.ui={}
  self.ui.tab = tab
  self.ui.port = tab:NewEdit(100, 10, "/dev/ttyUSB0")
  self.ui.port.label = tab:NewLabel(10, 10, "SR830 port")
  
  self.ui.volt = tab:NewSpinEdit(100, 90, 0.004, 5, 3)
  self.ui.volt.value = 0.8
  self.ui.volt.label = tab:NewLabel(0, 90, "Voltage (V)")
  
  self.ui.freq = tab:NewSpinEdit(100, 130, 1, 10000)
  self.ui.freq.value = 133
  self.ui.freq.label = tab:NewLabel(0, 130, "Frequency (Hz)")
end
function pwr_supl:connect()
  if self.srs == nil then
    if vmeter.srs ~= nil then 
      self.srs = vmeter.srs
    else
      self.srs = require("sr830"):connectNewDevice(self.ui.port.text)
    end
  end
  if self.srs == nil then return "SR830\n" end
  
  self.dev = self.srs.SineOut
  self.dev:setFreq(self.ui.freq.value)
  --self.dev:setVoltage(self.ui.volt.value)
  self.dev:setVoltage(0)
  self.Umax = self.ui.volt.value
  return ""
end
function pwr_supl:setVoltage(U)
  self.dev:setVoltage(U)
end


function vmeter:VisualTab(tab)
  self.ui={}
  self.ui.tab = tab
  self.ui.port = tab:NewEdit(100, 10, "/dev/ttyUSB0")
  self.ui.port.label = tab:NewLabel(0, 10, "SR830 port")
  
  self.ui.timeconst = tab:NewSpinEdit(100, 50, 0, 10, 6)
  self.ui.timeconst.value = 100e-3
  self.ui.timeconst.label = tab:NewLabel(0, 50, "Time Const (s)")
  
  self.ui.sens = tab:NewEdit(100, 90, "1e-0")
  self.ui.sens.label = tab:NewLabel(0, 90, "Sensetivity (V)")
  
  self.ui.input = tab:NewComboBox(100, 130)
  self.ui.input:SetItems({"A", "A-B", "Curr 1 MOhm", "Curr 100 MOhm"})
  self.ui.input.selected = 2
  self.ui.input.label = tab:NewLabel(0, 130, "Input")
  
  self.ui.float = tab:NewCheckBox(100, 170, "Floating input")
  self.ui.float.checked = true
  
  self.ui.acmode = tab:NewCheckBox(100, 210, "AC input mode")
  
  self.ui.slope = tab:NewComboBox(100, 250)
  self.ui.slope:SetItems({"6 dB/oct", "12 dB/oct", "18 dB/oct", "24 dB/oct"})
  self.ui.slope.selected = 2
end
function vmeter:connect()
  local sens = tonumber(self.ui.sens.text)
  if sens == nil then return self.ui.tab.text .. " sensitivity: uncorrect value\n" end
  if self.srs == nil then
    if pwr_supl.srs ~= nil then 
      self.srs = pwr_supl.srs
    else
      self.srs = require("sr830"):connectNewDevice(self.ui.port.text)
    end
  end
  if self.srs == nil then return "SR830\n" end
  
  self.dev = self.srs.SineIn
  self.dev:setInputMode(self.ui.input.selected-1)
  self.dev:setInputGrounded(not self.ui.float.checked)
  self.dev:setInputCap(self.ui.acmode.checked)
  self.dev:setRange(sens)
  self.dev:setTimeConst(self.ui.timeconst.value)
  self.dev:setSlope(self.ui.slope.selected)
  return ""
end
function vmeter:getVoltage()
  return self.dev:getVoltage()
end


function ameter:VisualTab(tab)
  self.ui={}
  self.ui.tab = tab
  self.ui.iport = tab:NewEdit(120, 10, "/dev/ttyACM0")
  self.ui.iport.label = tab:NewLabel(10, 10, "SR570 port")
  
  self.ui.isens = tab:NewEdit(120, 50, "1e-3")
  self.ui.isens.label = tab:NewLabel(10, 50, "Sensitivity (V/A)")
  
  self.ui.gainmode = tab:NewComboBox(120, 90)
  self.ui.gainmode:SetItems({"low noise", "high bandwidth", "low drift"})
  self.ui.gainmode.label = tab:NewLabel(10, 90, "gain mode");
  
  self.ui.filter = tab:NewComboBox(120, 130)
  self.ui.filter:SetItems({"6 dB Highpass", "12 dB Highpass", "6 dB Bandpass", "6 dB Lowpass", "12 dB Lowpass", "none"})
  self.ui.filter.selected = 3
  self.ui.filter.label = tab:NewLabel(10, 130, "Filter")
  
  self.ui.hpfilter = tab:NewEdit(120, 170, "0.3")
  self.ui.hpfilter.label = tab:NewLabel(20, 170, "Highpass (Hz)")
  self.ui.lpfilter = tab:NewEdit(120, 210, "10e+3")
  self.ui.lpfilter.label = tab:NewLabel(20, 210, "Lowpass (Hz)")
  
  
  
  self.ui.vport = tab:NewEdit(400, 10, "/dev/ttyUSB1")
  self.ui.vport.label = tab:NewLabel(310, 10, "SR5105 port")
  
  self.ui.vsens = tab:NewEdit(400, 50, "1e-0")
  self.ui.vsens.label = tab:NewLabel(310, 50, "Sensitivity")
  self.ui.vf_hi = tab:NewEdit(400, 90, "1")
  self.ui.vf_lo = tab:NewEdit(400, 130, "5000")
  self.ui.vf_label = tab:NewLabel(310, 110, "Filters")
  
  self.ui.slope = tab:NewComboBox(400, 170)
  self.ui.slope:SetItems({"6 dB/oct", "12 dB/oct"})
  self.ui.slope.selected = 2
  self.ui.slope.label = tab:NewLabel(310, 170, "Slope")
  
  self.ui.time = tab:NewEdit(400, 210, "100e-3")
  self.ui.time.label = tab:NewLabel(310, 210, "Time const")
end
function ameter:connect()
  --5105
  if self.sr5105 == nil then
    self.sr5105 = require("sr5105"):connectNewDevice(self.ui.vport.text)
  end
  if self.sr5105 == nil then return "Ampermeter: Can not connect SR5105" end
  self.sr5105:setRange(tonumber(self.ui.vsens.text))
  self.sr5105:setFilters(tonumber(self.ui.vf_hi.text), tonumber(self.ui.vf_lo.text))
  if self.ui.slope.selected == 2 then self.sr5105:setOutputSlope_12() else self.sr5105:setOutputSlope_6() end
  self.sr5105:setTimeConstant(tonumber(self.ui.time.text))
  --570
  if self.sr570 == nil then
    self.sr570 = require("sr570"):connectNewDevice(self.ui.iport.text)
  end
  if self.sr570 == nil then return "Ampermeter: Can not connect SR570" end
  self.sr570:setSens(tonumber(self.ui.isens.text))
  self.sr570:setTypeFilter(self.ui.gainmode.selected + 1)
  self.sr570:setHighFilter(tonumber(self.ui.hpfilter.text))
  self.sr570:setLowFilter(tonumber(self.ui.lpfilter.text))
  
  return ""
end
function ameter:getCurrent()
  local U = self.sr5105:getVoltage()
  local IU = self.sr570:getSens()
  return U*IU, U
end

results = {count=0}
function results:addPoint(p)
  self.count = self.count + 1
  self[ self.count ] = p
end
function results:display()
  local i, j, l, s
  for i=1, self.count do
    s = ""
    for n,v in pairs(self[i]) do
      s = s .. tonumber(v) .. "\t"
    end
    print(s)
  end
end

gui = require("mygui_gtk")
gui.wnd = gui:NewWindow(800, 600, "Test gtk")
gui.wnd.tabs = gui.wnd:NewTabControl(0, 0, 800, 500)
btnOk = gui.wnd:NewButton(100, 550, "OK")
btnClose = gui.wnd:NewButton(300, 550, "Close")
gui.wnd.tabs[1] = "Measure"
com_settings:VisualTab(gui.wnd.tabs[1])
gui.wnd.tabs[2] = "PowerSupl"
pwr_supl:VisualTab(gui.wnd.tabs[2])
gui.wnd.tabs[3] = "Voltmeter"
vmeter:VisualTab(gui.wnd.tabs[3])
gui.wnd.tabs[4] = "Ampermeter"
ameter:VisualTab(gui.wnd.tabs[4])
plot = gui.wnd.tabs[1]:NewPlot(0, 50, 300, 300)
plot.format={x=1, 5}
plot.data = results


-- Device connection stage
while true do
  if btnOk:WasClicked() then
    local conn = ""
    conn = conn .. com_settings:connect()
    conn = conn .. pwr_supl:connect()
    conn = conn .. vmeter:connect()
    conn = conn .. ameter:connect()
    if conn == "" then break end
    print("Connection failed:\n".. conn)
  end
  if btnClose:WasClicked() then goto PROG_END end
  if not gui:update() then goto PROG_END end
end

print("Connected!")

function measure(Uout)
  pwr_supl:setVoltage(Uout)
  --set some val
  gui:delay_ms(1000);
  local U = vmeter:getVoltage()
  local I, rawU = ameter:getCurrent()
  local p = {Uout, U, I, rawU, U/I}
  results:addPoint(p)
  plot:Refresh()
end

-- Measure stage
Uout = 0
step = pwr_supl.Umax / (com_settings.pnum/2)
for Uout=0, pwr_supl.Umax+step, step do
  if not gui:update() then goto PROG_END end
  measure(Uout)
end
for Uout=pwr_supl.Umax, 0, -step do
  if not gui:update() then goto PROG_END end
  measure(Uout)
end
measure(0)
pwr_supl:setVoltage(0)

-- save results
results:display()

print("---DONE---")

while gui:update()do
  if btnClose:WasClicked() then break end
end

::PROG_END::
print("END")
