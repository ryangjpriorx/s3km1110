import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import UNIT_EMPTY, ICON_RADAR

DEPENDENCIES = ["s3km1110"]

s3km1110_ns = cg.esphome_ns.namespace("s3km1110")
S3KM1110Sensor = s3km1110_ns.class_("S3KM1110Sensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema(
    S3KM1110Sensor,
    unit_of_measurement=UNIT_EMPTY,
    icon=ICON_RADAR,
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config["id"])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
