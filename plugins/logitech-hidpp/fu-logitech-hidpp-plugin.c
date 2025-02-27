/*
 * Copyright (C) 2016 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include "config.h"

#include "fu-logitech-hidpp-bootloader-nordic.h"
#include "fu-logitech-hidpp-bootloader-texas.h"
#include "fu-logitech-hidpp-common.h"
#include "fu-logitech-hidpp-device.h"
#include "fu-logitech-hidpp-plugin.h"
#include "fu-logitech-hidpp-runtime-bolt.h"
#include "fu-logitech-hidpp-runtime-unifying.h"

struct _FuLogitechHidppPlugin {
	FuPlugin parent_instance;
};

G_DEFINE_TYPE(FuLogitechHidppPlugin, fu_logitech_hidpp_plugin, FU_TYPE_PLUGIN)

static gboolean
fu_logitech_hidpp_plugin_startup(FuPlugin *plugin, FuProgress *progress, GError **error)
{
	/* check the kernel has CONFIG_HIDRAW */
	if (!g_file_test("/sys/class/hidraw", G_FILE_TEST_IS_DIR)) {
		g_set_error_literal(error,
				    FWUPD_ERROR,
				    FWUPD_ERROR_NOT_SUPPORTED,
				    "no kernel support for CONFIG_HIDRAW");
		return FALSE;
	}
	return TRUE;
}

static void
fu_logitech_hidpp_plugin_init(FuLogitechHidppPlugin *self)
{
}

static void
fu_logitech_hidpp_plugin_constructed(GObject *obj)
{
	FuPlugin *plugin = FU_PLUGIN(obj);
	FuContext *ctx = fu_plugin_get_context(plugin);
	fu_context_add_quirk_key(ctx, "LogitechHidppModelId");
	fu_plugin_add_udev_subsystem(plugin, "hidraw");
	fu_plugin_add_rule(plugin, FU_PLUGIN_RULE_CONFLICTS, "unifying");
	fu_plugin_add_device_gtype(plugin, FU_TYPE_UNIFYING_BOOTLOADER_NORDIC);
	fu_plugin_add_device_gtype(plugin, FU_TYPE_UNIFYING_BOOTLOADER_TEXAS);
	fu_plugin_add_device_gtype(plugin, FU_TYPE_HIDPP_RUNTIME_UNIFYING);
	fu_plugin_add_device_gtype(plugin, FU_TYPE_HIDPP_DEVICE);
	fu_plugin_add_device_gtype(plugin, FU_TYPE_HIDPP_RUNTIME_BOLT);
}

static void
fu_logitech_hidpp_plugin_class_init(FuLogitechHidppPluginClass *klass)
{
	FuPluginClass *plugin_class = FU_PLUGIN_CLASS(klass);
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = fu_logitech_hidpp_plugin_constructed;
	plugin_class->startup = fu_logitech_hidpp_plugin_startup;
}
