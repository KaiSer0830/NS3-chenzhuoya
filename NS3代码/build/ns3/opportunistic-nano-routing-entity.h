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


#ifndef OPPORTUNISTIC_NANO_ROUTING_ENTITY_H
#define OPPORTUNISTIC_NANO_ROUTING_ENTITY_H

#include "nano-routing-entity.h"
#include <list>
#include <iostream>
#include <utility>
#include <string>

namespace ns3 {

	class Packet;
	class SimpleNanoDevice;
	class NanoMacQueue;

	/**
	 * \ingroup nanonetworks
	 *
	 * This class implements the Random Routing protocol, as described in
	 *
	 * Giuseppe Piro, Luigi Alredo Grieco, Gennaro Boggia, and Pietro Camarda,"
	 * Nano-Sim: simulating electromagnetic-based nanonetworks in the Network Simulator 3",
	 * in Proc. of Workshop on NS- 3 (held in conjunction with SIMUTools 2013),
	 * Cannes, France, Mar., 2013
	 *
	 */
	class OpportunisticNanoRoutingEntity: public NanoRoutingEntity {
	public:
		static TypeId GetTypeId(void);

		OpportunisticNanoRoutingEntity(void);
		virtual ~OpportunisticNanoRoutingEntity(void);

		virtual void DoDispose(void);

		virtual void SendPacket(Ptr<Packet> p);
		virtual void ReceivePacket(Ptr<Packet> p);
		virtual void Forward(Ptr<Packet> p);
		//virtual void ForwardPacket (Ptr<Packet> p, uint32_t from);
		//  bool SendackPacket (Ptr<Packet> p, uint32_t prio);
		virtual void SendtestPacket(Ptr<Packet> p);

		//Selective flooding implementation
	public:
		void UpdateReceivedPacketId(uint32_t id);
		bool CheckAmongReceivedPacket(uint32_t id);
		void SetReceivedPacketListDim(int m);

		void UpdateSentPacketId(uint32_t id, uint32_t nextHop);
		bool CheckAmongSentPacket(uint32_t id, uint32_t nextHop);
		void SetSentPacketListDim(int m);
		void SetCurrentPacket(Ptr<Packet> p);
		Ptr<Packet> GetCurrentPacket();

	private:
		std::list<uint32_t> m_receivedPacketList;
		int m_receivedPacketListDim;
		int m_receivedPacketListDimtemp;
		std::list<Ptr<Packet> > m_sentPacketList;

	};
} // namespace ns3

#endif /* RANDOM_NANO_ROUTING_ENTITY_H */
