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
#include "nano-l3-header.h"
#include<list>

NS_LOG_COMPONENT_DEFINE("NanoL3Header");

namespace ns3 {

TypeId NanoL3Header::GetTypeId(void) {
	static TypeId tid = TypeId("NanoL3Header").SetParent<Header>().AddConstructor<NanoL3Header>();
	return tid;
}

TypeId NanoL3Header::GetInstanceTypeId(void) const {
	return GetTypeId();
}

uint32_t NanoL3Header::GetSerializedSize(void) const {
	return 4 + 4 + 4 + 4 + 4 + 4;
}

void NanoL3Header::Serialize(Buffer::Iterator start) const {
	start.WriteHtonU32(m_source);
	start.WriteHtonU32(m_destination);
	start.WriteHtonU32(m_ttl);
	start.WriteHtonU32(m_packetId);
	start.WriteHtonU32(m_mobility);
	start.WriteHtonU32(m_index);
}

uint32_t NanoL3Header::Deserialize(Buffer::Iterator start) {
	Buffer::Iterator rbuf = start;

	m_source = rbuf.ReadNtohU32();
	m_destination = rbuf.ReadNtohU32();
	m_ttl = rbuf.ReadNtohU32();
	m_packetId = rbuf.ReadNtohU32();
	m_mobility = rbuf.ReadNtohU32();
	m_index = rbuf.ReadNtohU32();

	return rbuf.GetDistanceFrom(start);
}

void NanoL3Header::Print(std::ostream &os) const {
	os << "src = " << m_source << " dst = " << m_destination << " ttl = " << m_ttl << " pkt id = " << m_packetId << " mobility = " << m_mobility << " index = " << m_index;
}

void NanoL3Header::SetSource(uint32_t source) {
	m_source = source;
}

uint32_t NanoL3Header::GetSource() const {
	return m_source;
}

void NanoL3Header::SetDestination(uint32_t dst) {
	m_destination = dst;
}

uint32_t NanoL3Header::GetDestination() const {
	return m_destination;
}

void NanoL3Header::SetTtl(uint32_t ttl) {
	m_ttl = ttl;
}

void NanoL3Header::SetPacketId(uint32_t id) {
	m_packetId = id;
}

uint32_t NanoL3Header::GetTtl() const {
	return m_ttl;
}

uint32_t NanoL3Header::GetPacketId() const {
	return m_packetId;
}

void NanoL3Header::Setmobility(uint32_t mobility) {
	m_mobility = mobility;
}

uint32_t NanoL3Header::Getmobility() {
	return m_mobility;
}

void NanoL3Header::Setindex(uint32_t index) {
	m_index = index;
}

uint32_t NanoL3Header::Getindex() {
	return m_index;
}

} // namespace ns3

