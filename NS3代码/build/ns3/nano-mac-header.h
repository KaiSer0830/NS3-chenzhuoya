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


#ifndef NANO_MAC_HEADER_H
#define NANO_MAC_HEADER_H

#include <ns3/header.h>
#include <ns3/mac48-address.h>
#include <ns3/address-utils.h>

#define MAXTTLVALUE 1000

namespace ns3 {
/**
 * \ingroup nanonetworks
 *
 * This class implements the MAC header for nano nodes
 */
class NanoMacHeader : public Header
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  /**
   * \brief set the source id
   * \param source the source id
   */
  void SetSource (uint32_t source);
  /**
   * \brief set the the destination id
   * \param destination the destination id
   */
  void SetDestination (uint32_t destination);
  /**
   * \brief set the ttl
   * \param ttl the ttl value
   */
//  void SetTtl (uint32_t ttl);

  /**
   * \brief get the the source id
   * \return the source id
   */
  uint32_t GetSource () const;
  /**
   * \brief get the the destination id
   * \return the destination id
   */
  uint32_t GetDestination () const;
  /**
   * \brief get the the ttl
   * \return the ttl
   */



private:
  uint32_t m_source;
  uint32_t m_destination;

//  uint32_t m_ttl;
};

} // namespace ns3

#endif /* NANO_MAC_HEADER_H */
