#!/usr/bin/env ruby

# talks with MQTT server, grabs JSON from ESP8266
require "mqtt"
require "yaml"
require "json"
require "pp"

CONFIG = YAML.load_file 'conf.yaml'

# logfile = File.open 'logfile', 'a+'
# at_exit do
#   logfile.close if logfile
# end

conn_opts = {
  remote_host: CONFIG[:mqtt_host],
  remote_port: CONFIG[:mqtt_port],
  username:    CONFIG[:mqtt_user_name],
  password:    CONFIG[:mqtt_pass]
}

begin
  MQTT::Client.connect(conn_opts) do |client|
    client.get('/data') do |topic, message|
      puts message
      open('logfile', 'a') { |f|
        f.puts message
      }
    end
  end
rescue Interrupt
  exit
rescue MQTT::ProtocolException => e # timeout
    puts e
    retry
end
