// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2026 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2013, The Linux Foundation. All rights reserved. (FIXME)

#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>

#include <video/mipi_display.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct sharp_rsp61322 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct gpio_desc *reset_gpio;
};

static inline struct sharp_rsp61322 *to_sharp_rsp61322(struct drm_panel *panel)
{
	return container_of_const(panel, struct sharp_rsp61322, panel);
}

static void sharp_rsp61322_reset(struct sharp_rsp61322 *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(5000, 6000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(10000, 11000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(10000, 11000);
}

static int sharp_rsp61322_on(struct sharp_rsp61322 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xb0, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xd6, 0x01);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xce,
					 0x3d, 0x40, 0x48, 0x56, 0x67, 0x78,
					 0x88, 0x98, 0xa7, 0xb5, 0xc3, 0xd1,
					 0xde, 0xe9, 0xf2, 0xfa, 0xff, 0x01,
					 0x5f, 0x04, 0x04, 0x00, 0x00, 0x00);
	mipi_dsi_usleep_range(&dsi_ctx, 16000, 17000);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xc7,
					 0x0b, 0x18, 0x20, 0x2b, 0x3a, 0x48,
					 0x51, 0x60, 0x45, 0x4d, 0x58, 0x65,
					 0x71, 0x73, 0x74, 0x09, 0x16, 0x1e,
					 0x29, 0x38, 0x48, 0x51, 0x60, 0x45,
					 0x4d, 0x58, 0x65, 0x71, 0x75, 0x76);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xc8,
					 0x01, 0x00, 0x09, 0x02, 0xfd, 0xcf,
					 0x00, 0x00, 0x05, 0xfe, 0x02, 0xfc,
					 0x00, 0x00, 0x0c, 0xfe, 0xf4, 0xe8,
					 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xb0, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x34, 0x00);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);
	mipi_dsi_dcs_set_display_brightness_multi(&dsi_ctx, 0x00ff);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_WRITE_CONTROL_DISPLAY,
				     0x2c);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_WRITE_POWER_SAVE, 0x01);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);
	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 20);
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);

	return dsi_ctx.accum_err;
}

static int sharp_rsp61322_off(struct sharp_rsp61322 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 20);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);

	return dsi_ctx.accum_err;
}

static int sharp_rsp61322_prepare(struct drm_panel *panel)
{
	struct sharp_rsp61322 *ctx = to_sharp_rsp61322(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	sharp_rsp61322_reset(ctx);

	ret = sharp_rsp61322_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		return ret;
	}

	return 0;
}

static int sharp_rsp61322_unprepare(struct drm_panel *panel)
{
	struct sharp_rsp61322 *ctx = to_sharp_rsp61322(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = sharp_rsp61322_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 1);

	return 0;
}

static const struct drm_display_mode sharp_rsp61322_mode = {
	.clock = (1080 + 100 + 10 + 50) * (1920 + 7 + 2 + 6) * 60 / 1000,
	.hdisplay = 1080,
	.hsync_start = 1080 + 100,
	.hsync_end = 1080 + 100 + 10,
	.htotal = 1080 + 100 + 10 + 50,
	.vdisplay = 1920,
	.vsync_start = 1920 + 7,
	.vsync_end = 1920 + 7 + 2,
	.vtotal = 1920 + 7 + 2 + 6,
	.width_mm = 61,
	.height_mm = 109,
	.type = DRM_MODE_TYPE_DRIVER,
};

static int sharp_rsp61322_get_modes(struct drm_panel *panel,
				    struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector, &sharp_rsp61322_mode);
}

static const struct drm_panel_funcs sharp_rsp61322_panel_funcs = {
	.prepare = sharp_rsp61322_prepare,
	.unprepare = sharp_rsp61322_unprepare,
	.get_modes = sharp_rsp61322_get_modes,
};

static int sharp_rsp61322_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct sharp_rsp61322 *ctx;
	int ret;

	ctx = devm_drm_panel_alloc(dev, struct sharp_rsp61322, panel,
				   &sharp_rsp61322_panel_funcs,
				   DRM_MODE_CONNECTOR_DSI);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE |
			  MIPI_DSI_MODE_NO_EOT_PACKET |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS | MIPI_DSI_MODE_LPM;

	ctx->panel.prepare_prev_first = true;

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void sharp_rsp61322_remove(struct mipi_dsi_device *dsi)
{
	struct sharp_rsp61322 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id sharp_rsp61322_of_match[] = {
	{ .compatible = "xiaomi,sharp-rsp61322" }, // FIXME
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sharp_rsp61322_of_match);

static struct mipi_dsi_driver sharp_rsp61322_driver = {
	.probe = sharp_rsp61322_probe,
	.remove = sharp_rsp61322_remove,
	.driver = {
		.name = "panel-xiaomi-sharp-rsp61322",
		.of_match_table = sharp_rsp61322_of_match,
	},
};
module_mipi_dsi_driver(sharp_rsp61322_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator <fix@me>"); // FIXME
MODULE_DESCRIPTION("DRM driver for sharp rsp61322 1080p video mode dsi panel");
MODULE_LICENSE("GPL");
