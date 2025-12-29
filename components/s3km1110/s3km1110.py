import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@ryangjpriorx"]

s3km1110_ns = cg.esphome_ns.namespace("s3km1110")
S3KM1110Component = s3km1110_ns.class_("S3KM1110Component", cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(S3KM1110Component),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
