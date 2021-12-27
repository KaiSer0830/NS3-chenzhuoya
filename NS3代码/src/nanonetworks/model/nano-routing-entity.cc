/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 TELEMATICS LAB, DEI - Politecnico di Bari
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

#include "nano-routing-entity.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-l3-header.h"
#include "nano-mac-entity.h"
#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/channel.h"
#include "simple-nano-device.h"
#include "nano-spectrum-phy.h"
#include "nano-mac-entity.h"
#include "nano-mac-header.h"
#include "ns3/seq-ts-header.h"
#include "ns3/simulator.h"
#include "nano-routing-entity.h"
#include "message-process-unit.h"

NS_LOG_COMPONENT_DEFINE("NanoRoutingEntity");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(NanoRoutingEntity);

TypeId NanoRoutingEntity::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::NanoRoutingEntity").SetParent<Object>();
	return tid;
}

NanoRoutingEntity::NanoRoutingEntity() {
	m_device = 0;
}

NanoRoutingEntity::~NanoRoutingEntity() {
	m_device = 0;
}

void NanoRoutingEntity::DoDispose(void) {
	m_device = 0;
}

void NanoRoutingEntity::SetDevice(Ptr<SimpleNanoDevice> d) {
	NS_LOG_FUNCTION(this << d);
	m_device = d;
}

Ptr<SimpleNanoDevice> NanoRoutingEntity::GetDevice() {
	NS_LOG_FUNCTION(this);
	return m_device;
}

void NanoRoutingEntity::SendPacket(Ptr<Packet> p) {		//这里是不执行的，因为调用的是自己设置的路由发送数据包函数

}

void NanoRoutingEntity::SendtestPacket(Ptr<Packet> p) {

}

void NanoRoutingEntity::SendackPacket(Ptr<Packet> p, uint32_t dst) {

}

void NanoRoutingEntity::ReceivePacket(Ptr<Packet> p) {

}

void NanoRoutingEntity::ForwardPacket(Ptr<Packet> p) {

}

TypeId SenderTypeTag::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::SenderTypeTag").SetParent<Tag>().AddConstructor<SenderTypeTag>();
	return tid;
}

TypeId SenderTypeTag::GetInstanceTypeId(void) const {
	return GetTypeId();
}

void SenderTypeTag::Print(std::ostream &os) const {
	os << type;
}

uint32_t SenderTypeTag::GetSerializedSize(void) const {
	return 4;
}

void SenderTypeTag::Serialize(TagBuffer i) const {
	i.WriteU32(type);
}

void SenderTypeTag::Deserialize(TagBuffer i) {
	type = i.ReadU32();
}

} // namespace ns3
