// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
#include "emu.h"
#include "pci-smbus.h"

DEFINE_DEVICE_TYPE(SMBUS, smbus_device, "smbus", "SMBus interface")

void smbus_device::map(address_map &map)
{
	map(0x00, 0x00).rw(FUNC(smbus_device::hst_sts_r), FUNC(smbus_device::hst_sts_w));
	map(0x02, 0x02).rw(FUNC(smbus_device::hst_cnt_r), FUNC(smbus_device::hst_cnt_w));
	map(0x03, 0x03).rw(FUNC(smbus_device::hst_cmd_r), FUNC(smbus_device::hst_cmd_w));
	map(0x04, 0x04).rw(FUNC(smbus_device::xmit_slva_r), FUNC(smbus_device::xmit_slva_w));
	map(0x05, 0x05).rw(FUNC(smbus_device::hst_d0_r), FUNC(smbus_device::hst_d0_w));
	map(0x06, 0x06).rw(FUNC(smbus_device::hst_d1_r), FUNC(smbus_device::hst_d1_w));
	map(0x07, 0x07).rw(FUNC(smbus_device::host_block_db_r), FUNC(smbus_device::host_block_db_w));
	map(0x08, 0x08).rw(FUNC(smbus_device::pec_r), FUNC(smbus_device::pec_w));
	map(0x09, 0x09).rw(FUNC(smbus_device::rcv_slva_r), FUNC(smbus_device::rcv_slva_w));
	map(0x0a, 0x0b).rw(FUNC(smbus_device::slv_data_r), FUNC(smbus_device::slv_data_w));
	map(0x0c, 0x0c).rw(FUNC(smbus_device::aux_sts_r), FUNC(smbus_device::aux_sts_w));
	map(0x0d, 0x0d).rw(FUNC(smbus_device::aux_ctl_r), FUNC(smbus_device::aux_ctl_w));
	map(0x0e, 0x0e).rw(FUNC(smbus_device::smlink_pin_ctl_r), FUNC(smbus_device::smlink_pin_ctl_w));
	map(0x0f, 0x0f).rw(FUNC(smbus_device::smbus_pin_ctl_r), FUNC(smbus_device::smbus_pin_ctl_w));
	map(0x10, 0x10).rw(FUNC(smbus_device::slv_sts_r), FUNC(smbus_device::slv_sts_w));
	map(0x11, 0x11).rw(FUNC(smbus_device::slv_cmd_r), FUNC(smbus_device::slv_cmd_w));
	map(0x14, 0x14).r(FUNC(smbus_device::notify_daddr_r));
	map(0x16, 0x16).r(FUNC(smbus_device::notify_dlow_r));
	map(0x17, 0x17).r(FUNC(smbus_device::notify_dhigh_r));
}

smbus_device::smbus_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: pci_device(mconfig, SMBUS, tag, owner, clock)
{
}

void smbus_device::device_start()
{
	pci_device::device_start();
	skip_map_regs(4);
	add_map(32, M_IO, FUNC(smbus_device::map));
}

void smbus_device::device_reset()
{
	pci_device::device_reset();
	hst_sts = 0x00;
	hst_cnt = 0x00;
	hst_cmd = 0x00;
	xmit_slva = 0x00;
	hst_d0 = 0x00;
	hst_d1 = 0x00;
	host_block_db = 0x00;
	pec = 0x00;
	rcv_slva = 0x44;
	slv_data = 0x0000;
	aux_sts = 0x00;
	aux_ctl = 0x00;
	smlink_pin_ctl = 0x00;
	smbus_pin_ctl = 0x00;
	slv_sts = 0x00;
	slv_cmd = 0x00;
	notify_daddr = 0x00;
	notify_dlow = 0x00;
	notify_dhigh = 0x00;
}

uint8_t smbus_device::hst_sts_r()
{
	return hst_sts;
}

void smbus_device::hst_sts_w(uint8_t data)
{
	hst_sts &= ~data;
	logerror("%s: hst_sts = %02x\n", tag(), hst_sts);
}

uint8_t smbus_device::hst_cnt_r()
{
	return hst_cnt;
}

