import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart, binary_sensor
from esphome.const import CONF_ID, CONF_UART_ID

s3km1110_ns = cg.esphome_ns.namespace("s3km1110")
S3KM1110Component = s3km1110_ns.class_("S3KM1110Component", cg.Component)

CONF_MOTION_ENERGY = "motion_energy"
CONF_PRESENCE = "presence"
CONF_PRESENCE_BINARY = "presence_binary"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(S3KM1110Component),

        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),

        cv.Optional(CONF_MOTION_ENERGY): sensor.sensor_schema(
            unit_of_measurement="",
            accuracy_decimals=0,
        ),

        cv.Optional(CONF_PRESENCE): sensor.sensor_schema(
            unit_of_measurement="",
            accuracy_decimals=0,
        ),

        cv.Optional(CONF_PRESENCE_BINARY): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))

    if CONF_MOTION_ENERGY in config:
        s = await sensor.new_sensor(config[CONF_MOTION_ENERGY])
        cg.add(var.set_motion_energy_sensor(s))

    if CONF_PRESENCE in config:
        s = await sensor.new_sensor(config[CONF_PRESENCE])
        cg.add(var.set_presence_sensor(s))

    if CONF_PRESENCE_BINARY in config:
        b = await binary_sensor.new_binary_sensor(config[CONF_PRESENCE_BINARY])
        cg.add(var.set_presence_binary(b))
