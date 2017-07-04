# puts data into the grid format that MATLAB/Octave can easily use with load()
require 'json'

logfile = File.open 'logfile', 'r'
data = File.open './data/data', 'w'

# header will be ignored by load()
data.puts "% secs temperature humidity rssi"

while log = logfile.gets
  begin
    json = JSON.parse log
  rescue JSON::ParserError => e
    # don't care. Happens when ESP8266 reports NaN for temp or humidity
  end

  temp  = json['temperature']
  humid = json['humidity']
  rssi  = json['rssi']
  secs  = json['secs_elapsed']
  data.puts "#{secs} #{temp} #{humid} #{rssi}"
end

for f in [logfile, data]
  f.close if f
end
