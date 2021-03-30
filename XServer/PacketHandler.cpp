/* 
 * XenServer, an online game server
 * Copyright (C) 2008 XenServer team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Shared.h"

Crypto::Crypto()
{
	uint8 tmpnewoffsets[] = {
		0x0B, 0xB3, 0x49, 0x4C, 0xA7, 0x53, 0x44, 0x09, 0xAA, 0xE4, 0x0A, 0x8D, 0xD9, 0x5D, 0xC5, 0x28,
		0xDA, 0x25, 0x4F, 0x25, 0xA0, 0xE2, 0x67, 0x69, 0x61, 0x73, 0x84, 0xEA, 0x97, 0xE2, 0x41, 0xD5,
		0xB6, 0x28, 0xC6, 0x7D, 0x53, 0xD5, 0x73, 0x98, 0x12, 0x9F, 0x80, 0xB0, 0x5A, 0xA1, 0x29, 0xE7,
		0x5E, 0xD0, 0x2A, 0x7F, 0x09, 0xBF, 0xD6, 0x4B, 0x6B, 0x83, 0x01, 0xAA, 0x7B, 0x67, 0x1B, 0x0D,
		0xBC, 0x0E, 0x26, 0x2E, 0xD5, 0x8F, 0x7E, 0xCE, 0x33, 0xF2, 0x1C, 0xD8, 0xAD, 0x03, 0xB4, 0xFC,
		0x40, 0x8D, 0x0B, 0xD0, 0x78, 0x01, 0x60, 0x26, 0xBB, 0x38, 0x36, 0x52, 0x6C, 0xBD, 0x1E, 0x6F,
		0xD2, 0x92, 0xC4, 0x4B, 0xFA, 0x25, 0x30, 0xDE, 0x16, 0x98, 0xC0, 0xAA, 0x79, 0x56, 0x10, 0x61,
		0xA1, 0x39, 0x9E, 0x57, 0x23, 0xCC, 0xD5, 0x19, 0x2A, 0xA7, 0xF4, 0x2A, 0xE3, 0x9C, 0xF5, 0xCF,
		0x3D, 0xF0, 0x12, 0x6A, 0xDD, 0x01, 0x49, 0xD0, 0xCE, 0x02, 0x0A, 0x7D, 0x10, 0x2D, 0x47, 0x11,
		0x15, 0xD7, 0x54, 0xFA, 0x63, 0x07, 0xB3, 0x58, 0x1E, 0xA6, 0xB0, 0x14, 0x0E, 0xA1, 0x93, 0x28,
		0xFE, 0x12, 0x3C, 0x02, 0x94, 0x1E, 0xEE, 0xA7, 0xAF, 0xED, 0x63, 0xDE, 0x8A, 0x1D, 0xD6, 0xC1,
		0x68, 0x34, 0xBF, 0xC0, 0x1E, 0x7B, 0x2C, 0x31, 0x86, 0x49, 0xE9, 0xC0, 0xC5, 0x06, 0x8B, 0x9A,
		0x3A, 0xE2, 0x85, 0x36, 0x10, 0xAB, 0x8A, 0x47, 0x0D, 0xE6, 0xFC, 0x43, 0x8A, 0x57, 0x36, 0x33,
		0x21, 0x9A, 0x95, 0xDB, 0x16, 0x33, 0xA2, 0x75, 0x49, 0xD5, 0x4E, 0xF8, 0x65, 0xCA, 0xCD, 0x9B,
		0x5B, 0x95, 0xA8, 0x95, 0x0F, 0xD6, 0x0F, 0x27, 0x51, 0x6F, 0x6C, 0xDA, 0x0E, 0xE5, 0x3D, 0x0D,
		0xC7, 0x09, 0x23, 0xAB, 0x43, 0xD7, 0x36, 0x3A, 0x81, 0x9E, 0x9B, 0x1A, 0xF0, 0x62, 0x59, 0x0A,
		0xE3, 0x7E, 0x68, 0xED, 0x6A, 0x89, 0xA7, 0x4D, 0x08, 0xC8, 0x32, 0x9E, 0xE7, 0xD2, 0x00, 0x00,
		0xB5, 0x3B, 0x12, 0x1F, 0xE5, 0x55, 0x9A, 0x15 };

	memcpy(m_key, tmpnewoffsets, CLIENT_KEY_SIZE);
}

void Crypto::Encrypt( uint8* packet, size_t len )
{
	if (len < 2)
		return;

	memmove(&packet[2], &packet[1], len - 1);
	packet[1] = 0; //yes, this is the only dynamic variable that affects the outcome

	packet[0] ^= 0xAA;

	uint16 key = packet[0] + packet[1];
	key &= 0xFF;

	if (packet[0] == 0)
		return;

	uint8 startkeyval = filecryptkey[key];
	startkeyval &= 0x24;
	startkeyval |= 1;

	uint32 writepos = 2;
	while (writepos < len + 1) //we moved everything a byte forward
	{
		uint8 keyval = filecryptkey[key];

		if (writepos & startkeyval)
		{
			if (keyval & startkeyval)
				packet[writepos] += keyval;
			else
				packet[writepos] ^= keyval;
		}
		else
		{
			if (keyval & startkeyval)
				packet[writepos] -= keyval;
			else
				packet[writepos] = ~packet[writepos];
		}

		++key;
		key &= 0xFF;
		++writepos;
	}
}

void Crypto::Decrypt( uint8* packet, size_t len )
{
	if (len < 2)
		return;

	uint16 key = packet[0] + packet[1];
	key &= 0xFF;
	packet[0] ^= 0xAA;

	if (packet[0] == 0)
		return;

	uint8 startkeyval = filecryptkey[key];
	startkeyval &= 0x24;
	startkeyval |= 1;

	uint32 writepos = 2;
	while (writepos < len)
	{
		uint8 keyval = filecryptkey[key];

		if (writepos & startkeyval)
		{
			if (keyval & startkeyval)
				packet[writepos] -= keyval;
			else
				packet[writepos] ^= keyval;
		}
		else
		{
			if (keyval & startkeyval)
				packet[writepos] += keyval;
			else
				packet[writepos] = ~packet[writepos];
		}

		++key;
		key &= 0xFF;
		++writepos;
	}
	uint8 seed = packet[1];
	memmove(&packet[1], &packet[2], len - 1);
	packet[len - 1] = seed; //sneaky, good way to catch commview
}

void Crypto::Decrypt( uint8* packet, size_t len, uint32 keystart )
{
	uint16 key = keystart & 0xFF;

	uint8 startkeyval = filecryptkey[key];
	startkeyval &= 0x24;
	startkeyval |= 1;

	uint32 writepos = 0;
	while (writepos < len)
	{
		uint8 keyval = filecryptkey[key];

		if (writepos & startkeyval)
		{
			if (!(keyval & startkeyval))
				packet[writepos] -= keyval;
			else
				packet[writepos] ^= keyval;
		}
		else
		{
			if (!(keyval & startkeyval))
				packet[writepos] += keyval;
			else
				packet[writepos] = ~packet[writepos];
		}

		++key;
		key &= 0xFF;
		++writepos;
	}
}