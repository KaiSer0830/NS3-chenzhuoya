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


#ifndef NANO_MAC_ENTITY_H
#define NANO_MAC_ENTITY_H

#include "ns3/object.h"
#include <list>
#include <iostream>
#include <utility>
#include <ns3/nano-detail.h>

namespace ns3 {

class Packet;
class SimpleNanoDevice;
class NanoMacQueue;

/**
 * \ingroup nanonetworks
 *
 * This class provides a basic implementation of the MAC
 * layer
 */
class NanoMacEntity : public Object
{
public:
  static TypeId GetTypeId (void);

  NanoMacEntity (void);
  virtual ~NanoMacEntity (void);

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

  /**
   * \brief Set the mac queue
   * \param q the queue
   */
  void SetMacQueue (Ptr<NanoMacQueue> q);
  void SetMacQueue1 (Ptr<NanoMacQueue> q);					//flow-guided
  void SetMacQueue2 (Ptr<NanoMacQueue> q);					//flow-guided
  /**
   * \brief Get the mac queue
   * \return the pointer to the queue
   */
  Ptr<NanoMacQueue> GetMacQueue ();

  virtual void Send (Ptr<Packet> p) = 0;
  virtual void Send (Ptr<Packet> p, uint32_t dst) = 0;
  virtual void Receive (Ptr<Packet> p)=0;

//  virtual bool Sendack (Ptr<Packet> p, uint32_t from,uint32_t pri);
  virtual void forward (Ptr<Packet> p);
  virtual void sendpacket(Ptr<Packet> p);
  virtual void sendtestpacket(Ptr<Packet> p);
  virtual void sendtestnodepacket(Ptr<Packet> p);
  virtual void sendackpacket(Ptr<Packet> p,uint32_t dst);

  void CheckForNeighborss ();
  void CheckForNeighbors ();

  std::vector<NanoDetail> m_neighborss;
  std::vector<std::pair <uint32_t,uint32_t> > m_neighbors;

private:
  Ptr<SimpleNanoDevice> m_device;
  Ptr<NanoMacQueue> m_queue;
  Ptr<NanoMacQueue> m_waitqueue;
  Ptr<NanoMacQueue> m_sendqueue;

};



} // namespace ns3

#endif /* NANO_MAC_ENTITY_H */
