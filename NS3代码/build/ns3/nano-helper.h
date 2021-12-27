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

#ifndef NANONETWORKS_HELPER_H
#define NANONETWORKS_HELPER_H

#include <string>
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include <ns3/mobility-model.h>
#include "ns3/net-device-container.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/nano-spectrum-channel.h"
#include "ns3/nano-spectrum-phy.h"
#include "ns3/simple-nano-device.h"


namespace ns3 {


/**
 * \ingroup nanonetworks
 * \brief helps to manage and create NanoNetDevice objects
 *
 * This class can help to create a NanoNetDevice objects
 * and to configure their attributes during creation.
 */
class NanoHelper
{
public:

  /**
   * \brief Create a Nano helper in an empty state.
   */
  NanoHelper (void);
  ~NanoHelper (void);


  enum NodeType
    {
	  nanonode, nanorouter, nanointerface
    };


  /**
   * \param c a set of nodes
   * \param type device LtePhytype to create
   */
  NetDeviceContainer Install (NodeContainer c);

  /**
   * \param c a set of nodes
   * \param type device LtePhytype to create
   */
  NetDeviceContainer Install (NodeContainer c, NodeType t);

  /**
  * \brief Add mobility model to a physical device
  * \param phy the physical device
  * \param m the mobility model
  */
  void AddMobility (Ptr<NanoSpectrumPhy> phy, Ptr<MobilityModel>m);


  /**
   * Helper to enable all Nano log components with one statement
   */
  void EnableLogComponents (void);

private:

  Ptr<NanoSpectrumChannel> m_channel;
};

} // namespace ns3

#endif /* NANONETWORKS_HELPER_H */
