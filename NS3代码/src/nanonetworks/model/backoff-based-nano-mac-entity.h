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
#ifndef BACKOFF_BASED_NANO_MAC_ENTITY_H
#define BACKOFF_BASED_NANO_MAC_ENTITY_H

#include "ns3/object.h"
#include <list>
#include "nano-mac-entity.h"
#include <deque>

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
	class BackoffBasedNanoMacEntity : public NanoMacEntity
	{
		public:
			static TypeId GetTypeId (void);

			BackoffBasedNanoMacEntity (void);
			virtual ~BackoffBasedNanoMacEntity (void);

			virtual void DoDispose (void);

			virtual void Send (Ptr<Packet> p);
			virtual void Send (Ptr<Packet> p, uint32_t dst);
			virtual void Receive (Ptr<Packet> p);
			void DoSendPacket ();


		private:
		  std::deque<Ptr<Packet> > m_queue;
	};
} // namespace ns3

#endif /* BACKOFF_BASED_NANO_MAC_ENTITY_H */
