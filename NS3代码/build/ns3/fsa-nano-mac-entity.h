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
#ifndef FSA_NANO_MAC_ENTITY_H
#define FSA_NANO_MAC_ENTITY_H

#include "ns3/object.h"
#include <list>
#include "nano-mac-entity.h"
#include <deque>
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"

namespace ns3 {

/**
 * \ingroup nanonetworks
 *
 * This class implements the Backoff MAC protocol, as described in
 *
 * Giuseppe Piro, Luigi Alredo Grieco, Gennaro Boggia, and Pietro Camarda,"
 * Nano-Sim: simulating electromagnetic-based nanonetworks in the Network Simulator 3",
 * in Proc. of Workshop on NS- 3 (held in conjunction with SIMUTools 2013),
 * Cannes, France, Mar., 2013
 *
 */
	class FSANanoMacEntity: public NanoMacEntity {
		public:
			static TypeId GetTypeId(void);

			FSANanoMacEntity(void);
			virtual ~FSANanoMacEntity(void);

			virtual void DoDispose(void);

			virtual void Send(Ptr<Packet> p);

			//virtual bool Sendack (Ptr<Packet> p,uint32_t from,uint32_t pri);
			//virtual void Sendtest (Ptr<Packet> p);

			void Send(Ptr<Packet> p, uint32_t dst);
			virtual void Receive(Ptr<Packet> p);
			void DoSendPacket();
			virtual void sendpacket(Ptr<Packet> p);
			virtual void sendtestpacket(Ptr<Packet> p);
			virtual void forward(Ptr<Packet> p);

		private:
			std::deque<Ptr<Packet> > m_queue;
			std::deque<Ptr<Packet> > m_waitqueue;			//已注释未使用
			std::deque<Ptr<Packet> > m_forwardqueue;			//已注释未使用
			//typedef void (* OutEnergyCallback) (double, int);
			//TracedCallback<double,int> m_outENERGY;
	};
} // namespace ns3

#endif /* BACKOFF_BASED_NANO_MAC_ENTITY_H */
