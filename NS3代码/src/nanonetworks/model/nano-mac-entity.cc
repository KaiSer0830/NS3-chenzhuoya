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

#include "nano-mac-entity.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-spectrum-channel.h"
#include "nano-spectrum-phy.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE("NanoMacEntity");

namespace ns3 {

	NS_OBJECT_ENSURE_REGISTERED(NanoMacEntity);

	TypeId NanoMacEntity::GetTypeId(void) {
		static TypeId tid = TypeId("ns3::NanoMacEntity").SetParent<Object>();
		return tid;
	}

	NanoMacEntity::NanoMacEntity() {
		m_device = 0;
		m_queue = CreateObject<NanoMacQueue>();
		Simulator::Schedule(Seconds(0.001), &NanoMacEntity::CheckForNeighbors, this);
	}

	NanoMacEntity::~NanoMacEntity() {
		m_device = 0;
		m_queue = 0;
	}

	void NanoMacEntity::DoDispose(void) {
		m_device = 0;
		m_queue = 0;
	}

	void NanoMacEntity::SetDevice(Ptr<SimpleNanoDevice> d) {
		NS_LOG_FUNCTION(this << d);
		m_device = d;
	}

	Ptr<SimpleNanoDevice> NanoMacEntity::GetDevice() {
		NS_LOG_FUNCTION(this);
		return m_device;
	}

	void NanoMacEntity::SetMacQueue(Ptr<NanoMacQueue> q) {
		NS_LOG_FUNCTION(this);
		m_queue = q;
	}

	void NanoMacEntity::SetMacQueue1(Ptr<NanoMacQueue> q) {
		NS_LOG_FUNCTION(this);
		m_waitqueue = q;
	}

	void NanoMacEntity::SetMacQueue2(Ptr<NanoMacQueue> q) {
		NS_LOG_FUNCTION(this);
		m_sendqueue = q;
	}

	Ptr<NanoMacQueue> NanoMacEntity::GetMacQueue() {
		NS_LOG_FUNCTION(this);
		return m_queue;
	}

	void NanoMacEntity::CheckForNeighborss() {
		NS_LOG_FUNCTION(this);
		m_neighborss.clear();
		Ptr<NanoSpectrumPhy> phy = GetDevice()->GetPhy();
		Ptr<NanoSpectrumChannel> channel = phy->GetChannel()->GetObject<NanoSpectrumChannel>();
		m_neighborss = channel->GetNeighborss(phy);

		Simulator::Schedule(Seconds(0.01), &NanoMacEntity::CheckForNeighborss, this);
	}

	void NanoMacEntity::CheckForNeighbors() {
		NS_LOG_FUNCTION(this);
		m_neighbors.clear();
		Ptr<NanoSpectrumPhy> phy = GetDevice()->GetPhy();
		Ptr<NanoSpectrumChannel> channel = phy->GetChannel()->GetObject<NanoSpectrumChannel>();
		m_neighbors = channel->GetNeighbors(phy);

		Simulator::Schedule(Seconds(0.01), &NanoMacEntity::CheckForNeighbors, this);
	}

	void NanoMacEntity::forward(Ptr<Packet> p) {

	}

	void NanoMacEntity::sendpacket(Ptr<Packet>) {

	}

	void NanoMacEntity::sendtestpacket(Ptr<Packet> p) {

	}

	void NanoMacEntity::sendtestnodepacket(Ptr<Packet> p) {

	}

	void NanoMacEntity::sendackpacket(Ptr<Packet> p, uint32_t dst) {

	}
} // namespace ns3
