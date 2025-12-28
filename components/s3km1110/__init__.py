import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import sensor, uart, binary_sensor
from esphome.const import UNIT_EMPTY, CONF_ID, CONF_UART_ID

DEPENDENCIES = ["uart"]

s3km1110_ns = cg.esphome_ns.namespace("s3km1110")
S3KM1110Sensor = s3km1110_ns.class_("S3KM1110Sensor", cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(S3KM1110Sensor),
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),

        # Three numeric outputs from the radar frame
        cv.Required("micro_motion"): sensor.sensor_schema(UNIT_EMPTY),
        cv.Required("presence_confidence"): sensor.sensor_schema(UNIT_EMPTY),
        cv.Required("motion_energy"): sensor.sensor_schema(UNIT_EMPTY),

        # Optional: derived binary presence sensor
        cv.Optional("presence"): binary_sensor.binary_sensor_schema(),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))

    mm_conf = config["micro_motion"]
    mm = await sensor.new_sensor(mm_conf)
    cg.add(var.set_micro_motion(mm))

    pc_conf = config["presence_confidence"]
    pc = await sensor.new_sensor(pc_conf)
    cg.add(var.set_presence_confidence(pc))

    me_conf = config["motion_energy"]
    me = await sensor.new_sensor(me_conf)
    cg.add(var.set_motion_energy(me))

    if "presence" in config:
        pres_conf = config["presence"]
        pres = await binary_sensor.new_binary_sensor(pres_conf)
        cg.add(var.set_presence_binary(pres))