void smbus_device::hst_cnt_w(uint8_t data)
{
	hst_cnt = data;
	logerror("%s: hst_cnt = %02x\n", tag(), hst_cnt);

	if(xmit_slva != 0xa1)
		hst_sts = 4;
	else {
		const uint8_t eeprom[256] = {
			0x80, 0x08, 0x07, 0x0D, 0x0A, 0x02, 0x40, 0x00, 0x04, 0x50, 0x60, 0x00, 0x82, 0x08, 0x00, 0x01,
			0x0E, 0x04, 0x08, 0x01, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x28, 0x3C, 0x28, 0x40,
			0x60, 0x60, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x46, 0x28, 0x28, 0x55, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE,
			0x7F, 0x7F, 0x9E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x56, 0x53, 0x35, 0x31, 0x32, 0x4D, 0x42,
			0x34, 0x30, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		};

		hst_sts = 2;
		hst_d0 = eeprom[hst_cmd];
	}
}

uint8_t smbus_device::hst_cmd_r()
{
	return hst_cmd;
}

void smbus_device::hst_cmd_w(uint8_t data)
{
	hst_cmd = data;
	logerror("%s: hst_cmd = %02x\n", tag(), hst_cmd);
}

uint8_t smbus_device::xmit_slva_r()
{
	return xmit_slva;
}

void smbus_device::xmit_slva_w(uint8_t data)
{
	xmit_slva = data;
	logerror("%s: xmit_slva = %02x\n", tag(), xmit_slva);
}

uint8_t smbus_device::hst_d0_r()
{
	return hst_d0;
}

void smbus_device::hst_d0_w(uint8_t data)
{
	hst_d0 = data;
	logerror("%s: hst_d0 = %02x\n", tag(), hst_d0);
}

uint8_t smbus_device::hst_d1_r()
{
	return hst_d1;
}

void smbus_device::hst_d1_w(uint8_t data)
{
	hst_d1 = data;
	logerror("%s: hst_d1 = %02x\n", tag(), hst_d1);
}

uint8_t smbus_device::host_block_db_r()
{
	return host_block_db;
}

void smbus_device::host_block_db_w(uint8_t data)
{
	host_block_db = data;
	logerror("%s: host_block_db = %02x\n", tag(), host_block_db);
}

uint8_t smbus_device::pec_r()
{
	return pec;
}

void smbus_device::pec_w(uint8_t data)
{
	pec = data;
	logerror("%s: pec = %02x\n", tag(), pec);
}

uint8_t smbus_device::rcv_slva_r()
{
	return rcv_slva;
}

void smbus_device::rcv_slva_w(uint8_t data)
{
	rcv_slva = data;
	logerror("%s: rcv_slva = %02x\n", tag(), rcv_slva);
}

uint16_t smbus_device::slv_data_r()
{
	return slv_data;
}

void smbus_device::slv_data_w(uint16_t data)
{
	slv_data = data;
	logerror("%s: slv_data = %02x\n", tag(), slv_data);
}

uint8_t smbus_device::aux_sts_r()
{
	return aux_sts;
}

void smbus_device::aux_sts_w(uint8_t data)
{
	aux_sts = data;
	logerror("%s: aux_sts = %02x\n", tag(), aux_sts);
}

uint8_t smbus_device::aux_ctl_r()
{
	return aux_ctl;
}

void smbus_device::aux_ctl_w(uint8_t data)
{
	aux_ctl = data;
	logerror("%s: aux_ctl = %02x\n", tag(), aux_ctl);
}

uint8_t smbus_device::smlink_pin_ctl_r()
{
	return smlink_pin_ctl;
}

void smbus_device::smlink_pin_ctl_w(uint8_t data)
{
	smlink_pin_ctl = data;
	logerror("%s: smlink_pin_ctl = %02x\n", tag(), smlink_pin_ctl);
}

uint8_t smbus_device::smbus_pin_ctl_r()
{
	return smbus_pin_ctl;
}

void smbus_device::smbus_pin_ctl_w(uint8_t data)
{
	smbus_pin_ctl = data;
	logerror("%s: smbus_pin_ctl = %02x\n", tag(), smbus_pin_ctl);
}

uint8_t smbus_device::slv_sts_r()
{
	return slv_sts;
}

void smbus_device::slv_sts_w(uint8_t data)
{
	slv_sts = data;
	logerror("%s: slv_sts = %02x\n", tag(), slv_sts);
}

uint8_t smbus_device::slv_cmd_r()
{
	return slv_cmd;
}

void smbus_device::slv_cmd_w(uint8_t data)
{
	slv_cmd = data;
	logerror("%s: slv_cmd = %02x\n", tag(), slv_cmd);
}

uint8_t smbus_device::notify_daddr_r()
{
	return notify_daddr;
}

uint8_t smbus_device::notify_dlow_r()
{
	return notify_dlow;
}

uint8_t smbus_device::notify_dhigh_r()
{
	return notify_dhigh;
}
