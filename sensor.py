import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import UNIT_MICROGRAMS_PER_CUBIC_METER

s3km1110_ns = cg.esphome_ns.namespace("s3km1110")
S3KM1110Sensor = s3km1110_ns.class_("S3KM1110Sensor", cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(S3KM1110Sensor),
    cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    cv.Required("pm1"): sensor.sensor_schema(UNIT_MICROGRAMS_PER_CUBIC_METER),
    cv.Required("pm25"): sensor.sensor_schema(UNIT_MICROGRAMS_PER_CUBIC_METER),
    cv.Required("pm10"): sensor.sensor_schema(UNIT_MICROGRAMS_PER_CUBIC_METER),
})

async def to_code(config):
    var = cg.new_Pvariable(config["id"])
    await cg.register_component(var, config)
    uart_comp = await cg.get_variable(config["uart_id"])
    cg.add(var.set_uart(uart_comp))

    for key, attr in [("pm1", "pm1_0"), ("pm25", "pm2_5"), ("pm10", "pm10")]:
        sens = await sensor.new_sensor(config[key])
        cg.add(getattr(var, f"set_{attr}")(sens))
