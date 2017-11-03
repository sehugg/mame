// license:BSD-3-Clause
// copyright-holders:smf
/**********************************************************************

    The Digital Excess & Hitmen 4-Player Joystick adapter emulation

**********************************************************************/

#ifndef MAME_BUS_C64_4DXH_H
#define MAME_BUS_C64_4DXH_H

#pragma once


#include "user.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> c64_4dxh_device

class c64_4dxh_device : public device_t,
	public device_pet_user_port_interface
{
public:
	// construction/destruction
	c64_4dxh_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual ioport_constructor device_input_ports() const override;

	// device_pet_user_port_interface overrides
	virtual WRITE_LINE_MEMBER( input_4 ) override { output_6(state); }
	virtual WRITE_LINE_MEMBER( input_6 ) override { output_4(state); }

protected:
	// device-level overrides
	virtual void device_start() override;
};


// device type definition
DECLARE_DEVICE_TYPE(C64_4DXH, c64_4dxh_device)


#endif // MAME_BUS_C64_4DXH_H
