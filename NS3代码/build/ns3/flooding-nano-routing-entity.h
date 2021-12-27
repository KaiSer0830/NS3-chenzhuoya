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


#ifndef FLOODING_NANO_ROUTING_ENTITY_H
#define FLOODING_NANO_ROUTING_ENTITY_H

#include "nano-routing-entity.h"
#include <list>
#include <iostream>
#include <utility>
#include <string>
#include "ns3/callback.h"
#include "ns3/traced-callback.h"

namespace ns3 {

	class Packet;
	class SimpleNanoDevice;
	class NanoMacQueue;

	/**
	 * \ingroup nanonetworks
	 *
	 * This class implements the Flooding Routing protocol, as described in
	 *
	 * Giuseppe Piro, Luigi Alredo Grieco, Gennaro Boggia, and Pietro Camarda,"
	 * Nano-Sim: simulating electromagnetic-based nanonetworks in the Network Simulator 3",
	 * in Proc. of Workshop on NS- 3 (held in conjunction with SIMUTools 2013),
	 * Cannes, France, Mar., 2013
	 *
	 */
	class FloodingNanoRoutingEntity: public NanoRoutingEntity {
		public:
			static TypeId GetTypeId(void);

			FloodingNanoRoutingEntity(void);
			virtual ~FloodingNanoRoutingEntity(void);

			virtual void DoDispose(void);

			virtual void SendPacket(Ptr<Packet> p);
			virtual void ReceivePacket(Ptr<Packet> p);
			virtual void ForwardPacket(Ptr<Packet> p);

		//Selective flooding implementation
		public:
			void UpdateReceivedPacketId(uint32_t id);
			bool CheckAmongReceivedPacket(uint32_t id);
			void SetReceivedPacketListDim(int m);

		private:
			std::list<uint32_t> m_receivedPacketList;		//该节点不需要接收的数据包id队列,自己发送过的和自己接收过的数据包不再进行接收，每个节点的该变量计数独立
			int m_receivedPacketListDim;					//
			typedef void (*OutEnergyCallback)(double);
			TracedCallback<double> m_outENERGY;
	};
} // namespace ns3

#endif /* FLOODING_NANO_ROUTING_ENTITY_H */
