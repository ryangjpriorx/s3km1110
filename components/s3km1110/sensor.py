async def to_code(config):
    var = cg.new_Pvariable(config["id"])
    await cg.register_component(var, config)

    uart_comp = await cg.get_variable(config["uart_id"])
    cg.add(var.set_uart(uart_comp))

    for key, attr in [("pm1", "pm1_0"), ("pm25", "pm2_5"), ("pm10", "pm10")]:
        sens_conf = config[key]
        sens = await sensor.new_sensor(sens_conf)
        cg.add(getattr(var, f"set_{attr}")(sens))

