#!/usr/bin/env ruby

require "mqtt"

conn_opts = {
  remote_host: "raspberrypi.local",
  remote_port: 1883
}

# otherwise it tries to buffer it instead of letting us pipe it
$stdout.sync = true

begin
  MQTT::Client.connect(conn_opts) do |client|
    client.get(['+/temperature', '+/humidity']) do |topic, message|
      $stdout.puts "#{topic}|#{message}"
    end
  end
rescue Interrupt
  exit
rescue MQTT::ProtocolException => e # timeout
  puts e
  retry
rescue Errno::ECONNRESET => e
  puts e
  retry
end

