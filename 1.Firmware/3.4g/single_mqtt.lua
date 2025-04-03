-- Uncomment to enable power saving mode.
-- pm.request(pm.LIGHT)

local mqtt_host = "0c5e592236.st1.iotda-device.cn-east-3.myhuaweicloud.com"
local mqtt_port = 8883
local mqtt_isssl = true
local client_id = "67ea52cbc957870e570e769f_LoRaGateway_0_0_2025040311"
local user_name = "67ea52cbc957870e570e769f_LoRaGateway"
local password = "password"

local pub_topic = "$oc/devices/" .. user_name .. "/sys/properties/report"
local sub_topic = ""

local mqttc = nil

-- Try to connect to 4G network.
sys.taskInit(function()
    local device_id = mobile.imei()
    sys.waitUntil("IP_READY")
    sys.publish("net_ready", device_id)
end)

-- MQTT thread.
sys.taskInit(function()
    -- Wait for 4G to be connected.
    local ret, device_id = sys.waitUntil("net_ready")

    -- Print publish topic.
    log.info("mqtt", "pub", pub_topic)

    mqttc = mqtt.create(nil, mqtt_host, mqtt_port, mqtt_isssl)
    mqttc:auth(client_id, user_name, password)
    mqttc:autoreconn(true, 3000)

    mqttc:on(function(mqtt_client, event, data, payload)
        -- Print event.
        log.info("mqtt", "event", event, mqtt_client, data, payload)
        if event == "conack" then
            sys.publish("mqtt_conack")
        end
    end)

    -- MQTT auto reconnect.
    mqttc:connect()
	sys.waitUntil("mqtt_conack")
    while true do
        -- Just wait.
        sys.wait(60000000)
    end
    mqttc:close()
    mqttc = nil
end)

-- Receive sensor date from uart2.
local uart_id = 2
uart.setup(uart_id, 115200)
uart.on(uart_id, "receive", function(id, len)
    local data = uart.read(id)
	local qos = 1

    -- Decode.
    local temp = tonumber(data:match("temp:%s*([%d%.]+)"))
    local humi = tonumber(data:match("humi:%s*([%d%.]+)"))
    local smoke = tonumber(data:match("smoke:%s*([%d%.]+)"))

    -- Publish.
    if mqttc and mqttc:ready() then
        if temp and temp ~= 0 then
            local json_str = string.format(
                '{"services":[{"service_id":"sensors","properties":{"temperature":%.2f}}]}',
                temp
            )
            log.info("uart", "temp:", json_str)
            mqttc:publish(pub_topic, json_str, qos)
        end

        if humi and humi ~= 0 then
            local json_str = string.format(
                '{"services":[{"service_id":"sensors","properties":{"humidity":%.2f}}]}',
                humi
            )
            log.info("uart", "humi:", json_str)
            mqttc:publish(pub_topic, json_str, qos)
        end

        if smoke and smoke ~= 0 then
            local json_str = string.format(
                '{"services":[{"service_id":"sensors","properties":{"smoke":%.2f}}]}',
                smoke
            )
            log.info("uart", "temp:", json_str)
            mqttc:publish(pub_topic, json_str, qos)
        end
    end
end)
