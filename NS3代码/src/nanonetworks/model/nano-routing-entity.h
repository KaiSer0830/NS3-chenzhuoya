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


#ifndef NANO_ROUTING_ENTITY_H
#define NANO_ROUTING_ENTITY_H

#include "ns3/object.h"
#include "ns3/tag.h"
#include <list>

namespace ns3 {

class Packet;
class SimpleNanoDevice;
class NanoMacQueue;

/**
 * \ingroup nanonetworks
 *
 * This class provides a basic implementation of the routing
 * layer
 */
class NanoRoutingEntity : public Object
{
	public:
	  static TypeId GetTypeId (void);

	  NanoRoutingEntity (void);
	  virtual ~NanoRoutingEntity (void);

	  virtual void DoDispose (void);

	  /**
	   * \brief Set the device where the mac entity is attached
	   * \param d the device
	   */
	  void SetDevice (Ptr<SimpleNanoDevice> d);
	  /**
	   * \brief Get the device where the mac entity is attached
	   * \return the pointer to the device
	   */
	  Ptr<SimpleNanoDevice> GetDevice ();

	  virtual void SendPacket (Ptr<Packet>);
	  virtual void SendtestPacket (Ptr<Packet> p);					//flow-guided
	  //virtual void SendtestnodePacket (Ptr<Packet> p);
	  virtual void SendackPacket (Ptr<Packet> p,uint32_t dst);		//flow-guided
	  virtual void ReceivePacket (Ptr<Packet> p);
	  virtual void ForwardPacket (Ptr<Packet> p);


	private:
	  Ptr<SimpleNanoDevice> m_device;
};


class SenderTypeTag : public Tag
{
	public:
	  static TypeId GetTypeId (void);
	  virtual TypeId GetInstanceTypeId (void) const;

	  virtual uint32_t GetSerializedSize (void) const;
	  virtual void Serialize (TagBuffer i) const;
	  virtual void Deserialize (TagBuffer i);
	  virtual void Print (std::ostream &os) const;

	  uint32_t type;

};

} // namespace ns3

#endif /* NANO_ROUTING_ENTITY_H */
