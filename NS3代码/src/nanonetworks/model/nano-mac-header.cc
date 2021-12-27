/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013,2014 TELEMATICS LAB, DEI - Politecnico di Bari
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Giuseppe Piro <peppe@giuseppepiro.com>, <g.piro@poliba.it>
 */

#include "ns3/log.h"
#include "nano-mac-header.h"

NS_LOG_COMPONENT_DEFINE("NanoMacHeader");

namespace ns3 {

TypeId NanoMacHeader::GetTypeId(void) {
	static TypeId tid = TypeId("NanoMacHeader").SetParent<Header>().AddConstructor<NanoMacHeader>();
	return tid;
}

TypeId NanoMacHeader::GetInstanceTypeId(void) const {
	return GetTypeId();
}

uint32_t NanoMacHeader::GetSerializedSize(void) const {
	return 4 + 4;
}

void NanoMacHeader::Serialize(Buffer::Iterator start) const {
	start.WriteHtonU32(m_source);
	start.WriteHtonU32(m_destination);
}

uint32_t NanoMacHeader::Deserialize(Buffer::Iterator start) {
	Buffer::Iterator rbuf = start;
	m_source = rbuf.ReadNtohU32();
	m_destination = rbuf.ReadNtohU32();
	return rbuf.GetDistanceFrom(start);
}

void NanoMacHeader::Print(std::ostream &os) const {
	os << "src = " << m_source << " dst = " << m_destination;
}

void NanoMacHeader::SetSource(uint32_t source) {
	m_source = source;
}

uint32_t NanoMacHeader::GetSource() const {
	return m_source;
}

void NanoMacHeader::SetDestination(uint32_t dst) {
	m_destination = dst;
}

uint32_t NanoMacHeader::GetDestination() const {
	return m_destination;
}

} // namespace ns3

